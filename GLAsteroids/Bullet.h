#ifndef asteroids_bullet_h
#define	asteroids_bullet_h

#include <array>
#include <string>

#include <boost/property_tree/ptree.hpp>

#include "test_filesystem_adapters/ContainerResource.h"
#include "test_filesystem_adapters/ContainerResource2D.h"

#include "config.h"
#include "GLEntity.h"

namespace database_adapters {
class Sqlite;
} // end database_adapters

namespace asteroids {

using ResourceGLubyte = ContainerResource<GLubyte>;
using ResourceGLfloat = ContainerResource<GLfloat>;
using Resource2DGLfloat = ContainerResource2D<GLfloat>;

class ASTEROIDS_DLL_EXPORT Bullet : public GLEntity
{
public:
	Bullet();
    Bullet(const GLfloat _x, const GLfloat _y);
	~Bullet();
	Bullet(const Bullet&);
	Bullet(Bullet&&);
	Bullet& operator=(const Bullet&);
	Bullet& operator=(Bullet&&);

	static void RegisterResources(const std::string& key);

    void Draw(const GLfloat _velocityAngle, const GLfloat _speed);

	bool IsOutOfBounds() const;

	void Save(boost::property_tree::ptree& tree, const std::string& path) const override;
	void Load(boost::property_tree::ptree& tree, const std::string& path) override;

	void Save(boost::property_tree::ptree& tree, database_adapters::Sqlite& database) const override;
	void Load(boost::property_tree::ptree& tree, database_adapters::Sqlite& database) override;

	static std::string BulletPrefix();
    
private:
	void InitializeBullet(const GLfloat _velocityAngle, const GLfloat _speed);
	void SetSMatrix();
	void SetTMatrix();
	void SetBulletOutOfBounds();

    bool bulletInitialized_ = false;
    bool outOfBounds_ = false;

	Resource2DGLfloat bulletVertices_;
	ResourceGLubyte bulletIndices_;
	Resource2DGLfloat projectionMatrix_;
};

} // end asteroids
#endif // asteroids_bullet_h	

