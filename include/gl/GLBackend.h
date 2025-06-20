/**
 * @file GLBackend.h
 * @brief Declaration of the GLBackend class for managing OpenGL and emitting input signals to game logic handlers.
 */

#ifndef asteroids_glbackend_h
#define asteroids_glbackend_h

#include <array>
#include <memory>

#include "configuration/config.h"
#include "gl/GL.h"
#include "gl/GLBackendEmitters.h"

namespace asteroids
{

    /**
     * @class GLBackend
     * @brief A class for managing OpenGL rendering and emitting input events to game logic.
     *
     * The GLBackend class initializes GLUT, sets up callbacks, and emits signals for various game events.
     */
    class ASTEROIDS_DLL_EXPORT GLBackend
    {
    public:
        /**
         * @brief Singleton Get function.
         * @param _argc Number of command-line arguments.
         * @param _argv Command-line argument values.
         */
        static GLBackend &Get(int _argc = 0, char *_argv[] = nullptr);

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

        /**
         * @brief Display callback for the underlying GL library.
         */
        static void DisplayCallback();

        /**
         * @brief Reshape callback for the underlying GL library.
         */
        static void ReshapeCallback(const int _w, const int _h);

        /**
         * @brief Keyboard callback for the underlying GL library.
         */
        static void KeyboardCallback(unsigned char _chr, int _x, int _y);

        /**
         * @brief KeyboardUp callback for the underlying GL library.
         */
        static void KeyboardUpCallback(const unsigned char _chr, const int _x, const int _y);

    private:
        /**
         * @brief Constructor for the GLBackend class.
         * @param _argc The argument count passed to the GLUT initialization.
         * @param _argv The argument vector passed to the GLUT initialization.
         */
        GLBackend(int _argc, char *_argv[]);

        /**
         * @brief GLUT display function.
         */
        void Display();

        /**
         * @brief GLUT reshape function.
         * @param _w Window width.
         * @param _h Window height.
         */
        void Reshape(const int _w, const int _h) const;

        /**
         * @brief GLUT keyboard input function.
         * @param _chr Pressed character.
         * @param _x X-coordinate of the mouse.
         * @param _y Y-coordinate of the mouse.
         */
        void Keyboard(const unsigned char _chr, const int _x, const int _y);

        /**
         * @brief GLUT keyboard release function.
         * @param _chr Released character.
         * @param _x X-coordinate of the mouse.
         * @param _y Y-coordinate of the mouse.
         */
        void KeyboardUp(const unsigned char _chr, const int _x, const int _y);

        /**
         * @brief Update the keyboard state.
         */
        void KeyboardUpdateState();

        // Members
        std::array<bool, 256> keysPressed_; /** @brief Array to track pressed keys. */
        GLBackendEmitters emitters_;        /**< Handles input events and actions. */

        /** @brief Pointer to the callback instance for static functions. */
        static std::unique_ptr<GLBackend> callbackInstance_;
    };

} // end asteroids

#endif // asteroids_glbackend_h
