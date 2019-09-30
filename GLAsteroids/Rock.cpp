#include "Rock.h"

#include <array>

using asteroids::GLEntity;
using asteroids::Rock;
using asteroids::State;

namespace
{
const std::array<GLEntity::Row3,8> rockVerticesL = { 
	GLEntity::Row3{-1.5f,-1.5f,0.5f}, GLEntity::Row3{1.5f,-1.5f,0.5f},
	GLEntity::Row3{1.5f,1.5f,0.5f}, GLEntity::Row3{-1.5f,1.5f,0.5f},
	GLEntity::Row3{-1.0f,-1.0f,1.0f}, GLEntity::Row3{1.0f,-1.0f,1.0f},
	GLEntity::Row3{1.0f,1.0f,1.0f}, GLEntity::Row3{-1.0f,1.0f,1.0f} 
};
const std::array<GLEntity::Row3, 8> rockVerticesM = { 
	GLEntity::Row3{-1.0f,-1.0f,0.5f},	GLEntity::Row3{1.0f,-1.0f,0.5f},
	GLEntity::Row3{1.0f,1.0f,0.5f}, GLEntity::Row3{-1.0f,1.0f,0.5f},
	GLEntity::Row3{-0.75f,-0.75f,1.0f}, GLEntity::Row3{0.75f,-0.75f,1.0f},
	GLEntity::Row3{0.75f,0.75f,1.0f}, GLEntity::Row3{-0.75f,0.75f,1.0f}
};
const std::array<GLEntity::Row3,8> rockVerticesS = { 
	GLEntity::Row3{-0.5f,-0.5f,0.5f}, GLEntity::Row3{0.5f,-0.5f,0.5f},
	GLEntity::Row3{0.5f,0.5f,0.5f}, GLEntity::Row3{-0.5f,0.5f,0.5f},
	GLEntity::Row3{-0.25f,-0.25f,1.0f}, GLEntity::Row3{0.25f,-0.25f,1.0f},
	GLEntity::Row3{0.25f,0.25f,1.0f}, GLEntity::Row3{-0.25f,0.25f,1.0f}
};
std::array<GLubyte, 24> rockIndices = { 0,3,2,1,2,3,7,6,0,4,7,3,1,2,6,5,4,5,6,7,0,1,5,4 };
}

Rock::Rock(const State _state, const GLfloat _x, const GLfloat _y, const GLint _index) : 
	state_(_state), index_(_index)
{
    GLint randy = rand();
    randy = randy%2;
    spinDirection_ = pow(-1,randy);

    SetVelocityAngle(0.0f);
    SetSpeed(0.02f);
    SetMass(5.0f);

    if (state_ == State::LARGE) 
		rockVertices_ = rockVerticesL;
    else if (state_ == State::MEDIUM) 
		rockVertices_ = rockVerticesM;
    else
		rockVertices_ = rockVerticesS;

    rockIndices_ = rockIndices;

    GetFrame() = { 
		Row4{_x,   0.0f,0.0f,0.0f},
        Row4{_y,   0.0f,0.0f,0.0f},
        Row4{0.0f,   0.0f,0.0f,0.0f},
        Row4{1.0f,   0.0f,0.0f,0.0f}
	};

    GetUnitVelocity() = {
		Row4{1.0f,   0.0f,0.0f,0.0f},
        Row4{0.0f,   0.0f,0.0f,0.0f},
        Row4{0.0f,   0.0f,0.0f,0.0f},
        Row4{1.0f,   0.0f,0.0f,0.0f}
	};
}

Rock::~Rock() = default;
Rock::Rock(const Rock&) = default;
Rock::Rock(Rock&&) = default;
Rock& Rock::operator=(const Rock&) = default;
Rock& Rock::operator=(Rock&&) = default;

void Rock::InitializeRock(const GLfloat _velocityAngle, const GLfloat _speed, const GLfloat _spin)
{
	if (rockInitialized_ == true)
		return;

	SetSpeed(GetSpeed() + _speed);
	SetVelocityAngle(_velocityAngle);
	/*==================== COMPUTE ROCK VELOCITY =========================*/
	GetUnitVelocity()[0][0] = cos(_velocityAngle);
	GetUnitVelocity()[1][0] = sin(_velocityAngle);

	spinEpsilon_ += _spin;
	spinEpsilon_ *= spinDirection_;

	rockInitialized_ = true;
}

void Rock::UpdateSpin()
{
	spin_ += spinEpsilon_;
	if (spin_ > 360.0f)
		spin_ -= 360.0f;
	else if (spin_ < -360.0f)
		spin_ += 360.0f;
}

void Rock::MoveRock()
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

void Rock::WrapAroundMoveRock()
{
	GLfloat projectionMatrix[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

	GLfloat epsilon;
	if (state_ == State::LARGE)
		epsilon = 0.7f;
	else if (state_ == State::MEDIUM)
		epsilon = 0.5f;
	else
		epsilon = 0.2f;

	GLfloat left, right, bottom, top;
	right = (1 / fabs(projectionMatrix[0]));
	left = -1 * right;
	top = (1 / fabs(projectionMatrix[5]));
	bottom = -1 * top;

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

void Rock::DrawRock()
{
	glVertexPointer(3, GL_FLOAT, 0, rockVertices_.data());
	glColor3f(1.0f, 1.0f, 1.0f);
	glLoadIdentity();
	glTranslatef(GetFrame()[0][0], GetFrame()[1][0], GetFrame()[2][0]);
	glRotatef(spin_*(180.0f / M_PI), 0.0f, 0.0f, 1.0f);
	glDrawElements(GL_LINE_LOOP, 24, GL_UNSIGNED_BYTE, rockIndices_.data());
}

void Rock::Draw(const GLfloat _velocityAngle, const GLfloat _speed, const GLfloat _spin) 
{
	InitializeRock(_velocityAngle, _speed, _spin);
	UpdateSpin();

    glPushMatrix();
		MoveRock();
		WrapAroundMoveRock();
		DrawRock();
    glPopMatrix();
}

GLint Rock::GetIndex() const
{
	return index_;
}

GLfloat Rock::GetSpin() const
{
	return spin_;
}

GLfloat Rock::GetSpinEpsilon() const
{
	return spinEpsilon_;
}

GLint Rock::GetSpinDirection() const
{
	return spinDirection_;
}

State Rock::GetState() const
{
	return state_;
}

void Rock::SetSpin(const GLfloat spin)
{
	spin_ = spin;
}

void Rock::SetSpinEpsilon(const GLfloat spinEpsilon)
{
	spinEpsilon_ = spinEpsilon;
}

void Rock::SetRockInitialized(const bool state)
{
	rockInitialized_ = state;
}
