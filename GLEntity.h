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
#include "test_filesystem_adapters/ContainerResource2D.h"

namespace asteroids {

using Resource2DGLfloat = ContainerResource2D<GLfloat>;

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

	virtual void Draw();

	Resource2DGLfloat& GetFrame();
	Resource2DGLfloat& GetUnitVelocity();
	GLfloat GetSpeed() const;
	GLfloat GetMass() const;
	GLfloat GetVelocityAngle() const;
	const Resource2DGLfloat& SMatrix() const;
	const Resource2DGLfloat& TMatrix() const;
	const Resource2DGLfloat& RMatrix() const;

	void SetFrame(int i, int j, GLfloat val);
	void SetFrame(const Resource2DGLfloat& frame);
	void SetUnitVelocity(int i, int j, GLfloat val);
	void SetUnitVelocity(const Resource2DGLfloat& unitVelocity);
	void SetSpeed(const GLfloat);
	void SetMass(const GLfloat);
	void SetVelocityAngle(const GLfloat);

	static void RegisterSerializationResources(const std::string& key);
	static void RegisterTabularizationResources(const std::string& key);

protected:
	Resource2DGLfloat S_;
	Resource2DGLfloat T_;
	Resource2DGLfloat R_;

	void Save(boost::property_tree::ptree& tree, const std::string& path) const override;
	void Load(boost::property_tree::ptree& tree, const std::string& path) override;

	void Save(boost::property_tree::ptree& tree, database_adapters::Sqlite& database) const override;
	void Load(boost::property_tree::ptree& tree, database_adapters::Sqlite& database) override;

	// both ISerializableEntity and ITabularizableEntity override this function so 
	// we must override it to remove the ambiguity
	SharedEntity& GetAggregatedMember(const Key& key) const override;
private:

	Resource2DGLfloat frame_;
	Resource2DGLfloat unitVelocity_;

    GLfloat velocityAngle_ = 0.0;
    GLfloat speed_ = 0.0;
    GLfloat mass_ = 1.0;
};

} // end asteroids

#endif // asteroids_glentity_h	
