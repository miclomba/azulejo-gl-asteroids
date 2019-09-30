#include <array>
#include <numeric>

#include "GLEntity.h"

using asteroids::GLEntity;

GLEntity::GLEntity() 
{
	for (Row4& row : frame_)
		std::fill(row.begin(), row.end(), 0.0);
	for (Row4& row : unitVelocity_)
		std::fill(row.begin(), row.end(), 0.0);
}

GLEntity::~GLEntity() = default;
GLEntity::GLEntity(const GLEntity&) = default;
GLEntity::GLEntity(GLEntity&&) = default;
GLEntity& GLEntity::operator=(const GLEntity&) = default;
GLEntity& GLEntity::operator=(GLEntity&&) = default;

std::array<GLEntity::Row4, 4>& GLEntity::GetFrame()
{
	return frame_;
}

std::array<GLEntity::Row4, 4>& GLEntity::GetUnitVelocity()
{
	return unitVelocity_;
}

GLfloat GLEntity::GetSpeed() const
{
	return speed_;
}

GLfloat GLEntity::GetMass() const
{
	return mass_;
}

GLfloat GLEntity::GetVelocityAngle() const
{
	return velocityAngle_;
}

void GLEntity::SetFrame(const int i, const int j, const GLfloat val)
{
	frame_[i][j] = val;
}

void GLEntity::SetFrame(const std::array<Row4, 4>& frame)
{
	frame_ = frame;
}

void GLEntity::SetUnitVelocity(const int i, const int j, const GLfloat val)
{
	unitVelocity_[i][j] = val;
}

void GLEntity::SetUnitVelocity(const std::array<Row4, 4>& unitVelocity)
{
	unitVelocity_ = unitVelocity;
}

void GLEntity::SetSpeed(const GLfloat speed)
{
	speed_ = speed;
}

void GLEntity::SetMass(const GLfloat mass)
{
	mass_ = mass;
}

void GLEntity::SetVelocityAngle(const GLfloat velocityAngle)
{
	velocityAngle_ = velocityAngle;
}

