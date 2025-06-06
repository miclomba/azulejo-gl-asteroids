/**
 * @file Asteroids.h
 * @brief Declaration of the Asteroids class which implements the game logic for the classic Asteroids game.
 */

#ifndef asteroids_asteroids_h
#define asteroids_asteroids_h

#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#include <boost/asio/thread_pool.hpp>
#include <boost/property_tree/ptree.hpp>

#include "Events/EventConsumer.h"
#include "configuration/config.h"
#include "game/Rock.h"
#include "game/Ship.h"
#include "gl/GLEntity.h"

namespace database_adapters
{
    class Sqlite;
} // end namespace database_adapters

namespace asteroids
{

    /**
     * @class Asteroids
     * @brief A class that handles game logic for the Asteroids game.
     *
     * The class manages game entities such as rocks and ships, processes game events,
     * and handles serialization and deserialization of game state.
     */
    class ASTEROIDS_DLL_EXPORT Asteroids : public GLEntity
    {
    public:
        /**
         * @brief Default constructor for the Asteroids class.
         */
        Asteroids();

        /**
         * @brief Destructor for the Asteroids class.
         */
        virtual ~Asteroids();

        Asteroids(const Asteroids &) = delete;
        Asteroids(Asteroids &&) = delete;
        Asteroids &operator=(const Asteroids &) = delete;
        Asteroids &operator=(Asteroids &&) = delete;

        /**
         * @brief Save the game state to a JSON file.
         * @param tree The property tree to save data into.
         * @param path The file path to save the JSON data.
         */
        void Save(boost::property_tree::ptree &tree, const std::string &path) const override;

        /**
         * @brief Load the game state from a JSON file.
         * @param tree The property tree to load data from.
         * @param path The file path to load the JSON data from.
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
         * @brief Render the game entities.
         * @param w_ the width of the window
         * @param h_ the height of the window
         * @param projOrtho_ the orthogonal projection matrix
         * @param projPerspective_ the perspective projection matrix
         */
        void Draw(GLint w_, GLint h_, const std::array<GLfloat, 16> &projOrtho_, const std::array<GLfloat, 16> &projPerspective_);

        /**
         * @brief Fire a bullet from the ship.
         */
        void Fire();

        /**
         * @brief Rotate the ship counter-clockwise.
         */
        void RotateLeft();

        /**
         * @brief Rotate the ship clockwise.
         */
        void RotateRight();

        /**
         * @brief Apply thrust to the ship.
         */
        void Thrust();

        /**
         * @brief Reset the game to start a new session.
         */
        void ResetGame();

        /**
         * @brief Start the game loop.
         */
        void Run();

        /**
         * @brief Serialize the game state.
         */
        void Serialize();

        /**
         * @brief Deserialize the game state.
         */
        void Deserialize();

    private:
        /**
         * @brief Generate a UUID.
         * @return A UUID.
         */
        std::string GenerateUUID() const;

        /**
         * @brief Get the aggregated ship entity.
         * @return A ship entity.
         */
        SharedEntity &GetShip();

        /**
         * @brief Get the aggregated rock entity by key.
         * @param key The key of the rock entity.
         * @return A rock entity.
         */
        SharedEntity &GetRock(const std::string &key);

        /**
         * @brief Get the aggregated rock entity keys.
         * @return A vector of aggregated rock entity keys.
         */
        std::vector<Key> GetRockKeys() const;

        /**
         * @brief Add out of scope bullet keys to the entity removal list.
         */
        void AddOutOfScopeBulletsToRemovalKeys();

        /**
         * @brief Clear unused serialization keys.
         */
        void ClearUnusedSerializationKeys();

        /**
         * @brief Clear unused tabularization keys.
         */
        void ClearUnusedTabularizationKeys();

        /**
         * @brief Clear or reset the game.
         */
        void ClearGame();

        /**
         * @brief Clear the rock entities.
         */
        void ClearRocks();

        /**
         * @brief Clear the bullet entities.
         */
        void ClearBullets();

        /**
         * @brief Clear the ship entities.
         */
        void ClearShip();

        /**
         * @brief Destroy or remove the Rock entity from the game.
         * @param rock The Rock entity.
         */
        void DestroyRock(Rock *rock);

        /**
         * @brief Destroy or remove the Bullet entity from the game.
         * @param rock The Bullet entity.
         */
        void DestroyBullet(Bullet *bullet);

        /**
         * @brief Get the list of keys to serialize on game save.
         * @return Set of entity keys.
         */
        std::set<std::string> GetKeysToSerialize();

        /**
         * @brief Add a key to the list of entity keys to remove.
         * @param key The entity key to remove.
         */
        void AddToRemoveKeys(const std::string &key);

        /**
         * @brief Split a rock into more smaller rocks.
         * @param rock The Rock entity to split.
         */
        void BreakRock(Rock *rock);

        /**
         * @brief Verify if the game has rocks.
         * @return true if the game has Rock entities; false otherwise.
         */
        bool HasRocks();

        /**
         * @brief Verify if the game has a Rock entity with the given key.
         * @param key Of the Rock entity.
         * @return true if the game has a Rock with the given key; false otherwise.
         */
        bool HasRock(const std::string &key) const;

        /**
         * @brief Make a Rock entity.
         * @param rockSize The size of the Rock entity.
         * @param rock The Rock entity.
         * @param halfMass The mass of the Rock.
         * @param clockwise true if clockwise spinning; false if counter clockwise.
         * @return smart pointer to a Rock entity.
         */
        std::shared_ptr<Rock> MakeRock(const State rockSize, Rock *rock, const bool halfMass, const bool clockwise);

        /**
         * @brief Set the Rock's physics based on a collision with a Bullet.
         * @param ithBullet The ith bullet in the collision.
         * @param rock The Rock entity.
         */
        void CalculateConservationOfMomentum(Bullet *_ithBullet, Rock *rock);

        /**
         * @brief Check for collisions between bullets and rocks, and between rocks and the ship.
         */
        void DetermineCollisions();

        /**
         * @brief Get the Rock which collided with the Ship.
         * @return A Rock if there's a collision; nullptr otherwise.
         */
        Rock *ShipCollision();

        /**
         * @brief Get the Rock which collided with the given Bullet.
         * @param bullet The Bullet entity.
         * @return A Rock if there's a collision; nullptr otherwise.
         */
        Rock *Collision(Bullet *bullet);

        /**
         * @brief Process the collision by breaking or destroying the Rock and Bullet.
         * @param bullet The Bullet entity.
         * @param rockKey The key of the Rock entity.
         */
        void ProcessCollision(Bullet *bullet, const Key &rockKey);

        /**
         * @brief Update the Rock per time step.
         * @param sharedRock The Rock entity.
         */
        void UpdateRockTask(GLEntity *sharedRock);

        /**
         * @brief Update the Ship per time step.
         * @param sharedShip The Ship entity.
         * @param futures The datastructure tracking the futures used in parallel computing of bullet, rock, and ship collisions.
         */
        void UpdateShipTask(GLEntity *sharedShip, std::vector<std::future<GLEntity *>> &futures);

        /**
         * @brief Draw the game entities.
         */
        void DrawGLEntities();

        /**
         * @brief Draw the game information in the UI.
         */
        void DrawGameInfo();

        /**
         * @brief Reset the thrust and rotation variables after user input or game reset.
         */
        void ResetThrustAndRotation();

        std::set<std::string> keysToRemove_;
        std::set<std::string> keysSerialized_;

        GLint score_{0};
        GLint rockCount_{0};
        GLfloat orientationAngle_{0.0f};
        GLfloat thrust_{0.0f};

        boost::asio::thread_pool threadPool_;
        std::mutex rockCollisionMutex_;
    };

} // end namespace asteroids

#endif // asteroids_asteroids_h
