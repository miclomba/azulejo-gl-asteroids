#include "Ship.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "Entities/EntityAggregationDeserializer.h"
#include "Resources/Resource.h"
#include "Resources/ResourceDeserializer.h"
#include "Resources/ResourceSerializer.h"

#include "Bullet.h"
#include "GLSerializer.h"
#include "Ship.h"

using boost::property_tree::ptree;
using asteroids::Bullet;
using asteroids::GLSerializer;
using asteroids::Ship;
using entity::EntityAggregationDeserializer;
using resource::IResource;
using resource::Resource;
using resource::ResourceDeserializer;
using resource::ResourceSerializer;

namespace
{
const std::string SHIP_VERTICES_KEY = "ship_vertices";
const std::string SHIP_INDICES_KEY = "ship_indices";
const std::string UNIT_ORIENTATION_KEY = "unit_orientation";
const std::string ORIENTATION_ANGLE_KEY = "orientation_angle";
const std::string BULLET_FIRED_KEY = "bullet_fired";
const std::string TRUE_VAL = "true";
EntityAggregationDeserializer* const Deserializer = EntityAggregationDeserializer::GetInstance();
} // end namespace

std::string Ship::ShipKey()
{
	return "Ship";
}

Ship::Ship()
{
	SetVelocityAngle(M_PI / 2);

	shipVertices_ = {
		Row3{-0.5f,-0.5f,0.5f}, Row3{0.5f,-0.0f,0.5f},
		Row3{0.5f,0.0f,0.5f}, Row3{-0.5f,0.5f,0.5f},
		Row3{-0.5f,-0.5f,1.0f}, Row3{0.1f,-0.0f,1.0f},
		Row3{0.1f,0.0f,1.0f}, Row3{-0.5f,0.5f,1.0f}
	};

	shipIndices_.Data() = { 0,3,2,1,2,3,7,6,0,4,7,3,1,2,6,5,4,5,6,7,0,1,5,4 };

	unitOrientation_ = {
		Row4{0.0f,   0.0f,0.0f,0.0f},
		Row4{1.0f,   0.0f,0.0f,0.0f},
		Row4{0.0f,   0.0f,0.0f,0.0f},
		Row4{1.0f,   0.0f,0.0f,0.0f}
	};

	GetUnitVelocity() = {
		Row4{0.0f,   0.0f,0.0f,0.0f},
		Row4{1.0f,   0.0f,0.0f,0.0f},
		Row4{0.0f,   0.0f,0.0f,0.0f},
		Row4{1.0f,   0.0f,0.0f,0.0f}
	};

	ResourceDeserializer* deserializer = ResourceDeserializer::GetInstance();
	if (!deserializer->HasSerializationKey(SHIP_INDICES_KEY))
		deserializer->RegisterResource<std::vector<GLubyte>>(SHIP_INDICES_KEY);
}

Ship::Ship(const Ship::Key& key) :
	Ship()
{
	SetKey(key);
}

Ship::~Ship() = default;
Ship::Ship(const Ship&) = default;
Ship::Ship(Ship&&) = default;
Ship& Ship::operator=(const Ship&) = default;
Ship& Ship::operator=(Ship&&) = default;

std::string Ship::GenerateUUID() const
{
	boost::uuids::uuid tag = boost::uuids::random_generator()();
	std::string val = boost::lexical_cast<std::string>(tag);
	return val;
}

Ship::SharedEntity& Ship::GetBullet(const std::string& key) const
{
	return GetAggregatedMember(key);
}

std::vector<Ship::Key> Ship::GetBulletKeys() const
{
	return GetAggregatedMemberKeys();
}

void Ship::RecomputeShipVelocity(const GLfloat _thrust)
{
	/*====================== RECOMPUTE SHIP VELOCITY =========================*/
	/*================= F(dt) = m(dv) where _thrust = F(dt) ==================*/
	/*========================================================================*/
	if (fabs(_thrust - 0.0f) <= 0.00001f)
		return;

	GLfloat xComponentOrientation = _thrust * cos(orientationAngle_) / GetMass();
	GLfloat yComponentOrientation = _thrust * sin(orientationAngle_) / GetMass();
	GLfloat xComponentVelocity = GetSpeed()*cos(GetVelocityAngle());
	GLfloat yComponentVelocity = GetSpeed()*sin(GetVelocityAngle());

	xComponentVelocity += xComponentOrientation;
	yComponentVelocity += yComponentOrientation;

	SetSpeed(sqrt(pow(xComponentVelocity, 2) + pow(yComponentVelocity, 2)));
	GetUnitVelocity()[0][0] = xComponentVelocity / GetSpeed();
	GetUnitVelocity()[1][0] = yComponentVelocity / GetSpeed();

	if (GetSpeed() > 5.0f)
		SetSpeed(5.0f);

	SetVelocityAngle((GLfloat)(atan(GetUnitVelocity()[1][0] / GetUnitVelocity()[0][0])));
	if (GetUnitVelocity()[0][0] < 0)
		SetVelocityAngle(GetVelocityAngle() + M_PI);
	else if (GetUnitVelocity()[1][0] < 0)
		SetVelocityAngle(GetVelocityAngle() + 2 * M_PI);
}

void Ship::ChangeShipOrientation(const GLfloat _orientationAngle)
{
	if (fabs(_orientationAngle - 0.0f) <= 0.00001f)
		return;

	R_ = { 
		Row4{cos(_orientationAngle),-sin(_orientationAngle),0.0f,0.0f},
		Row4{sin(_orientationAngle),cos(_orientationAngle),0.0f,0.0f},
		Row4{0.0f,0.0f,1.0f,0.0f},
		Row4{0.0f,0.0f,0.0f,1.0f} 
	};

	glLoadMatrixf((GLfloat*)unitOrientation_.data());
	glMultMatrixf((GLfloat*)R_.data());
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)unitOrientation_.data());

	orientationAngle_ = (GLfloat)(atan(unitOrientation_[1][0] /
		unitOrientation_[0][0]));
	if (unitOrientation_[0][0] < 0)
		orientationAngle_ += M_PI;
	else if (unitOrientation_[1][0] < 0)
		orientationAngle_ += 2 * M_PI;
}

void Ship::MoveShip()
{
	S_ = { 
		Row4{GetSpeed(),0.0f,0.0f,0.0f},
		Row4{0.0f,GetSpeed(),0.0f,0.0f},
		Row4{0.0f,0.0f,GetSpeed(),0.0f},
		Row4{0.0f,0.0f,0.0f,1.0f} 
	};

	T_ = { 
		Row4{1.0f,0.0f,0.0f,GetFrame()[0][0]},
		Row4{0.0f,1.0f,0.0f,GetFrame()[1][0]},
		Row4{0.0f,0.0f,1.0f,GetFrame()[2][0]},
		Row4{0.0f,0.0f,0.0f,1.0f} 
	};

	/*======================= p = av + frame =============================*/
	glLoadMatrixf((GLfloat*)GetUnitVelocity().data());
	glMultMatrixf((GLfloat*)S_.data());
	glMultMatrixf((GLfloat*)T_.data());
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)GetFrame().data());
}

void Ship::WrapAroundMoveShip()
{
	GLfloat projectionMatrix[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

	GLfloat epsilon = 0.5f;

	GLfloat right = (1 / fabs(projectionMatrix[0]));
	GLfloat left = -1 * right;
	GLfloat top = (1 / fabs(projectionMatrix[5]));
	GLfloat bottom = -1 * top;

	if (GetFrame()[0][0] <= left - epsilon) {
		SetFrame(0,0,right + epsilon);
		SetFrame(1,0, GetFrame()[1][0] * -1);
	}
	else if (GetFrame()[0][0] >= right + epsilon) {
		SetFrame(0,0,left - epsilon);
		SetFrame(1,0, GetFrame()[1][0] * -1);
	}
	else if (GetFrame()[1][0] >= top + epsilon) {
		SetFrame(1,0,bottom - epsilon);
		SetFrame(0,0, GetFrame()[0][0] * -1);
	}
	else if (GetFrame()[1][0] <= bottom - epsilon) {
		SetFrame(1,0,top + epsilon);
		SetFrame(0,0, GetFrame()[0][0] * -1);
	}
}

void Ship::DrawBullets()
{
	if (bulletFired_ == false)
		return;

	bulletFired_ = false;

	std::map<Key, SharedEntity>& bullets = GetAggregatedMembers();

	std::vector<std::string> bulletsToRemove;
	for (auto iter = bullets.begin(); iter != bullets.end(); ++iter)
	{
		SharedEntity& sharedEntity = iter->second;
		Bullet* bullet = static_cast<Bullet*>(sharedEntity.get());
		if (bullet && bullet->IsOutOfBounds())
		{
			bulletsToRemove.push_back(bullet->GetKey());
		}
		else if (bullet)
		{
			bullet->Draw(orientationAngle_, GetSpeed());
			bulletFired_ = true;
		}
	}

	for (std::string& bulletKey : bulletsToRemove)
		RemoveBullet(bulletKey);
}

void Ship::DrawShip()
{
	glVertexPointer(3, GL_FLOAT, 0, shipVertices_.data());
	glColor3f(0.0f, 1.0f, 0.0f);
	glLoadIdentity();
	glTranslatef(GetFrame()[0][0], GetFrame()[1][0], GetFrame()[2][0]);
	glRotatef(orientationAngle_*(180.0f / M_PI), 0.0f, 0.0f, 1.0f);
	glDrawElements(GL_LINE_LOOP, 24, GL_UNSIGNED_BYTE, shipIndices_.Data().data());
}

void Ship::Draw(const GLfloat _orientationAngle, const GLfloat _thrust) 
{
	RecomputeShipVelocity(_thrust);

    glPushMatrix();
		ChangeShipOrientation(_orientationAngle);
		MoveShip();
		WrapAroundMoveShip();
		DrawBullets();
		DrawShip();
    glPopMatrix();
}

void Ship::RemoveBullet(const Ship::Key& key)
{
	RemoveMember(key);
}

void Ship::AddBullet(const SharedEntity& bullet)
{
	AggregateMember(bullet);
	Deserializer->RegisterEntity<Bullet>(bullet->GetKey());
}

GLint Ship::BulletNumber()
{
	return 5;
}

void Ship::Fire()
{
	std::map<Key, SharedEntity>& bullets = GetAggregatedMembers();
	if (bullets.size() > BulletNumber())
		return;

	std::string key = Bullet::BulletPrefix() + GenerateUUID();

	SharedEntity bullet = std::make_shared<Bullet>(GetFrame()[0][0], GetFrame()[1][0]);
	bullet->SetKey(key);
	bulletFired_ = true;

	AddBullet(bullet);
}

void Ship::Save(ptree& tree, const std::string& path) const
{
	GLEntity::Save(tree, path);

	tree.put(BULLET_FIRED_KEY, bulletFired_);
	tree.put(ORIENTATION_ANGLE_KEY, orientationAngle_);
	ptree unitOrientation = GLSerializer::GetSerial4x4Matrix(unitOrientation_);
	tree.add_child(UNIT_ORIENTATION_KEY, unitOrientation);
	ptree shipVertices = GLSerializer::GetSerial8x3Matrix(shipVertices_);
	tree.add_child(SHIP_VERTICES_KEY, shipVertices);

	ResourceSerializer* serializer = ResourceSerializer::GetInstance();
	serializer->SetSerializationPath("c:/users/miclomba/Desktop");
	serializer->Serialize(shipIndices_, SHIP_INDICES_KEY);
}

void Ship::Load(ptree& tree, const std::string& path)
{
	GLEntity::Load(tree, path);

	bulletFired_ = tree.get_child(BULLET_FIRED_KEY).data() == TRUE_VAL ? true : false;
	orientationAngle_ = std::stof(tree.get_child(ORIENTATION_ANGLE_KEY).data());
	unitOrientation_ = GLSerializer::Get4x4Matrix(tree.get_child(UNIT_ORIENTATION_KEY));
	shipVertices_ = GLSerializer::Get8x3Matrix(tree.get_child(SHIP_VERTICES_KEY));

	ResourceDeserializer* deserializer = ResourceDeserializer::GetInstance();
	deserializer->SetSerializationPath("c:/users/miclomba/Desktop");
	std::unique_ptr<IResource> deserializedResource = deserializer->Deserialize(SHIP_INDICES_KEY);
	shipIndices_ = *static_cast<Resource<std::vector<GLubyte>>*>(deserializedResource.get());
}

