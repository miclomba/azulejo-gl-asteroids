#ifndef asteroids_asteroids_h
#define	asteroids_asteroids_h

#include <memory>
#include <set>
#include <string>
#include <vector>

#include <boost/asio/thread_pool.hpp>
#include <boost/property_tree/ptree.hpp>

#include "Events/EventConsumer.h"

#include "config.h"
#include "Rock.h"
#include "Ship.h"
#include "GLEntity.h"

namespace database_adapters {
class Sqlite;
} // end database_adapters

namespace asteroids {

class ASTEROIDS_DLL_EXPORT Asteroids : public GLEntity
{
public:
	Asteroids();
	~Asteroids();
	Asteroids(const Asteroids&) = delete;
	Asteroids(Asteroids&&) = delete;
	Asteroids& operator=(const Asteroids&) = delete;
	Asteroids& operator=(Asteroids&&) = delete;

	void Save(boost::property_tree::ptree& tree, const std::string& path) const override;
	void Load(boost::property_tree::ptree& tree, const std::string& path) override;

	void Save(boost::property_tree::ptree& tree, database_adapters::Sqlite& database) const override;
	void Load(boost::property_tree::ptree& tree, database_adapters::Sqlite& database) override;

	std::shared_ptr<events::EventConsumer<void(void)>> GetLeftArrowConsumer();
	std::shared_ptr<events::EventConsumer<void(void)>> GetRightArrowConsumer();
	std::shared_ptr<events::EventConsumer<void(void)>> GetThrustConsumer();
	std::shared_ptr<events::EventConsumer<void(void)>> GetFireConsumer();
	std::shared_ptr<events::EventConsumer<void(void)>> GetResetConsumer();
	std::shared_ptr<events::EventConsumer<void(void)>> GetDrawConsumer();
	std::shared_ptr<events::EventConsumer<void(void)>> GetRunConsumer();
	std::shared_ptr<events::EventConsumer<void(void)>> GetSerializeConsumer();
	std::shared_ptr<events::EventConsumer<void(void)>> GetDeserializeConsumer();

private:
	// commands
	void Draw() override;
	void Fire();
	void RotateLeft();
	void RotateRight();
	void Thrust();
	void ResetGame();
	void Run();
	void Serialize();
	void Deserialize();

	std::string GenerateUUID() const;

	SharedEntity& GetShip();
	SharedEntity& GetRock(const std::string& key);
	std::vector<Key> GetRockKeys() const;

	void AddOutOfScopeBulletsToRemovalKeys();
	void ClearUnusedSerializationKeys();
	void ClearUnusedTabularizationKeys();
	void ClearGame();
	void ClearRocks();
	void ClearBullets();
	void ClearShip();
	void DestroyRock(Rock* rock);
	void DestroyBullet(Bullet* bullet);
	std::set<std::string> keysToRemove_;
	std::set<std::string> keysSerialized_;
	std::set<std::string> GetKeysToSerialize();
	void AddToRemoveKeys(const std::string& key);

	void BreakRock(Rock* rock);

	bool HasRocks();
	bool HasRock(const std::string& key) const;
	std::shared_ptr<Rock> MakeRock(const State rockSize, Rock* rock, const bool halfMass, const bool clockwise);

	void CalculateConservationOfMomentum(Bullet* _ithBullet, Rock* rock);

	void DetermineCollisions();
    Rock* ShipCollision();
    Rock* Collision(Bullet* bullet);
    void ProcessCollision(Bullet* bullet, Rock* rock);

	void UpdateRockTask(GLEntity* sharedRock);
	void UpdateShipTask(GLEntity* sharedShip, std::vector<std::future<GLEntity*>>& futures);

	void DrawGLEntities();
	void DrawGameInfo();
	void ResetThrustAndRotation();

	GLint score_{0};
	GLint rockCount_{0};
	GLfloat orientationAngle_{0.0f};
	GLfloat thrust_{0.0f};

	boost::asio::thread_pool threadPool_;

	std::shared_ptr<events::EventConsumer<void(void)>> leftArrowConsumer_;
	std::shared_ptr<events::EventConsumer<void(void)>> rightArrowConsumer_;
	std::shared_ptr<events::EventConsumer<void(void)>> thrustConsumer_;
	std::shared_ptr<events::EventConsumer<void(void)>> fireConsumer_;
	std::shared_ptr<events::EventConsumer<void(void)>> resetConsumer_;
	std::shared_ptr<events::EventConsumer<void(void)>> drawConsumer_;
	std::shared_ptr<events::EventConsumer<void(void)>> runConsumer_;
	std::shared_ptr<events::EventConsumer<void(void)>> serializeConsumer_;
	std::shared_ptr<events::EventConsumer<void(void)>> deserializeConsumer_;
};

} // end asteroids
#endif	// asteroids_asteroids_h

