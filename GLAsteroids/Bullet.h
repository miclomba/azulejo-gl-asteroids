#ifndef asteroids_bullet_h
#define	asteroids_bullet_h

#include <array>
#include <string>

#include <boost/property_tree/ptree.hpp>

#include "Resources/Resource.h"

#include "config.h"
#include "GLEntity.h"

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

	static std::string BulletPrefix();
    
private:
	void InitializeBullet(const GLfloat _velocityAngle, const GLfloat _speed);
	void SetSMatrix();
	void SetTMatrix();
	void SetBulletOutOfBounds();

    bool bulletInitialized_ = false;
    bool outOfBounds_ = false;
	std::array<Row3, 8> bulletVertices_; 

	resource::Resource<std::vector<GLubyte>> bulletIndices_;
	resource::Resource<std::vector<GLfloat>> projectionMatrix_;
};

} // end asteroids
#endif // asteroids_bullet_h	

