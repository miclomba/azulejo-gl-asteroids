/**
 * @file GLBackend.h
 * @brief Declaration of the GLBackend class for managing OpenGL and emitting input signals to game logic handlers.
 */

#ifndef asteroids_glbackend_h
#define asteroids_glbackend_h

#include <array>
#include <memory>

#include <QKeyEvent>
#include <QOpenGLWidget>
#include <QTimer>

#include "configuration/config.h"
#include "gl/GL.h"
#include "gl/GLBackendEmitters.h"

namespace asteroids
{

    /**
     * @class GLBackend
     * @brief A class for managing OpenGL rendering and emitting input events to game logic.
     *
     * The GLBackend class initializes Qt OpenGL Widget, sets up callbacks, and emits signals for various game events.
     */
    class ASTEROIDS_DLL_EXPORT GLBackend : public QOpenGLWidget
    {
        Q_OBJECT

    private slots:
        /**
         * @brief on frame update slot
         */
        void onFrame();

    public:
        /**
         * @brief Singleton Get function.
         * @return the GLBackend singleton.
         */
        static GLBackend &Get();

        /**
         * @brief Destructor for the GLBackend class.
         */
        virtual ~GLBackend();

        GLBackend(const GLBackend &) = delete;
        GLBackend(GLBackend &&) = delete;
        GLBackend &operator=(const GLBackend &) = delete;
        GLBackend &operator=(GLBackend &&) = delete;

        /**
         * @brief Start the game loop.
         */
        void Run();

        /**
         * @brief Get the event emitters associated with the game.
         * @return Reference to the GLBackendEmitters instance.
         */
        GLBackendEmitters &GetEmitters();

    private:
        /**
         * @brief Constructor for the GLBackend class.
         */
        GLBackend();

        /**
         * @brief Initialize OpenGL function pointers for this class.
         */
        void initializeGL() override;

        /**
         * @brief QOpenGLWidget paint function.
         */
        void paintGL() override;

        /**
         * @brief QOpenGLWidget resize function.
         * @param _w Window width.
         * @param _h Window height.
         */
        void resizeGL(const int _w, const int _h) override;

        /**
         * @brief keyboard press event handler.
         * @param event keyboard event.
         */
        void keyPressEvent(QKeyEvent *event) override;

        /**
         * @brief keyboard release event handler.
         * @param event keyboard event.
         */
        void keyReleaseEvent(QKeyEvent *event) override;

        /**
         * @brief Update the keyboard state.
         */
        void KeyboardUpdateState();

        // Members
        std::array<bool, 256> keysPressed_; /** @brief Array to track pressed keys. */
        GLBackendEmitters emitters_;        /**< Handles input events and actions. */

        /** @brief Pointer to the callback instance for static functions. */
        static GLBackend *callbackInstance_;

        /** @brief a timer for periodically rendering this widget. */
        std::unique_ptr<QTimer> frameTimer_;
    };

} // end asteroids

#endif // asteroids_glbackend_h
