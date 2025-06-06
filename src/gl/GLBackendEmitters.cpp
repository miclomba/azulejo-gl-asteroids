#include "gl/GLBackendEmitters.h"

#include <array>
#include <memory>
#include <string>

#include "Events/EventEmitter.h"

using entity::Entity;
using events::EventEmitter;

using asteroids::GLBackendEmitters;

namespace
{
	const std::string GL_BACKEND_EMITTERS_KEY = "GLBackendEmittersKey";
} // end namespace

GLBackendEmitters::~GLBackendEmitters() = default;

GLBackendEmitters::GLBackendEmitters() : Entity()
{
	SetKey(GL_BACKEND_EMITTERS_KEY);

	leftArrowEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
	rightArrowEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
	thrustEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
	fireEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
	resetEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
	runEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
	serializeEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
	deserializeEmitter_ = std::make_shared<events::EventEmitter<void(void)>>();
	drawEmitter_ = std::make_shared<events::EventEmitter<void()>>();
}

std::shared_ptr<EventEmitter<void(void)>> GLBackendEmitters::GetLeftArrowEmitter()
{
	return leftArrowEmitter_;
}

std::shared_ptr<events::EventEmitter<void(void)>> GLBackendEmitters::GetRightArrowEmitter()
{
	return rightArrowEmitter_;
}

std::shared_ptr<events::EventEmitter<void(void)>> GLBackendEmitters::GetThrustEmitter()
{
	return thrustEmitter_;
}

std::shared_ptr<events::EventEmitter<void(void)>> GLBackendEmitters::GetFireEmitter()
{
	return fireEmitter_;
}

std::shared_ptr<events::EventEmitter<void(void)>> GLBackendEmitters::GetResetEmitter()
{
	return resetEmitter_;
}

std::shared_ptr<EventEmitter<void()>> GLBackendEmitters::GetDrawEmitter()
{
	return drawEmitter_;
}

std::shared_ptr<events::EventEmitter<void(void)>> GLBackendEmitters::GetRunEmitter()
{
	return runEmitter_;
}

std::shared_ptr<events::EventEmitter<void(void)>> GLBackendEmitters::GetSerializeEmitter()
{
	return serializeEmitter_;
}

std::shared_ptr<events::EventEmitter<void(void)>> GLBackendEmitters::GetDeserializeEmitter()
{
	return deserializeEmitter_;
}