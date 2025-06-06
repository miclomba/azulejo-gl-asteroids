#include "gl/GLBackend.h"

#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include "gl/GL.h"
#include "gl/GLBackendEmitters.h"
#include "gl/GLWindow.h"

using asteroids::GL;
using asteroids::GLBackend;
using asteroids::GLBackendEmitters;
using asteroids::GLWindow;
using events::EventEmitter;

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
GLBackend *GLBackend::callbackInstance_ = nullptr;
/*============================================================================*/

GLBackend::~GLBackend() = default;

GLBackend::GLBackend(int _argc, char *_argv[])
{
	if (!callbackInstance_)
		callbackInstance_ = this;

	std::fill(keysPressed_.begin(), keysPressed_.end(), false);

	gl_ = std::make_unique<GL>(_argc, _argv);
}

void GLBackend::Run()
{
	emitters_.GetRunEmitter()->Signal()();

	gl_->Run();
};

GLBackendEmitters &GLBackend::GetEmitters()
{
	return emitters_;
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

	gl_->DisplayClear();

	GLWindow &gameWindow = gl_->GetGameWindow();
	emitters_.GetDrawEmitter()->Signal()(gameWindow.GetWidth(), gameWindow.GetHeight(), gameWindow.GetProjOrthoMatrix(), gameWindow.GetProjPerspectiveMatrix());

	gl_->DisplayFlush();
}

void GLBackend::Reshape(const int _w, const int _h) const
{
	gl_->Reshape(_w, _h);
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
				emitters_.GetLeftArrowEmitter()->Signal()();
				break;
			case 'f':
				emitters_.GetRightArrowEmitter()->Signal()();
				break;
			case 'e':
				emitters_.GetThrustEmitter()->Signal()();
				break;
			case 'j':
				emitters_.GetFireEmitter()->Signal()();
				break;
			case 'x':
				emitters_.GetResetEmitter()->Signal()();
				break;
			case 'u':
				emitters_.GetSerializeEmitter()->Signal()();
				break;
			case 'i':
				emitters_.GetDeserializeEmitter()->Signal()();
				break;
			default:
				break;
			}
		}
	}
}
