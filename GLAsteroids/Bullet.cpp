#include "Bullet.h"

using asteroids::Bullet;

namespace
{
const GLfloat BULLET_SPEED_ = 0.5;
} // end namespace

Bullet::Bullet(const GLfloat _x, const GLfloat _y) 
{
    GetMass() = 0.5f;
    bulletIndices_ = {0,3,2,1,2,3,7,6,0,4,7,3,1,2,6,5,4,5,6,7,0,1,5,4};

    bulletVertices_ = {
		Row3{-0.2f,-0.1f,0.5f}, Row3{0.2f,-0.0f,0.5f},
        Row3{0.2f,0.0f,0.5f}, Row3{-0.2f,0.1f,0.5f},
        Row3{-0.2f,-0.1f,1.0f}, Row3{0.2f,-0.0f,1.0f},
        Row3{0.2f,0.0f,1.0f}, Row3{-0.2f,0.1f,1.0f}
	};

    GetFrame() = {
		Row4{_x,   0.0f,0.0f,0.0f},
		Row4{_y,   0.0f,0.0f,0.0f},
		Row4{0.0f,   0.0f,0.0f,0.0f},
		Row4{1.0f,   0.0f,0.0f,0.0f}
	};

    GetUnitVelocity() = {
		Row4{1.0f,   0.0f,0.0f,0.0f},
        Row4{0.0f,   0.0f,0.0f,0.0f},
        Row4{0.0f,   0.0f,0.0f,0.0f},
        Row4{1.0f,   0.0f,0.0f,0.0f}
	};
}

Bullet::~Bullet() = default;
Bullet::Bullet(const Bullet&) = default;
Bullet::Bullet(Bullet&&) = default;
Bullet& Bullet::operator=(const Bullet&) = default;
Bullet& Bullet::operator=(Bullet&&) = default;

void Bullet::InitializeBullet(const GLfloat _velocityAngle, const GLfloat _speed)
{
	GetSpeed() = _speed + BULLET_SPEED_;
	GetVelocityAngle() = _velocityAngle;
	GetUnitVelocity()[0][0] = cos(_velocityAngle);
	GetUnitVelocity()[1][0] = sin(_velocityAngle);
}

void Bullet::SetSMatrix()
{
	S_ = {
		Row4{GetSpeed(),0.0f,0.0f,0.0f},
		Row4{0.0f,GetSpeed(),0.0f,0.0f},
		Row4{0.0f,0.0f,GetSpeed(),0.0f},
		Row4{0.0f,0.0f,0.0f,1.0f}
	};
}

void Bullet::SetTMatrix()
{
	T_ = {
		Row4{1.0f,0.0f,0.0f,GetFrame()[0][0]},
		Row4{0.0f,1.0f,0.0f,GetFrame()[1][0]},
		Row4{0.0f,0.0f,1.0f,GetFrame()[2][0]},
		Row4{0.0f,0.0f,0.0f,1.0f}
	};
}

void Bullet::SetBulletOutOfBounds()
{
	GLfloat epsilon = 3.0f;

	GLfloat right = (1 / fabs(projectionMatrix_[0]));
	GLfloat left = -1 * right;
	GLfloat top = (1 / fabs(projectionMatrix_[5]));
	GLfloat bottom = -1 * top;

	if ((GetFrame()[0][0] <= left - epsilon) || (GetFrame()[0][0] >= right + epsilon) ||
		(GetFrame()[1][0] >= top + epsilon) || (GetFrame()[1][0] <= bottom - epsilon))
	{
		outOfBounds_ = true;
	}
}

void Bullet::Draw(const GLfloat _velocityAngle, const GLfloat _speed) 
{
	if (!bulletInitialized_)
		InitializeBullet(_velocityAngle, _speed);
	bulletInitialized_ = true;

	auto MoveBullet = [this]() 
	{
		// Move the bullet ( p = av + frame )
		SetSMatrix();
		SetTMatrix();

		glLoadMatrixf(GetUnitVelocity().data()->data());
		glMultMatrixf(S_.data()->data());
		glMultMatrixf(T_.data()->data());
		glGetFloatv(GL_MODELVIEW_MATRIX, GetFrame().data()->data());
	};

	auto DrawBullet = [this]()
	{
		glVertexPointer(3, GL_FLOAT, 0, bulletVertices_.data());
		glColor3f(0.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(GetFrame()[0][0], GetFrame()[1][0], GetFrame()[2][0]);
		glRotatef(GetVelocityAngle()*(180.0f / M_PI), 0.0f, 0.0f, 1.0f);
		glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, bulletIndices_.data());
	};

	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix_.data());

    glPushMatrix();
	{
		MoveBullet();
		SetBulletOutOfBounds();
		DrawBullet();
	}
    glPopMatrix();
}

bool Bullet::IsOutOfBounds() const
{
	return outOfBounds_;
}

