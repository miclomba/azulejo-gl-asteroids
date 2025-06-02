#include "Rock.h"

#include <array>
#include <memory>
#include <string>
#include <vector>
#include "config/filesystem.hpp"

#include <boost/property_tree/ptree.hpp>

#include "DatabaseAdapters/ITabularizableResource.h"
#include "DatabaseAdapters/ResourceDetabularizer.h"
#include "DatabaseAdapters/ResourceTabularizer.h"
#include "DatabaseAdapters/Sqlite.h"
#include "FilesystemAdapters/ISerializableResource.h"
#include "FilesystemAdapters/ResourceDeserializer.h"
#include "FilesystemAdapters/ResourceSerializer.h"
#include "test_filesystem_adapters/ContainerResource.h"
#include "test_filesystem_adapters/ContainerResource2D.h"

#include "Common.h"
#include "gl/GLEntity.h"
#include "Rock.h"

using asteroids::GLEntity;
using asteroids::Rock;
using asteroids::State;
using boost::property_tree::ptree;
using database_adapters::ITabularizableResource;
using database_adapters::ResourceDetabularizer;
using database_adapters::ResourceTabularizer;
using database_adapters::Sqlite;
using filesystem_adapters::ISerializableResource;
using filesystem_adapters::ResourceDeserializer;
using filesystem_adapters::ResourceSerializer;

using ResourceGLubyte = ContainerResource<GLubyte>;
using Resource2DGLfloat = ContainerResource2D<GLfloat>;

namespace
{
	const std::string &INDEX_KEY = "index";
	const std::string &SPIN_KEY = "spin";
	const std::string &SPIN_EPSILON_KEY = "spin_epsilon";
	const std::string &SPIN_DIRECTION_KEY = "spin_direction";
	const std::string &STATE_KEY = "state";
	const std::string &ROCK_INITIALIZED_KEY = "rock_initialized";
	const std::string &ROCK_VERTICES_KEY = "rock_vertices";
	const std::string &ROCK_INDICES_KEY = "rock_indices";
	const std::string TRUE_VAL = "true";

	const Resource2DGLfloat rockVerticesL({{-1.5f, -1.5f, 0.5f}, {1.5f, -1.5f, 0.5f}, {1.5f, 1.5f, 0.5f}, {-1.5f, 1.5f, 0.5f}, {-1.0f, -1.0f, 1.0f}, {1.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {-1.0f, 1.0f, 1.0f}});
	const Resource2DGLfloat rockVerticesM({{-1.0f, -1.0f, 0.5f}, {1.0f, -1.0f, 0.5f}, {1.0f, 1.0f, 0.5f}, {-1.0f, 1.0f, 0.5f}, {-0.75f, -0.75f, 1.0f}, {0.75f, -0.75f, 1.0f}, {0.75f, 0.75f, 1.0f}, {-0.75f, 0.75f, 1.0f}});
	const Resource2DGLfloat rockVerticesS({{-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}, {-0.25f, -0.25f, 1.0f}, {0.25f, -0.25f, 1.0f}, {0.25f, 0.25f, 1.0f}, {-0.25f, 0.25f, 1.0f}});
	ResourceGLubyte rockIndices({0, 3, 2, 1, 2, 3, 7, 6, 0, 4, 7, 3, 1, 2, 6, 5, 4, 5, 6, 7, 0, 1, 5, 4});

	auto RES_GLUBYTE_CONSTRUCTOR_S = []() -> std::unique_ptr<ISerializableResource>
	{ return std::make_unique<ResourceGLubyte>(); };
	auto RES_GLUBYTE_CONSTRUCTOR_T = []() -> std::unique_ptr<ITabularizableResource>
	{ return std::make_unique<ResourceGLubyte>(); };
	auto RES2D_GLFLOAT_CONSTRUCTOR_S = []() -> std::unique_ptr<ISerializableResource>
	{ return std::make_unique<Resource2DGLfloat>(); };
	auto RES2D_GLFLOAT_CONSTRUCTOR_T = []() -> std::unique_ptr<ITabularizableResource>
	{ return std::make_unique<Resource2DGLfloat>(); };
} // end namespace

std::string Rock::RockPrefix()
{
	return "Rock";
}

Rock::Rock() = default;

void Rock::RegisterSerializationResources(const std::string &key)
{
	GLEntity::RegisterSerializationResources(key);

	ResourceDeserializer *deserializer = ResourceDeserializer::GetInstance();
	if (!deserializer->HasSerializationKey(ROCK_VERTICES_KEY))
		deserializer->RegisterResource<GLfloat>(ROCK_VERTICES_KEY, RES2D_GLFLOAT_CONSTRUCTOR_S);
	if (!deserializer->HasSerializationKey(ROCK_INDICES_KEY))
		deserializer->RegisterResource<GLubyte>(ROCK_INDICES_KEY, RES_GLUBYTE_CONSTRUCTOR_S);
}

void Rock::RegisterTabularizationResources(const std::string &key)
{
	GLEntity::RegisterTabularizationResources(key);

	ResourceDetabularizer *tabularizer = ResourceDetabularizer::GetInstance();
	if (!tabularizer->HasTabularizationKey(FormatKey(key + ROCK_VERTICES_KEY)))
		tabularizer->RegisterResource<GLfloat>(FormatKey(key + ROCK_VERTICES_KEY), RES2D_GLFLOAT_CONSTRUCTOR_T);
	if (!tabularizer->HasTabularizationKey(FormatKey(key + ROCK_INDICES_KEY)))
		tabularizer->RegisterResource<GLubyte>(FormatKey(key + ROCK_INDICES_KEY), RES_GLUBYTE_CONSTRUCTOR_T);
}

Rock::Rock(const State _state, const GLfloat _x, const GLfloat _y) : Rock()
{
	state_ = _state;

	GLint randy = rand();
	randy = randy % 2;
	spinDirection_ = pow(-1, randy);

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

	GetFrame() = Resource2DGLfloat({{_x, 0.0f, 0.0f, 0.0f},
									{_y, 0.0f, 0.0f, 0.0f},
									{0.0f, 0.0f, 0.0f, 0.0f},
									{1.0f, 0.0f, 0.0f, 0.0f}});

	GetUnitVelocity() = Resource2DGLfloat({{1.0f, 0.0f, 0.0f, 0.0f},
										   {0.0f, 0.0f, 0.0f, 0.0f},
										   {0.0f, 0.0f, 0.0f, 0.0f},
										   {1.0f, 0.0f, 0.0f, 0.0f}});
}

Rock::~Rock() = default;
Rock::Rock(const Rock &) = default;
Rock::Rock(Rock &&) = default;
Rock &Rock::operator=(const Rock &) = default;
Rock &Rock::operator=(Rock &&) = default;

void Rock::InitializeRock(const GLfloat _velocityAngle, const GLfloat _speed, const GLfloat _spin)
{
	if (rockInitialized_ == true)
		return;

	SetSpeed(GetSpeed() + _speed);
	SetVelocityAngle(_velocityAngle);
	/*==================== COMPUTE ROCK VELOCITY =========================*/
	GetUnitVelocity().GetData(0, 0) = cos(_velocityAngle);
	GetUnitVelocity().GetData(1, 0) = sin(_velocityAngle);

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
	/*======================= p = av + frame =============================*/
	glLoadMatrixf(static_cast<GLfloat *>(GetUnitVelocity().Data()));
	glMultMatrixf(static_cast<GLfloat *>(S_.Data()));
	glMultMatrixf(static_cast<GLfloat *>(T_.Data()));
	glGetFloatv(GL_MODELVIEW_MATRIX, static_cast<GLfloat *>(GetFrame().Data()));
}

void Rock::WrapAroundMoveRock()
{
	GLfloat projectionMatrix[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

	GLfloat left, right, bottom, top;
	right = (1 / fabs(projectionMatrix[0]));
	left = -1 * right;
	top = (1 / fabs(projectionMatrix[5]));
	bottom = -1 * top;

	if (GetFrame().GetData(0, 0) <= left - epsilon_)
	{
		SetFrame(0, 0, right + epsilon_);
		SetFrame(1, 0, GetFrame().GetData(1, 0) * -1);
	}
	else if (GetFrame().GetData(0, 0) >= right + epsilon_)
	{
		SetFrame(0, 0, left - epsilon_);
		SetFrame(1, 0, GetFrame().GetData(1, 0) * -1);
	}
	else if (GetFrame().GetData(1, 0) >= top + epsilon_)
	{
		SetFrame(1, 0, bottom - epsilon_);
		SetFrame(0, 0, GetFrame().GetData(0, 0) * -1);
	}
	else if (GetFrame().GetData(1, 0) <= bottom - epsilon_)
	{
		SetFrame(1, 0, top + epsilon_);
		SetFrame(0, 0, GetFrame().GetData(0, 0) * -1);
	}
}

void Rock::Draw()
{
	glPushMatrix();

	MoveRock();
	WrapAroundMoveRock();

	glVertexPointer(3, GL_FLOAT, 0, rockVertices_.Data());
	glColor3f(1.0f, 1.0f, 1.0f);
	glLoadIdentity();
	glTranslatef(GetFrame().GetData(0, 0), GetFrame().GetData(1, 0), GetFrame().GetData(2, 0));
	glRotatef(spin_ * (180.0f / M_PI), 0.0f, 0.0f, 1.0f);
	glDrawElements(GL_LINE_LOOP, 24, GL_UNSIGNED_BYTE, rockIndices_.Data());

	glPopMatrix();
}

void Rock::Update(const GLfloat _velocityAngle, const GLfloat _speed, const GLfloat _spin)
{
	InitializeRock(_velocityAngle, _speed, _spin);
	UpdateSpin();

	S_ = Resource2DGLfloat({{GetSpeed(), 0.0f, 0.0f, 0.0f},
							{0.0f, GetSpeed(), 0.0f, 0.0f},
							{0.0f, 0.0f, GetSpeed(), 0.0f},
							{0.0f, 0.0f, 0.0f, 1.0f}});

	T_ = Resource2DGLfloat({{1.0f, 0.0f, 0.0f, GetFrame().GetData(0, 0)},
							{0.0f, 1.0f, 0.0f, GetFrame().GetData(1, 0)},
							{0.0f, 0.0f, 1.0f, GetFrame().GetData(2, 0)},
							{0.0f, 0.0f, 0.0f, 1.0f}});

	if (state_ == State::LARGE)
		epsilon_ = 0.7f;
	else if (state_ == State::MEDIUM)
		epsilon_ = 0.5f;
	else
		epsilon_ = 0.2f;
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

void Rock::Save(boost::property_tree::ptree &tree, const std::string &path) const
{
	tree.put(SPIN_KEY, spin_);
	tree.put(SPIN_EPSILON_KEY, spinEpsilon_);
	tree.put(SPIN_DIRECTION_KEY, spinDirection_);
	tree.put(STATE_KEY, static_cast<int>(state_));
	tree.put(ROCK_INITIALIZED_KEY, rockInitialized_);

	if (!fs::exists(path))
		fs::create_directories(path);

	ResourceSerializer *serializer = ResourceSerializer::GetInstance();
	serializer->SetSerializationPath(path);

	serializer->Serialize(rockVertices_, ROCK_VERTICES_KEY);
	serializer->Serialize(rockIndices_, ROCK_INDICES_KEY);

	GLEntity::Save(tree, path);
}

void Rock::Load(boost::property_tree::ptree &tree, const std::string &path)
{
	GLEntity::Load(tree, path);

	spin_ = std::stof(tree.get_child(SPIN_KEY).data());
	spinEpsilon_ = std::stof(tree.get_child(SPIN_EPSILON_KEY).data());
	spinDirection_ = std::stoi(tree.get_child(SPIN_DIRECTION_KEY).data());
	state_ = static_cast<State>(std::stoi(tree.get_child(STATE_KEY).data()));
	rockInitialized_ = tree.get_child(ROCK_INITIALIZED_KEY).data() == TRUE_VAL ? true : false;

	ResourceDeserializer *deserializer = ResourceDeserializer::GetInstance();
	deserializer->SetSerializationPath(path);

	std::unique_ptr<ISerializableResource> deserializedVertices = deserializer->Deserialize(ROCK_VERTICES_KEY);
	rockVertices_ = *static_cast<Resource2DGLfloat *>(deserializedVertices.get());
	std::unique_ptr<ISerializableResource> deserializedIndices = deserializer->Deserialize(ROCK_INDICES_KEY);
	rockIndices_ = *static_cast<ResourceGLubyte *>(deserializedIndices.get());
}

void Rock::Save(boost::property_tree::ptree &tree, Sqlite &database) const
{
	tree.put(SPIN_KEY, spin_);
	tree.put(SPIN_EPSILON_KEY, spinEpsilon_);
	tree.put(SPIN_DIRECTION_KEY, spinDirection_);
	tree.put(STATE_KEY, static_cast<int>(state_));
	tree.put(ROCK_INITIALIZED_KEY, rockInitialized_);

	ResourceTabularizer *tabularizer = ResourceTabularizer::GetInstance();

	tabularizer->Tabularize(rockVertices_, FormatKey(GetKey() + ROCK_VERTICES_KEY));
	tabularizer->Tabularize(rockIndices_, FormatKey(GetKey() + ROCK_INDICES_KEY));

	GLEntity::Save(tree, database);
}

void Rock::Load(boost::property_tree::ptree &tree, Sqlite &database)
{
	GLEntity::Load(tree, database);

	spin_ = std::stof(tree.get_child(SPIN_KEY).data());
	spinEpsilon_ = std::stof(tree.get_child(SPIN_EPSILON_KEY).data());
	spinDirection_ = std::stoi(tree.get_child(SPIN_DIRECTION_KEY).data());
	state_ = static_cast<State>(std::stoi(tree.get_child(STATE_KEY).data()));
	rockInitialized_ = tree.get_child(ROCK_INITIALIZED_KEY).data() == TRUE_VAL ? true : false;

	ResourceDetabularizer *detabularizer = ResourceDetabularizer::GetInstance();

	std::unique_ptr<ITabularizableResource> detabularizedVertices = detabularizer->Detabularize(FormatKey(GetKey() + ROCK_VERTICES_KEY));
	rockVertices_ = *static_cast<Resource2DGLfloat *>(detabularizedVertices.get());
	std::unique_ptr<ITabularizableResource> detabularizedIndices = detabularizer->Detabularize(FormatKey(GetKey() + ROCK_INDICES_KEY));
	rockIndices_ = *static_cast<ResourceGLubyte *>(detabularizedIndices.get());
}
