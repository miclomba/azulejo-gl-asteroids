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

#include "config.h"
#include "GLEntity.h"

namespace database_adapters {
class Sqlite;
} // end database_adapters

namespace asteroids {

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
    ~Bullet();

    Bullet(const Bullet&);
    Bullet(Bullet&&);
    Bullet& operator=(const Bullet&);
    Bullet& operator=(Bullet&&);

    /**
     * @brief Register serialization resources for the bullet.
     * @param key The serialization key.
     */
    static void RegisterSerializationResources(const std::string& key);

    /**
     * @brief Register tabularization resources for the bullet.
     * @param key The tabularization key.
     */
    static void RegisterTabularizationResources(const std::string& key);

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

    void Save(boost::property_tree::ptree& tree, const std::string& path) const override;
    void Load(boost::property_tree::ptree& tree, const std::string& path) override;

    void Save(boost::property_tree::ptree& tree, database_adapters::Sqlite& database) const override;
    void Load(boost::property_tree::ptree& tree, database_adapters::Sqlite& database) override;

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
    bool outOfBounds_ = false; /**< Flag indicating if the bullet is out of bounds. */

    Resource2DGLfloat bulletVertices_; /**< Vertex data for the bullet. */
    ResourceGLubyte bulletIndices_; /**< Index data for the bullet rendering. */
    Resource2DGLfloat projectionMatrix_; /**< Projection matrix for the bullet rendering. */
};

} // end namespace asteroids

#endif // asteroids_bullet_h
