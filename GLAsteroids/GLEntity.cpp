#include <algorithm>
#include <array>
#include <numeric>
#include <string>

#include <boost/property_tree/ptree.hpp>

#include "GLEntity.h"
#include "GLSerializer.h"

using boost::property_tree::ptree;
using asteroids::GLEntity;
using asteroids::GLSerializer;

namespace
{
const std::string FRAME_KEY = "frame";
const std::string UNIT_VELOCITY_KEY = "unit_velocity";
const std::string S_KEY = "S";
const std::string T_KEY = "T";
const std::string R_KEY = "R";
const std::string VELOCITY_ANGLE_KEY = "velocity_angle";
const std::string SPEED_KEY = "speed";
const std::string MASS_KEY = "mass";
} // end namespace

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

void GLEntity::Save(ptree& tree, const std::string&) const 
{
	ptree frameMatrix = GLSerializer::GetSerial4x4Matrix(frame_);
	tree.add_child(FRAME_KEY, frameMatrix);
	ptree unitVelMatrix = GLSerializer::GetSerial4x4Matrix(unitVelocity_);
	tree.add_child(UNIT_VELOCITY_KEY, unitVelMatrix);
	ptree sMatrix = GLSerializer::GetSerial4x4Matrix(S_);
	tree.add_child(S_KEY, sMatrix);
	ptree tMatrix = GLSerializer::GetSerial4x4Matrix(T_);
	tree.add_child(T_KEY, tMatrix);
	ptree rMatrix = GLSerializer::GetSerial4x4Matrix(R_);
	tree.add_child(R_KEY, rMatrix);

	tree.put(VELOCITY_ANGLE_KEY, velocityAngle_);
	tree.put(SPEED_KEY, speed_);
	tree.put(MASS_KEY, mass_);
}

void GLEntity::Load(ptree& tree, const std::string&) 
{
	frame_ = GLSerializer::Get4x4Matrix(tree.get_child(FRAME_KEY));
	unitVelocity_ = GLSerializer::Get4x4Matrix(tree.get_child(UNIT_VELOCITY_KEY));
	S_ = GLSerializer::Get4x4Matrix(tree.get_child(S_KEY));
	T_ = GLSerializer::Get4x4Matrix(tree.get_child(T_KEY));
	R_ = GLSerializer::Get4x4Matrix(tree.get_child(R_KEY));

	velocityAngle_ = std::stof(tree.get_child(VELOCITY_ANGLE_KEY).data());
	speed_ = std::stof(tree.get_child(SPEED_KEY).data());
	mass_ = std::stof(tree.get_child(MASS_KEY).data());
}
