/**
 * @file GLBackend.h
 * @brief Declaration of the GLBackend class for managing OpenGL and emitting input signals to game logic handlers.
 */

#ifndef asteroids_glbackend_h
#define asteroids_glbackend_h

#include <array>
#include <memory>

#include "Events/EventEmitter.h"
#include "configuration/config.h"
#include "gl/GL.h"

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
         * @brief Constructor for the GLBackend class.
         * @param _argc The argument count passed to the GLUT initialization.
         * @param _argv The argument vector passed to the GLUT initialization.
         */
        GLBackend(int _argc, char *_argv[]);

        /**
         * @brief Destructor for the GLBackend class.
         */
        virtual ~GLBackend();

        /**
         * @brief Deleted copy constructor.
         */
        GLBackend(const GLBackend &) = delete;

        /**
         * @brief Deleted move constructor.
         */
        GLBackend(GLBackend &&) = delete;

        /**
         * @brief Deleted copy assignment operator.
         */
        GLBackend &operator=(const GLBackend &) = delete;

        /**
         * @brief Deleted move assignment operator.
         */
        GLBackend &operator=(GLBackend &&) = delete;

        /**
         * @brief Start the game loop.
         */
        void Run();

        /**
         * @brief Get the event emitter for the left arrow input.
         * @return Shared pointer to the event emitter.
         */
        std::shared_ptr<events::EventEmitter<void(void)>> GetLeftArrowEmitter();

        /**
         * @brief Get the event emitter for the right arrow input.
         * @return Shared pointer to the event emitter.
         */
        std::shared_ptr<events::EventEmitter<void(void)>> GetRightArrowEmitter();

        std::shared_ptr<events::EventEmitter<void(void)>> GetThrustEmitter();
        std::shared_ptr<events::EventEmitter<void(void)>> GetFireEmitter();
        std::shared_ptr<events::EventEmitter<void(void)>> GetResetEmitter();
        std::shared_ptr<events::EventEmitter<void(void)>> GetDrawEmitter();
        std::shared_ptr<events::EventEmitter<void(void)>> GetRunEmitter();
        std::shared_ptr<events::EventEmitter<void(void)>> GetSerializeEmitter();
        std::shared_ptr<events::EventEmitter<void(void)>> GetDeserializeEmitter();

        static void DisplayWrapper();
        static void ReshapeWrapper(const int _w, const int _h);
        static void KeyboardWrapper(unsigned char _chr, int _x, int _y);
        static void KeyboardUpWrapper(const unsigned char _chr, const int _x, const int _y);

    private:
        /** @brief Pointer to the callback instance for static functions. */
        static GLBackend *callbackInstance_;

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
        std::unique_ptr<GL> gl_;            /**< Graphics library wrapper. */
        std::array<bool, 256> keysPressed_; /** @brief Array to track pressed keys. */

        /** @brief Event emitters for various game actions. */
        std::shared_ptr<events::EventEmitter<void(void)>> leftArrowEmitter_;
        std::shared_ptr<events::EventEmitter<void(void)>> rightArrowEmitter_;
        std::shared_ptr<events::EventEmitter<void(void)>> thrustEmitter_;
        std::shared_ptr<events::EventEmitter<void(void)>> fireEmitter_;
        std::shared_ptr<events::EventEmitter<void(void)>> resetEmitter_;
        std::shared_ptr<events::EventEmitter<void(void)>> drawEmitter_;
        std::shared_ptr<events::EventEmitter<void(void)>> runEmitter_;
        std::shared_ptr<events::EventEmitter<void(void)>> serializeEmitter_;
        std::shared_ptr<events::EventEmitter<void(void)>> deserializeEmitter_;
    };

} // end asteroids

#endif // asteroids_glbackend_h
