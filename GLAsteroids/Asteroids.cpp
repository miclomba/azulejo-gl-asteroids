#include "Asteroids.h"

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

using asteroids::Asteroids;
using asteroids::Bullet;
using asteroids::Rock;
using asteroids::Ship;
using asteroids::State;

namespace
{
const int ROCK_NUMBER = 6;
const std::string SHIP_KEY = "Ship";
const std::string ROCK_KEY = "Rock";
}

Asteroids::Asteroids()
{
	for (GLint i = 0; i < ROCK_NUMBER; i++)
		AggregateMember(ROCK_KEY + std::to_string(i));
	AggregateMember(SHIP_KEY);

	InitGame();
}

Asteroids::~Asteroids() = default;
Asteroids::Asteroids(const Asteroids&) = default;
Asteroids::Asteroids(Asteroids&&) = default;
Asteroids& Asteroids::operator=(const Asteroids&) = default;
Asteroids& Asteroids::operator=(Asteroids&&) = default;

Asteroids::SharedEntity& Asteroids::GetRock(const std::string& key)
{
	return GetAggregatedMember(key);
}

std::vector<Asteroids::Key> Asteroids::GetRockKeys() const
{
	return GetAggregatedMemberKeys<Rock>();
}

Asteroids::SharedEntity& Asteroids::GetShip()
{
	return GetAggregatedMember(SHIP_KEY);
}

void Asteroids::InitGame() 
{
	auto CreateRock = [this](const GLint randy1, const GLint randy2, const GLint i)
	{
		SharedEntity& rock = GetRock(ROCK_KEY + std::to_string(i));
        rock = std::make_shared<Rock>(State::LARGE,randy1,randy2,i);
		rock->SetKey(ROCK_KEY + std::to_string(i));
	};
	auto CreateShip = [this]()
	{
		SharedEntity& sharedShip = GetShip();
		sharedShip = std::make_shared<Ship>(SHIP_KEY);
	};

    GLint randy1, randy2;
    for (GLint i = 0; i < ROCK_NUMBER; i++) {
        do {
            randy1 = rand();
            randy2 = rand();
            randy1 = pow(-1,i) * randy1;
            randy1 = randy1 % 15;
            randy2 = pow(-1,i % 2) * randy2;
            randy2 = randy2 % 15;
        } while (fabs(randy1) < 3 || fabs(randy2) < 3);

		CreateRock(randy1, randy2, i);
    }
    rockCount_ = ROCK_NUMBER;
	CreateShip();
}

void Asteroids::DrawRockAndShip()
{
    GLint randy;
	std::vector<Key> rockKeys = GetRockKeys();

	for (Key& key : rockKeys)
	{
		SharedEntity& sharedRock = GetRock(key);
		if (!sharedRock)
			continue;

		Rock* rock = static_cast<Rock*>(sharedRock.get());
		randy = rand();
		randy = (randy % 9) + 1;
		rock->Draw((GLfloat)(M_PI*randy / 5),
			(GLfloat)(randy % 3) / 100,
			(GLfloat)(randy % 6) / 100);
	}

	SharedEntity& sharedShip = GetShip();
	if (sharedShip)
	{
		Ship* ship = static_cast<Ship*>(sharedShip.get());
		ship->Draw(orientationAngle_, thrust_);
	}
}

void Asteroids::DrawGameInfo()
{
	glColor3f(0.0f, 1.0f, 1.0f);

	GLint i;
	glRasterPos3f(7.5f, -9.0f, 0.0f);
	char reset[16] = { 'P','r','e','s','s',' ','X',' ','t','o',' ',
					  'R','E','S','E','T' };
	for (i = 0; i < 16; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, reset[i]);

	glRasterPos3f(-12.0f, 9.0f, 0.0f);
	char score[7] = { 'S','C','O','R','E',':',' ' };
	for (i = 0; i < 7; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, score[i]);
	char amount[4];
	i = sprintf_s(amount, "%d", score_);
	for (i = 0; i < 4; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, amount[i]);
}

void Asteroids::Draw()
{
	DrawRockAndShip();
	DrawGameInfo();
	DetermineCollisions();
	ResetThrustAndRotation();
}

void Asteroids::ClearRocks()
{
	std::vector<Key> rockKeys = GetRockKeys();
	for (Key& key : rockKeys)
	{
		SharedEntity& sharedRock = GetRock(key);
		if (sharedRock)
		{
			Rock* rock = static_cast<Rock*>(sharedRock.get());
			if (rock->GetIndex() >= ROCK_NUMBER)
				RemoveMember(sharedRock);
			else
				sharedRock.reset();
		}
	}
}

bool Asteroids::HasRocks()
{
	std::vector<Key> rockKeys = GetRockKeys();
	for (Key& key : rockKeys)
	{
		SharedEntity& sharedRock = GetRock(key);
		if (sharedRock)
			return true;
	}
	return false;
}

void Asteroids::DetermineCollisions()
{
	SharedEntity& sharedShip = GetShip();
	if (!sharedShip)
		return;

	Ship* ship = static_cast<Ship*>(sharedShip.get());
	std::vector<Ship::Key> bulletKeys = ship->GetBulletKeys();

	for (Ship::Key& bulletKey : bulletKeys)
	{
		SharedEntity& sharedShipEntity = GetShip();
		if (!sharedShipEntity)
			break;

		Ship* ship = static_cast<Ship*>(sharedShipEntity.get());
		Ship::SharedEntity& sharedBullet = ship->GetBullet(bulletKey);
		if (sharedBullet) {
			Bullet* bullet = static_cast<Bullet*>(sharedBullet.get());
			Rock* ithRock = Collision(bullet);

			if (ithRock)
				ComputeCollision(bullet, ithRock);
		}
	}

	if (Bump() != -1) 
	{
		ClearRocks();
		SharedEntity& sharedShipEntity = GetShip();
		sharedShipEntity.reset();
		InitGame();
	}
}

Rock* Asteroids::Collision(Bullet* _bullet) 
{
    GLfloat epsilon;
    GLfloat ray;

	std::vector<Key> rockKeys = GetRockKeys();
	for (Key& key : rockKeys)
	{
		SharedEntity& sharedRock = GetRock(key);
		if (!sharedRock)
			continue;

		Rock* rock = static_cast<Rock*>(sharedRock.get());
        ray= sqrt(pow(fabs(_bullet->GetFrame()[0][0] - rock->GetFrame()[0][0]),2) +
                pow(fabs(_bullet->GetFrame()[1][0] - rock->GetFrame()[1][0]),2));
        
        if (rock->GetState() == State::LARGE)
            epsilon = 1.7f;
        else if (rock->GetState() == State::MEDIUM)
            epsilon = 1.3f;
        else
            epsilon = 0.8f;

        if (ray < epsilon)
            return rock;
    }
    return nullptr;
}

GLint Asteroids::Bump() {
    GLfloat epsilon;
    GLfloat ray;

	SharedEntity& sharedShip = GetShip();
	if (sharedShip)
	{
		Ship* ship = static_cast<Ship*>(sharedShip.get());
		std::vector<Key> rockKeys = GetRockKeys();
		for (Key& key : rockKeys)
		{
			SharedEntity& sharedRock = GetRock(key);
			if (!sharedRock)
				continue;

			Rock* rock = static_cast<Rock*>(sharedRock.get());
			ray = sqrt(pow(fabs(ship->GetFrame()[0][0] - rock->GetFrame()[0][0]), 2) +
				pow(fabs(ship->GetFrame()[1][0] - rock->GetFrame()[1][0]), 2));

			if (rock->GetState() == State::LARGE)
				epsilon = 2.2f;
			else if (rock->GetState() == State::MEDIUM)
				epsilon = 1.6f;
			else
				epsilon = 1.0f;

			if (ray < epsilon)
				return rock->GetIndex();
		}
	}
    return -1;
}

void Asteroids::CalculateConservationOfMomentum(Bullet* bullet, Rock* rock)
{
	GLfloat xCompMomentumB = bullet->GetMass() * bullet->GetSpeed() * cos(bullet->GetVelocityAngle());
	GLfloat yCompMomentumB = bullet->GetMass() * bullet->GetSpeed() * sin(bullet->GetVelocityAngle());
	GLfloat xCompMomentumR = rock->GetMass() * rock->GetSpeed() * cos(rock->GetVelocityAngle());
	GLfloat yCompMomentumR = rock->GetMass() * rock->GetSpeed() * sin(rock->GetVelocityAngle());

	xCompMomentumR += xCompMomentumB;
	yCompMomentumR += yCompMomentumB;

	GLfloat momentumMagnitude = sqrt(pow(xCompMomentumR, 2) + pow(yCompMomentumR, 2));

	GLfloat momentumAngle = (GLfloat)(atan(yCompMomentumR / xCompMomentumR));
	if (xCompMomentumR < 0)
		momentumAngle += M_PI;
	else if (yCompMomentumR < 0)
		momentumAngle += 2 * M_PI;

	rock->SetMass(rock->GetMass() + bullet->GetMass());
	rock->SetSpeed(momentumMagnitude / rock->GetMass());

	rock->SetVelocityAngle(momentumAngle);

	rock->SetUnitVelocity(0,0,cos(momentumAngle));
	rock->SetUnitVelocity(1,0,sin(momentumAngle));
}

std::shared_ptr<Rock> Asteroids::MakeRock(const State rockSize, Rock* rock, const bool halfMass, const bool clockwise)
{
	GLint massDenominator = halfMass ? 2 : 1;
	GLfloat angleMultiplier = clockwise ? 1.0 : -1.0;

	auto rock1 = std::make_shared<Rock>(rockSize, rock->GetFrame()[0][0], rock->GetFrame()[1][0], rockCount_);
	rock1->SetKey(ROCK_KEY + std::to_string(rockCount_++));

	rock1->SetMass(rock->GetMass() / massDenominator);
	rock1->SetSpeed(rock->GetSpeed());
	rock1->SetSpin(rock->GetSpin());
	rock1->SetSpinEpsilon(rock->GetSpinEpsilon());
	rock1->SetVelocityAngle(rock->GetVelocityAngle() + angleMultiplier * M_PI / 4);
	rock1->SetUnitVelocity(0,0,cos(rock1->GetVelocityAngle()));
	rock1->SetUnitVelocity(1,0,sin(rock1->GetVelocityAngle()));
	rock1->SetRockInitialized(true);

	return rock1;
}

void Asteroids::BreakRock(Rock* rock)
{
	std::shared_ptr<Rock> rock1;
	std::shared_ptr<Rock> rock2;
	if (rock->GetState() == State::LARGE)
	{
		rock1 = MakeRock(State::MEDIUM, rock, true, true);
		rock2 = MakeRock(State::MEDIUM, rock, true, false);
	}
	else if (rock->GetState() == State::MEDIUM)
	{
		rock1 = MakeRock(State::SMALL, rock, false, true);
		rock2 = MakeRock(State::SMALL, rock, false, false);
	}
	AggregateMember(rock1);
	AggregateMember(rock2);
}

void Asteroids::DestroyBullet(Bullet* bullet)
{
	SharedEntity& sharedShip = GetShip();
	if (!sharedShip)
		return;

	Ship* ship = static_cast<Ship*>(sharedShip.get());

	std::string bulletKey = bullet->GetKey(); 
	Ship::SharedEntity& sharedBullet = ship->GetBullet(bulletKey);
	sharedBullet.reset();
}

void Asteroids::DestroyRock(Rock* rock)
{
	GLint id = rock->GetIndex();
	if (id >= ROCK_NUMBER)
	{
		RemoveMember(ROCK_KEY + std::to_string(id));
	}
	else
	{
		SharedEntity& sharedRock = GetRock(rock->GetKey());
		sharedRock.reset();
	}
}

void Asteroids::DestroyGeometry(Bullet* bullet, Rock* rock)
{
	DestroyBullet(bullet);
	DestroyRock(rock);

	if (!HasRocks()) {
		SharedEntity& sharedShip = GetShip();
		sharedShip.reset();
		InitGame();
	}
}

void Asteroids::ComputeCollision(Bullet* bullet, Rock* rock) {

    score_ += 1;

    if (rock->GetState() != State::SMALL) 
	{
		CalculateConservationOfMomentum(bullet, rock);
		BreakRock(rock);
    }

	DestroyGeometry(bullet, rock);
}

void Asteroids::ResetGame()
{
	ClearRocks();

	SharedEntity& ship = GetShip();
	ship.reset();
	score_ = 0;
	InitGame();
}

void Asteroids::ResetThrustAndRotation()
{
	orientationAngle_ = 0.0f;
	thrust_ = 0.0f;
}

void Asteroids::Fire()
{
	SharedEntity& sharedShip = GetShip();
	if (sharedShip)
	{
		Ship* ship = static_cast<Ship*>(sharedShip.get());
		ship->Fire();
	}
}

void Asteroids::RotateLeft()
{
	orientationAngle_ = 0.15f; 
}

void Asteroids::RotateRight()
{
	orientationAngle_ = -0.15f; 
}

void Asteroids::Thrust()
{
	thrust_ = 0.01f; 
}
