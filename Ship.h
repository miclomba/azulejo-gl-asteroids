/**
 * @file Ship.h
 * @brief Declaration of the Ship class which represents the player's ship in the Asteroids game.
 */

#ifndef asteroids_ship_h
#define asteroids_ship_h

#include <array>
#include <future>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <boost/asio/thread_pool.hpp>
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

/**
 * @class Ship
 * @brief A class representing the player's ship in the Asteroids game.
 *
 * This class handles the movement, orientation, and firing of bullets from the ship.
 */
class ASTEROIDS_DLL_EXPORT Ship : public GLEntity
{
public:
    /**
     * @brief Default constructor for the Ship class.
     */
    Ship();

    /**
     * @brief Constructor that initializes the ship with a given key.
     * @param key The unique key identifier for the ship.
     */
    Ship(const Ship::Key& key);

    /**
     * @brief Destructor for the Ship class.
     */
    ~Ship();

    Ship(const Ship&);
    Ship(Ship&&);
    Ship& operator=(const Ship&);
    Ship& operator=(Ship&&);

    /**
     * @brief Register serialization resources for the ship.
     * @param key The serialization key.
     */
    static void RegisterSerializationResources(const std::string& key);

    /**
     * @brief Register tabularization resources for the ship.
     * @param key The tabularization key.
     */
    static void RegisterTabularizationResources(const std::string& key);

    /**
     * @brief Update the ship's position, orientation, and bullets.
     * @param _orientationAngle The angle to rotate the ship.
     * @param _thrust The thrust to apply to the ship.
     * @param serializedKeys Keys of serialized objects.
     * @param threadPool Thread pool for concurrent processing.
     * @param futures Vector of futures for parallel execution.
     */
    void Update(
        const GLfloat _orientationAngle, 
        const GLfloat _thrust, 
        const std::set<std::string>& serializedKeys, 
        boost::asio::thread_pool& threadPool,
        std::vector<std::future<GLEntity*>>& futures
    );

    /**
     * @brief Draw the ship.
     */
    void Draw() override;

    /**
     * @brief Fire a bullet from the ship.
     */
    void Fire();

    /**
     * @brief Get the current number of bullets.
     * @return The number of bullets.
     */
    static GLint BulletNumber();

    SharedEntity& GetBullet(const std::string& key) const;
    std::vector<Key> GetBulletKeys() const;
    const std::set<Key>& GetOutOfScopeBulletKeys() const;
    void ClearOutOfScopeBulletKeys();
    void RemoveBullet(const Key& key, const std::set<std::string>& serializedKeys);
    void AddBullet(const SharedEntity& bullet);

    void Save(boost::property_tree::ptree& tree, const std::string& path) const override;
    void Load(boost::property_tree::ptree& tree, const std::string& path) override;

    void Save(boost::property_tree::ptree& tree, database_adapters::Sqlite& database) const override;
    void Load(boost::property_tree::ptree& tree, database_adapters::Sqlite& database) override;

    /**
     * @brief Get the unique key for the ship.
     * @return The ship's key as a string.
     */
    static std::string ShipKey();

    const Resource2DGLfloat& GetUnitOrientation() const;
    const Resource2DGLfloat& GetShipVertices() const;
    const ResourceGLubyte& GetShipIndices() const;

private:
    /**
     * @brief Generate a unique identifier for the ship.
     * @return A unique identifier string.
     */
    std::string GenerateUUID() const;

    void RecomputeShipVelocity(const GLfloat _thrust);
    void ChangeShipOrientation();
    void MoveShip();
    void WrapAroundMoveShip();
    void UpdateBullets(
        const std::set<std::string>& serializedKeys, 
        boost::asio::thread_pool& threadPool, 
        std::vector<std::future<GLEntity*>>& futures
    );
    void UpdateBulletTask(GLEntity* sharedBullet);

    mutable std::set<std::string> outOfScopeBulletKeys_;

    bool bulletFired_{ false };
    bool doRotate_{ false };
    GLfloat orientationAngle_{ static_cast<GLfloat>(M_PI) / 2 };

    Resource2DGLfloat unitOrientation_;
    Resource2DGLfloat shipVertices_;
    ResourceGLubyte shipIndices_;
};

} // end asteroids

#endif // asteroids_ship_h
