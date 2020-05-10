#ifndef asteroids_ship_h
#define	asteroids_ship_h

#include <array>
#include <map>
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>

#include "test_filesystem_adapters/ContainerResource.h"
#include "test_filesystem_adapters/ContainerResource2D.h"

#include "config.h"
#include "Bullet.h"
#include "GLEntity.h"

namespace database_adapters {
class Sqlite;
} // end database_adapters

namespace asteroids {

using ResourceGLubyte = ContainerResource<GLubyte>;
using ResourceGLfloat = ContainerResource<GLfloat>;
using Resource2DGLfloat = ContainerResource2D<GLfloat>;

class ASTEROIDS_DLL_EXPORT Ship : public GLEntity
{
public:
	Ship();
	Ship(const Ship::Key& key);
	~Ship();
	Ship(const Ship&);
	Ship(Ship&&);
	Ship& operator=(const Ship&);
	Ship& operator=(Ship&&);

	void Draw(const GLfloat _orientationAngle, const GLfloat _thrust);
	void Fire();

	static GLint BulletNumber();

	SharedEntity& GetBullet(const std::string& key) const;
	std::vector<Key> GetBulletKeys() const;
	void RemoveBullet(const Key& key);
	void AddBullet(const SharedEntity& bullet);

	void Save(boost::property_tree::ptree& tree, const std::string& path) const override;
	void Load(boost::property_tree::ptree& tree, const std::string& path) override;

	void Save(database_adapters::Sqlite& database) const override;
	void Load(database_adapters::Sqlite& database) override;

	static std::string ShipKey();

private:
	std::string GenerateUUID() const;

	void RecomputeShipVelocity(const GLfloat _thrust);
	void ChangeShipOrientation(const GLfloat _orientationAngle);
	void MoveShip();
	void WrapAroundMoveShip();
	void DrawBullets();
	void DrawShip();

	bool bulletFired_{false};
	GLfloat orientationAngle_{ static_cast<GLfloat>(M_PI) / 2 };

    Resource2DGLfloat unitOrientation_;
    Resource2DGLfloat shipVertices_;
	ResourceGLubyte shipIndices_;
};

} // end asteroids
#endif	// asteroids_ship_h 

