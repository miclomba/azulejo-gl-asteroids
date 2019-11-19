#ifndef asteroids_glgame_h
#define asteroids_glgame_h

#include <array>
#include <memory>

#include "Events/EventEmitter.h"

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

	void Run();
	static void TimerCallback(int _idx);

	std::shared_ptr<events::EventEmitter<void(void)>> GetLeftArrowEmitter();
	std::shared_ptr<events::EventEmitter<void(void)>> GetRightArrowEmitter();
	std::shared_ptr<events::EventEmitter<void(void)>> GetThrustEmitter();
	std::shared_ptr<events::EventEmitter<void(void)>> GetFireEmitter();
	std::shared_ptr<events::EventEmitter<void(void)>> GetResetEmitter();
	std::shared_ptr<events::EventEmitter<void(void)>> GetDrawEmitter();
	std::shared_ptr<events::EventEmitter<void(void)>> GetClearEmitter();

	Asteroids& GetAsteroids();

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
	Asteroids game_;

	std::shared_ptr<events::EventEmitter<void(void)>> leftArrowEmitter_;
	std::shared_ptr<events::EventEmitter<void(void)>> rightArrowEmitter_;
	std::shared_ptr<events::EventEmitter<void(void)>> thrustEmitter_;
	std::shared_ptr<events::EventEmitter<void(void)>> fireEmitter_;
	std::shared_ptr<events::EventEmitter<void(void)>> resetEmitter_;
	std::shared_ptr<events::EventEmitter<void(void)>> drawEmitter_;
	std::shared_ptr<events::EventEmitter<void(void)>> clearEmitter_;
};

} // end asteroids
#endif // asteroids_glgame_h
