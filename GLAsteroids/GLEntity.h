#ifndef asteroids_glentity_h 
#define asteroids_glentity_h	

#include <array>
#include <math.h>
#include <string>

#include <boost/property_tree/ptree.hpp>

#include "config.h"
#include "Entities/ISerializableEntity.h"
#include "Resources/Resource.h"

namespace asteroids {

class ASTEROIDS_DLL_EXPORT GLEntity : public entity::ISerializableEntity
{
public:
    GLEntity();
	~GLEntity();
	GLEntity(const GLEntity&);
	GLEntity(GLEntity&&);
	GLEntity& operator=(const GLEntity&);
	GLEntity& operator=(GLEntity&&);

	resource::Resource<GLfloat>& GetFrame();
	resource::Resource<GLfloat>& GetUnitVelocity();
	GLfloat GetSpeed() const;
	GLfloat GetMass() const;
	GLfloat GetVelocityAngle() const;

	void SetFrame(int i, int j, GLfloat val);
	void SetFrame(const resource::Resource<GLfloat>& frame);
	void SetUnitVelocity(int i, int j, GLfloat val);
	void SetUnitVelocity(const resource::Resource<GLfloat>& unitVelocity);
	void SetSpeed(const GLfloat);
	void SetMass(const GLfloat);
	void SetVelocityAngle(const GLfloat);

protected:
	resource::Resource<GLfloat> S_;
	resource::Resource<GLfloat> T_;
	resource::Resource<GLfloat> R_;

	void Save(boost::property_tree::ptree& tree, const std::string& path) const override;
	void Load(boost::property_tree::ptree& tree, const std::string& path) override;

private:
	resource::Resource<GLfloat> frame_;
	resource::Resource<GLfloat> unitVelocity_;

    GLfloat velocityAngle_ = 0.0;
    GLfloat speed_ = 0.0;
    GLfloat mass_ = 1.0;
};

} // end asteroids

#endif // asteroids_glentity_h	
