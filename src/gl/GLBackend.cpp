#include "gl/GLBackend.h"

#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include <QKeyEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QTimer>

#include "gl/GL.h"
#include "gl/GLBackendEmitters.h"

using asteroids::GL;
using asteroids::GLBackend;
using asteroids::GLBackendEmitters;
using events::EventEmitter;

namespace
{
	static constexpr int FRAME_MS = 20;
	const int WIN_WIDTH = 600;
	const int WIN_HEIGHT = 480;
	const int NUMBER_KEYS = 256;
	const int INIT_WIN_X = 100;
	const int INIT_WIN_Y = 100;
	const std::string ASTEROIDS_TITLE = "Asteroids";
} // end namespace

std::unique_ptr<GLBackend> GLBackend::callbackInstance_ = nullptr;

GLBackend &GLBackend::Get()
{
	if (!GLBackend::callbackInstance_)
	{
		GLBackend::callbackInstance_.reset(new GLBackend());
	}
	return *GLBackend::callbackInstance_;
}

GLBackend::~GLBackend() = default;

GLBackend::GLBackend() : QOpenGLWidget(),
						 frameTimer_(std::make_unique<QTimer>(this)),
						 keysPressed_({})
{
	// Initialize the graphics library singleton
	GL::Get();

	// Make this a repeating timer:
	frameTimer_->setInterval(FRAME_MS);
	frameTimer_->setSingleShot(false);

	// connect QTimer to repaint
	connect(frameTimer_.get(), &QTimer::timeout, this, &GLBackend::onFrame);

	// initialize the window
	setWindowTitle(QString::fromStdString(ASTEROIDS_TITLE));
	setGeometry(INIT_WIN_X, INIT_WIN_Y, WIN_WIDTH, WIN_HEIGHT);
	show();
}

void GLBackend::Run()
{
	emitters_.GetRunEmitter()->Signal()();
}

GLBackendEmitters &GLBackend::GetEmitters()
{
	return emitters_;
}

void GLBackend::initializeGL()
{
	GL::Get().InitOpenGLFunctions();

	// make sure we can actually receive key events:
	setFocusPolicy(Qt::StrongFocus);
	setFocus();

	// start the timer loop
	frameTimer_->start();
}

void GLBackend::onFrame()
{
	// schedule paintGL() to repaint
	update();
}

void GLBackend::paintGL()
{
	KeyboardUpdateState();

	GL &gl = GL::Get();
	gl.DisplayClear();

	emitters_.GetDrawEmitter()->Signal()();

	gl.DisplayFlush();
}

void GLBackend::resizeGL(const int _w, const int _h)
{
	// Qt’s backing FBO is actually (w * DPR) × (h * DPR)
	const qreal dpr = devicePixelRatioF(); // typically 2.0 on Retina

	int pw = qRound(_w * dpr);
	int ph = qRound(_h * dpr);

	GL::Get().Reshape(pw, ph);
}

void GLBackend::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape)
	{
		close();
	}

	QString txt = event->text();
	if (!txt.isEmpty())
	{
		unsigned char c = txt.at(0).toLatin1();
		keysPressed_[c] = true;
	}
}

void GLBackend::keyReleaseEvent(QKeyEvent *event)
{
	auto txt = event->text();
	if (!txt.isEmpty())
	{
		unsigned char c = txt.at(0).toLatin1();
		keysPressed_[c] = false;
	}
}

void GLBackend::KeyboardUpdateState()
{
	for (int i = 0; i < NUMBER_KEYS; i++)
	{
		if (keysPressed_[i])
		{
			switch (i)
			{
			case 's':
				emitters_.GetLeftArrowEmitter()->Signal()();
				break;
			case 'f':
				emitters_.GetRightArrowEmitter()->Signal()();
				break;
			case 'e':
				emitters_.GetThrustEmitter()->Signal()();
				break;
			case 'j':
				emitters_.GetFireEmitter()->Signal()();
				break;
			case 'x':
				emitters_.GetResetEmitter()->Signal()();
				break;
			case 'u':
				emitters_.GetSerializeEmitter()->Signal()();
				break;
			case 'i':
				emitters_.GetDeserializeEmitter()->Signal()();
				break;
			default:
				break;
			}
		}
	}
}
