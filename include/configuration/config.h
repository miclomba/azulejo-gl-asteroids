#ifndef asteroids_config_h

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK
#endif

#if defined(__APPLE__) || defined(__MACH__)
#define ASTEROIDS_DLL_EXPORT __attribute__((visibility("default")))
#elif defined(__linux__)
#define ASTEROIDS_DLL_EXPORT __attribute__((visibility("default")))
#elif defined(__FreeBSD__)
#define ASTEROIDS_DLL_EXPORT __attribute__((visibility("default")))
#elif defined(__ANDROID__)
#define ASTEROIDS_DLL_EXPORT __attribute__((visibility("default")))
#endif

#define USERS_PATH "/users"

#define SAVE_TO_DB

#define WIN_WIDTH (600)
#define WIN_HEIGHT (480)
#define INIT_WIN_X (100)
#define INIT_WIN_Y (100)

#endif // end config_h
