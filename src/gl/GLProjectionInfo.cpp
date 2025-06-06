#include "gl/GLProjectionInfo.h"

#include <array>
#include <string>

#include "Entities/Entity.h"

using entity::Entity;

using asteroids::GLProjectionInfo;

namespace
{
	const std::string GAME_PROJ_INFO_KEY = "GameProjectionInfo";
	const int WIN_WIDTH = 600;
	const int WIN_HEIGHT = 480;
	const int INIT_WIN_X = 100;
	const int INIT_WIN_Y = 100;
} // end namespace

GLProjectionInfo::~GLProjectionInfo() = default;

GLProjectionInfo::GLProjectionInfo() : Entity()
{
	SetKey(GAME_PROJ_INFO_KEY);

	std::fill(projOrtho_.begin(), projOrtho_.end(), 0.0f);
	std::fill(projPerspective_.begin(), projPerspective_.end(), 0.0f);

	w_ = WIN_WIDTH;
	h_ = WIN_HEIGHT;
}

void GLProjectionInfo::SetHeight(const GLint h)
{
	h_ = h;
}

void GLProjectionInfo::SetWidth(const GLint w)
{
	w_ = w;
}

void GLProjectionInfo::SetProjOrthoMatrix(std::array<GLfloat, 16> projOrtho)
{
	projOrtho_ = std::move(projOrtho);
}

void GLProjectionInfo::SetProjOrthoMatrix(const GLfloat val, const int index)
{
	projOrtho_[index] = val;
}

void GLProjectionInfo::SetProjPerspectiveMatrix(std::array<GLfloat, 16> projPerspective)
{
	projPerspective_ = std::move(projPerspective);
}

void GLProjectionInfo::SetProjPerspectiveMatrix(const GLfloat val, const int index)
{
	projPerspective_[index] = val;
}

GLint GLProjectionInfo::GetHeight() const
{
	return h_;
}

GLint GLProjectionInfo::GetWidth() const
{
	return w_;
}

const std::array<GLfloat, 16> &GLProjectionInfo::GetProjOrthoMatrix() const
{
	return projOrtho_;
}

const std::array<GLfloat, 16> &GLProjectionInfo::GetProjPerspectiveMatrix() const
{
	return projPerspective_;
}