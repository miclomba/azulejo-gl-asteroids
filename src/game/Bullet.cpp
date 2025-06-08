#include "game/Bullet.h"

#include <memory>
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>

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

using asteroids::Bullet;
using asteroids::GLEntity;
using boost::property_tree::ptree;
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
	const GLfloat BULLET_SPEED_ = 0.5;
	const std::string BULLET_INITIALIZED_KEY = "bullet_initialized";
	const std::string OUT_OF_BOUNDS_KEY = "out_of_bounds";
	const std::string BULLET_VERTICES_KEY = "bullet_vertices";
	const std::string BULLET_INDICES_KEY = "bullet_indices";
	const std::string PROJECTION_MATRIX_KEY = "projection_matrix";
	const std::string TRUE_VAL = "true";

	std::vector<std::vector<GLfloat>> BUFFER({std::vector<GLfloat>(16)});

	auto RES_GLUBYTE_CONSTRUCTOR_S = []() -> std::unique_ptr<ISerializableResource>
	{ return std::make_unique<ResourceGLubyte>(); };
	auto RES_GLUBYTE_CONSTRUCTOR_T = []() -> std::unique_ptr<ITabularizableResource>
	{ return std::make_unique<ResourceGLubyte>(); };
	auto RES2D_GLFLOAT_CONSTRUCTOR_S = []() -> std::unique_ptr<ISerializableResource>
	{ return std::make_unique<Resource2DGLfloat>(); };
	auto RES2D_GLFLOAT_CONSTRUCTOR_T = []() -> std::unique_ptr<ITabularizableResource>
	{ return std::make_unique<Resource2DGLfloat>(); };
} // end namespace

std::string Bullet::BulletPrefix()
{
	return "Bullet";
}

Bullet::Bullet() : GLEntity(),
				   bulletIndices_(
					   ResourceGLubyte({0, 3, 2, 1, 2, 3, 7, 6, 0, 4, 7, 3, 1, 2, 6, 5, 4, 5, 6, 7, 0, 1, 5, 4})),
				   projectionMatrix_(
					   Resource2DGLfloat(
						   std::vector<std::vector<GLfloat>>(4, std::vector<GLfloat>(4)))),

				   bulletVertices_(
					   Resource2DGLfloat(
						   {{-0.2f, -0.1f, 0.5f},
							{0.2f, -0.0f, 0.5f},
							{0.2f, 0.0f, 0.5f},
							{-0.2f, 0.1f, 0.5f},
							{-0.2f, -0.1f, 1.0f},
							{0.2f, -0.0f, 1.0f},
							{0.2f, 0.0f, 1.0f},
							{-0.2f, 0.1f, 1.0f}}))
{
}

void Bullet::RegisterSerializationResources(const std::string &key)
{
	GLEntity::RegisterSerializationResources(key);

	ResourceDeserializer *deserializer = ResourceDeserializer::GetInstance();
	if (!deserializer->HasSerializationKey(BULLET_VERTICES_KEY))
		deserializer->RegisterResource<GLfloat>(BULLET_VERTICES_KEY, RES2D_GLFLOAT_CONSTRUCTOR_S);
	if (!deserializer->HasSerializationKey(BULLET_INDICES_KEY))
		deserializer->RegisterResource<GLubyte>(BULLET_INDICES_KEY, RES_GLUBYTE_CONSTRUCTOR_S);
	if (!deserializer->HasSerializationKey(PROJECTION_MATRIX_KEY))
		deserializer->RegisterResource<GLfloat>(PROJECTION_MATRIX_KEY, RES2D_GLFLOAT_CONSTRUCTOR_S);
}

void Bullet::RegisterTabularizationResources(const std::string &key)
{
	GLEntity::RegisterTabularizationResources(key);

	ResourceDetabularizer *detabularizer = ResourceDetabularizer::GetInstance();
	if (!detabularizer->HasTabularizationKey(FormatKey(key + BULLET_VERTICES_KEY)))
		detabularizer->RegisterResource<GLfloat>(FormatKey(key + BULLET_VERTICES_KEY), RES2D_GLFLOAT_CONSTRUCTOR_T);
	if (!detabularizer->HasTabularizationKey(FormatKey(key + BULLET_INDICES_KEY)))
		detabularizer->RegisterResource<GLubyte>(FormatKey(key + BULLET_INDICES_KEY), RES_GLUBYTE_CONSTRUCTOR_T);
	if (!detabularizer->HasTabularizationKey(FormatKey(key + PROJECTION_MATRIX_KEY)))
		detabularizer->RegisterResource<GLfloat>(FormatKey(key + PROJECTION_MATRIX_KEY), RES2D_GLFLOAT_CONSTRUCTOR_T);
}

Bullet::Bullet(const GLfloat _x, const GLfloat _y) : Bullet()
{
	SetMass(0.5f);

	SetFrame(Resource2DGLfloat({{_x, 0.0f, 0.0f, 0.0f},
								{_y, 0.0f, 0.0f, 0.0f},
								{0.0f, 0.0f, 0.0f, 0.0f},
								{1.0f, 0.0f, 0.0f, 0.0f}}));

	SetUnitVelocity(Resource2DGLfloat({{1.0f, 0.0f, 0.0f, 0.0f},
									   {0.0f, 0.0f, 0.0f, 0.0f},
									   {0.0f, 0.0f, 0.0f, 0.0f},
									   {1.0f, 0.0f, 0.0f, 0.0f}}));
}

Bullet::~Bullet() = default;
Bullet::Bullet(const Bullet &) = default;
Bullet::Bullet(Bullet &&) = default;
Bullet &Bullet::operator=(const Bullet &) = default;
Bullet &Bullet::operator=(Bullet &&) = default;

void Bullet::InitializeBullet(const GLfloat _velocityAngle, const GLfloat _speed)
{
	SetSpeed(_speed + BULLET_SPEED_);
	SetVelocityAngle(_velocityAngle);
	SetUnitVelocity(0, 0, cos(_velocityAngle));
	SetUnitVelocity(1, 0, sin(_velocityAngle));
}

void Bullet::SetSMatrix()
{
	GLfloat speed = GetSpeed();
	S_ = Resource2DGLfloat({{speed, 0.0f, 0.0f, 0.0f},
							{0.0f, speed, 0.0f, 0.0f},
							{0.0f, 0.0f, speed, 0.0f},
							{0.0f, 0.0f, 0.0f, 1.0f}});
}

void Bullet::SetTMatrix()
{
	Resource2DGLfloat &frame = GetFrame();
	T_ = Resource2DGLfloat({{1.0f, 0.0f, 0.0f, frame.GetData(0, 0)},
							{0.0f, 1.0f, 0.0f, frame.GetData(1, 0)},
							{0.0f, 0.0f, 1.0f, frame.GetData(2, 0)},
							{0.0f, 0.0f, 0.0f, 1.0f}});
}

void Bullet::SetBulletOutOfBounds()
{
	GLfloat epsilon = 3.0f;

	GLfloat right = (1 / fabs(static_cast<GLfloat *>(projectionMatrix_.Data())[0]));
	GLfloat left = -1 * right;
	GLfloat top = (1 / fabs(static_cast<GLfloat *>(projectionMatrix_.Data())[5]));
	GLfloat bottom = -1 * top;

	Resource2DGLfloat &frame = GetFrame();
	if ((frame.GetData(0, 0) <= left - epsilon) || (frame.GetData(0, 0) >= right + epsilon) ||
		(frame.GetData(1, 0) >= top + epsilon) || (frame.GetData(1, 0) <= bottom - epsilon))
	{
		outOfBounds_ = true;
	}
}

void Bullet::Update(const GLfloat _velocityAngle, const GLfloat _speed)
{
	if (!bulletInitialized_)
		InitializeBullet(_velocityAngle, _speed);
	bulletInitialized_ = true;

	// Move the bullet ( p = av + frame )
	SetSMatrix();
	SetTMatrix();
}

void Bullet::Draw()
{
	auto MoveBullet = [this]()
	{
		glLoadMatrixf(static_cast<GLfloat *>(GetUnitVelocity().Data()));
		glMultMatrixf(static_cast<GLfloat *>(S_.Data()));
		glMultMatrixf(static_cast<GLfloat *>(T_.Data()));

		glGetFloatv(GL_MODELVIEW_MATRIX, static_cast<GLfloat *>(GetFrame().Data()));
	};

	auto DrawBullet = [this]()
	{
		glVertexPointer(3, GL_FLOAT, 0, bulletVertices_.Data());
		glColor3f(0.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(GetFrame().GetData(0, 0), GetFrame().GetData(1, 0), GetFrame().GetData(2, 0));
		glRotatef(GetVelocityAngle() * (180.0f / M_PI), 0.0f, 0.0f, 1.0f);
		glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, bulletIndices_.Data());
	};

	glGetFloatv(GL_PROJECTION_MATRIX, BUFFER[0].data());
	projectionMatrix_ = Resource2DGLfloat(BUFFER);

	glPushMatrix();

	MoveBullet();
	SetBulletOutOfBounds();
	DrawBullet();

	glPopMatrix();
}

bool Bullet::IsOutOfBounds() const
{
	return outOfBounds_;
}

void Bullet::Save(boost::property_tree::ptree &tree, const std::string &path) const
{
	tree.put(BULLET_INITIALIZED_KEY, bulletInitialized_);
	tree.put(OUT_OF_BOUNDS_KEY, outOfBounds_);

	if (!fs::exists(path))
		fs::create_directories(path);

	ResourceSerializer *serializer = ResourceSerializer::GetInstance();
	serializer->SetSerializationPath(path);

	serializer->Serialize(bulletVertices_, BULLET_VERTICES_KEY);
	serializer->Serialize(bulletIndices_, BULLET_INDICES_KEY);
	serializer->Serialize(projectionMatrix_, PROJECTION_MATRIX_KEY);

	GLEntity::Save(tree, path);
}

void Bullet::Load(boost::property_tree::ptree &tree, const std::string &path)
{
	GLEntity::Load(tree, path);

	bulletInitialized_ = tree.get_child(BULLET_INITIALIZED_KEY).data() == TRUE_VAL ? true : false;
	outOfBounds_ = tree.get_child(OUT_OF_BOUNDS_KEY).data() == TRUE_VAL ? true : false;

	ResourceDeserializer *deserializer = ResourceDeserializer::GetInstance();
	deserializer->SetSerializationPath(path);

	std::unique_ptr<ISerializableResource> deserializedVertices = deserializer->Deserialize(BULLET_VERTICES_KEY);
	bulletVertices_ = *static_cast<Resource2DGLfloat *>(deserializedVertices.release());
	std::unique_ptr<ISerializableResource> deserializedIndices = deserializer->Deserialize(BULLET_INDICES_KEY);
	bulletIndices_ = *static_cast<ResourceGLubyte *>(deserializedIndices.release());
	std::unique_ptr<ISerializableResource> deserializedProjection = deserializer->Deserialize(PROJECTION_MATRIX_KEY);
	projectionMatrix_ = *static_cast<Resource2DGLfloat *>(deserializedProjection.release());
}

void Bullet::Save(boost::property_tree::ptree &tree, Sqlite &database) const
{
	tree.put(BULLET_INITIALIZED_KEY, bulletInitialized_);
	tree.put(OUT_OF_BOUNDS_KEY, outOfBounds_);

	ResourceTabularizer *tabularizer = ResourceTabularizer::GetInstance();

	tabularizer->Tabularize(bulletVertices_, FormatKey(GetKey() + BULLET_VERTICES_KEY));
	tabularizer->Tabularize(bulletIndices_, FormatKey(GetKey() + BULLET_INDICES_KEY));
	tabularizer->Tabularize(projectionMatrix_, FormatKey(GetKey() + PROJECTION_MATRIX_KEY));

	GLEntity::Save(tree, database);
}

void Bullet::Load(boost::property_tree::ptree &tree, Sqlite &database)
{
	GLEntity::Load(tree, database);

	bulletInitialized_ = tree.get_child(BULLET_INITIALIZED_KEY).data() == TRUE_VAL ? true : false;
	outOfBounds_ = tree.get_child(OUT_OF_BOUNDS_KEY).data() == TRUE_VAL ? true : false;

	ResourceDetabularizer *detabularizer = ResourceDetabularizer::GetInstance();

	std::unique_ptr<ITabularizableResource> deserializedVertices = detabularizer->Detabularize(FormatKey(GetKey() + BULLET_VERTICES_KEY));
	bulletVertices_ = *static_cast<Resource2DGLfloat *>(deserializedVertices.release());
	std::unique_ptr<ITabularizableResource> deserializedIndices = detabularizer->Detabularize(FormatKey(GetKey() + BULLET_INDICES_KEY));
	bulletIndices_ = *static_cast<ResourceGLubyte *>(deserializedIndices.release());
	std::unique_ptr<ITabularizableResource> deserializedProjection = detabularizer->Detabularize(FormatKey(GetKey() + PROJECTION_MATRIX_KEY));
	projectionMatrix_ = *static_cast<Resource2DGLfloat *>(deserializedProjection.release());
}
