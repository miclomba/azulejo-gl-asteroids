#include "GLGame.h"

#include <filesystem>
#include <string>
#include <utility>

#include <boost/property_tree/ptree.hpp>

#include "Entities/EntityAggregationDeserializer.h"
#include "Entities/EntityAggregationSerializer.h"
#include "Events/EventEmitter.h"

#include "Asteroids.h"
#include "Bullet.h"
#include "Rock.h"
#include "Ship.h"

using boost::property_tree::ptree;
using entity::EntityAggregationDeserializer;
using entity::EntityAggregationSerializer;
using events::EventEmitter;

using asteroids::Asteroids;
using asteroids::Bullet;
using asteroids::GLGame;
using asteroids::Rock;
using asteroids::Ship;

namespace fs = std::filesystem;

namespace
{
const int WIN_WIDTH = 600;
const int WIN_HEIGHT = 480;
const int NUMBER_KEYS = 256;
const int INIT_WIN_X = 100;
const int INIT_WIN_Y = 100;

const fs::path SERIALIZATION_PATH = fs::path(USERS_PATH) / "miclomba" / "desktop" / "asteroids.json";

EntityAggregationDeserializer* const Deserializer = EntityAggregationDeserializer::GetInstance();
EntityAggregationSerializer* const Serializer = EntityAggregationSerializer::GetInstance();

void RegisterEntities(const ptree& tree)
{
	for (const std::pair<std::string, ptree>& keyValue : tree)
	{
		std::string nodeKey = keyValue.first;
		ptree node = keyValue.second;

		if (nodeKey.substr(0,Rock::RockPrefix().length()) == Rock::RockPrefix())
			Deserializer->RegisterEntity<Rock>(nodeKey);
		else if (nodeKey.substr(0,Bullet::BulletPrefix().length()) == Bullet::BulletPrefix())
			Deserializer->RegisterEntity<Bullet>(nodeKey);
		else if (nodeKey == Ship::ShipKey())
			Deserializer->RegisterEntity<Ship>(nodeKey);
		else if (nodeKey == Asteroids::AsteroidsKey())
			Deserializer->RegisterEntity<Asteroids>(nodeKey);
		RegisterEntities(node);
	}
}
} // end namespace

/*======================== CALLBACK POINTER ==================================*/
GLGame* GLGame::callbackInstance_(nullptr);
/*============================================================================*/

GLGame::~GLGame() = default;

void GLGame::TimerCallback(int _idx)
{
	switch (_idx)
	{
	case 0:
		glutPostRedisplay();
		glutTimerFunc(25, TimerCallback, 0);
		break;
	default:
		break;
	}
}

GLGame::GLGame(int _argc, char* _argv[])
{
	if (!callbackInstance_)
		callbackInstance_ = this;

	fill(keysPressed_.begin(), keysPressed_.end(), false);

	InitGlut(_argc, _argv);
	RegisterCallbacks();
	InitServer();
	InitClient();

	leftArrowEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
	rightArrowEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
	thrustEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
	fireEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
	resetEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
	drawEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
	clearEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
}

void GLGame::Run()
{
	game_.SetKey(Asteroids::AsteroidsKey());
	Deserializer->RegisterEntity<Asteroids>(Asteroids::AsteroidsKey());
	if (fs::exists(SERIALIZATION_PATH))
	{
		clearEmitter_->Signal()();

		Deserializer->UnregisterAll();
		Deserializer->LoadSerializationStructure(SERIALIZATION_PATH.string());
		RegisterEntities(Deserializer->GetSerializationStructure());

		Deserializer->Deserialize(game_);
	}

	glutMainLoop();
};

void GLGame::RegisterCallbacks() const
{
	glutDisplayFunc(DisplayWrapper);   
	glutReshapeFunc(ReshapeWrapper);
	glutKeyboardFunc(KeyboardWrapper); 
	glutKeyboardUpFunc(KeyboardUpWrapper);
}

void GLGame::InitGlut(int _argc, char* _argv[]) const
{
	glutInit(&_argc, _argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(INIT_WIN_X, INIT_WIN_Y);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow(Asteroids::AsteroidsKey().c_str());
}

void GLGame::InitServer() const
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

void GLGame::InitClient() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
}

void GLGame::DisplayWrapper()
{
	callbackInstance_->Display();
}

void GLGame::ReshapeWrapper(const int _w, const int _h)
{
	callbackInstance_->Reshape(_w, _h);
}

void GLGame::KeyboardWrapper(const unsigned char _chr, const int _x, const int _y)
{
	callbackInstance_->Keyboard(_chr, _x, _y);
}

void GLGame::KeyboardUpWrapper(const unsigned char _chr, const int _x, const int _y)
{
	callbackInstance_->KeyboardUp(_chr, _x, _y);
}

void GLGame::Display()
{
	KeyboardUpdateState();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawEmitter_->Signal()();

	// RENDER
	glFlush();
	glutSwapBuffers();
}

void GLGame::Reshape(const int _w, const int _h) const
{
	//========================= DEFINE VIEWPORT ==============================*/
	glViewport(0, 0, _w, _h);
	/*========================= ORTHO PROJECTION =============================*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (_w <= _h)
		glOrtho(-10.0, 10.0, -10.0*((GLfloat)_h / (GLfloat)_w),
			10.0*((GLfloat)_h / (GLfloat)_w), 10, -10);
	else
		glOrtho(-10.0*((GLfloat)_w / (GLfloat)_h),
			10.0*((GLfloat)_w / (GLfloat)_h), -10.0, 10.0, 10, -10);
	/*========================= REDISPLAY ====================================*/
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void GLGame::Keyboard(const unsigned char _chr, const int _x, const int _y)
{
	keysPressed_[_chr] = true;
}

void GLGame::KeyboardUp(const unsigned char _chr, const int _x, const int _y)
{
	keysPressed_[_chr] = false;
}

void GLGame::KeyboardUpdateState()
{
	ptree tree;
	for (int i = 0; i < NUMBER_KEYS; i++)
	{
		if (keysPressed_[i]) 
		{
			switch (i)
			{
			case 's':
				leftArrowEmitter_->Signal()(); break;
			case 'f':
				rightArrowEmitter_->Signal()(); break;
			case 'e':
				thrustEmitter_->Signal()(); break;
			case 'j':
				fireEmitter_->Signal()(); break;
			case 'x':
				resetEmitter_->Signal()(); break;
			case 'u':
				Serializer->SetSerializationPath(SERIALIZATION_PATH.string());
				Serializer->Serialize(game_);
				break;
			case 'i':
				Deserializer->UnregisterAll();
				Deserializer->LoadSerializationStructure(SERIALIZATION_PATH.string());
				RegisterEntities(Deserializer->GetSerializationStructure());

				clearEmitter_->Signal()();
				Deserializer->Deserialize(game_);
				break;
			default:
				break;
			}
		}
	}
}

std::shared_ptr<EventEmitter<void(void)>> GLGame::GetLeftArrowEmitter()
{
	return leftArrowEmitter_;
}

std::shared_ptr<EventEmitter<void(void)>> GLGame::GetRightArrowEmitter()
{
	return rightArrowEmitter_;
}

std::shared_ptr<EventEmitter<void(void)>> GLGame::GetThrustEmitter()
{
	return thrustEmitter_;
}

std::shared_ptr<EventEmitter<void(void)>> GLGame::GetFireEmitter()
{
	return fireEmitter_;
}

std::shared_ptr<EventEmitter<void(void)>> GLGame::GetResetEmitter()
{
	return resetEmitter_;
}

std::shared_ptr<EventEmitter<void(void)>> GLGame::GetDrawEmitter()
{
	return drawEmitter_;
}

std::shared_ptr<EventEmitter<void(void)>> GLGame::GetClearEmitter()
{
	return clearEmitter_;
}

Asteroids& GLGame::GetAsteroids()
{
	return game_;
}
