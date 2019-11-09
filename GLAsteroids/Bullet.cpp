#include "Bullet.h"

#include <memory>
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>

#include "Resources/Resource.h"
#include "Resources/ResourceDeserializer.h"
#include "Resources/ResourceSerializer.h"

#include "Bullet.h"
#include "GLSerializer.h"

using boost::property_tree::ptree;
using asteroids::Bullet;
using asteroids::GLSerializer;
using resource::IResource;
using resource::Resource;
using resource::ResourceDeserializer;
using resource::ResourceSerializer;

namespace
{
const GLfloat BULLET_SPEED_ = 0.5;
const std::string BULLET_INITIALIZED_KEY = "bullet_initialized";
const std::string OUT_OF_BOUNDS_KEY = "out_of_bounds";
const std::string BULLET_VERTICES_KEY = "bullet_vertices";
const std::string BULLET_INDICES_KEY = "bullet_indices";
const std::string PROJECTION_MATRIX_KEY = "projection_matrix";
const std::string TRUE_VAL = "true";
} // end namespace

std::string Bullet::BulletPrefix()
{
	return "Bullet";
}

Bullet::Bullet()
{
	bulletIndices_ = Resource<GLubyte>({{0,3,2,1,2,3,7,6,0,4,7,3,1,2,6,5,4,5,6,7,0,1,5,4}});
	projectionMatrix_ = Resource<GLfloat>({std::vector<GLfloat>(16)}); 

	bulletVertices_ = {
		Row3{-0.2f,-0.1f,0.5f}, Row3{0.2f,-0.0f,0.5f},
		Row3{0.2f,0.0f,0.5f}, Row3{-0.2f,0.1f,0.5f},
		Row3{-0.2f,-0.1f,1.0f}, Row3{0.2f,-0.0f,1.0f},
		Row3{0.2f,0.0f,1.0f}, Row3{-0.2f,0.1f,1.0f}
	};

	ResourceDeserializer* deserializer = ResourceDeserializer::GetInstance();
	if (!deserializer->HasSerializationKey(BULLET_INDICES_KEY))
		deserializer->RegisterResource<GLubyte>(BULLET_INDICES_KEY);
	if (!deserializer->HasSerializationKey(PROJECTION_MATRIX_KEY))
		deserializer->RegisterResource<GLfloat>(PROJECTION_MATRIX_KEY);
}

Bullet::Bullet(const GLfloat _x, const GLfloat _y) :
	Bullet()
{
    SetMass(0.5f);

    SetFrame({
		Row4{_x,   0.0f,0.0f,0.0f},
		Row4{_y,   0.0f,0.0f,0.0f},
		Row4{0.0f,   0.0f,0.0f,0.0f},
		Row4{1.0f,   0.0f,0.0f,0.0f}
	});

    SetUnitVelocity({
		Row4{1.0f,   0.0f,0.0f,0.0f},
        Row4{0.0f,   0.0f,0.0f,0.0f},
        Row4{0.0f,   0.0f,0.0f,0.0f},
        Row4{1.0f,   0.0f,0.0f,0.0f}
	});
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

	GLfloat right = (1 / fabs(projectionMatrix_.Data(0,0)));
	GLfloat left = -1 * right;
	GLfloat top = (1 / fabs(projectionMatrix_.Data(0,5)));
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
		glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, bulletIndices_.Data()[0].data());
	};

	std::vector<std::vector<GLfloat>> buffer({ std::vector<GLfloat>(16) });
	glGetFloatv(GL_PROJECTION_MATRIX, buffer[0].data());
	projectionMatrix_ = Resource<GLfloat>(buffer);

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
	GLEntity::Save(tree, path);

	tree.put(BULLET_INITIALIZED_KEY, bulletInitialized_);
	tree.put(OUT_OF_BOUNDS_KEY, outOfBounds_);
	ptree bulletVertices = GLSerializer::GetSerial8x3Matrix(bulletVertices_);
	tree.add_child(BULLET_VERTICES_KEY, bulletVertices);

	ResourceSerializer* serializer = ResourceSerializer::GetInstance();
	serializer->SetSerializationPath("c:/users/miclomba/Desktop");

	serializer->Serialize(bulletIndices_, BULLET_INDICES_KEY);
	serializer->Serialize(projectionMatrix_, PROJECTION_MATRIX_KEY);
}

void Bullet::Load(boost::property_tree::ptree& tree, const std::string& path)
{
	GLEntity::Load(tree, path);

	bulletInitialized_ = tree.get_child(BULLET_INITIALIZED_KEY).data() == TRUE_VAL ? true : false;
	outOfBounds_ = tree.get_child(OUT_OF_BOUNDS_KEY).data() == TRUE_VAL ? true : false;
	bulletVertices_ = GLSerializer::Get8x3Matrix(tree.get_child(BULLET_VERTICES_KEY));

	ResourceDeserializer* deserializer = ResourceDeserializer::GetInstance();
	deserializer->SetSerializationPath("c:/users/miclomba/Desktop");

	std::unique_ptr<IResource> deserializedResource = deserializer->Deserialize(BULLET_INDICES_KEY);
	bulletIndices_ = *static_cast<Resource<GLubyte>*>(deserializedResource.get());
	std::unique_ptr<IResource> deserializedProjection = deserializer->Deserialize(PROJECTION_MATRIX_KEY);
	projectionMatrix_ = *static_cast<Resource<GLfloat>*>(deserializedProjection.get());
}
