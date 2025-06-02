#include "GLBackend.h"

#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include "Events/EventEmitter.h"

using events::EventEmitter;

using asteroids::GLBackend;

namespace
{
	const int WIN_WIDTH = 600;
	const int WIN_HEIGHT = 480;
	const int NUMBER_KEYS = 256;
	const int INIT_WIN_X = 100;
	const int INIT_WIN_Y = 100;
	const std::string ASTEROIDS_TITLE = "Asteroids";
} // end namespace

/*======================== CALLBACK POINTER ==================================*/
GLBackend *GLBackend::callbackInstance_(nullptr);
/*============================================================================*/

GLBackend::~GLBackend() = default;

void GLBackend::TimerCallback(int _idx)
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

GLBackend::GLBackend(int _argc, char *_argv[])
{
	if (!callbackInstance_)
		callbackInstance_ = this;

	std::fill(keysPressed_.begin(), keysPressed_.end(), false);

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
	runEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
	serializeEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
	deserializeEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
}

void GLBackend::Run()
{
	runEmitter_->Signal()();

	glutMainLoop();
};

void GLBackend::RegisterCallbacks() const
{
	glutDisplayFunc(DisplayWrapper);
	glutReshapeFunc(ReshapeWrapper);
	glutKeyboardFunc(KeyboardWrapper);
	glutKeyboardUpFunc(KeyboardUpWrapper);
}

void GLBackend::InitGlut(int _argc, char *_argv[]) const
{
	glutInit(&_argc, _argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(INIT_WIN_X, INIT_WIN_Y);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow(ASTEROIDS_TITLE.c_str());
}

void GLBackend::InitServer() const
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

void GLBackend::InitClient() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
}

void GLBackend::DisplayWrapper()
{
	callbackInstance_->Display();
}

void GLBackend::ReshapeWrapper(const int _w, const int _h)
{
	callbackInstance_->Reshape(_w, _h);
}

void GLBackend::KeyboardWrapper(const unsigned char _chr, const int _x, const int _y)
{
	callbackInstance_->Keyboard(_chr, _x, _y);
}

void GLBackend::KeyboardUpWrapper(const unsigned char _chr, const int _x, const int _y)
{
	callbackInstance_->KeyboardUp(_chr, _x, _y);
}

void GLBackend::Display()
{
	KeyboardUpdateState();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawEmitter_->Signal()();

	// RENDER
	glFlush();
	glutSwapBuffers();
}

void GLBackend::Reshape(const int _w, const int _h) const
{
	//========================= DEFINE VIEWPORT ==============================*/
	glViewport(0, 0, _w, _h);
	/*========================= ORTHO PROJECTION =============================*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (_w <= _h)
		glOrtho(-10.0, 10.0, -10.0 * ((GLfloat)_h / (GLfloat)_w),
				10.0 * ((GLfloat)_h / (GLfloat)_w), 10, -10);
	else
		glOrtho(-10.0 * ((GLfloat)_w / (GLfloat)_h),
				10.0 * ((GLfloat)_w / (GLfloat)_h), -10.0, 10.0, 10, -10);
	/*========================= REDISPLAY ====================================*/
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void GLBackend::Keyboard(const unsigned char _chr, const int _x, const int _y)
{
	keysPressed_[_chr] = true;
}

void GLBackend::KeyboardUp(const unsigned char _chr, const int _x, const int _y)
{
	keysPressed_[_chr] = false;
}

void GLBackend::KeyboardUpdateState()
{
	for (int i = 0; i < NUMBER_KEYS; i++)
	{
		if (keysPressed_[i])
		{
			switch (i)
			{
			case 's':
				leftArrowEmitter_->Signal()();
				break;
			case 'f':
				rightArrowEmitter_->Signal()();
				break;
			case 'e':
				thrustEmitter_->Signal()();
				break;
			case 'j':
				fireEmitter_->Signal()();
				break;
			case 'x':
				resetEmitter_->Signal()();
				break;
			case 'u':
				serializeEmitter_->Signal()();
				break;
			case 'i':
				deserializeEmitter_->Signal()();
				break;
			default:
				break;
			}
		}
	}
}

std::shared_ptr<EventEmitter<void(void)>> GLBackend::GetLeftArrowEmitter()
{
	return leftArrowEmitter_;
}

std::shared_ptr<EventEmitter<void(void)>> GLBackend::GetRightArrowEmitter()
{
	return rightArrowEmitter_;
}

std::shared_ptr<EventEmitter<void(void)>> GLBackend::GetThrustEmitter()
{
	return thrustEmitter_;
}

std::shared_ptr<EventEmitter<void(void)>> GLBackend::GetFireEmitter()
{
	return fireEmitter_;
}

std::shared_ptr<EventEmitter<void(void)>> GLBackend::GetResetEmitter()
{
	return resetEmitter_;
}

std::shared_ptr<EventEmitter<void(void)>> GLBackend::GetDrawEmitter()
{
	return drawEmitter_;
}

std::shared_ptr<EventEmitter<void(void)>> GLBackend::GetRunEmitter()
{
	return runEmitter_;
}

std::shared_ptr<EventEmitter<void(void)>> GLBackend::GetSerializeEmitter()
{
	return serializeEmitter_;
}

std::shared_ptr<EventEmitter<void(void)>> GLBackend::GetDeserializeEmitter()
{
	return deserializeEmitter_;
}
