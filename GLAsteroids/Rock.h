#ifndef asteroids_rock_h
#define	asteroids_rock_h

#include <array>
#include <cstdlib>

#include "config.h"
#include "GLEntity.h"

namespace asteroids {

enum class State {LARGE,MEDIUM,SMALL};

class Rock : public GLEntity 
{
public:

	Rock(const State _state, const GLfloat _x, const GLfloat _y, const GLint _index);
	void Draw(const GLfloat _velocityAngle, const GLfloat _speed, const GLfloat _spin);

    GLint GetIndex() const;
    GLfloat& GetSpin();
    GLfloat& GetSpinEpsilon();
    GLint GetSpinDirection() const;
    State GetState() const;
    void SetRockInitialized(const bool state);

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
    std::array<Row3, 8> rockVertices_;
    std::array<GLubyte, 24> rockIndices_;

    Rock();
};

} // end asteroids
#endif	// asteroids_rock_h

