#ifndef asteroids_glentity_h 
#define asteroids_glentity_h	

#include <array>
#include <math.h>
#include <string>

#include <boost/property_tree/ptree.hpp>

#include "config.h"
#include "DatabaseAdapters/ITabularizableEntity.h"
#include "DatabaseAdapters/Sqlite.h"
#include "FilesystemAdapters/ISerializableEntity.h"
#include "Resources/Resource2D.h"

namespace asteroids {

class ASTEROIDS_DLL_EXPORT GLEntity : 
	public filesystem_adapters::ISerializableEntity, 
	public database_adapters::ITabularizableEntity
{
public:
    GLEntity();
	~GLEntity();
	GLEntity(const GLEntity&);
	GLEntity(GLEntity&&);
	GLEntity& operator=(const GLEntity&);
	GLEntity& operator=(GLEntity&&);

	resource::Resource2D<GLfloat>& GetFrame();
	resource::Resource2D<GLfloat>& GetUnitVelocity();
	GLfloat GetSpeed() const;
	GLfloat GetMass() const;
	GLfloat GetVelocityAngle() const;

	void SetFrame(int i, int j, GLfloat val);
	void SetFrame(const resource::Resource2D<GLfloat>& frame);
	void SetUnitVelocity(int i, int j, GLfloat val);
	void SetUnitVelocity(const resource::Resource2D<GLfloat>& unitVelocity);
	void SetSpeed(const GLfloat);
	void SetMass(const GLfloat);
	void SetVelocityAngle(const GLfloat);

protected:
	resource::Resource2D<GLfloat> S_;
	resource::Resource2D<GLfloat> T_;
	resource::Resource2D<GLfloat> R_;

	void Save(boost::property_tree::ptree& tree, const std::string& path) const override;
	void Load(boost::property_tree::ptree& tree, const std::string& path) override;

	void Save(database_adapters::Sqlite& database) const override;
	void Load(database_adapters::Sqlite& database) override;

	// both ISerializableEntity and ITabularizableEntity override this function so 
	// we must override it to remove the ambiguity
	SharedEntity& GetAggregatedMember(const Key& key) const override;
private:

	resource::Resource2D<GLfloat> frame_;
	resource::Resource2D<GLfloat> unitVelocity_;

    GLfloat velocityAngle_ = 0.0;
    GLfloat speed_ = 0.0;
    GLfloat mass_ = 1.0;
};

} // end asteroids

#endif // asteroids_glentity_h	
