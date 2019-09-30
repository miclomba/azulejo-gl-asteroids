#include "GLGame.h"

using asteroids::GLGame;

namespace
{
const int WIN_WIDTH = 600;
const int WIN_HEIGHT = 480;
const int NUMBER_KEYS = 256;
const int INIT_WIN_X = 100;
const int INIT_WIN_Y = 100;
const std::string TITLE = "Asteroids";
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
}

void GLGame::Run()
{
	glutMainLoop();
};

void GLGame::RegisterCallbacks()
{
	glutDisplayFunc(DisplayWrapper);   glutReshapeFunc(ReshapeWrapper);
	glutKeyboardFunc(KeyboardWrapper); glutKeyboardUpFunc(KeyboardUpWrapper);
}

void GLGame::InitGlut(int _argc, char* _argv[])
{
	glutInit(&_argc, _argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(INIT_WIN_X, INIT_WIN_Y);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow(TITLE.c_str());
}

void GLGame::InitServer()
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

void GLGame::InitClient()
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

	game.DrawRockAndShip();
	game.ResetThrustAndRotation();
	game.DrawGameInfo();

	// RENDER
	glFlush();
	glutSwapBuffers();

	game.DetermineCollisions();
}

void GLGame::Reshape(const int _w, const int _h)
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
	for (int i = 0; i < NUMBER_KEYS; i++)
	{
		if (keysPressed_[i]) {
			switch (i)
			{
			case 's':
				game.RotateLeft(); break;
			case 'f':
				game.RotateRight(); break;
			case 'e':
				game.Thrust(); break;
			case 'j':
				game.Fire(); break;
			case 'x':
				game.ResetGame(); break;
			case 'X':
				game.ResetGame(); break;
			default:
				break;
			}
		}
	}
}
