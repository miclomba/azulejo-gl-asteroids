#include "gl/GLWindow.h"

#include <array>
#include <string>

#include "Entities/Entity.h"

using entity::Entity;

using asteroids::GLWindow;

namespace
{
	const std::string GAME_WIN_KEY = "GameWindow";
	const int WIN_WIDTH = 600;
	const int WIN_HEIGHT = 480;
	const int INIT_WIN_X = 100;
	const int INIT_WIN_Y = 100;
} // end namespace

GLWindow::~GLWindow() = default;

GLWindow::GLWindow() : Entity()
{
	SetKey(GAME_WIN_KEY);

	std::fill(projOrtho_.begin(), projOrtho_.end(), 0.0f);
	std::fill(projPerspective_.begin(), projPerspective_.end(), 0.0f);

	w_ = WIN_WIDTH;
	h_ = WIN_HEIGHT;
}

void GLWindow::SetHeight(const GLint h)
{
	h_ = h;
}

void GLWindow::SetWidth(const GLint w)
{
	w_ = w;
}

void GLWindow::SetProjOrthoMatrix(std::array<GLfloat, 16> projOrtho)
{
	projOrtho_ = std::move(projOrtho);
}

void GLWindow::SetProjOrthoMatrix(const GLfloat val, const int index)
{
	projOrtho_[index] = val;
}

void GLWindow::SetProjPerspectiveMatrix(std::array<GLfloat, 16> projPerspective)
{
	projPerspective_ = std::move(projPerspective);
}

void GLWindow::SetProjPerspectiveMatrix(const GLfloat val, const int index)
{
	projPerspective_[index] = val;
}

GLint GLWindow::GetHeight() const
{
	return h_;
}

GLint GLWindow::GetWidth() const
{
	return w_;
}

const std::array<GLfloat, 16> &GLWindow::GetProjOrthoMatrix() const
{
	return projOrtho_;
}

const std::array<GLfloat, 16> &GLWindow::GetProjPerspectiveMatrix() const
{
	return projPerspective_;
}