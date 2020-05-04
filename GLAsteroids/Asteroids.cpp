#include "Asteroids.h"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "FilesystemAdapters/EntityAggregationDeserializer.h"
#include "FilesystemAdapters/EntityAggregationSerializer.h"
#include "Events/EventConsumer.h"
#include "Bullet.h"
#include "Rock.h"
#include "Ship.h"

using boost::property_tree::ptree;

using asteroids::Asteroids;
using asteroids::Bullet;
using asteroids::Rock;
using asteroids::Ship;
using asteroids::State;
using events::EventConsumer;
using filesystem_adapters::EntityAggregationDeserializer;
using filesystem_adapters::EntityAggregationSerializer;

namespace fs = std::filesystem;
namespace pt = boost::property_tree;

namespace
{
const int ROCK_NUMBER = 6;
const std::string ASTEROIDS_KEY = "Asteroids";
const std::string SCORE_KEY = "score";
const std::string ROCK_COUNT_KEY = "rock_count";
const std::string ORIENTATION_ANGLE_KEY = "orientation_angle";
const std::string THRUST_KEY = "thrust";
const fs::path SERIALIZATION_PATH = fs::path(USERS_PATH) / "miclomba" / "desktop" / "asteroids.json";

EntityAggregationDeserializer* const Deserializer = EntityAggregationDeserializer::GetInstance();
EntityAggregationSerializer* const Serializer = EntityAggregationSerializer::GetInstance();

void RegisterEntities(const ptree& tree)
{
	for (const std::pair<std::string, ptree>& keyValue : tree)
	{
		std::string nodeKey = keyValue.first;
		ptree node = keyValue.second;

		if (nodeKey.substr(0, Rock::RockPrefix().length()) == Rock::RockPrefix())
			Deserializer->GetRegistry().RegisterEntity<Rock>(nodeKey);
		else if (nodeKey.substr(0, Bullet::BulletPrefix().length()) == Bullet::BulletPrefix())
			Deserializer->GetRegistry().RegisterEntity<Bullet>(nodeKey);
		else if (nodeKey == Ship::ShipKey())
			Deserializer->GetRegistry().RegisterEntity<Ship>(nodeKey);
		else if (nodeKey == ASTEROIDS_KEY)
			Deserializer->GetRegistry().RegisterEntity<Asteroids>(nodeKey);
		RegisterEntities(node);
	}
}
} // end namespace

Asteroids::Asteroids()
{
	AggregateMember(Ship::ShipKey());
	Deserializer->GetRegistry().RegisterEntity<Ship>(Ship::ShipKey());

	leftArrowConsumer_ = std::make_shared<EventConsumer<void(void)>>([this]() { this->RotateLeft(); });
	rightArrowConsumer_ = std::make_shared<EventConsumer<void(void)>>([this]() { this->RotateRight(); });
	thrustConsumer_ = std::make_shared<EventConsumer<void(void)>>([this]() { this->Thrust(); });
	fireConsumer_ = std::make_shared<EventConsumer<void(void)>>([this]() { this->Fire(); });
	resetConsumer_ = std::make_shared<EventConsumer<void(void)>>([this]() { this->ResetGame(); });
	drawConsumer_ = std::make_shared<EventConsumer<void(void)>>([this]() { this->Draw(); });
	runConsumer_ = std::make_shared<EventConsumer<void(void)>>([this]() { this->Run(); });
	serializeConsumer_ = std::make_shared<EventConsumer<void(void)>>([this]() { this->Serialize(); });
	deserializeConsumer_ = std::make_shared<EventConsumer<void(void)>>([this]() { this->Deserialize(); });

	ResetGame();
}

Asteroids::~Asteroids() = default;
Asteroids::Asteroids(const Asteroids&) = default;
Asteroids::Asteroids(Asteroids&&) = default;
Asteroids& Asteroids::operator=(const Asteroids&) = default;
Asteroids& Asteroids::operator=(Asteroids&&) = default;

void Asteroids::Run()
{
	SetKey(ASTEROIDS_KEY);
	Deserializer->GetRegistry().RegisterEntity<Asteroids>(ASTEROIDS_KEY);
	if (fs::exists(SERIALIZATION_PATH))
	{
		ClearGame();

		Deserializer->GetRegistry().UnregisterAll();
		Deserializer->GetHierarchy().LoadSerializationStructure(SERIALIZATION_PATH.string());
		RegisterEntities(Deserializer->GetHierarchy().GetSerializationStructure());

		Deserializer->LoadEntity(*this);
	}
}

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
	return GetAggregatedMember(Ship::ShipKey());
}

void Asteroids::ClearGame()
{
	std::vector<Key> rockKeys = GetRockKeys();
	for (Key& key : rockKeys)
		RemoveMember(key);
	RemoveMember(GetShip()->GetKey());
}

std::string Asteroids::GenerateUUID() const
{
	boost::uuids::uuid tag = boost::uuids::random_generator()();
	std::string val = boost::lexical_cast<std::string>(tag);
	return val;
}

void Asteroids::ResetGame() 
{
	auto CreateRock = [this](const GLint randy1, const GLint randy2, const std::string& uuidStr)
	{
        SharedEntity rock = std::make_shared<Rock>(State::LARGE,randy1,randy2);
		rock->SetKey(Rock::RockPrefix() + uuidStr);
		AggregateMember(rock);
		Deserializer->GetRegistry().RegisterEntity<Rock>(rock->GetKey());
	};
	auto CreateShip = [this]()
	{
		SharedEntity& sharedShip = GetShip();
		sharedShip = std::make_shared<Ship>(Ship::ShipKey());
	};

	ClearRocks();

    GLint randy1, randy2;
    for (GLint nextRock = 0; nextRock < 10; ++nextRock) {
        do {
            randy1 = rand();
            randy2 = rand();
            randy1 = pow(-1,nextRock) * randy1;
            randy1 = randy1 % 15;
            randy2 = pow(-1,nextRock % 2) * randy2;
            randy2 = randy2 % 15;
        } while (fabs(randy1) < 3 || fabs(randy2) < 3);

		CreateRock(randy1, randy2, GenerateUUID()); 
    }

	CreateShip();
	score_ = 0;
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
		Rock* rock = static_cast<Rock*>(sharedRock.get());
		RemoveMember(sharedRock);
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

bool Asteroids::HasRock(const std::string& key) const
{
	std::vector<Key> rockKeys = GetRockKeys();
	return std::any_of(rockKeys.begin(), rockKeys.end(), [&key](const std::string& rockKey) { return rockKey == key; });
}

void Asteroids::DetermineCollisions()
{
	SharedEntity& sharedShip = GetShip();
	if (!sharedShip)
		return;

	Ship* ship = static_cast<Ship*>(sharedShip.get());
	std::vector<Ship::Key> bulletKeys = ship->GetBulletKeys();

	std::vector<std::pair<Bullet*, Key>> collisionPairs;
	for (Ship::Key& bulletKey : bulletKeys)
	{
		Ship::SharedEntity& sharedBullet = ship->GetBullet(bulletKey);
		Bullet* bullet = static_cast<Bullet*>(sharedBullet.get());
		Rock* rock = Collision(bullet);

		if (rock)
			collisionPairs.push_back(std::make_pair(bullet,rock->GetKey()));
	}

	for (std::pair<Bullet*, Key>& collisions : collisionPairs)
	{
		if (HasRock(collisions.second))
		{
			SharedEntity& rock = GetRock(collisions.second);
			ProcessCollision(collisions.first, static_cast<Rock*>(rock.get()));
			if (!HasRocks())
			{
				ResetGame();
				return;
			}
		}
	}

	if (ShipCollision()) 
		ResetGame();
}

void Asteroids::ProcessCollision(Bullet* bullet, Rock* rock) 
{
	score_ += 1;
	if (rock->GetState() != State::SMALL)
	{
		CalculateConservationOfMomentum(bullet, rock);
		BreakRock(rock);
	}
	DestroyRock(rock);
	DestroyBullet(bullet);
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
        ray= sqrt(pow(fabs(_bullet->GetFrame().GetData(0,0) - rock->GetFrame().GetData(0,0)),2) +
                pow(fabs(_bullet->GetFrame().GetData(1,0) - rock->GetFrame().GetData(1,0)),2));
        
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

Rock* Asteroids::ShipCollision() {
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
			ray = sqrt(pow(fabs(ship->GetFrame().GetData(0,0) - rock->GetFrame().GetData(0,0)), 2) +
				pow(fabs(ship->GetFrame().GetData(1,0) - rock->GetFrame().GetData(1,0)), 2));

			if (rock->GetState() == State::LARGE)
				epsilon = 2.2f;
			else if (rock->GetState() == State::MEDIUM)
				epsilon = 1.6f;
			else
				epsilon = 1.0f;

			if (ray < epsilon)
				return rock;
		}
	}
    return nullptr;
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

	auto rock1 = std::make_shared<Rock>(rockSize, rock->GetFrame().GetData(0,0), rock->GetFrame().GetData(1,0));

	rock1->SetKey(Rock::RockPrefix() + GenerateUUID()); 

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
	Deserializer->GetRegistry().RegisterEntity<Rock>(rock1->GetKey());
	Deserializer->GetRegistry().RegisterEntity<Rock>(rock2->GetKey());
}

void Asteroids::DestroyBullet(Bullet* bullet)
{
	SharedEntity& sharedShip = GetShip();
	if (!sharedShip)
		return;

	Ship* ship = static_cast<Ship*>(sharedShip.get());

	std::string bulletKey = bullet->GetKey();

	Deserializer->GetRegistry().UnregisterEntity(bulletKey);
	ship->RemoveBullet(bulletKey);
}

void Asteroids::DestroyRock(Rock* rock)
{
	std::string rockKey = rock->GetKey();

	Deserializer->GetRegistry().UnregisterEntity(rockKey);
	RemoveMember(rockKey);
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

void Asteroids::Save(boost::property_tree::ptree& tree, const std::string& path) const
{
	if (!fs::exists(path))
		fs::create_directories(path);

	tree.put(SCORE_KEY, score_);
	tree.put(ORIENTATION_ANGLE_KEY, orientationAngle_);
	tree.put(THRUST_KEY, thrust_);
}

void Asteroids::Load(boost::property_tree::ptree& tree, const std::string& path)
{
	score_ = std::stoi(tree.get_child(SCORE_KEY).data());
	orientationAngle_ = std::stof(tree.get_child(ORIENTATION_ANGLE_KEY).data());
	thrust_ = std::stoi(tree.get_child(THRUST_KEY).data());
}

void Asteroids::Serialize()
{
	Serializer->GetHierarchy().SetSerializationPath(SERIALIZATION_PATH.string());
	Serializer->Serialize(*this);
}

void Asteroids::Deserialize()
{
	Deserializer->GetRegistry().UnregisterAll();
	Deserializer->GetHierarchy().LoadSerializationStructure(SERIALIZATION_PATH.string());
	RegisterEntities(Deserializer->GetHierarchy().GetSerializationStructure());

	ClearGame();
	Deserializer->LoadEntity(*this);
}

std::shared_ptr<EventConsumer<void(void)>> Asteroids::GetLeftArrowConsumer()
{
	return leftArrowConsumer_;
}

std::shared_ptr<EventConsumer<void(void)>> Asteroids::GetRightArrowConsumer()
{
	return rightArrowConsumer_;
}

std::shared_ptr<EventConsumer<void(void)>> Asteroids::GetThrustConsumer()
{
	return thrustConsumer_;
}

std::shared_ptr<EventConsumer<void(void)>> Asteroids::GetFireConsumer()
{
	return fireConsumer_;
}

std::shared_ptr<EventConsumer<void(void)>> Asteroids::GetResetConsumer()
{
	return resetConsumer_;
}

std::shared_ptr<EventConsumer<void(void)>> Asteroids::GetDrawConsumer()
{
	return drawConsumer_;
}

std::shared_ptr<EventConsumer<void(void)>> Asteroids::GetRunConsumer()
{
	return runConsumer_;
}

std::shared_ptr<EventConsumer<void(void)>> Asteroids::GetSerializeConsumer()
{
	return serializeConsumer_;
}

std::shared_ptr<EventConsumer<void(void)>> Asteroids::GetDeserializeConsumer()
{
	return deserializeConsumer_;
}
