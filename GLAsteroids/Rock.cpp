#include "Rock.h"

#include <array>
#include <memory>
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>

#include "Resources/Resource.h"
#include "Resources/ResourceDeserializer.h"
#include "Resources/ResourceSerializer.h"

#include "GLEntity.h"
#include "GLSerializer.h"
#include "Rock.h"

using boost::property_tree::ptree;
using asteroids::GLEntity;
using asteroids::GLSerializer;
using asteroids::Rock;
using asteroids::State;
using resource::IResource;
using resource::Resource;
using resource::ResourceDeserializer;
using resource::ResourceSerializer;

namespace
{
const std::string& INDEX_KEY = "index";
const std::string& SPIN_KEY = "spin";
const std::string& SPIN_EPSILON_KEY = "spin_epsilon";
const std::string& SPIN_DIRECTION_KEY = "spin_direction";
const std::string& STATE_KEY = "state";
const std::string& ROCK_INITIALIZED_KEY = "rock_initialized";
const std::string& ROCK_VERTICES_KEY = "rock_vertices";
const std::string& ROCK_INDICES_KEY = "rock_indices";
const std::string TRUE_VAL = "true";

const Resource<GLfloat> rockVerticesL({ 
	{-1.5f,-1.5f,0.5f}, {1.5f,-1.5f,0.5f},
	{1.5f,1.5f,0.5f}, {-1.5f,1.5f,0.5f},
	{-1.0f,-1.0f,1.0f}, {1.0f,-1.0f,1.0f},
	{1.0f,1.0f,1.0f}, {-1.0f,1.0f,1.0f} 
});
const Resource<GLfloat> rockVerticesM({ 
	{-1.0f,-1.0f,0.5f},	{1.0f,-1.0f,0.5f},
	{1.0f,1.0f,0.5f}, {-1.0f,1.0f,0.5f},
	{-0.75f,-0.75f,1.0f}, {0.75f,-0.75f,1.0f},
	{0.75f,0.75f,1.0f}, {-0.75f,0.75f,1.0f}
});
const Resource<GLfloat> rockVerticesS({ 
	{-0.5f,-0.5f,0.5f}, {0.5f,-0.5f,0.5f},
	{0.5f,0.5f,0.5f}, {-0.5f,0.5f,0.5f},
	{-0.25f,-0.25f,1.0f}, {0.25f,-0.25f,1.0f},
	{0.25f,0.25f,1.0f}, {-0.25f,0.25f,1.0f}
});
Resource<GLubyte> rockIndices({ { 0,3,2,1,2,3,7,6,0,4,7,3,1,2,6,5,4,5,6,7,0,1,5,4 } });
} // end namespace

std::string Rock::RockPrefix()
{
	return "Rock";
}

Rock::Rock()
{
	ResourceDeserializer* deserializer = ResourceDeserializer::GetInstance();
	if (!deserializer->HasSerializationKey(ROCK_VERTICES_KEY))
		deserializer->RegisterResource<GLfloat>(ROCK_VERTICES_KEY);
	if (!deserializer->HasSerializationKey(ROCK_INDICES_KEY))
		deserializer->RegisterResource<GLubyte>(ROCK_INDICES_KEY);
}

Rock::Rock(const State _state, const GLfloat _x, const GLfloat _y) : 
	Rock()
{
	state_ = _state;

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
	std::array<std::array<GLfloat, 3>, 8> vertices;
	for (size_t i = 0; i < rockVertices_.Data().size(); ++i)
	{
		for (size_t j = 0; j < rockVertices_.Data()[i].size(); ++j)
			vertices[i][j] = rockVertices_.Data(i, j);
	}

	glVertexPointer(3, GL_FLOAT, 0, vertices.data());
	glColor3f(1.0f, 1.0f, 1.0f);
	glLoadIdentity();
	glTranslatef(GetFrame()[0][0], GetFrame()[1][0], GetFrame()[2][0]);
	glRotatef(spin_*(180.0f / M_PI), 0.0f, 0.0f, 1.0f);
	glDrawElements(GL_LINE_LOOP, 24, GL_UNSIGNED_BYTE, rockIndices_.Data()[0].data());
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

void Rock::Save(boost::property_tree::ptree& tree, const std::string& path) const
{
	GLEntity::Save(tree, path);

	tree.put(SPIN_KEY, spin_);
	tree.put(SPIN_EPSILON_KEY, spinEpsilon_);
	tree.put(SPIN_DIRECTION_KEY, spinDirection_);
	tree.put(STATE_KEY, static_cast<int>(state_));
	tree.put(ROCK_INITIALIZED_KEY, rockInitialized_);

	ResourceSerializer* serializer = ResourceSerializer::GetInstance();
	serializer->SetSerializationPath("c:/users/miclomba/Desktop");
	serializer->Serialize(rockVertices_, ROCK_VERTICES_KEY);
	serializer->Serialize(rockIndices_, ROCK_INDICES_KEY);
}

void Rock::Load(boost::property_tree::ptree& tree, const std::string& path)
{
	GLEntity::Load(tree, path);

	spin_ = std::stof(tree.get_child(SPIN_KEY).data());
	spinEpsilon_ = std::stof(tree.get_child(SPIN_EPSILON_KEY).data());
	spinDirection_ = std::stoi(tree.get_child(SPIN_DIRECTION_KEY).data());
	state_ = static_cast<State>(std::stoi(tree.get_child(STATE_KEY).data()));
	rockInitialized_ = tree.get_child(ROCK_INITIALIZED_KEY).data() == TRUE_VAL ? true : false;

	ResourceDeserializer* deserializer = ResourceDeserializer::GetInstance();
	deserializer->SetSerializationPath("c:/users/miclomba/Desktop");
	std::unique_ptr<IResource> deserializedVertices = deserializer->Deserialize(ROCK_VERTICES_KEY);
	rockVertices_ = *static_cast<Resource<GLfloat>*>(deserializedVertices.get());
	std::unique_ptr<IResource> deserializedIndices = deserializer->Deserialize(ROCK_INDICES_KEY);
	rockIndices_ = *static_cast<Resource<GLubyte>*>(deserializedIndices.get());
}
