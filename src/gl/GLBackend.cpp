#include "gl/GLBackend.h"

#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include "gl/GL.h"
#include "gl/GLBackendEmitters.h"
#include "gl/GLProjectionInfo.h"

using asteroids::GL;
using asteroids::GLBackend;
using asteroids::GLBackendEmitters;
using asteroids::GLProjectionInfo;
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

std::unique_ptr<GLBackend> GLBackend::callbackInstance_ = nullptr;

GLBackend &GLBackend::Get(int _argc, char *_argv[])
{
	if (!GLBackend::callbackInstance_)
	{
		GLBackend::callbackInstance_.reset(new GLBackend(_argc, _argv));
	}
	return *GLBackend::callbackInstance_;
}

GLBackend::~GLBackend() = default;

GLBackend::GLBackend(int _argc, char *_argv[])
{
	std::fill(keysPressed_.begin(), keysPressed_.end(), false);

	// Initialize the graphics library singleton
	GL::Get(_argc, _argv);
}

void GLBackend::Run()
{
	emitters_.GetRunEmitter()->Signal()();

	GL::Get().Run();
};

GLBackendEmitters &GLBackend::GetEmitters()
{
	return emitters_;
}

void GLBackend::DisplayCallback()
{
	callbackInstance_->Display();
}

void GLBackend::ReshapeCallback(const int _w, const int _h)
{
	callbackInstance_->Reshape(_w, _h);
}

void GLBackend::KeyboardCallback(const unsigned char _chr, const int _x, const int _y)
{
	callbackInstance_->Keyboard(_chr, _x, _y);
}

void GLBackend::KeyboardUpCallback(const unsigned char _chr, const int _x, const int _y)
{
	callbackInstance_->KeyboardUp(_chr, _x, _y);
}

void GLBackend::Display()
{
	KeyboardUpdateState();

	GL &gl = GL::Get();
	gl.DisplayClear();

	GLProjectionInfo &gameProjectionInfo = gl.GetGameProjectionInfo();
	emitters_.GetDrawEmitter()->Signal()(gameProjectionInfo.GetWidth(), gameProjectionInfo.GetHeight(), gameProjectionInfo.GetProjOrthoMatrix(), gameProjectionInfo.GetProjPerspectiveMatrix());

	gl.DisplayFlush();
}

void GLBackend::Reshape(const int _w, const int _h) const
{
	GL::Get().Reshape(_w, _h);
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
