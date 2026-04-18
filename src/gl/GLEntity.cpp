#include "gl/GLEntity.h"

#include <algorithm>
#include <array>
#include <numeric>
#include <string>
#include <string_view>

#include <boost/property_tree/ptree.hpp>

#include "DatabaseAdapters/IPersistableResource.h"
#include "DatabaseAdapters/ResourceLoader.h"
#include "DatabaseAdapters/ResourcePersister.h"
#include "FilesystemAdapters/EntityDeserializer.h"
#include "FilesystemAdapters/ISerializableResource.h"
#include "FilesystemAdapters/ResourceDeserializer.h"
#include "FilesystemAdapters/ResourceSerializer.h"
#include "test_filesystem_adapters/ContainerResource2D.h"

#include "configuration/filesystem.hpp"
#include "configuration/serialization.h"

using asteroids::GLEntity;
using boost::property_tree::ptree;
using database_adapters::IPersistableResource;
using database_adapters::ResourceLoader;
using database_adapters::ResourcePersister;
using entity::Entity;
using filesystem_adapters::EntityDeserializer;
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

auto RES2D_GLFLOAT_CONSTRUCTOR_S = []() -> std::unique_ptr<ISerializableResource>
{ return std::make_unique<Resource2DGLfloat>(); };
auto RES2D_GLFLOAT_CONSTRUCTOR_T = []() -> std::unique_ptr<IPersistableResource>
{ return std::make_unique<Resource2DGLfloat>(); };
} // end namespace

GLEntity::GLEntity() : frame_(Resource2DGLfloat({{0.0, 0.0, 0.0, 0.0}, // NOTE: make sure frame_ appears first in the class
												 {0.0, 0.0, 0.0, 0.0},
												 {0.0, 0.0, 0.0, 0.0},
												 {0.0, 0.0, 0.0, 0.0}})),
					   unitVelocity_(frame_),
					   S_(frame_),
					   R_(frame_),
					   T_(frame_)

{
}

GLEntity::GLEntity(
	Resource2DGLfloat frame,
	Resource2DGLfloat unitVelocity) : frame_(std::move(frame)),
									  unitVelocity_(std::move(unitVelocity)),
									  S_(Resource2DGLfloat({{0.0, 0.0, 0.0, 0.0}, // NOTE: make sure S_ appears first in the class
															{0.0, 0.0, 0.0, 0.0},
															{0.0, 0.0, 0.0, 0.0},
															{0.0, 0.0, 0.0, 0.0}})),
									  T_(S_), R_(S_)
{
}

void GLEntity::RegisterSerializationResources(const std::string_view key)
{
	ResourceDeserializer *deserializer = ResourceDeserializer::GetInstance();

	if (!deserializer->HasSerializationKey(UNIT_VELOCITY_KEY))
		deserializer->RegisterResource<GLfloat>(UNIT_VELOCITY_KEY, RES2D_GLFLOAT_CONSTRUCTOR_S);
	if (!deserializer->HasSerializationKey(FRAME_KEY))
		deserializer->RegisterResource<GLfloat>(FRAME_KEY, RES2D_GLFLOAT_CONSTRUCTOR_S);
	if (!deserializer->HasSerializationKey(S_KEY))
		deserializer->RegisterResource<GLfloat>(S_KEY, RES2D_GLFLOAT_CONSTRUCTOR_S);
	if (!deserializer->HasSerializationKey(R_KEY))
		deserializer->RegisterResource<GLfloat>(R_KEY, RES2D_GLFLOAT_CONSTRUCTOR_S);
	if (!deserializer->HasSerializationKey(T_KEY))
		deserializer->RegisterResource<GLfloat>(T_KEY, RES2D_GLFLOAT_CONSTRUCTOR_S);
}

void GLEntity::RegisterPersistenceResources(const std::string_view resourceKey)
{
	ResourceLoader *loader = ResourceLoader::GetInstance();

	const std::string key(resourceKey);
	if (!loader->HasPersistenceKey(FormatKey(key + UNIT_VELOCITY_KEY)))
		loader->RegisterResource<GLfloat>(FormatKey(key + UNIT_VELOCITY_KEY), RES2D_GLFLOAT_CONSTRUCTOR_T);
	if (!loader->HasPersistenceKey(FormatKey(key + FRAME_KEY)))
		loader->RegisterResource<GLfloat>(FormatKey(key + FRAME_KEY), RES2D_GLFLOAT_CONSTRUCTOR_T);
	if (!loader->HasPersistenceKey(FormatKey(key + S_KEY)))
		loader->RegisterResource<GLfloat>(FormatKey(key + S_KEY), RES2D_GLFLOAT_CONSTRUCTOR_T);
	if (!loader->HasPersistenceKey(FormatKey(key + R_KEY)))
		loader->RegisterResource<GLfloat>(FormatKey(key + R_KEY), RES2D_GLFLOAT_CONSTRUCTOR_T);
	if (!loader->HasPersistenceKey(FormatKey(key + T_KEY)))
		loader->RegisterResource<GLfloat>(FormatKey(key + T_KEY), RES2D_GLFLOAT_CONSTRUCTOR_T);
}

GLEntity::~GLEntity() = default;
GLEntity::GLEntity(const GLEntity &) = default;
GLEntity::GLEntity(GLEntity &&) = default;
GLEntity &GLEntity::operator=(const GLEntity &) = default;
GLEntity &GLEntity::operator=(GLEntity &&) = default;

void GLEntity::Draw()
{
}

Resource2DGLfloat &GLEntity::GetFrame()
{
	return frame_;
}

Resource2DGLfloat &GLEntity::GetUnitVelocity()
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

const Resource2DGLfloat &GLEntity::SMatrix() const
{
	return S_;
}

const Resource2DGLfloat &GLEntity::TMatrix() const
{
	return T_;
}

const Resource2DGLfloat &GLEntity::RMatrix() const
{
	return R_;
}

void GLEntity::SetFrame(const int i, const int j, const GLfloat val)
{
	frame_.GetData(i, j) = val;
}

void GLEntity::SetFrame(const Resource2DGLfloat &frame)
{
	frame_ = frame;
}

void GLEntity::SetUnitVelocity(const int i, const int j, const GLfloat val)
{
	unitVelocity_.GetData(i, j) = val;
}

void GLEntity::SetUnitVelocity(const Resource2DGLfloat &unitVelocity)
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

Entity::SharedEntity &GLEntity::GetAggregatedMember(const Entity::Key &key) const
{
	return Entity::GetAggregatedMember(key);
};

void GLEntity::Save(ptree &tree, const std::string &path) const
{
	tree.put(VELOCITY_ANGLE_KEY, velocityAngle_);
	tree.put(SPEED_KEY, speed_);
	tree.put(MASS_KEY, mass_);

	ResourceSerializer *serializer = ResourceSerializer::GetInstance();

	serializer->Serialize(unitVelocity_.Lock(), UNIT_VELOCITY_KEY, path);
	serializer->Serialize(frame_.Lock(), FRAME_KEY, path);
	serializer->Serialize(S_.Lock(), S_KEY, path);
	serializer->Serialize(R_.Lock(), R_KEY, path);
	serializer->Serialize(T_.Lock(), T_KEY, path);
}

void GLEntity::Load(ptree &tree, const std::string &path)
{
	velocityAngle_ = std::stof(tree.get_child(VELOCITY_ANGLE_KEY).data());
	speed_ = std::stof(tree.get_child(SPEED_KEY).data());
	mass_ = std::stof(tree.get_child(MASS_KEY).data());

	ResourceDeserializer *deserializer = ResourceDeserializer::GetInstance();

	std::unique_ptr<ISerializableResource> deserializedUnitVelocity = deserializer->Deserialize(UNIT_VELOCITY_KEY, path);
	unitVelocity_ = *static_cast<Resource2DGLfloat *>(deserializedUnitVelocity.get());
	std::unique_ptr<ISerializableResource> deserializedFrame = deserializer->Deserialize(FRAME_KEY, path);
	frame_ = *static_cast<Resource2DGLfloat *>(deserializedFrame.get());
	std::unique_ptr<ISerializableResource> deserializedS = deserializer->Deserialize(S_KEY, path);
	S_ = *static_cast<Resource2DGLfloat *>(deserializedS.get());
	std::unique_ptr<ISerializableResource> deserializedR = deserializer->Deserialize(R_KEY, path);
	R_ = *static_cast<Resource2DGLfloat *>(deserializedR.get());
	std::unique_ptr<ISerializableResource> deserializedT = deserializer->Deserialize(T_KEY, path);
	T_ = *static_cast<Resource2DGLfloat *>(deserializedT.get());
}

void GLEntity::Save(boost::property_tree::ptree &tree, database_adapters::Sqlite &database) const
{
	tree.put(VELOCITY_ANGLE_KEY, velocityAngle_);
	tree.put(SPEED_KEY, speed_);
	tree.put(MASS_KEY, mass_);

	ResourcePersister *persister = ResourcePersister::GetInstance();

	persister->Persist(unitVelocity_, FormatKey(GetKey() + UNIT_VELOCITY_KEY));
	persister->Persist(frame_, FormatKey(GetKey() + FRAME_KEY));
	persister->Persist(S_, FormatKey(GetKey() + S_KEY));
	persister->Persist(R_, FormatKey(GetKey() + R_KEY));
	persister->Persist(T_, FormatKey(GetKey() + T_KEY));
}

void GLEntity::Load(boost::property_tree::ptree &tree, database_adapters::Sqlite &database)
{
	velocityAngle_ = std::stof(tree.get_child(VELOCITY_ANGLE_KEY).data());
	speed_ = std::stof(tree.get_child(SPEED_KEY).data());
	mass_ = std::stof(tree.get_child(MASS_KEY).data());

	ResourceLoader *loader = ResourceLoader::GetInstance();

	std::unique_ptr<IPersistableResource> deserializedUnitVelocity = loader->Load(FormatKey(GetKey() + UNIT_VELOCITY_KEY));
	unitVelocity_ = *static_cast<Resource2DGLfloat *>(deserializedUnitVelocity.get());
	std::unique_ptr<IPersistableResource> deserializedFrame = loader->Load(FormatKey(GetKey() + FRAME_KEY));
	frame_ = *static_cast<Resource2DGLfloat *>(deserializedFrame.get());
	std::unique_ptr<IPersistableResource> deserializedS = loader->Load(FormatKey(GetKey() + S_KEY));
	S_ = *static_cast<Resource2DGLfloat *>(deserializedS.get());
	std::unique_ptr<IPersistableResource> deserializedR = loader->Load(FormatKey(GetKey() + R_KEY));
	R_ = *static_cast<Resource2DGLfloat *>(deserializedR.get());
	std::unique_ptr<IPersistableResource> deserializedT = loader->Load(FormatKey(GetKey() + T_KEY));
	T_ = *static_cast<Resource2DGLfloat *>(deserializedT.get());
}
