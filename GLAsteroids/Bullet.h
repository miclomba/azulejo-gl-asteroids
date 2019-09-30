#ifndef asteroids_bullet_h
#define	asteroids_bullet_h

#include <array>

#include "config.h"
#include "GLEntity.h"

namespace asteroids {

class Bullet : public GLEntity
{
public:
    Bullet(const GLfloat _x, const GLfloat _y);
	~Bullet();
	Bullet(const Bullet&);
	Bullet(Bullet&&);
	Bullet& operator=(const Bullet&);
	Bullet& operator=(Bullet&&);

    void Draw(const GLfloat _velocityAngle, const GLfloat _speed);

	bool IsOutOfBounds() const;
    
private:
	void InitializeBullet(const GLfloat _velocityAngle, const GLfloat _speed);
	void SetSMatrix();
	void SetTMatrix();
	void SetBulletOutOfBounds();

    bool bulletInitialized_ = false;
    bool outOfBounds_ = false;
	std::array<Row3, 8> bulletVertices_; 
    std::array<GLubyte, 24> bulletIndices_;
	std::array<GLfloat, 16> projectionMatrix_;
};

} // end asteroids
#endif // asteroids_bullet_h	

