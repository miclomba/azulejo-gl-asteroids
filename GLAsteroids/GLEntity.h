#ifndef asteroids_glentity_h 
#define asteroids_glentity_h	

#include <array>
#include <math.h>
#include <string>

#include <boost/property_tree/ptree.hpp>

#include "config.h"
#include "Entities/ISerializableEntity.h"

namespace asteroids {

class ASTEROIDS_DLL_EXPORT GLEntity : public entity::ISerializableEntity
{
public:
	typedef std::array<GLfloat, 4> Row4;
	typedef std::array<GLfloat, 3> Row3;

    GLEntity();
	~GLEntity();
	GLEntity(const GLEntity&);
	GLEntity(GLEntity&&);
	GLEntity& operator=(const GLEntity&);
	GLEntity& operator=(GLEntity&&);

	std::array<Row4,4>& GetFrame();
	std::array<Row4,4>& GetUnitVelocity();
	GLfloat GetSpeed() const;
	GLfloat GetMass() const;
	GLfloat GetVelocityAngle() const;

	void SetFrame(int i, int j, GLfloat val);
	void SetFrame(const std::array<Row4,4>& frame);
	void SetUnitVelocity(int i, int j, GLfloat val);
	void SetUnitVelocity(const std::array<Row4,4>& unitVelocity);
	void SetSpeed(const GLfloat);
	void SetMass(const GLfloat);
	void SetVelocityAngle(const GLfloat);

protected:
	std::array<Row4, 4> S_;
	std::array<Row4, 4> T_;
	std::array<Row4, 4> R_;

	void Save(boost::property_tree::ptree& tree, const std::string& path) const override;
	void Load(boost::property_tree::ptree& tree, const std::string& path) override;

	boost::property_tree::ptree GetSerialMatrix(const std::array<GLEntity::Row4, 4>& matrixData) const;
	boost::property_tree::ptree GetRow3SerialMatrix(const std::array<GLEntity::Row3, 8>& matrixData) const;
	std::array<GLEntity::Row4, 4> GetMatrix(const boost::property_tree::ptree& serialMatrixData) const;
	std::array<GLEntity::Row3, 8> GetRow3Matrix(const boost::property_tree::ptree& serialMatrixData) const;
	boost::property_tree::ptree GetSerialArray(const std::array<GLubyte, 24>& arrayData) const;
	boost::property_tree::ptree GetSerialArray16(const std::array<GLfloat, 16>& arrayData) const;
	std::array<GLubyte, 24> GetArray(const boost::property_tree::ptree& serialArrayData) const;
	std::array<GLfloat, 16> GetArray16(const boost::property_tree::ptree& serialArrayData) const;

private:
	std::array<Row4,4> frame_;
    std::array<Row4,4> unitVelocity_;

    GLfloat velocityAngle_ = 0.0;
    GLfloat speed_ = 0.0;
    GLfloat mass_ = 1.0;
};

} // end asteroids

#endif // asteroids_glentity_h	
