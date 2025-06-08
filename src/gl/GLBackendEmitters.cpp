#include "gl/GLBackendEmitters.h"

#include <array>
#include <memory>
#include <string>

#include "Events/EventEmitter.h"

using entity::Entity;
using events::EventEmitter;

using asteroids::GLBackendEmitters;
using Emitter = EventEmitter<void(void)>;

namespace
{
	const std::string GL_BACKEND_EMITTERS_KEY = "GLBackendEmittersKey";
} // end namespace

GLBackendEmitters::~GLBackendEmitters() = default;

GLBackendEmitters::GLBackendEmitters() : Entity(),
										 leftArrowEmitter_(std::make_shared<Emitter>()),
										 rightArrowEmitter_(std::make_shared<Emitter>()),
										 thrustEmitter_(std::make_shared<Emitter>()),
										 fireEmitter_(std::make_shared<Emitter>()),
										 resetEmitter_(std::make_shared<Emitter>()),
										 runEmitter_(std::make_shared<Emitter>()),
										 serializeEmitter_(std::make_shared<Emitter>()),
										 deserializeEmitter_(std::make_shared<Emitter>()),
										 drawEmitter_(std::make_shared<Emitter>())
{
	SetKey(GL_BACKEND_EMITTERS_KEY);
}

std::shared_ptr<EventEmitter<void(void)>> GLBackendEmitters::GetLeftArrowEmitter()
{
	return leftArrowEmitter_;
}

std::shared_ptr<Emitter> GLBackendEmitters::GetRightArrowEmitter()
{
	return rightArrowEmitter_;
}

std::shared_ptr<Emitter> GLBackendEmitters::GetThrustEmitter()
{
	return thrustEmitter_;
}

std::shared_ptr<Emitter> GLBackendEmitters::GetFireEmitter()
{
	return fireEmitter_;
}

std::shared_ptr<Emitter> GLBackendEmitters::GetResetEmitter()
{
	return resetEmitter_;
}

std::shared_ptr<Emitter> GLBackendEmitters::GetDrawEmitter()
{
	return drawEmitter_;
}

std::shared_ptr<Emitter> GLBackendEmitters::GetRunEmitter()
{
	return runEmitter_;
}

std::shared_ptr<Emitter> GLBackendEmitters::GetSerializeEmitter()
{
	return serializeEmitter_;
}

std::shared_ptr<Emitter> GLBackendEmitters::GetDeserializeEmitter()
{
	return deserializeEmitter_;
}