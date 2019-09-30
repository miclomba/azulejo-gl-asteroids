#include "Ship.h"

#include <array>
#include <map>
#include <memory>
#include <string>

using asteroids::Bullet;
using asteroids::Ship;

namespace
{
const std::string BULLET_PREFIX = "bullet_";
} // end namespace

Ship::Ship(const Ship::Key& key)
{
	SetKey(key);

	for (int i = 0; i < BulletNumber(); ++i)
		AggregateMember(BULLET_PREFIX + std::to_string(i));

	SetVelocityAngle(M_PI / 2);

	shipVertices_ = {
		Row3{-0.5f,-0.5f,0.5f}, Row3{0.5f,-0.0f,0.5f},
		Row3{0.5f,0.0f,0.5f}, Row3{-0.5f,0.5f,0.5f},
		Row3{-0.5f,-0.5f,1.0f}, Row3{0.1f,-0.0f,1.0f},
		Row3{0.1f,0.0f,1.0f}, Row3{-0.5f,0.5f,1.0f}
	};

	shipIndices_ = { 0,3,2,1,2,3,7,6,0,4,7,3,1,2,6,5,4,5,6,7,0,1,5,4 };

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
}

Ship::~Ship() = default;
Ship::Ship(const Ship&) = default;
Ship::Ship(Ship&&) = default;
Ship& Ship::operator=(const Ship&) = default;
Ship& Ship::operator=(Ship&&) = default;

GLint Ship::BulletNumber()
{
	return 10;
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

	for (auto iter = bullets.begin(); iter != bullets.end(); ++iter)
	{
		SharedEntity& sharedEntity = iter->second;
		Bullet* bullet = static_cast<Bullet*>(sharedEntity.get());
		if (bullet && bullet->IsOutOfBounds())
		{
			sharedEntity.reset();
		}
		else if (bullet)
		{
			bullet->Draw(orientationAngle_, GetSpeed());
			bulletFired_ = true;
		}
	}
}

void Ship::DrawShip()
{
	glVertexPointer(3, GL_FLOAT, 0, shipVertices_.data());
	glColor3f(0.0f, 1.0f, 0.0f);
	glLoadIdentity();
	glTranslatef(GetFrame()[0][0], GetFrame()[1][0], GetFrame()[2][0]);
	glRotatef(orientationAngle_*(180.0f / M_PI), 0.0f, 0.0f, 1.0f);
	glDrawElements(GL_LINE_LOOP, 24, GL_UNSIGNED_BYTE, shipIndices_.data());
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

void Ship::Fire()
{
	std::map<Key, SharedEntity>& bullets = GetAggregatedMembers();
	for (auto iter = bullets.begin(); iter != bullets.end(); ++iter)
	{
		SharedEntity& bullet = iter->second;
		if (bullet)
			continue;
		bullet = std::make_shared<Bullet>(GetFrame()[0][0],GetFrame()[1][0]);
		bullet->SetKey(iter->first);
		bulletFired_ = true;
		break;
	}
}

