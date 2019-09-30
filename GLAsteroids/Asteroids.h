#ifndef asteroids_asteroids_h
#define	asteroids_asteroids_h

#include <memory>
#include <string>
#include <vector>

#include "config.h"
#include "Rock.h"
#include "Ship.h"
#include "GLEntity.h"

namespace asteroids {

class Asteroids : public GLEntity
{
public:
	Asteroids();
	~Asteroids();
	Asteroids(const Asteroids&);
	Asteroids(Asteroids&&);
	Asteroids& operator=(const Asteroids&);
	Asteroids& operator=(Asteroids&&);

	void Draw();

	// commands
	void Fire();
	void RotateLeft();
	void RotateRight();
	void Thrust();
	void ResetGame();

private:
	SharedEntity& GetShip();

	SharedEntity& GetRock(const std::string& key);
	std::vector<Key> GetRockKeys() const;
	void ClearRocks();
	bool HasRocks();
	void BreakRock(Rock* rock);
	void DestroyRock(Rock* rock);
	std::shared_ptr<Rock> MakeRock(const State rockSize, Rock* rock, const bool halfMass, const bool clockwise);

	void CalculateConservationOfMomentum(Bullet* _ithBullet, Rock* rock);
	void DestroyBullet(Bullet* bullet);
	void DestroyGeometry(Bullet* bullet, Rock* rock);

    void InitGame();

	void DrawRockAndShip();
	void DrawGameInfo();
	void DetermineCollisions();
	void ResetThrustAndRotation();

    /*========================= COLLISION DETECTION ==========================*/
    Rock* Collision(Bullet* bullet);
    void ComputeCollision(Bullet* bullet, Rock* rock);
    GLint Bump();

	GLint score_{0};
	GLint rockCount_{0};
	GLfloat orientationAngle_{0.0f};
	GLfloat thrust_{0.0f};
};

} // end asteroids
#endif	// asteroids_asteroids_h

