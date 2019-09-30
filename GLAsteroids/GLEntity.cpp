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

GLfloat& GLEntity::GetSpeed()
{
	return speed_;
}

GLfloat& GLEntity::GetMass()
{
	return mass_;
}

GLfloat& GLEntity::GetVelocityAngle()
{
	return velocityAngle_;
}

