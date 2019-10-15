#ifndef asteroids_glserializer_h
#define asteroids_glserializer_h

#include <array>
#include <boost/property_tree/ptree.hpp>

#include "config.h"
#include "GLEntity.h"

namespace asteroids {

class GLSerializer
{
public:
	static ASTEROIDS_DLL_EXPORT boost::property_tree::ptree GetSerial4x4Matrix(const std::array<GLEntity::Row4, 4>& matrixData);
	static ASTEROIDS_DLL_EXPORT boost::property_tree::ptree GetSerial8x3Matrix(const std::array<GLEntity::Row3, 8>& matrixData);
	static ASTEROIDS_DLL_EXPORT std::array<GLEntity::Row4, 4> Get4x4Matrix(const boost::property_tree::ptree& serialMatrixData);
	static ASTEROIDS_DLL_EXPORT std::array<GLEntity::Row3, 8> Get8x3Matrix(const boost::property_tree::ptree& serialMatrixData);

private:
	GLSerializer() = delete;
	~GLSerializer() = delete;

	GLSerializer(const GLSerializer&) = delete;
	GLSerializer& operator=(const GLSerializer&) = delete;
	GLSerializer(GLSerializer&&) = delete;
	GLSerializer operator=(GLSerializer&&) = delete;
};

} // asteroids

#endif // asteroids_glserializer_h
