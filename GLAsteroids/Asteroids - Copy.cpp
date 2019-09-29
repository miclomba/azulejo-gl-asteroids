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
	return GetAggregatedMember<Rock>(key);
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

	AggregateMember(SHIP_KEY);

    InitGame();
}

void Asteroids::InitGame() 
{
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
        rockVector_.push_back(std::make_shared<Rock>(State::LARGE,randy1,randy2,i));
        rockCount_ = i;
    }

	SharedEntity& sharedShip = GetShip();
	sharedShip = std::make_shared<Ship>(SHIP_KEY);
}

void Asteroids::DrawRockAndShip()
{
    GLint randy;
	auto iter = rockVector_.begin();
	while (iter != rockVector_.end()) {
		randy = rand();
		randy = (randy % 9) + 1;
		(*(iter))->Draw((GLfloat)(M_PI*randy / 5),
			(GLfloat)(randy % 3) / 100,
			(GLfloat)(randy % 6) / 100);
		iter++;
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
			GLint ithRock = Collision(bullet);

			if (ithRock != -1)
				ComputeCollision(bullet, ithRock);
		}
	}

	if (Bump() != -1) {
		rockVector_.clear();
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
		rockVector_.clear();
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

GLint Asteroids::Collision(Bullet* _bullet) 
{
    auto iter = rockVector_.begin();
    GLfloat epsilon;
    GLfloat ray;

    while (iter != rockVector_.end()) {
        ray= sqrt(pow(fabs(_bullet->GetFrame()[0][0]-(*(iter))->GetFrame()[0][0]),2) +
                pow(fabs(_bullet->GetFrame()[1][0]-(*(iter))->GetFrame()[1][0]),2));
        
        if ((*(iter))->GetState() == State::LARGE)
            epsilon = 1.7f;
        else if ((*(iter))->GetState() == State::MEDIUM)
            epsilon = 1.3f;
        else
            epsilon = 0.8f;

        if (ray < epsilon)
            return (*(iter))->GetIndex();
        iter++;
    }
    return -1;
}

GLint Asteroids::Bump() {
    auto iter = rockVector_.begin();
    GLfloat epsilon;
    GLfloat ray;

	SharedEntity& sharedShip = GetShip();
	if (sharedShip)
	{
		Ship* ship = static_cast<Ship*>(sharedShip.get());
		while (iter != rockVector_.end()) {
			ray = sqrt(pow(fabs(ship->GetFrame()[0][0] - (*(iter))->GetFrame()[0][0]), 2) +
				pow(fabs(ship->GetFrame()[1][0] - (*(iter))->GetFrame()[1][0]), 2));

			if ((*(iter))->GetState() == State::LARGE)
				epsilon = 2.2f;
			else if ((*(iter))->GetState() == State::MEDIUM)
				epsilon = 1.6f;
			else
				epsilon = 1.0f;

			if (ray < epsilon)
				return (*(iter))->GetIndex();
			iter++;
		}
	}
    return -1;
}

void Asteroids::CalculateConservationOfMomentum(Bullet* bullet, const GLint i)
{
	GLfloat xCompMomentumB = bullet->GetMass() * bullet->GetSpeed() * cos(bullet->GetVelocityAngle());
	GLfloat yCompMomentumB = bullet->GetMass() * bullet->GetSpeed() * sin(bullet->GetVelocityAngle());
	GLfloat xCompMomentumR = rockVector_.at(i)->GetMass() * rockVector_.at(i)->GetSpeed() * cos(rockVector_.at(i)->GetVelocityAngle());
	GLfloat yCompMomentumR = rockVector_.at(i)->GetMass() * rockVector_.at(i)->GetSpeed() * sin(rockVector_.at(i)->GetVelocityAngle());

	xCompMomentumR += xCompMomentumB;
	yCompMomentumR += yCompMomentumB;

	GLfloat momentumMagnitude = sqrt(pow(xCompMomentumR, 2) + pow(yCompMomentumR, 2));

	GLfloat momentumAngle = (GLfloat)(atan(yCompMomentumR / xCompMomentumR));
	if (xCompMomentumR < 0)
		momentumAngle += M_PI;
	else if (yCompMomentumR < 0)
		momentumAngle += 2 * M_PI;

	rockVector_.at(i)->GetMass() = rockVector_.at(i)->GetMass() + bullet->GetMass();
	rockVector_.at(i)->GetSpeed() = momentumMagnitude / rockVector_.at(i)->GetMass();

	rockVector_.at(i)->GetVelocityAngle() = momentumAngle;

	rockVector_.at(i)->GetUnitVelocity()[0][0] = cos(momentumAngle);
	rockVector_.at(i)->GetUnitVelocity()[1][0] = sin(momentumAngle);
}


std::shared_ptr<Rock> Asteroids::MakeRock(const State rockSize, const GLint i, const bool halfMass, const bool clockwise)
{
	GLint massDenominator = halfMass ? 2 : 1;
	GLfloat angleMultiplier = clockwise ? 1.0 : -1.0;

	auto rock1 = std::make_shared<Rock>(rockSize, rockVector_.at(i)->GetFrame()[0][0],
		rockVector_.at(i)->GetFrame()[1][0],
		++rockCount_);
	rock1->GetMass() = rockVector_.at(i)->GetMass() / massDenominator;
	rock1->GetSpeed() = rockVector_.at(i)->GetSpeed();
	rock1->GetSpin() = rockVector_.at(i)->GetSpin();
	rock1->GetSpinEpsilon() = rockVector_.at(i)->GetSpinEpsilon();
	rock1->GetVelocityAngle() = rockVector_.at(i)->GetVelocityAngle() + angleMultiplier * M_PI / 4;
	rock1->GetUnitVelocity()[0][0] = cos(rock1->GetVelocityAngle());
	rock1->GetUnitVelocity()[1][0] = sin(rock1->GetVelocityAngle());
	rock1->SetRockInitialized(true);

	return rock1;
}

void Asteroids::BreakRock(const GLint i)
{
	if (rockVector_.at(i)->GetState() == State::LARGE)
	{
		std::shared_ptr<Rock> rock1 = MakeRock(State::MEDIUM, i, true, true);
		std::shared_ptr<Rock> rock2 = MakeRock(State::MEDIUM, i, true, false);

		rockVector_.push_back(rock1);
		rockVector_.push_back(rock2);
	}
	else if (rockVector_.at(i)->GetState() == State::MEDIUM)
	{
		std::shared_ptr<Rock> rock1 = MakeRock(State::SMALL, i, false, true);
		std::shared_ptr<Rock> rock2 = MakeRock(State::SMALL, i, false, false);

		rockVector_.push_back(rock1);
		rockVector_.push_back(rock2);
	}
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

void Asteroids::DestroyGeometry(Bullet* bullet, const GLint i)
{
	DestroyBullet(bullet);

	rockVector_.erase(rockVector_.begin() + i);

	if (rockVector_.empty()) {
		rockVector_.clear();
		SharedEntity& sharedShip = GetShip();
		sharedShip.reset();
		InitGame();
	}
}


void Asteroids::ComputeCollision(Bullet* bullet, const GLint _ithRock) {

    score_ += 1;

    GLint i;
    for (i = 0; (GLuint)i < rockVector_.size(); i++) {
        if (rockVector_.at(i)->GetIndex() == _ithRock)
            break;
    }

    if (rockVector_.at(i)->GetState() != State::SMALL) 
	{
		CalculateConservationOfMomentum(bullet, i);
		BreakRock(i);
    }

	DestroyGeometry(bullet, i);
}
