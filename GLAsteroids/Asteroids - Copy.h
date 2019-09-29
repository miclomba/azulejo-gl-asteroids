#ifndef asteroids_h
#define	asteroids_h

#include <array>
#include <memory>
#include <vector>

#include "config.h"
#include "Rock.h"
#include "Ship.h"
#include "GLEntity.h"

namespace asteroids {

class Asteroids : public GLEntity
{
public:
	static void TimerCallback(int _idx);

	Asteroids(int _argc, char* _argv[]);
	void Run();

    /*======================== GLUT CALLBACK MECHANISM =======================*/
	static Asteroids* callbackInstance_;

private:
	SharedEntity& GetShip();
	SharedEntity& GetRock(const std::string& key);
	std::vector<Key> GetRockKeys() const;

	void RegisterCallbacks();
	void InitGlut(int _argc, char* _argv[]);
	void InitServer();
	void InitClient();

	void CalculateConservationOfMomentum(Bullet* _ithBullet, const GLint i);
	std::shared_ptr<Rock> MakeRock(const State rockSize, const GLint i, const bool halfMass, const bool clockwise);
	void BreakRock(const GLint i);
	void DestroyBullet(Bullet* bullet);
	void DestroyGeometry(Bullet* bullet, const GLint i);
	void DrawRockAndShip();
	void DrawGameInfo();
	void DetermineCollisions();
    
    void Draw();
	static void DisplayWrapper();

    void Reshape(const int _w, const int _h);
	static void ReshapeWrapper(const int _w, const int _h);
        
    void Keyboard(const unsigned char _chr, const int _x, const int _y);
	static void KeyboardWrapper(unsigned char _chr, int _x, int _y);

    void KeyboardUp(const unsigned char _chr, const int _x, const int _y);
	static void KeyboardUpWrapper(const unsigned char _chr, const int _x, const int _y);

    void KeyboardUpdateState();

    void InitGame();
	Asteroids();

    /*========================= COLLISION DETECTION ==========================*/
    GLint Collision(Bullet* bullet);
    void ComputeCollision(Bullet* bullet, const GLint ithRock);
    GLint Bump();

	std::vector<std::shared_ptr<Rock>> rockVector_;

	GLint score_{0};
	std::array<bool, 256> keysPressed_;

	GLint rockCount_{0};
	GLfloat orientationAngle_{0.0f};
	GLfloat thrust_{0.0f};
};

} // end asteroids
#endif	// asteroids_h

