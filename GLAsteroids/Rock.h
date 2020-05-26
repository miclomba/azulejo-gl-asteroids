#ifndef asteroids_rock_h
#define	asteroids_rock_h

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

enum class State {LARGE,MEDIUM,SMALL};

class ASTEROIDS_DLL_EXPORT Rock : public GLEntity
{
public:

	Rock();
	Rock(const State _state, const GLfloat _x, const GLfloat _y);
	~Rock();
	Rock(const Rock&);
	Rock(Rock&&);
	Rock& operator=(const Rock&);
	Rock& operator=(Rock&&);

	static void RegisterResources(const std::string& key);

	void Draw(const GLfloat _velocityAngle, const GLfloat _speed, const GLfloat _spin);

    GLfloat GetSpin() const;
    GLfloat GetSpinEpsilon() const;
    GLint GetSpinDirection() const;
    State GetState() const;
    void SetRockInitialized(const bool state);

    void SetSpin(const GLfloat);
    void SetSpinEpsilon(const GLfloat);

	void Save(boost::property_tree::ptree& tree, const std::string& path) const override;
	void Load(boost::property_tree::ptree& tree, const std::string& path) override;

	void Save(database_adapters::Sqlite& database) const override;
	void Load(database_adapters::Sqlite& database) override;

	static std::string RockPrefix();

private:
	void InitializeRock(const GLfloat _velocityAngle, const GLfloat _speed, const GLfloat _spin);
	void UpdateSpin();
	void MoveRock();
	void WrapAroundMoveRock();
	void DrawRock();

    GLint index_;
    /*=============================== State ==================================*/
	GLfloat spin_{0.0f};
	GLfloat spinEpsilon_{0.01f};
    GLint spinDirection_;
    State state_;
	bool rockInitialized_{false};
    /*=============================== Geometry ===============================*/
    Resource2DGLfloat rockVertices_;
	ResourceGLubyte rockIndices_;
};

} // end asteroids
#endif	// asteroids_rock_h

