#include <algorithm>
#include <array>
#include <numeric>
#include <string>

#include <boost/property_tree/ptree.hpp>

#include "GLEntity.h"
#include "GLSerializer.h"
#include "Resources/Resource.h"
#include "Resources/ResourceDeserializer.h"
#include "Resources/ResourceSerializer.h"

using boost::property_tree::ptree;
using asteroids::GLEntity;
using asteroids::GLSerializer;
using resource::IResource;
using resource::Resource;
using resource::ResourceDeserializer;
using resource::ResourceSerializer;

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
	frame_ = Resource<GLfloat>({
		{0.0,0.0,0.0,0.0},
		{0.0,0.0,0.0,0.0},
		{0.0,0.0,0.0,0.0},
		{0.0,0.0,0.0,0.0}
	});

	unitVelocity_ = Resource<GLfloat>({
		{0.0,0.0,0.0,0.0},
		{0.0,0.0,0.0,0.0},
		{0.0,0.0,0.0,0.0},
		{0.0,0.0,0.0,0.0}
	});

	ResourceDeserializer* deserializer = ResourceDeserializer::GetInstance();

	if (!deserializer->HasSerializationKey(UNIT_VELOCITY_KEY))
		deserializer->RegisterResource<GLfloat>(UNIT_VELOCITY_KEY);
	if (!deserializer->HasSerializationKey(FRAME_KEY))
		deserializer->RegisterResource<GLfloat>(FRAME_KEY);
}

GLEntity::~GLEntity() = default;
GLEntity::GLEntity(const GLEntity&) = default;
GLEntity::GLEntity(GLEntity&&) = default;
GLEntity& GLEntity::operator=(const GLEntity&) = default;
GLEntity& GLEntity::operator=(GLEntity&&) = default;

Resource<GLfloat>& GLEntity::GetFrame()
{
	return frame_;
}

Resource<GLfloat>& GLEntity::GetUnitVelocity()
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
	frame_.Data(i,j) = val;
}

void GLEntity::SetFrame(const Resource<GLfloat>& frame)
{
	frame_ = frame;
}

void GLEntity::SetUnitVelocity(const int i, const int j, const GLfloat val)
{
	unitVelocity_.Data(i,j) = val;
}

void GLEntity::SetUnitVelocity(const Resource<GLfloat>& unitVelocity)
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

void GLEntity::Save(ptree& tree, const std::string& path) const 
{
	ptree sMatrix = GLSerializer::GetSerial4x4Matrix(S_);
	tree.add_child(S_KEY, sMatrix);
	ptree tMatrix = GLSerializer::GetSerial4x4Matrix(T_);
	tree.add_child(T_KEY, tMatrix);
	ptree rMatrix = GLSerializer::GetSerial4x4Matrix(R_);
	tree.add_child(R_KEY, rMatrix);

	tree.put(VELOCITY_ANGLE_KEY, velocityAngle_);
	tree.put(SPEED_KEY, speed_);
	tree.put(MASS_KEY, mass_);

	ResourceSerializer* serializer = ResourceSerializer::GetInstance();
	serializer->SetSerializationPath(path);

	serializer->Serialize(unitVelocity_, UNIT_VELOCITY_KEY);
	serializer->Serialize(frame_, FRAME_KEY);
}

void GLEntity::Load(ptree& tree, const std::string& path) 
{
	S_ = GLSerializer::Get4x4Matrix(tree.get_child(S_KEY));
	T_ = GLSerializer::Get4x4Matrix(tree.get_child(T_KEY));
	R_ = GLSerializer::Get4x4Matrix(tree.get_child(R_KEY));

	velocityAngle_ = std::stof(tree.get_child(VELOCITY_ANGLE_KEY).data());
	speed_ = std::stof(tree.get_child(SPEED_KEY).data());
	mass_ = std::stof(tree.get_child(MASS_KEY).data());

	ResourceDeserializer* deserializer = ResourceDeserializer::GetInstance();
	deserializer->SetSerializationPath(path);

	std::unique_ptr<IResource> deserializedUnitVelocity = deserializer->Deserialize(UNIT_VELOCITY_KEY);
	unitVelocity_ = *static_cast<Resource<GLfloat>*>(deserializedUnitVelocity.get());
	std::unique_ptr<IResource> deserializedFrame = deserializer->Deserialize(FRAME_KEY);
	frame_ = *static_cast<Resource<GLfloat>*>(deserializedFrame.get());
}
