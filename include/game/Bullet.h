/**
 * @file Bullet.h
 * @brief Declaration of the Bullet class, representing projectiles in the Asteroids game.
 * @note This enhanced version includes support for different bullet types,
 * data-driven properties, and lifetime management.
 */

#ifndef ASTEROIDS_BULLET_H
#define ASTEROIDS_BULLET_H

#include <string>
#include <vector>
#include <memory>

#include <boost/property_tree/ptree.hpp>

#include "configuration/config.h"
#include "gl/GLEntity.h"
#include "math/Vector2D.h" // Assuming a simple 2D vector class exists

// Forward declarations
namespace database_adapters {
    class Sqlite;
}

namespace asteroids {

/**
 * @enum class BulletType
 * @brief Defines different types of bullets with distinct behaviors.
 */
enum class BulletType {
    Standard,
    Piercing,
    Explosive,
    Homing
};

/**
 * @struct BulletStats
 * @brief Data-driven structure holding properties for a bullet type.
 * This allows tweaking gameplay without recompiling.
 */
struct BulletStats {
    float speed = 10.0f;
    float lifetime = 2.0f; // in seconds
    float damage = 1.0f;
    // Visual properties
    std::vector<GLfloat> vertices;
    std::vector<GLubyte> indices;
    // Behavior flags
    bool canPierce = false;
};

/**
 * @class Bullet
 * @brief A class representing a bullet fired in the game.
 *
 * This class manages bullet properties such as position, velocity, lifetime,
 * and visual representation. It is designed to be updated per frame.
 */
class ASTEROIDS_DLL_EXPORT Bullet : public GLEntity {
public:
    /**
     * @brief Constructor to create a bullet with specific properties.
     * @param startPosition The initial position (x, y) of the bullet.
     * @param direction The normalized direction vector for the bullet's travel.
     * @param stats A shared pointer to the stats for this bullet type.
     */
    Bullet(const Vector2D& startPosition, const Vector2D& direction, std::shared_ptr<const BulletStats> stats);

    /**
     * @brief Destructor.
     */
    virtual ~Bullet() = default;

    // C++ Rule of 5: Explicitly default or delete constructors/operators
    Bullet(const Bullet&) = delete; // Bullets are unique entities, copying is disabled.
    Bullet& operator=(const Bullet&) = delete;
    Bullet(Bullet&&) = default; // Move semantics are fine.
    Bullet& operator=(Bullet&&) = default;

    /**
     * @brief Updates the bullet's state over time.
     * @param deltaTime The time elapsed since the last frame.
     */
    void Update(float deltaTime);

    /**
     * @brief Draws the bullet to the screen.
     */
    void Draw() override;

    /**
     * @brief Checks if the bullet's lifetime has expired.
     * @return True if the bullet should be destroyed, false otherwise.
     */
    [[nodiscard]] bool HasExpired() const;

    /**
     * @brief Gets the damage this bullet inflicts.
     * @return The damage value.
     */
    [[nodiscard]] float GetDamage() const;
    
    /**
     * @brief Gets the piercing capability of the bullet.
     * @return True if the bullet can pierce through objects.
     */
    [[nodiscard]] bool CanPierce() const;

    // --- Persistence ---
    // The persistence logic remains similar, but now saves/loads more state.

    void Save(boost::property_tree::ptree& tree, const std::string& path) const override;
    void Load(boost::property_tree::ptree& tree, const std::string& path) override;
    void Save(boost::property_tree::ptree& tree, database_adapters::Sqlite& database) const override;
    void Load(boost::property_tree::ptree& tree, database_adapters::Sqlite& database) override;

    /**
     * @brief Gets the prefix used for bullet serialization.
     * @return The bullet prefix as a string.
     */
    static std::string BulletPrefix();

private:
    /**
     * @brief Sets the model-to-world transformation matrix.
     */
    void UpdateTransformMatrix();

    std::shared_ptr<const BulletStats> stats_; ///< Data-driven properties of the bullet.
    Vector2D velocity_;                         ///< The bullet's velocity vector (direction * speed).
    float remainingLifetime_;                   ///< Time remaining before the bullet expires.

    // GL resources would be initialized from stats_ in the constructor
    // For example, VBOs and IBOs for vertices and indices.
};

} // namespace asteroids

#endif // ASTEROIDS_BULLET_H
