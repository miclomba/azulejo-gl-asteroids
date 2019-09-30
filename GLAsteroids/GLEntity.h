#ifndef asteroids_glentity_h 
#define asteroids_glentity_h	

#include <array>
#include <math.h>

#include "config.h"
#include "Entities/Entity.h"

namespace asteroids {

class GLEntity : public entity::Entity
{
public:
	typedef std::array<GLfloat, 4> Row4;
	typedef std::array<GLfloat, 3> Row3;

    GLEntity();
	~GLEntity();
	GLEntity(const GLEntity&);
	GLEntity(GLEntity&&);
	GLEntity& operator=(const GLEntity&);
	GLEntity& operator=(GLEntity&&);

	std::array<Row4,4>& GetFrame();
	std::array<Row4,4>& GetUnitVelocity();
	GLfloat GetSpeed() const;
	GLfloat GetMass() const;
	GLfloat GetVelocityAngle() const;

	void SetFrame(int i, int j, GLfloat val);
	void SetFrame(const std::array<Row4,4>& frame);
	void SetUnitVelocity(int i, int j, GLfloat val);
	void SetUnitVelocity(const std::array<Row4,4>& unitVelocity);
	void SetSpeed(const GLfloat);
	void SetMass(const GLfloat);
	void SetVelocityAngle(const GLfloat);

protected:
	std::array<Row4, 4> S_;
	std::array<Row4, 4> T_;
	std::array<Row4, 4> R_;

private:
	std::array<Row4,4> frame_;
    std::array<Row4,4> unitVelocity_;

    GLfloat velocityAngle_ = 0.0;
    GLfloat speed_ = 0.0;
    GLfloat mass_ = 1.0;
};

} // end asteroids

#endif // asteroids_glentity_h	
