#ifndef asteroids_config_h

#if defined (__APPLE__) || defined (MACOSX)
#include <GL/freeglut.h>
#else
#include <GL/freeglut.h>
#endif

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define BOOST_ALL_DYN_LINK 1

#if defined(_WIN64) || defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define ASTEROIDS_DLL_EXPORT __declspec(dllexport)
#elif defined(__APPLE__) || defined(__MACH__)
#define ASTEROIDS_DLL_EXPORT __attribute__((visibility ("default")))
#elif defined(__linux__)
#define ASTEROIDS_DLL_EXPORT __attribute__((visibility ("default")))
#elif defined(__FreeBSD__)
#define ASTEROIDS_DLL_EXPORT __attribute__((visibility ("default")))
#elif defined(__ANDROID__)
#define ASTEROIDS_DLL_EXPORT __attribute__((visibility ("default")))
#endif

#ifdef _WIN32
#define USERS_PATH "c:/users"
#else
#define USERS_PATH "/users"
#endif

#define SAVE_TO_DB

#endif // end config_h
