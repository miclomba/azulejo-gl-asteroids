#include <stdlib.h>

#include "asteroids.h"

namespace
{
const int TIME = 25;
const int VAL = 0;
}

int main(int _argc, char* _argv[]) 
{
    asteroids::Asteroids game(_argc,_argv);
    glutTimerFunc(TIME,game.TimerCallback,VAL);

    game.Run();
}
