#ifndef asteroids_ship_h
#define	asteroids_ship_h

#include <array>
#include <map>

#include "config.h"
#include "Bullet.h"
#include "GLEntity.h"

namespace asteroids {

class Ship : public GLEntity 
{
public:
	Ship(const Ship::Key& key);
	~Ship();
	Ship(const Ship&);
	Ship(Ship&&);
	Ship& operator=(const Ship&);
	Ship& operator=(Ship&&);

	void Draw(const GLfloat _orientationAngle, const GLfloat _thrust);
	void Fire();

	static GLint BulletNumber();

	SharedEntity& GetBullet(const std::string& key);
	std::vector<Key> GetBulletKeys() const;

private:
	void RecomputeShipVelocity(const GLfloat _thrust);
	void ChangeShipOrientation(const GLfloat _orientationAngle);
	void MoveShip();
	void WrapAroundMoveShip();
	void DrawBullets();
	void DrawShip();

	bool bulletFired_{false};

    std::array<Row4,4> unitOrientation_;
	GLfloat orientationAngle_{ static_cast<GLfloat>(M_PI) / 2 };

    std::array<Row3,8> shipVertices_;
    std::array<GLubyte, 24> shipIndices_;
};

} // end asteroids
#endif	// asteroids_ship_h 

