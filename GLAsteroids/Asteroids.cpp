#include "Asteroids.h"

#include <cstdlib>
#include <memory>
#include <string>

using asteroids::Asteroids;
using asteroids::Bullet;
using asteroids::Rock;
using asteroids::Ship;
using asteroids::State;

namespace
{
const int WIN_WIDTH = 600;
const int WIN_HEIGHT = 480;
const int NUMBER_KEYS = 256;
const int ROCK_NUMBER = 6;
const int INIT_WIN_X = 100;
const int INIT_WIN_Y = 100;
const std::string TITLE = "Asteroids";
const std::string SHIP_KEY = "Ship";
const std::string ROCK_KEY = "Rock";
}

/*======================== CALLBACK POINTER ==================================*/
Asteroids* Asteroids::callbackInstance_(nullptr);
/*============================================================================*/

Asteroids::Asteroids() = default;

void Asteroids::Run() 
{ 
	glutMainLoop(); 
};

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

void Asteroids::RegisterCallbacks() 
{
	glutDisplayFunc(DisplayWrapper);   glutReshapeFunc(ReshapeWrapper);
	glutKeyboardFunc(KeyboardWrapper); glutKeyboardUpFunc(KeyboardUpWrapper);
}

void Asteroids::DisplayWrapper() 
{
	callbackInstance_->Draw();
}

void Asteroids::ReshapeWrapper(const int _w, const int _h) 
{
	callbackInstance_->Reshape(_w, _h);
}

void Asteroids::KeyboardWrapper(const unsigned char _chr, const int _x, const int _y) 
{
	callbackInstance_->Keyboard(_chr, _x, _y);
}

void Asteroids::KeyboardUpWrapper(const unsigned char _chr, const int _x, const int _y) 
{
	callbackInstance_->KeyboardUp(_chr, _x, _y);
}

void Asteroids::InitGlut(int _argc, char* _argv[])
{
	glutInit(&_argc, _argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(INIT_WIN_X, INIT_WIN_Y);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow(TITLE.c_str());
}

void Asteroids::InitServer()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthRange(0, 1);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Asteroids::InitClient()
{
    glEnableClientState(GL_VERTEX_ARRAY);
}

Asteroids::Asteroids(int _argc, char* _argv[]) 
{
    if (!callbackInstance_)
        callbackInstance_ = this;

	fill(keysPressed_.begin(), keysPressed_.end(), false);

	InitGlut(_argc, _argv);
    RegisterCallbacks();
	InitServer();
	InitClient();

	for (GLint i = 0; i < ROCK_NUMBER; i++) 
		AggregateMember(ROCK_KEY + std::to_string(i));
	AggregateMember(SHIP_KEY);

    InitGame();
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

void Asteroids::Draw() 
{
    KeyboardUpdateState();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawRockAndShip();

    orientationAngle_ = 0.0f;
    thrust_ = 0.0f;

	DrawGameInfo();

    // RENDER
    glFlush();
    glutSwapBuffers();

	DetermineCollisions();
}

void Asteroids::Reshape(const int _w, const int _h) 
{
    //========================= DEFINE VIEWPORT ==============================*/
    glViewport(0,0,_w,_h);
    /*========================= ORTHO PROJECTION =============================*/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if ( _w <= _h )
        glOrtho(-10.0,10.0,-10.0*((GLfloat)_h / (GLfloat)_w),
                 10.0*((GLfloat)_h / (GLfloat)_w),10,-10);
    else
        glOrtho(-10.0*((GLfloat)_w / (GLfloat)_h),
                 10.0*((GLfloat)_w / (GLfloat)_h),-10.0,10.0,10,-10);
    /*========================= REDISPLAY ====================================*/
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void Asteroids::Keyboard(const unsigned char _chr, const int _x, const int _y) 
{
    keysPressed_[_chr] = true;
}

void Asteroids::KeyboardUp(const unsigned char _chr, const int _x, const int _y) 
{
    keysPressed_[_chr] = false;
}

void Asteroids::KeyboardUpdateState() 
{
	auto Reset = [this](SharedEntity& ship)
	{
		ClearRocks();
		ship.reset();
		score_ = 0;
		InitGame();
	};

    for (int i = 0; i < NUMBER_KEYS; i++) 
	{
		SharedEntity& sharedShip = GetShip();
        if (keysPressed_[i]) {
            switch (i)
            {
                case 's':
                    orientationAngle_ = 0.15f; break;
                case 'f':
                    orientationAngle_ = -0.15f; break;
                case 'e':
                    thrust_ = 0.01f; break;
                case 'j':
					if (sharedShip)
					{
						Ship* ship = static_cast<Ship*>(sharedShip.get());
						ship->Fire();
					}
					break;
                case 'x':
					Reset(sharedShip); break;
                case 'X':
					Reset(sharedShip); break;
                default:
                    break;
            }
        }
    }
}

void Asteroids::TimerCallback(int _idx) 
{
    switch (_idx)
    {
        case 0:
            glutPostRedisplay();
            glutTimerFunc(25,TimerCallback,0);
            break;
        default:
            break;
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

	rock->GetMass() = rock->GetMass() + bullet->GetMass();
	rock->GetSpeed() = momentumMagnitude / rock->GetMass();

	rock->GetVelocityAngle() = momentumAngle;

	rock->GetUnitVelocity()[0][0] = cos(momentumAngle);
	rock->GetUnitVelocity()[1][0] = sin(momentumAngle);
}


std::shared_ptr<Rock> Asteroids::MakeRock(const State rockSize, Rock* rock, const bool halfMass, const bool clockwise)
{
	GLint massDenominator = halfMass ? 2 : 1;
	GLfloat angleMultiplier = clockwise ? 1.0 : -1.0;

	auto rock1 = std::make_shared<Rock>(rockSize, rock->GetFrame()[0][0], rock->GetFrame()[1][0], rockCount_);
	rock1->SetKey(ROCK_KEY + std::to_string(rockCount_++));

	rock1->GetMass() = rock->GetMass() / massDenominator;
	rock1->GetSpeed() = rock->GetSpeed();
	rock1->GetSpin() = rock->GetSpin();
	rock1->GetSpinEpsilon() = rock->GetSpinEpsilon();
	rock1->GetVelocityAngle() = rock->GetVelocityAngle() + angleMultiplier * M_PI / 4;
	rock1->GetUnitVelocity()[0][0] = cos(rock1->GetVelocityAngle());
	rock1->GetUnitVelocity()[1][0] = sin(rock1->GetVelocityAngle());
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
