#ifndef asteroids_bullet_h
#define	asteroids_bullet_h

#include <array>
#include <string>

#include <boost/property_tree/ptree.hpp>

#include "Resources/Resource.h"
#include "Resources/Resource2D.h"

#include "config.h"
#include "GLEntity.h"

namespace database_adapters {
class Sqlite;
} // end database_adapters

namespace asteroids {

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

    void Draw(const GLfloat _velocityAngle, const GLfloat _speed);

	bool IsOutOfBounds() const;

	void Save(boost::property_tree::ptree& tree, const std::string& path) const override;
	void Load(boost::property_tree::ptree& tree, const std::string& path) override;

	void Save(database_adapters::Sqlite& database) const override;
	void Load(database_adapters::Sqlite& database) override;

	static std::string BulletPrefix();
    
private:
	void InitializeBullet(const GLfloat _velocityAngle, const GLfloat _speed);
	void SetSMatrix();
	void SetTMatrix();
	void SetBulletOutOfBounds();

    bool bulletInitialized_ = false;
    bool outOfBounds_ = false;

	resource::Resource2D<GLfloat> bulletVertices_;
	resource::Resource<GLubyte> bulletIndices_;
	resource::Resource2D<GLfloat> projectionMatrix_;
};

} // end asteroids
#endif // asteroids_bullet_h	

