#include "game/Ship.h"

#include <cmath>
#include <future>
#include <map>
#include <memory>
#include <numbers>
#include <string>
#include <string_view>
#include <vector>

#include <boost/asio/post.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "DatabaseAdapters/EntityDetabularizer.h"
#include "DatabaseAdapters/ITabularizableResource.h"
#include "DatabaseAdapters/ResourceDetabularizer.h"
#include "DatabaseAdapters/ResourceTabularizer.h"
#include "DatabaseAdapters/Sqlite.h"
#include "FilesystemAdapters/EntityDeserializer.h"
#include "FilesystemAdapters/ISerializableResource.h"
#include "FilesystemAdapters/ResourceDeserializer.h"
#include "FilesystemAdapters/ResourceSerializer.h"
#include "test_filesystem_adapters/ContainerResource.h"
#include "test_filesystem_adapters/ContainerResource2D.h"

#include "configuration/filesystem.hpp"
#include "configuration/serialization.h"
#include "game/Bullet.h"
#include "gl/GLEntityTask.h"

using asteroids::Bullet;
using asteroids::GLEntity;
using asteroids::GLEntityTask;
using asteroids::Ship;
using boost::property_tree::ptree;
using database_adapters::EntityDetabularizer;
using database_adapters::ITabularizableResource;
using database_adapters::ResourceDetabularizer;
using database_adapters::ResourceTabularizer;
using database_adapters::Sqlite;
using filesystem_adapters::EntityDeserializer;
using filesystem_adapters::ISerializableResource;
using filesystem_adapters::ResourceDeserializer;
using filesystem_adapters::ResourceSerializer;

using ResourceGLubyte = ContainerResource<GLubyte>;
using Resource2DGLfloat = ContainerResource2D<GLfloat>;

namespace
{
const double PI = std::numbers::pi;
const std::string SHIP_VERTICES_KEY = "ship_vertices";
const std::string SHIP_INDICES_KEY = "ship_indices";
const std::string UNIT_ORIENTATION_KEY = "unit_orientation";
const std::string ORIENTATION_ANGLE_KEY = "orientation_angle";
const std::string BULLET_FIRED_KEY = "bullet_fired";
const std::string TRUE_VAL = "true";

GLfloat PROJECTION_BUFFER[16];
const GLint BULLET_COUNT = 5;

EntityDeserializer *const Deserializer = EntityDeserializer::GetInstance();
EntityDetabularizer *const Detabularizer = EntityDetabularizer::GetInstance();

auto RES_GLUBYTE_CONSTRUCTOR_S = []() -> std::unique_ptr<ISerializableResource>
{ return std::make_unique<ResourceGLubyte>(); };
auto RES_GLUBYTE_CONSTRUCTOR_T = []() -> std::unique_ptr<ITabularizableResource>
{ return std::make_unique<ResourceGLubyte>(); };
auto RES2D_GLFLOAT_CONSTRUCTOR_S = []() -> std::unique_ptr<ISerializableResource>
{ return std::make_unique<Resource2DGLfloat>(); };
auto RES2D_GLFLOAT_CONSTRUCTOR_T = []() -> std::unique_ptr<ITabularizableResource>
{ return std::make_unique<Resource2DGLfloat>(); };
} // end namespace

std::string Ship::ShipKey()
{
	return "Ship";
}

Ship::Ship() : GLEntity(Resource2DGLfloat({{0.0f, 0.0f, 0.0f, 0.0f},
										   {0.0f, 0.0f, 0.0f, 0.0f},
										   {0.0f, 0.0f, 0.0f, 0.0f},
										   {0.0f, 0.0f, 0.0f, 0.0f}}),
						Resource2DGLfloat({{0.0f, 0.0f, 0.0f, 0.0f},
										   {1.0f, 0.0f, 0.0f, 0.0f},
										   {0.0f, 0.0f, 0.0f, 0.0f},
										   {1.0f, 0.0f, 0.0f, 0.0f}})),
			   shipVertices_(Resource2DGLfloat({{-0.5f, -0.5f, 0.5f},
												{0.5f, -0.0f, 0.5f},
												{0.5f, 0.0f, 0.5f},
												{-0.5f, 0.5f, 0.5f},
												{-0.5f, -0.5f, 1.0f},
												{0.1f, -0.0f, 1.0f},
												{0.1f, 0.0f, 1.0f},
												{-0.5f, 0.5f, 1.0f}})),
			   shipIndices_(ResourceGLubyte({0, 3, 2, 1, 2, 3, 7, 6, 0, 4, 7, 3, 1, 2, 6, 5, 4, 5, 6, 7, 0, 1, 5, 4})),
			   unitOrientation_(Resource2DGLfloat({{0.0f, 0.0f, 0.0f, 0.0f},
												   {1.0f, 0.0f, 0.0f, 0.0f},
												   {0.0f, 0.0f, 0.0f, 0.0f},
												   {1.0f, 0.0f, 0.0f, 0.0f}}))
{
	SetVelocityAngle(PI / 2);
}

void Ship::RegisterSerializationResources(const std::string_view resourceKey)
{
	GLEntity::RegisterSerializationResources(resourceKey);

	ResourceDeserializer * const deserializer = ResourceDeserializer::GetInstance();
	if (!deserializer->HasSerializationKey(SHIP_VERTICES_KEY))
		deserializer->RegisterResource<GLfloat>(SHIP_VERTICES_KEY, RES2D_GLFLOAT_CONSTRUCTOR_S);
	if (!deserializer->HasSerializationKey(SHIP_INDICES_KEY))
		deserializer->RegisterResource<GLubyte>(SHIP_INDICES_KEY, RES_GLUBYTE_CONSTRUCTOR_S);
	if (!deserializer->HasSerializationKey(UNIT_ORIENTATION_KEY))
		deserializer->RegisterResource<GLfloat>(UNIT_ORIENTATION_KEY, RES2D_GLFLOAT_CONSTRUCTOR_S);
}

void Ship::RegisterTabularizationResources(const std::string_view resourceKey)
{
	GLEntity::RegisterTabularizationResources(resourceKey);

	const std::string key{resourceKey};
	ResourceDetabularizer * const detabularizer = ResourceDetabularizer::GetInstance();
	if (!detabularizer->HasTabularizationKey(FormatKey(key + SHIP_VERTICES_KEY)))
		detabularizer->RegisterResource<GLfloat>(FormatKey(key + SHIP_VERTICES_KEY), RES2D_GLFLOAT_CONSTRUCTOR_T);
	if (!detabularizer->HasTabularizationKey(FormatKey(key + SHIP_INDICES_KEY)))
		detabularizer->RegisterResource<GLubyte>(FormatKey(key + SHIP_INDICES_KEY), RES_GLUBYTE_CONSTRUCTOR_T);
	if (!detabularizer->HasTabularizationKey(FormatKey(key + UNIT_ORIENTATION_KEY)))
		detabularizer->RegisterResource<GLfloat>(FormatKey(key + UNIT_ORIENTATION_KEY), RES2D_GLFLOAT_CONSTRUCTOR_T);
}

Ship::Ship(const std::string_view key) : Ship()
{
	SetKey(std::string(key));
#ifndef SAVE_TO_DB
	Ship::RegisterSerializationResources(GetKey());
#else
	Ship::RegisterTabularizationResources(GetKey());
#endif
}

Ship::~Ship() = default;
Ship::Ship(const Ship &) = default;
Ship::Ship(Ship &&) = default;
Ship &Ship::operator=(const Ship &) = default;
Ship &Ship::operator=(Ship &&) = default;

std::string Ship::GenerateUUID() const
{
	const boost::uuids::uuid tag = boost::uuids::random_generator()();
	return boost::lexical_cast<std::string>(tag);
}

Ship::SharedEntity &Ship::GetBullet(const std::string_view key) const
{
	return ISerializableEntity::GetAggregatedMember(std::string(key));
}

std::vector<Ship::Key> Ship::GetBulletKeys() const
{
	return GetAggregatedMemberKeys();
}

const std::set<Ship::Key, std::less<>> &Ship::GetOutOfScopeBulletKeys() const
{
	return outOfScopeBulletKeys_;
}

void Ship::ClearOutOfScopeBulletKeys()
{
	outOfScopeBulletKeys_.clear();
}

void Ship::RecomputeShipVelocity(const GLfloat _thrust)
{
	/*====================== RECOMPUTE SHIP VELOCITY =========================*/
	/*================= F(dt) = m(dv) where _thrust = F(dt) ==================*/
	/*========================================================================*/
	if (fabs(_thrust - 0.0f) <= 0.00001f)
		return;

	Resource2DGLfloat& unitVel = GetUnitVelocity();
	const GLfloat mass = GetMass();
	GLfloat speed = GetSpeed();
	GLfloat velAngle = GetVelocityAngle();

	const GLfloat xComponentOrientation = _thrust * cos(orientationAngle_) / mass;
	const GLfloat yComponentOrientation = _thrust * sin(orientationAngle_) / mass;
	GLfloat xComponentVelocity = speed * cos(velAngle);
	GLfloat yComponentVelocity = speed * sin(velAngle);

	xComponentVelocity += xComponentOrientation;
	yComponentVelocity += yComponentOrientation;

	SetSpeed(std::hypot(xComponentVelocity, yComponentVelocity));
	speed = GetSpeed();

	unitVel.GetData(0, 0) = xComponentVelocity / speed;
	unitVel.GetData(1, 0) = yComponentVelocity / speed;

	if (speed > 5.0f)
		SetSpeed(5.0f);

	SetVelocityAngle(static_cast<GLfloat>(atan(unitVel.GetData(1, 0) / unitVel.GetData(0, 0))));
	velAngle = GetVelocityAngle();

	if (unitVel.GetData(0, 0) < 0)
		SetVelocityAngle(velAngle + PI);
	else if (unitVel.GetData(1, 0) < 0)
		SetVelocityAngle(velAngle + 2 * PI);
}

void Ship::ChangeShipOrientation()
{
	glLoadMatrixf(static_cast<GLfloat *>(unitOrientation_.Data()));
	glMultMatrixf(static_cast<GLfloat *>(R_.Data()));
	glGetFloatv(GL_MODELVIEW_MATRIX, static_cast<GLfloat *>(unitOrientation_.Data()));

	orientationAngle_ = static_cast<GLfloat>(atan(unitOrientation_.GetData(1, 0) /
									   unitOrientation_.GetData(0, 0)));
	if (unitOrientation_.GetData(0, 0) < 0)
		orientationAngle_ += PI;
	else if (unitOrientation_.GetData(1, 0) < 0)
		orientationAngle_ += 2 * PI;
}

void Ship::MoveShip()
{
	/*======================= p = av + frame =============================*/
	glLoadMatrixf(static_cast<GLfloat *>(GetUnitVelocity().Data()));
	glMultMatrixf(static_cast<GLfloat *>(S_.Data()));
	glMultMatrixf(static_cast<GLfloat *>(T_.Data()));
	glGetFloatv(GL_MODELVIEW_MATRIX, static_cast<GLfloat *>(GetFrame().Data()));
}

void Ship::WrapAroundMoveShip()
{
	glGetFloatv(GL_PROJECTION_MATRIX, PROJECTION_BUFFER);

	const GLfloat epsilon = 0.5f;

	const GLfloat right = (1 / fabs(PROJECTION_BUFFER[0]));
	const GLfloat left = -1 * right;
	const GLfloat top = (1 / fabs(PROJECTION_BUFFER[5]));
	const GLfloat bottom = -1 * top;

	Resource2DGLfloat& frame = GetFrame();
	if (frame.GetData(0, 0) <= left - epsilon)
	{
		SetFrame(0, 0, right + epsilon);
		SetFrame(1, 0, frame.GetData(1, 0) * -1);
	}
	else if (frame.GetData(0, 0) >= right + epsilon)
	{
		SetFrame(0, 0, left - epsilon);
		SetFrame(1, 0, frame.GetData(1, 0) * -1);
	}
	else if (frame.GetData(1, 0) >= top + epsilon)
	{
		SetFrame(1, 0, bottom - epsilon);
		SetFrame(0, 0, frame.GetData(0, 0) * -1);
	}
	else if (frame.GetData(1, 0) <= bottom - epsilon)
	{
		SetFrame(1, 0, top + epsilon);
		SetFrame(0, 0, frame.GetData(0, 0) * -1);
	}
}

void Ship::UpdateBulletTask(GLEntity *sharedBullet)
{
	Bullet * const bullet = dynamic_cast<Bullet *>(sharedBullet);
	bullet->Update(orientationAngle_, GetSpeed());
}

void Ship::UpdateBullets(
	const std::set<std::string, std::less<>> &serializedKeys,
	boost::asio::thread_pool &threadPool,
	std::vector<std::future<GLEntity *>> &futures)
{
	if (bulletFired_ == false)
		return;

	bulletFired_ = false;

	std::map<Key, SharedEntity> &bullets = GetAggregatedMembers();

	std::vector<std::string> bulletsToRemove;
	for (auto iter = bullets.begin(); iter != bullets.end(); ++iter)
	{
		auto& [entityKey, sharedEntity] = *iter;
		Bullet * const bullet = dynamic_cast<Bullet *>(sharedEntity.get());
		if (bullet && bullet->IsOutOfBounds())
		{
			bulletsToRemove.push_back(bullet->GetKey());
		}
		else if (bullet)
		{
			GLEntityTask task([this, bullet]()
							  { UpdateBulletTask(bullet); return bullet; });
			futures.push_back(task.GetFuture());
			boost::asio::post(threadPool, task);

			bulletFired_ = true;
		}
	}

	for (const std::string &bulletKey : bulletsToRemove)
		RemoveBullet(bulletKey, serializedKeys);
}

void Ship::Draw()
{
	glPushMatrix();

	if (doRotate_)
		ChangeShipOrientation();
	doRotate_ = false;

	MoveShip();
	WrapAroundMoveShip();

	glVertexPointer(3, GL_FLOAT, 0, shipVertices_.Data());
	glColor3f(0.0f, 1.0f, 0.0f);
	glLoadIdentity();
	Resource2DGLfloat& frame = GetFrame();
	glTranslatef(frame.GetData(0, 0), frame.GetData(1, 0), frame.GetData(2, 0));
	glRotatef(orientationAngle_ * (180.0f / PI), 0.0f, 0.0f, 1.0f);
	glDrawElements(GL_LINE_LOOP, 24, GL_UNSIGNED_BYTE, shipIndices_.Data());

	glPopMatrix();
}

void Ship::Update(
	const GLfloat _orientationAngle,
	const GLfloat _thrust,
	const std::set<std::string, std::less<>> &serializedKeys,
	boost::asio::thread_pool &threadPool,
	std::vector<std::future<GLEntity *>> &futures)
{
	RecomputeShipVelocity(_thrust);

	const GLfloat speed = GetSpeed();
	S_ = Resource2DGLfloat({{speed, 0.0f, 0.0f, 0.0f},
							{0.0f, speed, 0.0f, 0.0f},
							{0.0f, 0.0f, speed, 0.0f},
							{0.0f, 0.0f, 0.0f, 1.0f}});

	Resource2DGLfloat& frame = GetFrame();
	T_ = Resource2DGLfloat({{1.0f, 0.0f, 0.0f, frame.GetData(0, 0)},
							{0.0f, 1.0f, 0.0f, frame.GetData(1, 0)},
							{0.0f, 0.0f, 1.0f, frame.GetData(2, 0)},
							{0.0f, 0.0f, 0.0f, 1.0f}});

	if (!(fabs(_orientationAngle - 0.0f) <= 0.00001f))
	{
		R_ = Resource2DGLfloat({{cos(_orientationAngle), -sin(_orientationAngle), 0.0f, 0.0f},
								{sin(_orientationAngle), cos(_orientationAngle), 0.0f, 0.0f},
								{0.0f, 0.0f, 1.0f, 0.0f},
								{0.0f, 0.0f, 0.0f, 1.0f}});

		doRotate_ = true;
	}

	UpdateBullets(serializedKeys, threadPool, futures);
}

void Ship::RemoveBullet(const std::string_view key, const std::set<std::string, std::less<>> &serializedKeys)
{
	if (serializedKeys.find(key) != serializedKeys.cend())
		outOfScopeBulletKeys_.insert(std::string(key));
	RemoveMember(std::string(key));
}

void Ship::AddBullet(const SharedEntity &bullet)
{
	AggregateMember(bullet);
	Deserializer->GetRegistry().RegisterEntity<Bullet>(bullet->GetKey());
	Detabularizer->GetRegistry().RegisterEntity<Bullet>(bullet->GetKey());
}

GLint Ship::BulletNumber()
{
	return BULLET_COUNT;
}

void Ship::Fire()
{
	if (std::map<Key, SharedEntity> &bullets = GetAggregatedMembers(); bullets.size() > BulletNumber())
		return;

	const std::string key = Bullet::BulletPrefix() + GenerateUUID();

	Resource2DGLfloat& frame = GetFrame();
	SharedEntity bullet = std::make_shared<Bullet>(frame.GetData(0, 0), frame.GetData(1, 0));
	bullet->SetKey(key);
#ifndef SAVE_TO_DB
	Bullet::RegisterSerializationResources(bullet->GetKey());
#else
	Bullet::RegisterTabularizationResources(bullet->GetKey());
#endif

	bulletFired_ = true;

	AddBullet(bullet);
}

const Resource2DGLfloat &Ship::GetUnitOrientation() const
{
	return unitOrientation_;
}

const Resource2DGLfloat &Ship::GetShipVertices() const
{
	return shipVertices_;
}

const ResourceGLubyte &Ship::GetShipIndices() const
{
	return shipIndices_;
}

void Ship::Save(ptree &tree, const std::string &path) const
{
	tree.put(BULLET_FIRED_KEY, bulletFired_);
	tree.put(ORIENTATION_ANGLE_KEY, orientationAngle_);

	if (!fs::exists(path))
		fs::create_directories(path);

	ResourceSerializer * const serializer = ResourceSerializer::GetInstance();

	serializer->Serialize(shipVertices_.Lock(), SHIP_VERTICES_KEY, path);
	serializer->Serialize(shipIndices_.Lock(), SHIP_INDICES_KEY, path);
	serializer->Serialize(unitOrientation_.Lock(), UNIT_ORIENTATION_KEY, path);

	GLEntity::Save(tree, path);
}

void Ship::Load(ptree &tree, const std::string &path)
{
	GLEntity::Load(tree, path);

	bulletFired_ = tree.get_child(BULLET_FIRED_KEY).data() == TRUE_VAL ? true : false;
	orientationAngle_ = std::stof(tree.get_child(ORIENTATION_ANGLE_KEY).data());

	ResourceDeserializer * const deserializer = ResourceDeserializer::GetInstance();

	std::unique_ptr<ISerializableResource> deserializedVertices = deserializer->Deserialize(SHIP_VERTICES_KEY, path);
	shipVertices_ = *static_cast<Resource2DGLfloat *>(deserializedVertices.get());
	std::unique_ptr<ISerializableResource> deserializedIndices = deserializer->Deserialize(SHIP_INDICES_KEY, path);
	shipIndices_ = *static_cast<ResourceGLubyte *>(deserializedIndices.get());
	std::unique_ptr<ISerializableResource> deserializedOrientation = deserializer->Deserialize(UNIT_ORIENTATION_KEY, path);
	unitOrientation_ = *static_cast<Resource2DGLfloat *>(deserializedOrientation.get());
}

void Ship::Save(boost::property_tree::ptree &tree, Sqlite &database) const
{
	tree.put(BULLET_FIRED_KEY, bulletFired_);
	tree.put(ORIENTATION_ANGLE_KEY, orientationAngle_);

	ResourceTabularizer *tabularizer = ResourceTabularizer::GetInstance();

	tabularizer->Tabularize(shipVertices_, FormatKey(GetKey() + SHIP_VERTICES_KEY));
	tabularizer->Tabularize(shipIndices_, FormatKey(GetKey() + SHIP_INDICES_KEY));
	tabularizer->Tabularize(unitOrientation_, FormatKey(GetKey() + UNIT_ORIENTATION_KEY));

	GLEntity::Save(tree, database);
}

void Ship::Load(boost::property_tree::ptree &tree, Sqlite &database)
{
	GLEntity::Load(tree, database);

	bulletFired_ = tree.get_child(BULLET_FIRED_KEY).data() == TRUE_VAL ? true : false;
	orientationAngle_ = std::stof(tree.get_child(ORIENTATION_ANGLE_KEY).data());

	ResourceDetabularizer * const detabularizer = ResourceDetabularizer::GetInstance();

	std::unique_ptr<ITabularizableResource> deserializedVertices = detabularizer->Detabularize(FormatKey(GetKey() + SHIP_VERTICES_KEY));
	shipVertices_ = *static_cast<Resource2DGLfloat *>(deserializedVertices.get());
	std::unique_ptr<ITabularizableResource> deserializedIndices = detabularizer->Detabularize(FormatKey(GetKey() + SHIP_INDICES_KEY));
	shipIndices_ = *static_cast<ResourceGLubyte *>(deserializedIndices.get());
	std::unique_ptr<ITabularizableResource> deserializedOrientation = detabularizer->Detabularize(FormatKey(GetKey() + UNIT_ORIENTATION_KEY));
	unitOrientation_ = *static_cast<Resource2DGLfloat *>(deserializedOrientation.get());
}
