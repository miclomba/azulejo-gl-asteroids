#ifndef asteroids_glgame_h
#define asteroids_glgame_h

#include <array>

#include "config.h"
#include "Asteroids.h"

namespace asteroids {

class ASTEROIDS_DLL_EXPORT GLGame 
{
public:
	GLGame(int _argc, char* _argv[]);
	~GLGame();
	GLGame(const GLGame&) = delete;
	GLGame(GLGame&&) = delete;
	GLGame& operator=(const GLGame&) = delete;
	GLGame& operator=(GLGame&&) = delete;

	void Run() const;
	static void TimerCallback(int _idx);

	static GLGame* callbackInstance_;

private:
	void RegisterCallbacks() const;
	void InitGlut(int _argc, char* _argv[]) const;
	void InitServer() const;
	void InitClient() const;

	void Display();
	static void DisplayWrapper();

	void Reshape(const int _w, const int _h) const;
	static void ReshapeWrapper(const int _w, const int _h);

	void Keyboard(const unsigned char _chr, const int _x, const int _y);
	static void KeyboardWrapper(unsigned char _chr, int _x, int _y);

	void KeyboardUp(const unsigned char _chr, const int _x, const int _y);
	static void KeyboardUpWrapper(const unsigned char _chr, const int _x, const int _y);

	void KeyboardUpdateState();

	std::array<bool, 256> keysPressed_;
	Asteroids game;
};

} // end asteroids
#endif // asteroids_glgame_h
