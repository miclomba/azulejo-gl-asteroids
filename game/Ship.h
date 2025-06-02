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

#include "Bullet.h"
#include "configuration/config.h"
#include "gl/GLEntity.h"

namespace database_adapters
{
    class Sqlite;
} // end database_adapters

namespace asteroids
{

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
        Ship(const Ship::Key &key);

        /**
         * @brief Destructor for the Ship class.
         */
        virtual ~Ship();

        /**
         * @brief Copy constructor.
         */
        Ship(const Ship &);

        /**
         * @brief Move constructor.
         */
        Ship(Ship &&);

        /**
         * @brief Copy assignment.
         */
        Ship &operator=(const Ship &);

        /**
         * @brief Move assignment.
         */
        Ship &operator=(Ship &&);

        /**
         * @brief Register serialization resources for the ship.
         * @param key The serialization key.
         */
        static void RegisterSerializationResources(const std::string &key);

        /**
         * @brief Register tabularization resources for the ship.
         * @param key The tabularization key.
         */
        static void RegisterTabularizationResources(const std::string &key);

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
            const std::set<std::string> &serializedKeys,
            boost::asio::thread_pool &threadPool,
            std::vector<std::future<GLEntity *>> &futures);

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

        /**
         * @brief Get a Bullet entity by key.
         * @param key The key by which a Bullet is registered.
         * @return A Bullet entity.
         */
        SharedEntity &GetBullet(const std::string &key) const;

        /**
         * @brief Get all Bullet entity by keys.
         * @return All Bullet entity keys.
         */
        std::vector<Key> GetBulletKeys() const;

        /**
         * @brief Get all Bullet entity by keys which have gone out of scope.
         * @return All Bullet entity keys which have gone out of scope.
         */
        const std::set<Key> &GetOutOfScopeBulletKeys() const;

        /**
         * @brief Clear all Bullet entity by keys which have gone out of scope.
         */
        void ClearOutOfScopeBulletKeys();

        /**
         * @brief Remove a Bullet by entity key and clear serialized keys since the last save operation.
         * @param serializedKeys The entity keys which have been serialized last.
         */
        void RemoveBullet(const Key &key, const std::set<std::string> &serializedKeys);

        /**
         * @brief Add a Bullet by entity.
         * @param bullet The entity of a bullet.
         */
        void AddBullet(const SharedEntity &bullet);

        /**
         * @brief Save the entity data to a property tree.
         * @param tree The property tree to save data into.
         * @param path The path within the tree where the data should be saved.
         */
        void Save(boost::property_tree::ptree &tree, const std::string &path) const override;

        /**
         * @brief Load the entity data from a property tree.
         * @param tree The property tree containing the data.
         * @param path The path within the tree where the data is stored.
         */
        void Load(boost::property_tree::ptree &tree, const std::string &path) override;

        /**
         * @brief Save the entity data to a database.
         * @param tree The property tree containing the entity's data.
         * @param database The SQLite database instance to save data into.
         */
        void Save(boost::property_tree::ptree &tree, database_adapters::Sqlite &database) const override;

        /**
         * @brief Load the entity data from a database.
         * @param tree The property tree to populate with the entity's data.
         * @param database The SQLite database instance to load data from.
         */
        void Load(boost::property_tree::ptree &tree, database_adapters::Sqlite &database) override;

        /**
         * @brief Get the unique key for the ship.
         * @return The ship's key as a string.
         */
        static std::string ShipKey();

        /**
         * @brief Return the orientation unit matrix.
         * @return The orientation matrix.
         */
        const Resource2DGLfloat &GetUnitOrientation() const;

        /**
         * @brief Return the ship vertices.
         * @return The vertices matrix.
         */
        const Resource2DGLfloat &GetShipVertices() const;

        /**
         * @brief Return the ship indices.
         * @return The indices array.
         */
        const ResourceGLubyte &GetShipIndices() const;

    private:
        /**
         * @brief Generate a unique identifier for the ship.
         * @return A unique identifier string.
         */
        std::string GenerateUUID() const;

        /**
         * @brief Recompute the ship velocity given the added thrust.
         * @param _thrust The added thrust to the ship.
         */
        void RecomputeShipVelocity(const GLfloat _thrust);

        /**
         * @brief Update the orientation per time step.
         */
        void ChangeShipOrientation();

        /**
         * @brief Move the ship per time step.
         */
        void MoveShip();

        /**
         * @brief Teleport the ship if it moves out of bounds per time step.
         */
        void WrapAroundMoveShip();

        /**
         * @brief Update the bullets per time step.
         * @param serializedKeys The set of bullet keys to serialize on save operation.
         * @param threadPool The thread pool for parallel bullet calculations.
         * @param futures The vector of calculation futures to be returned.
         */
        void UpdateBullets(
            const std::set<std::string> &serializedKeys,
            boost::asio::thread_pool &threadPool,
            std::vector<std::future<GLEntity *>> &futures);

        /**
         * @brief Update the bullets per time step.
         * @param sharedBullet The entity to update.
         */
        void UpdateBulletTask(GLEntity *sharedBullet);

        mutable std::set<std::string> outOfScopeBulletKeys_;

        bool bulletFired_{false};
        bool doRotate_{false};
        GLfloat orientationAngle_{static_cast<GLfloat>(M_PI) / 2};

        Resource2DGLfloat unitOrientation_;
        Resource2DGLfloat shipVertices_;
        ResourceGLubyte shipIndices_;
    };

} // end asteroids

#endif // asteroids_ship_h
