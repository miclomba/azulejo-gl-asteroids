#ifndef asteroids_glgame_h
#define asteroids_glgame_h

#include <array>

#include "config.h"
#include "Asteroids.h"
#include "GLEntity.h"

namespace asteroids {

class GLGame : public GLEntity
{
public:
	GLGame(int _argc, char* _argv[]);
	~GLGame();
	GLGame(const GLGame&) = delete;
	GLGame(GLGame&&) = delete;
	GLGame& operator=(const GLGame&) = delete;
	GLGame& operator=(GLGame&&) = delete;

	void Run();
	static void TimerCallback(int _idx);

	/*======================== GLUT CALLBACK MECHANISM =======================*/
	static GLGame* callbackInstance_;

private:
	void RegisterCallbacks();
	void InitGlut(int _argc, char* _argv[]);
	void InitServer();
	void InitClient();

	void Display();
	static void DisplayWrapper();

	void Reshape(const int _w, const int _h);
	static void ReshapeWrapper(const int _w, const int _h);

	void Keyboard(const unsigned char _chr, const int _x, const int _y);
	static void KeyboardWrapper(unsigned char _chr, int _x, int _y);

	void KeyboardUp(const unsigned char _chr, const int _x, const int _y);
	static void KeyboardUpWrapper(const unsigned char _chr, const int _x, const int _y);

	void KeyboardUpdateState();

	/*========================= COLLISION DETECTION ==========================*/
	std::array<bool, 256> keysPressed_;
	Asteroids game;
};

} // end asteroids
#endif // asteroids_glgame_h
