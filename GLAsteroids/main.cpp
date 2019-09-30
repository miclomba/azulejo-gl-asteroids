#include <stdlib.h>

#include "GLGame.h"

namespace
{
const int TIME = 25;
const int VAL = 0;
}

int main(int _argc, char* _argv[]) 
{
    asteroids::GLGame game(_argc,_argv);
    glutTimerFunc(TIME,game.TimerCallback,VAL);

    game.Run();
}
