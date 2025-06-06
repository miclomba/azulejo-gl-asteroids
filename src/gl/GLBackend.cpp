#include "gl/GLBackend.h"

#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include "Events/EventEmitter.h"
#include "gl/GL.h"

using asteroids::GL;
using asteroids::GLBackend;
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

	gl_->Run();
};

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

	drawEmitter_->Signal()();

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
