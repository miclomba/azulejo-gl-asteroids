#ifndef asteroids_asteroids_h
#define	asteroids_asteroids_h

#include <memory>
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>

#include "config.h"
#include "Rock.h"
#include "Ship.h"
#include "GLEntity.h"

namespace asteroids {

class ASTEROIDS_DLL_EXPORT Asteroids : public GLEntity
{
public:
	Asteroids();
	~Asteroids();
	Asteroids(const Asteroids&);
	Asteroids(Asteroids&&);
	Asteroids& operator=(const Asteroids&);
	Asteroids& operator=(Asteroids&&);

	void Draw();

	// commands
	void Fire();
	void RotateLeft();
	void RotateRight();
	void Thrust();
	void ResetGame();
	void ClearGame();

	void Save(boost::property_tree::ptree& tree, const std::string& path) const override;
	void Load(boost::property_tree::ptree& tree, const std::string& path) override;

private:
	std::string GenerateUUID() const;

	SharedEntity& GetShip();
	SharedEntity& GetRock(const std::string& key);
	std::vector<Key> GetRockKeys() const;

	void ClearRocks();
	bool HasRocks();
	bool HasRock(const std::string& key) const;
	void BreakRock(Rock* rock);
	void DestroyRock(Rock* rock);
	std::shared_ptr<Rock> MakeRock(const State rockSize, Rock* rock, const bool halfMass, const bool clockwise);

	void CalculateConservationOfMomentum(Bullet* _ithBullet, Rock* rock);
	void DestroyBullet(Bullet* bullet);

	void DetermineCollisions();
    Rock* ShipCollision();
    Rock* Collision(Bullet* bullet);
    void ProcessCollision(Bullet* bullet, Rock* rock);

	void DrawRockAndShip();
	void DrawGameInfo();
	void ResetThrustAndRotation();

	GLint score_{0};
	GLint rockCount_{0};
	GLfloat orientationAngle_{0.0f};
	GLfloat thrust_{0.0f};
};

} // end asteroids
#endif	// asteroids_asteroids_h

