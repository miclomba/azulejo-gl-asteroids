#ifndef asteroids_config_h

#if defined (__APPLE__) || defined (MACOSX)
#include <GLUT/freeglut.h>
#else
#include <GL/freeglut.h>
#endif

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define BOOST_ALL_DYN_LINK 1

#define ASTEROIDS_DLL_EXPORT __declspec(dllexport)

#ifdef _WIN32
#define USERS_PATH "c:/users"
#else
#define USERS_PATH "/users"
#endif

#define SAVE_TO_DB

#endif // end config_h
