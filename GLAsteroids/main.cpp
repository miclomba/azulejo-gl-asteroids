#include <stdlib.h>
#include <string>

#include "Events/EventChannel.h"

#include "Asteroids.h"
#include "GLGame.h"

using asteroids::Asteroids;
using asteroids::GLGame;
using events::EventChannel;

namespace
{
const int TIME = 25;
const int VAL = 0;
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
const std::string CLEAR_EVENT = "clear_event";
const std::string CLEAR_ACTION = "clear_action";
}

void RegisterEvents(Asteroids& asteroids, GLGame& game, EventChannel& channel)
{
	channel.RegisterEmitter(LEFT_EVENT, game.GetLeftArrowEmitter());
	channel.RegisterConsumer(LEFT_ACTION, LEFT_EVENT, asteroids.GetLeftArrowConsumer());
	channel.RegisterEmitter(RIGHT_EVENT, game.GetRightArrowEmitter());
	channel.RegisterConsumer(RIGHT_ACTION, RIGHT_EVENT, asteroids.GetRightArrowConsumer());
	channel.RegisterEmitter(THRUST_EVENT, game.GetThrustEmitter());
	channel.RegisterConsumer(THRUST_ACTION, THRUST_EVENT, asteroids.GetThrustConsumer());
	channel.RegisterEmitter(FIRE_EVENT, game.GetFireEmitter());
	channel.RegisterConsumer(FIRE_ACTION, FIRE_EVENT, asteroids.GetFireConsumer());
	channel.RegisterEmitter(RESET_EVENT, game.GetResetEmitter());
	channel.RegisterConsumer(RESET_ACTION, RESET_EVENT, asteroids.GetResetConsumer());
	channel.RegisterEmitter(DRAW_EVENT, game.GetDrawEmitter());
	channel.RegisterConsumer(DRAW_ACTION, DRAW_EVENT, asteroids.GetDrawConsumer());
	channel.RegisterEmitter(CLEAR_EVENT, game.GetClearEmitter());
	channel.RegisterConsumer(CLEAR_ACTION, CLEAR_EVENT, asteroids.GetClearConsumer());
}

int main(int _argc, char* _argv[]) 
{
	GLGame game(_argc,_argv);
    glutTimerFunc(TIME,game.TimerCallback,VAL);

	Asteroids& asteroids = game.GetAsteroids();

	EventChannel channel;

	RegisterEvents(asteroids, game, channel);

    game.Run();
}
