/**
 * @file Bullet.h
 * @brief Declaration of the Bullet class which represents bullets fired by the ship in the Asteroids game.
 */

#ifndef asteroids_bullet_h
#define asteroids_bullet_h

#include <array>
#include <string>

#include <boost/property_tree/ptree.hpp>

#include "test_filesystem_adapters/ContainerResource.h"
#include "test_filesystem_adapters/ContainerResource2D.h"

#include "config/config.h"
#include "GLEntity.h"

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
     * @class Bullet
     * @brief A class representing a bullet fired by the ship in the Asteroids game.
     *
     * This class manages bullet properties such as position, movement, and state.
     */
    class ASTEROIDS_DLL_EXPORT Bullet : public GLEntity
    {
    public:
        /**
         * @brief Default constructor for the Bullet class.
         */
        Bullet();

        /**
         * @brief Constructor to initialize a bullet at a given position.
         * @param _x Initial x-coordinate.
         * @param _y Initial y-coordinate.
         */
        Bullet(const GLfloat _x, const GLfloat _y);

        /**
         * @brief Destructor for the Bullet class.
         */
        virtual ~Bullet();

        /**
         * @brief Copy constructor.
         * @param other The Bullet instance to copy from.
         */
        Bullet(const Bullet &other);

        /**
         * @brief Move constructor.
         * @param other The Bullet instance to move from.
         */
        Bullet(Bullet &&other);

        /**
         * @brief Copy assignment operator.
         * @param other The Bullet instance to copy from.
         * @return Reference to the updated instance.
         */
        Bullet &operator=(const Bullet &other);

        /**
         * @brief Move assignment operator.
         * @param other The Bullet instance to move from.
         * @return Reference to the updated instance.
         */
        Bullet &operator=(Bullet &&other);

        /**
         * @brief Register serialization resources for the bullet.
         * @param key The serialization key.
         */
        static void RegisterSerializationResources(const std::string &key);

        /**
         * @brief Register tabularization resources for the bullet.
         * @param key The tabularization key.
         */
        static void RegisterTabularizationResources(const std::string &key);

        /**
         * @brief Update the bullet's position based on velocity and speed.
         * @param _velocityAngle The angle of movement.
         * @param _speed The speed of the bullet.
         */
        void Update(const GLfloat _velocityAngle, const GLfloat _speed);

        /**
         * @brief Draw the bullet.
         */
        void Draw() override;

        /**
         * @brief Check if the bullet is out of bounds.
         * @return True if the bullet is out of bounds, otherwise false.
         */
        bool IsOutOfBounds() const;

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
         * @brief Get the prefix used for bullet serialization.
         * @return The bullet prefix as a string.
         */
        static std::string BulletPrefix();

    private:
        /**
         * @brief Initialize the bullet with velocity and speed.
         * @param _velocityAngle The angle of movement.
         * @param _speed The speed of the bullet.
         */
        void InitializeBullet(const GLfloat _velocityAngle, const GLfloat _speed);

        /**
         * @brief Set the scale transformation matrix for the bullet.
         */
        void SetSMatrix();

        /**
         * @brief Set the translation transformation matrix for the bullet.
         */
        void SetTMatrix();

        /**
         * @brief Mark the bullet as out of bounds.
         */
        void SetBulletOutOfBounds();

        bool bulletInitialized_ = false; /**< Indicates whether the bullet has been initialized. */
        bool outOfBounds_ = false;       /**< Flag indicating if the bullet is out of bounds. */

        Resource2DGLfloat bulletVertices_;   /**< Vertex data for the bullet. */
        ResourceGLubyte bulletIndices_;      /**< Index data for the bullet rendering. */
        Resource2DGLfloat projectionMatrix_; /**< Projection matrix for the bullet rendering. */
    };

} // end namespace asteroids

#endif // asteroids_bullet_h
