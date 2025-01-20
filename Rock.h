/**
 * @file Rock.h
 * @brief Declaration of the Rock class which represents asteroids in the Asteroids game.
 */

#ifndef asteroids_rock_h
#define asteroids_rock_h

#include <array>
#include <cstdlib>
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
using Resource2DGLfloat = ContainerResource2D<GLfloat>;

/**
 * @enum State
 * @brief Enumeration for the size of the rock.
 */
enum class State { LARGE, MEDIUM, SMALL };

/**
 * @class Rock
 * @brief A class representing an asteroid in the Asteroids game.
 *
 * This class handles the rock's movement, spinning, and interaction within the game.
 */
class ASTEROIDS_DLL_EXPORT Rock : public GLEntity
{
public:
    /**
     * @brief Default constructor for the Rock class.
     */
    Rock();

    /**
     * @brief Parameterized constructor for initializing a rock with a given state and position.
     * @param _state The size of the rock (LARGE, MEDIUM, SMALL).
     * @param _x The initial x-coordinate.
     * @param _y The initial y-coordinate.
     */
    Rock(const State _state, const GLfloat _x, const GLfloat _y);

    /**
     * @brief Destructor for the Rock class.
     */
    ~Rock();

    Rock(const Rock&);
    Rock(Rock&&);
    Rock& operator=(const Rock&);
    Rock& operator=(Rock&&);

    /**
     * @brief Register serialization resources for the rock.
     * @param key The serialization key.
     */
    static void RegisterSerializationResources(const std::string& key);

    /**
     * @brief Register tabularization resources for the rock.
     * @param key The tabularization key.
     */
    static void RegisterTabularizationResources(const std::string& key);

    /**
     * @brief Update the rock's position and spin.
     * @param _velocityAngle The velocity angle.
     * @param _speed The speed of the rock.
     * @param _spin The spinning rate.
     */
    void Update(const GLfloat _velocityAngle, const GLfloat _speed, const GLfloat _spin);

    /**
     * @brief Draw the rock.
     */
    void Draw() override;

    /**
     * @brief Get the current spin of the rock.
     * @return The rock's spin value.
     */
    GLfloat GetSpin() const;

    /**
     * @brief Get the epsilon value used for spin calculations.
     * @return The spin epsilon value.
     */
    GLfloat GetSpinEpsilon() const;

    /**
     * @brief Get the spin direction.
     * @return The spin direction as an integer.
     */
    GLint GetSpinDirection() const;

    /**
     * @brief Get the current size state of the rock.
     * @return The state of the rock.
     */
    State GetState() const;

    /**
     * @brief Set whether the rock has been initialized.
     * @param state The initialization state.
     */
    void SetRockInitialized(const bool state);

    /**
     * @brief Set the spin value of the rock.
     * @param spin The new spin value.
     */
    void SetSpin(const GLfloat spin);

    /**
     * @brief Set the spin epsilon value.
     * @param spinEpsilon The new spin epsilon value.
     */
    void SetSpinEpsilon(const GLfloat spinEpsilon);

    void Save(boost::property_tree::ptree& tree, const std::string& path) const override;
    void Load(boost::property_tree::ptree& tree, const std::string& path) override;

    void Save(boost::property_tree::ptree& tree, database_adapters::Sqlite& database) const override;
    void Load(boost::property_tree::ptree& tree, database_adapters::Sqlite& database) override;

    /**
     * @brief Get the prefix used for rock serialization.
     * @return The rock prefix as a string.
     */
    static std::string RockPrefix();

private:
    void InitializeRock(const GLfloat _velocityAngle, const GLfloat _speed, const GLfloat _spin);
    void UpdateSpin();
    void MoveRock();
    void WrapAroundMoveRock();

    GLint index_; /**< Rock index in the game. */
    GLfloat epsilon_; /**< Small adjustment value for movement calculations. */
    GLfloat spin_{0.0f}; /**< Rock spin value. */
    GLfloat spinEpsilon_{0.01f}; /**< Increment value for spin calculations. */
    GLint spinDirection_; /**< Direction of the rock spin. */
    State state_; /**< Current state (size) of the rock. */
    bool rockInitialized_{false}; /**< Indicates whether the rock is initialized. */

    Resource2DGLfloat rockVertices_; /**< Vertices of the rock shape. */
    ResourceGLubyte rockIndices_; /**< Indices for rendering the rock. */
};

} // end asteroids

#endif // asteroids_rock_h
