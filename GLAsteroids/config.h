#ifndef asteroids_config_h

#if defined (__APPLE__) || defined (MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
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

#endif // end config_h
