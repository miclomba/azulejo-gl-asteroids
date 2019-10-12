#include <array>
#include <numeric>
#include <string>

#include <boost/property_tree/ptree.hpp>

#include "GLEntity.h"

using boost::property_tree::ptree;
using asteroids::GLEntity;

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
} // end namespace

ptree GLEntity::GetSerialArray16(const std::array<GLfloat, 16>& arrayData) const
{
	ptree mrow;
	for (const GLfloat& col : arrayData)
	{
		ptree cell;
		cell.put_value(col);
		mrow.push_back(std::make_pair("", cell));
	}
	return mrow;
}

ptree GLEntity::GetSerialArray(const std::array<GLubyte, 24>& arrayData) const
{
	ptree mrow;
	for (const GLubyte& col : arrayData)
	{
		ptree cell;
		cell.put_value(col);
		mrow.push_back(std::make_pair("", cell));
	}
	return mrow;
}

ptree GLEntity::GetSerialMatrix(const std::array<GLEntity::Row4,4>& matrixData) const
{
	ptree matrix;
	for (const GLEntity::Row4& row : matrixData)
	{
		ptree mrow;
		for (const GLfloat& col : row)
		{
			ptree cell;
			cell.put_value(col);
			mrow.push_back(std::make_pair("",cell));
		}
		matrix.push_back(std::make_pair("", mrow));
	}
	return matrix;
}

ptree GLEntity::GetRow3SerialMatrix(const std::array<GLEntity::Row3, 8>& matrixData) const
{
	ptree matrix;
	for (const GLEntity::Row3& row : matrixData)
	{
		ptree mrow;
		for (const GLfloat& col : row)
		{
			ptree cell;
			cell.put_value(col);
			mrow.push_back(std::make_pair("", cell));
		}
		matrix.push_back(std::make_pair("", mrow));
	}
	return matrix;
}

std::array<GLfloat, 16> GLEntity::GetArray16(const ptree& serialArrayData) const
{
	int x = 0;
	std::array<GLfloat, 16> arr;
	for (const ptree::value_type& cell : serialArrayData)
		arr[x++] = cell.second.get_value<GLfloat>();
	return arr;
}

std::array<GLubyte, 24> GLEntity::GetArray(const ptree& serialArrayData) const
{
	int x = 0;
	std::array<GLubyte, 24> arr;
	for (const ptree::value_type& cell : serialArrayData)
		arr[x++] = cell.second.get_value<GLfloat>();
	return arr;
}

std::array<GLEntity::Row4, 4> GLEntity::GetMatrix(const ptree& serialMatrixData) const
{
	int x = 0;
	std::array<GLEntity::Row4, 4> matrix;
	for (const ptree::value_type& row : serialMatrixData)
	{
		int y = 0;
		for (const ptree::value_type& cell : row.second)
		{
			matrix[x][y++] = cell.second.get_value<GLfloat>();
		}
		++x;
	}
	return matrix;
}

std::array<GLEntity::Row3, 8> GLEntity::GetRow3Matrix(const ptree& serialMatrixData) const
{
	int x = 0;
	std::array<GLEntity::Row3, 8> matrix;
	for (const ptree::value_type& row : serialMatrixData)
	{
		int y = 0;
		for (const ptree::value_type& cell : row.second)
		{
			matrix[x][y++] = cell.second.get_value<GLfloat>();
		}
		++x;
	}
	return matrix;
}

GLEntity::GLEntity() 
{
	for (Row4& row : frame_)
		std::fill(row.begin(), row.end(), 0.0);
	for (Row4& row : unitVelocity_)
		std::fill(row.begin(), row.end(), 0.0);
}

GLEntity::~GLEntity() = default;
GLEntity::GLEntity(const GLEntity&) = default;
GLEntity::GLEntity(GLEntity&&) = default;
GLEntity& GLEntity::operator=(const GLEntity&) = default;
GLEntity& GLEntity::operator=(GLEntity&&) = default;

std::array<GLEntity::Row4, 4>& GLEntity::GetFrame()
{
	return frame_;
}

std::array<GLEntity::Row4, 4>& GLEntity::GetUnitVelocity()
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

void GLEntity::SetFrame(const int i, const int j, const GLfloat val)
{
	frame_[i][j] = val;
}

void GLEntity::SetFrame(const std::array<Row4, 4>& frame)
{
	frame_ = frame;
}

void GLEntity::SetUnitVelocity(const int i, const int j, const GLfloat val)
{
	unitVelocity_[i][j] = val;
}

void GLEntity::SetUnitVelocity(const std::array<Row4, 4>& unitVelocity)
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

void GLEntity::Save(ptree& tree, const std::string&) const 
{
	ptree frameMatrix = GetSerialMatrix(frame_);
	tree.add_child(FRAME_KEY, frameMatrix);
	ptree unitVelMatrix = GetSerialMatrix(unitVelocity_);
	tree.add_child(UNIT_VELOCITY_KEY, unitVelMatrix);
	ptree sMatrix = GetSerialMatrix(S_);
	tree.add_child(S_KEY, sMatrix);
	ptree tMatrix = GetSerialMatrix(T_);
	tree.add_child(T_KEY, tMatrix);
	ptree rMatrix = GetSerialMatrix(R_);
	tree.add_child(R_KEY, rMatrix);

	tree.put(VELOCITY_ANGLE_KEY, velocityAngle_);
	tree.put(SPEED_KEY, speed_);
	tree.put(MASS_KEY, mass_);
}

void GLEntity::Load(ptree& tree, const std::string&) 
{
	frame_ = GetMatrix(tree.get_child(FRAME_KEY));
	unitVelocity_ = GetMatrix(tree.get_child(UNIT_VELOCITY_KEY));
	S_ = GetMatrix(tree.get_child(S_KEY));
	T_ = GetMatrix(tree.get_child(T_KEY));
	R_ = GetMatrix(tree.get_child(R_KEY));

	velocityAngle_ = std::stof(tree.get_child(VELOCITY_ANGLE_KEY).data());
	speed_ = std::stof(tree.get_child(SPEED_KEY).data());
	mass_ = std::stof(tree.get_child(MASS_KEY).data());
}
