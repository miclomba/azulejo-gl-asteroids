/**
 * @file GLEntity.h
 * @brief Declaration of the GLEntity class representing an OpenGL entity with transform properties.
 */

#ifndef asteroids_glentity_h 
#define asteroids_glentity_h	

#include <array>
#include <math.h>
#include <string>

#include <boost/property_tree/ptree.hpp>

#include "config.h"
#include "DatabaseAdapters/ITabularizableEntity.h"
#include "DatabaseAdapters/Sqlite.h"
#include "FilesystemAdapters/ISerializableEntity.h"
#include "test_filesystem_adapters/ContainerResource2D.h"

namespace asteroids {

using Resource2DGLfloat = ContainerResource2D<GLfloat>;

/**
 * @class GLEntity
 * @brief A class representing an OpenGL entity with scale, translation, and rotation transformations.
 *
 * This class supports both serialization and database tabularization operations.
 */
class ASTEROIDS_DLL_EXPORT GLEntity : 
    public filesystem_adapters::ISerializableEntity, 
    public database_adapters::ITabularizableEntity
{
public:
    /**
     * @brief Default constructor for GLEntity.
     */
    GLEntity();

    /**
     * @brief Destructor for GLEntity.
     */
    ~GLEntity();

    /**
     * @brief Copy constructor.
     * @param other The GLEntity instance to copy from.
     */
    GLEntity(const GLEntity& other);

    /**
     * @brief Move constructor.
     * @param other The GLEntity instance to move from.
     */
    GLEntity(GLEntity&& other);

    /**
     * @brief Copy assignment operator.
     * @param other The GLEntity instance to copy from.
     * @return Reference to the updated instance.
     */
    GLEntity& operator=(const GLEntity& other);

    /**
     * @brief Move assignment operator.
     * @param other The GLEntity instance to move from.
     * @return Reference to the updated instance.
     */
    GLEntity& operator=(GLEntity&& other);

    /**
     * @brief Draw the entity using OpenGL.
     */
    virtual void Draw();

    /**
     * @brief Get the transformation matrix describing the entity's geometry.
     * @return Reference to the frame matrix.
     */
    Resource2DGLfloat& GetFrame();

    /**
     * @brief Get the unit velocity matrix of the entity.
     * @return Reference to the unit velocity matrix.
     */
    Resource2DGLfloat& GetUnitVelocity();

    /**
     * @brief Get the entity's speed.
     * @return The speed of the entity.
     */
    GLfloat GetSpeed() const;

    /**
     * @brief Get the entity's mass.
     * @return The mass of the entity.
     */
    GLfloat GetMass() const;

    /**
     * @brief Get the velocity angle of the entity.
     * @return The velocity angle.
     */
    GLfloat GetVelocityAngle() const;

    /**
     * @brief Get the scale transformation matrix.
     * @return A constant reference to the scale matrix.
     */
    const Resource2DGLfloat& SMatrix() const;

    /**
     * @brief Get the translation transformation matrix.
     * @return A constant reference to the translation matrix.
     */
    const Resource2DGLfloat& TMatrix() const;

    /**
     * @brief Get the rotation transformation matrix.
     * @return A constant reference to the rotation matrix.
     */
    const Resource2DGLfloat& RMatrix() const;

    /**
     * @brief Set a value in the frame matrix.
     * @param i Row index.
     * @param j Column index.
     * @param val Value to set.
     */
    void SetFrame(int i, int j, GLfloat val);

    /**
     * @brief Set the entire frame matrix.
     * @param frame The frame matrix to set.
     */
    void SetFrame(const Resource2DGLfloat& frame);

    /**
     * @brief Set a value in the unit velocity matrix.
     * @param i Row index.
     * @param j Column index.
     * @param val Value to set.
     */
    void SetUnitVelocity(int i, int j, GLfloat val);

    /**
     * @brief Set the unit velocity matrix.
     * @param unitVelocity The unit velocity matrix to set.
     */
    void SetUnitVelocity(const Resource2DGLfloat& unitVelocity);

    void SetSpeed(const GLfloat speed);
    void SetMass(const GLfloat mass);
    void SetVelocityAngle(const GLfloat angle);

    static void RegisterSerializationResources(const std::string& key);
    static void RegisterTabularizationResources(const std::string& key);

protected:
    Resource2DGLfloat S_; /**< Scale transformation matrix. */
    Resource2DGLfloat T_; /**< Translation transformation matrix. */
    Resource2DGLfloat R_; /**< Rotation transformation matrix. */

    void Save(boost::property_tree::ptree& tree, const std::string& path) const override;
    void Load(boost::property_tree::ptree& tree, const std::string& path) override;

    void Save(boost::property_tree::ptree& tree, database_adapters::Sqlite& database) const override;
    void Load(boost::property_tree::ptree& tree, database_adapters::Sqlite& database) override;

    SharedEntity& GetAggregatedMember(const Key& key) const override;

private:
    Resource2DGLfloat frame_; /**< Matrix representing entity geometry. */
    Resource2DGLfloat unitVelocity_; /**< Unit velocity matrix. */

    GLfloat velocityAngle_ = 0.0; /**< Velocity angle. */
    GLfloat speed_ = 0.0; /**< Speed of the entity. */
    GLfloat mass_ = 1.0; /**< Mass of the entity. */
};

} // end asteroids

#endif // asteroids_glentity_h
