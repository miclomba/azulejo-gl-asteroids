#include <algorithm>
#include <array>
#include <numeric>
#include <string>

#include <boost/property_tree/ptree.hpp>

#include "FilesystemAdapters/ISerializableResource.h"
#include "FilesystemAdapters/ResourceDeserializer.h"
#include "FilesystemAdapters/ResourceSerializer.h"

#include "GLEntity.h"
#include "test_filesystem_adapters/ContainerResource2D.h"

using boost::property_tree::ptree;
using asteroids::GLEntity;
using entity::Entity;
using filesystem_adapters::ISerializableResource;
using filesystem_adapters::ResourceDeserializer;
using filesystem_adapters::ResourceSerializer;

using Resource2DGLfloat = ContainerResource2D<GLfloat>;

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

auto RES2D_GLFLOAT_CONSTRUCTOR = []()->std::unique_ptr<ISerializableResource> { return std::make_unique<Resource2DGLfloat>(); };
} // end namespace

GLEntity::GLEntity() 
{
	frame_ = Resource2DGLfloat({
		{0.0,0.0,0.0,0.0},
		{0.0,0.0,0.0,0.0},
		{0.0,0.0,0.0,0.0},
		{0.0,0.0,0.0,0.0}
	});

	unitVelocity_ = Resource2DGLfloat({
		{0.0,0.0,0.0,0.0},
		{0.0,0.0,0.0,0.0},
		{0.0,0.0,0.0,0.0},
		{0.0,0.0,0.0,0.0}
	});

	ResourceDeserializer* deserializer = ResourceDeserializer::GetInstance();

	if (!deserializer->HasSerializationKey(UNIT_VELOCITY_KEY))
		deserializer->RegisterResource<GLfloat>(UNIT_VELOCITY_KEY, RES2D_GLFLOAT_CONSTRUCTOR);
	if (!deserializer->HasSerializationKey(FRAME_KEY))
		deserializer->RegisterResource<GLfloat>(FRAME_KEY, RES2D_GLFLOAT_CONSTRUCTOR);
	if (!deserializer->HasSerializationKey(S_KEY))
		deserializer->RegisterResource<GLfloat>(S_KEY, RES2D_GLFLOAT_CONSTRUCTOR);
	if (!deserializer->HasSerializationKey(R_KEY))
		deserializer->RegisterResource<GLfloat>(R_KEY, RES2D_GLFLOAT_CONSTRUCTOR);
	if (!deserializer->HasSerializationKey(T_KEY))
		deserializer->RegisterResource<GLfloat>(T_KEY, RES2D_GLFLOAT_CONSTRUCTOR);
}

GLEntity::~GLEntity() = default;
GLEntity::GLEntity(const GLEntity&) = default;
GLEntity::GLEntity(GLEntity&&) = default;
GLEntity& GLEntity::operator=(const GLEntity&) = default;
GLEntity& GLEntity::operator=(GLEntity&&) = default;

Resource2DGLfloat& GLEntity::GetFrame()
{
	return frame_;
}

Resource2DGLfloat& GLEntity::GetUnitVelocity()
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
	frame_.GetData(i,j) = val;
}

void GLEntity::SetFrame(const Resource2DGLfloat& frame)
{
	frame_ = frame;
}

void GLEntity::SetUnitVelocity(const int i, const int j, const GLfloat val)
{
	unitVelocity_.GetData(i,j) = val;
}

void GLEntity::SetUnitVelocity(const Resource2DGLfloat& unitVelocity)
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

Entity::SharedEntity& GLEntity::GetAggregatedMember(const Entity::Key& key) const
{
	return Entity::GetAggregatedMember(key);
};

void GLEntity::Save(ptree& tree, const std::string& path) const 
{
	tree.put(VELOCITY_ANGLE_KEY, velocityAngle_);
	tree.put(SPEED_KEY, speed_);
	tree.put(MASS_KEY, mass_);

	ResourceSerializer* serializer = ResourceSerializer::GetInstance();
	serializer->SetSerializationPath(path);

	serializer->Serialize(unitVelocity_, UNIT_VELOCITY_KEY);
	serializer->Serialize(frame_, FRAME_KEY);
	serializer->Serialize(S_, S_KEY);
	serializer->Serialize(R_, R_KEY);
	serializer->Serialize(T_, T_KEY);
}

void GLEntity::Load(ptree& tree, const std::string& path) 
{
	velocityAngle_ = std::stof(tree.get_child(VELOCITY_ANGLE_KEY).data());
	speed_ = std::stof(tree.get_child(SPEED_KEY).data());
	mass_ = std::stof(tree.get_child(MASS_KEY).data());

	ResourceDeserializer* deserializer = ResourceDeserializer::GetInstance();
	deserializer->SetSerializationPath(path);

	std::unique_ptr<ISerializableResource> deserializedUnitVelocity = deserializer->Deserialize(UNIT_VELOCITY_KEY);
	unitVelocity_ = *static_cast<Resource2DGLfloat*>(deserializedUnitVelocity.get());
	std::unique_ptr<ISerializableResource> deserializedFrame = deserializer->Deserialize(FRAME_KEY);
	frame_ = *static_cast<Resource2DGLfloat*>(deserializedFrame.get());
	std::unique_ptr<ISerializableResource> deserializedS = deserializer->Deserialize(S_KEY);
	S_ = *static_cast<Resource2DGLfloat*>(deserializedS.get());
	std::unique_ptr<ISerializableResource> deserializedR = deserializer->Deserialize(R_KEY);
	R_ = *static_cast<Resource2DGLfloat*>(deserializedR.get());
	std::unique_ptr<ISerializableResource> deserializedT = deserializer->Deserialize(T_KEY);
	T_ = *static_cast<Resource2DGLfloat*>(deserializedT.get());
}

void GLEntity::Save(database_adapters::Sqlite& database) const
{

}

void GLEntity::Load(database_adapters::Sqlite& database)
{

}

