#include "Bullet.h"

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>

#include "DatabaseAdapters/Sqlite.h"
#include "FilesystemAdapters/ISerializableResource.h"
#include "FilesystemAdapters/ResourceDeserializer.h"
#include "FilesystemAdapters/ResourceSerializer.h"
#include "test_filesystem_adapters/ContainerResource.h"
#include "test_filesystem_adapters/ContainerResource2D.h"

#include "Bullet.h"

using boost::property_tree::ptree;
using asteroids::Bullet;
using asteroids::GLEntity;
using database_adapters::Sqlite;
using filesystem_adapters::ISerializableResource;
using filesystem_adapters::ResourceDeserializer;
using filesystem_adapters::ResourceSerializer;

using ResourceGLubyte = ContainerResource<GLubyte>;
using Resource2DGLfloat = ContainerResource2D<GLfloat>;

namespace fs = std::filesystem;

namespace
{
const GLfloat BULLET_SPEED_ = 0.5;
const std::string BULLET_INITIALIZED_KEY = "bullet_initialized";
const std::string OUT_OF_BOUNDS_KEY = "out_of_bounds";
const std::string BULLET_VERTICES_KEY = "bullet_vertices";
const std::string BULLET_INDICES_KEY = "bullet_indices";
const std::string PROJECTION_MATRIX_KEY = "projection_matrix";
const std::string TRUE_VAL = "true";

auto RES_GLUBYTE_CONSTRUCTOR = []()->std::unique_ptr<ISerializableResource> { return std::make_unique<ResourceGLubyte>(); };
auto RES2D_GLFLOAT_CONSTRUCTOR = []()->std::unique_ptr<ISerializableResource> { return std::make_unique<Resource2DGLfloat>(); };
} // end namespace

std::string Bullet::BulletPrefix()
{
	return "Bullet";
}

Bullet::Bullet() :
	GLEntity()
{
	bulletIndices_ = ResourceGLubyte({0,3,2,1,2,3,7,6,0,4,7,3,1,2,6,5,4,5,6,7,0,1,5,4});
	projectionMatrix_ = Resource2DGLfloat(
		std::vector<std::vector<GLfloat>>(4, std::vector<GLfloat>(4))
	); 

	bulletVertices_ = Resource2DGLfloat({
		{-0.2f,-0.1f,0.5f}, {0.2f,-0.0f,0.5f},
		{0.2f,0.0f,0.5f}, {-0.2f,0.1f,0.5f},
		{-0.2f,-0.1f,1.0f}, {0.2f,-0.0f,1.0f},
		{0.2f,0.0f,1.0f}, {-0.2f,0.1f,1.0f}
	});

	ResourceDeserializer* deserializer = ResourceDeserializer::GetInstance();
	if (!deserializer->HasSerializationKey(BULLET_VERTICES_KEY))
		deserializer->RegisterResource<GLfloat>(BULLET_VERTICES_KEY, RES2D_GLFLOAT_CONSTRUCTOR);
	if (!deserializer->HasSerializationKey(BULLET_INDICES_KEY))
		deserializer->RegisterResource<GLubyte>(BULLET_INDICES_KEY, RES_GLUBYTE_CONSTRUCTOR);
	if (!deserializer->HasSerializationKey(PROJECTION_MATRIX_KEY))
		deserializer->RegisterResource<GLfloat>(PROJECTION_MATRIX_KEY, RES2D_GLFLOAT_CONSTRUCTOR);
}

Bullet::Bullet(const GLfloat _x, const GLfloat _y) :
	Bullet()
{
    SetMass(0.5f);

    SetFrame(Resource2DGLfloat({
		{_x,   0.0f,0.0f,0.0f},
		{_y,   0.0f,0.0f,0.0f},
		{0.0f,   0.0f,0.0f,0.0f},
		{1.0f,   0.0f,0.0f,0.0f}
	}));

    SetUnitVelocity(Resource2DGLfloat({
		{1.0f,   0.0f,0.0f,0.0f},
        {0.0f,   0.0f,0.0f,0.0f},
        {0.0f,   0.0f,0.0f,0.0f},
        {1.0f,   0.0f,0.0f,0.0f}
	}));
}

Bullet::~Bullet() = default;
Bullet::Bullet(const Bullet&) = default;
Bullet::Bullet(Bullet&&) = default;
Bullet& Bullet::operator=(const Bullet&) = default;
Bullet& Bullet::operator=(Bullet&&) = default;

void Bullet::InitializeBullet(const GLfloat _velocityAngle, const GLfloat _speed)
{
	SetSpeed(_speed + BULLET_SPEED_);
	SetVelocityAngle(_velocityAngle);
	SetUnitVelocity(0,0,cos(_velocityAngle));
	SetUnitVelocity(1,0,sin(_velocityAngle));
}

void Bullet::SetSMatrix()
{
	S_ = Resource2DGLfloat({
		{GetSpeed(),0.0f,0.0f,0.0f},
		{0.0f,GetSpeed(),0.0f,0.0f},
		{0.0f,0.0f,GetSpeed(),0.0f},
		{0.0f,0.0f,0.0f,1.0f}
	});
}

void Bullet::SetTMatrix()
{
	T_ = Resource2DGLfloat({
		{1.0f,0.0f,0.0f,GetFrame().GetData(0,0)},
		{0.0f,1.0f,0.0f,GetFrame().GetData(1,0)},
		{0.0f,0.0f,1.0f,GetFrame().GetData(2,0)},
		{0.0f,0.0f,0.0f,1.0f}
	});
}

void Bullet::SetBulletOutOfBounds()
{
	GLfloat epsilon = 3.0f;

	GLfloat right = (1 / fabs(static_cast<GLfloat*>(projectionMatrix_.Data())[0]));
	GLfloat left = -1 * right;
	GLfloat top = (1 / fabs(static_cast<GLfloat*>(projectionMatrix_.Data())[5]));
	GLfloat bottom = -1 * top;

	if ((GetFrame().GetData(0,0) <= left - epsilon) || (GetFrame().GetData(0,0) >= right + epsilon) ||
		(GetFrame().GetData(1,0) >= top + epsilon) || (GetFrame().GetData(1,0) <= bottom - epsilon))
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

		glLoadMatrixf(static_cast<GLfloat*>(GetUnitVelocity().Data()));
		glMultMatrixf(static_cast<GLfloat*>(S_.Data()));
		glMultMatrixf(static_cast<GLfloat*>(T_.Data()));

		glGetFloatv(GL_MODELVIEW_MATRIX, static_cast<GLfloat*>(GetFrame().Data()));
	};

	auto DrawBullet = [this]()
	{
		glVertexPointer(3, GL_FLOAT, 0, bulletVertices_.Data());
		glColor3f(0.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(GetFrame().GetData(0,0), GetFrame().GetData(1,0), GetFrame().GetData(2,0));
		glRotatef(GetVelocityAngle()*(180.0f / M_PI), 0.0f, 0.0f, 1.0f);
		glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, bulletIndices_.Data());
	};

	std::vector<std::vector<GLfloat>> buffer({ std::vector<GLfloat>(16) });
	glGetFloatv(GL_PROJECTION_MATRIX, buffer[0].data());
	projectionMatrix_ = Resource2DGLfloat(buffer);

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

void Bullet::Save(boost::property_tree::ptree& tree, const std::string& path) const
{
	if (!fs::exists(path))
		fs::create_directories(path);

	GLEntity::Save(tree, path);

	tree.put(BULLET_INITIALIZED_KEY, bulletInitialized_);
	tree.put(OUT_OF_BOUNDS_KEY, outOfBounds_);

	ResourceSerializer* serializer = ResourceSerializer::GetInstance();
	serializer->SetSerializationPath(path);

	serializer->Serialize(bulletVertices_, BULLET_VERTICES_KEY);
	serializer->Serialize(bulletIndices_, BULLET_INDICES_KEY);
	serializer->Serialize(projectionMatrix_, PROJECTION_MATRIX_KEY);
}

void Bullet::Load(boost::property_tree::ptree& tree, const std::string& path)
{
	GLEntity::Load(tree, path);

	bulletInitialized_ = tree.get_child(BULLET_INITIALIZED_KEY).data() == TRUE_VAL ? true : false;
	outOfBounds_ = tree.get_child(OUT_OF_BOUNDS_KEY).data() == TRUE_VAL ? true : false;

	ResourceDeserializer* deserializer = ResourceDeserializer::GetInstance();
	deserializer->SetSerializationPath(path);

	std::unique_ptr<ISerializableResource> deserializedVertices = deserializer->Deserialize(BULLET_VERTICES_KEY);
	bulletVertices_ = *static_cast<Resource2DGLfloat*>(deserializedVertices.release());
	std::unique_ptr<ISerializableResource> deserializedIndices = deserializer->Deserialize(BULLET_INDICES_KEY);
	bulletIndices_ = *static_cast<ResourceGLubyte*>(deserializedIndices.release());
	std::unique_ptr<ISerializableResource> deserializedProjection = deserializer->Deserialize(PROJECTION_MATRIX_KEY);
	projectionMatrix_ = *static_cast<Resource2DGLfloat*>(deserializedProjection.release());
}

void Bullet::Save(Sqlite& database) const
{

}

void Bullet::Load(Sqlite& database)
{

}
