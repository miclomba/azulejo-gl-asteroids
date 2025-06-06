#include <stdlib.h>
#include <string>

#include "Events/EventChannel.h"

#include "game/Asteroids.h"
#include "gl/GLBackend.h"
#include "gl/GLBackendEmitters.h"

using asteroids::Asteroids;
using asteroids::GLBackend;
using asteroids::GLBackendEmitters;
using events::EventChannel;

namespace
{
	const std::string LEFT_EVENT = "left_event";
	const std::string LEFT_ACTION = "left_action";
	const std::string RIGHT_EVENT = "right_event";
	const std::string RIGHT_ACTION = "right_action";
	const std::string THRUST_EVENT = "thrust_event";
	const std::string THRUST_ACTION = "thrust_action";
	const std::string FIRE_EVENT = "fire_event";
	const std::string FIRE_ACTION = "fire_action";
	const std::string RESET_EVENT = "reset_event";
	const std::string RESET_ACTION = "reset_action";
	const std::string DRAW_EVENT = "draw_event";
	const std::string DRAW_ACTION = "draw_action";
	const std::string RUN_EVENT = "run_event";
	const std::string RUN_ACTION = "run_action";
	const std::string SERIALIZE_EVENT = "serialize_event";
	const std::string SERIALIZE_ACTION = "serialize_action";
	const std::string DESERIALIZE_EVENT = "deserialize_event";
	const std::string DESERIALIZE_ACTION = "deserialize_action";
}

void RegisterEvents(Asteroids &frontend, GLBackend &backend, EventChannel &channel)
{
	GLBackendEmitters &emitters = backend.GetEmitters();
	channel.RegisterEmitter(LEFT_EVENT, emitters.GetLeftArrowEmitter());
	channel.RegisterConsumer(LEFT_ACTION, LEFT_EVENT, frontend.GetLeftArrowConsumer());
	channel.RegisterEmitter(RIGHT_EVENT, emitters.GetRightArrowEmitter());
	channel.RegisterConsumer(RIGHT_ACTION, RIGHT_EVENT, frontend.GetRightArrowConsumer());
	channel.RegisterEmitter(THRUST_EVENT, emitters.GetThrustEmitter());
	channel.RegisterConsumer(THRUST_ACTION, THRUST_EVENT, frontend.GetThrustConsumer());
	channel.RegisterEmitter(FIRE_EVENT, emitters.GetFireEmitter());
	channel.RegisterConsumer(FIRE_ACTION, FIRE_EVENT, frontend.GetFireConsumer());
	channel.RegisterEmitter(RESET_EVENT, emitters.GetResetEmitter());
	channel.RegisterConsumer(RESET_ACTION, RESET_EVENT, frontend.GetResetConsumer());
	channel.RegisterEmitter(DRAW_EVENT, emitters.GetDrawEmitter());
	channel.RegisterConsumer(DRAW_ACTION, DRAW_EVENT, frontend.GetDrawConsumer());
	channel.RegisterEmitter(RUN_EVENT, emitters.GetRunEmitter());
	channel.RegisterConsumer(RUN_ACTION, RUN_EVENT, frontend.GetRunConsumer());
	channel.RegisterEmitter(SERIALIZE_EVENT, emitters.GetSerializeEmitter());
	channel.RegisterConsumer(SERIALIZE_ACTION, SERIALIZE_EVENT, frontend.GetSerializeConsumer());
	channel.RegisterEmitter(DESERIALIZE_EVENT, emitters.GetDeserializeEmitter());
	channel.RegisterConsumer(DESERIALIZE_ACTION, DESERIALIZE_EVENT, frontend.GetDeserializeConsumer());
}

int main(int _argc, char *_argv[])
{
	GLBackend backend(_argc, _argv);

	Asteroids frontend;
	EventChannel channel;

	RegisterEvents(frontend, backend, channel);

	backend.Run();
}
