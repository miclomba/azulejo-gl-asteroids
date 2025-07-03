#include "gl/GL.h"
#include "gl/GLBackend.h"

#include <algorithm>
#include <string>

using asteroids::GL;
using asteroids::GLBackend;

std::unique_ptr<GL> GL::instance_ = nullptr;

GL &GL::Get()
{
	if (!GL::instance_)
	{
		GL::instance_.reset(new GL());
	}
	return *GL::instance_;
}

GL::~GL() = default;

GL::GL() = default;

void GL::InitOpenGLFunctions()
{
	// Qt Initialize OpenGL Functions
	initializeOpenGLFunctions();

	InitServer();
	InitClient();
}

void GL::DisplayClear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GL::DisplayFlush()
{
	glFlush();
}

void GL::InitServer()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);
	glDepthRange(0, 1);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void GL::InitClient() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
}

void GL::Reshape(const int _w, const int _h)
{
	//========================= DEFINE VIEWPORT ==============================*/
	// define the viewport within the window where NDC will map to
	glViewport(0, 0, _w, _h);
	// define pixel clipping zone
	glScissor(0, 0, _w, _h);
	/*========================= ORTHO PROJECTION =============================*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (_w <= _h)
		glOrtho(-10.0, 10.0, -10.0 * ((GLfloat)_h / (GLfloat)_w),
				10.0 * ((GLfloat)_h / (GLfloat)_w), 10, -10);
	else
		glOrtho(-10.0 * ((GLfloat)_w / (GLfloat)_h),
				10.0 * ((GLfloat)_w / (GLfloat)_h), -10.0, 10.0, 10, -10);

	/*========================= REDISPLAY ====================================*/
	glMatrixMode(GL_MODELVIEW);
}
