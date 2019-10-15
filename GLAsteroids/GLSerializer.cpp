#include "GLSerializer.h"

#include <array>
#include <boost/property_tree/ptree.hpp>

#include "GLEntity.h"

using boost::property_tree::ptree;
using asteroids::GLEntity;
using asteroids::GLSerializer;

ptree GLSerializer::GetSerial4x4Matrix(const std::array<GLEntity::Row4, 4>& matrixData)
{
	ptree matrix;
	for (const GLEntity::Row4& row : matrixData)
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

ptree GLSerializer::GetSerial8x3Matrix(const std::array<GLEntity::Row3, 8>& matrixData)
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

std::array<GLEntity::Row4, 4> GLSerializer::Get4x4Matrix(const ptree& serialMatrixData)
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

std::array<GLEntity::Row3, 8> GLSerializer::Get8x3Matrix(const ptree& serialMatrixData)
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

