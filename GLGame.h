/**
 * @file GLGame.h
 * @brief Declaration of the GLGame class for managing OpenGL and emitting input signals to game logic handlers.
 */

#ifndef asteroids_glgame_h
#define asteroids_glgame_h

#include <array>
#include <memory>

#include "Events/EventEmitter.h"
#include "config.h"

namespace asteroids {

/**
 * @class GLGame
 * @brief A class for managing OpenGL rendering and emitting input events to game logic.
 *
 * The GLGame class initializes GLUT, sets up callbacks, and emits signals for various game events.
 */
class ASTEROIDS_DLL_EXPORT GLGame 
{
public:
    /**
     * @brief Constructor for the GLGame class.
     * @param _argc The argument count passed to the GLUT initialization.
     * @param _argv The argument vector passed to the GLUT initialization.
     */
    GLGame(int _argc, char* _argv[]);

    /**
     * @brief Destructor for the GLGame class.
     */
    ~GLGame();

    /**
     * @brief Deleted copy constructor.
     */
    GLGame(const GLGame&) = delete;

    /**
     * @brief Deleted move constructor.
     */
    GLGame(GLGame&&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    GLGame& operator=(const GLGame&) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    GLGame& operator=(GLGame&&) = delete;

    /**
     * @brief Start the game loop.
     */
    void Run();

    /**
     * @brief GLUT timer callback function.
     * @param _idx Timer index.
     */
    static void TimerCallback(int _idx);

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

    /** @brief Pointer to the callback instance for static functions. */
    static GLGame* callbackInstance_;

private:
    /**
     * @brief Register GLUT callbacks.
     */
    void RegisterCallbacks() const;

    /**
     * @brief Initialize GLUT.
     * @param _argc The argument count.
     * @param _argv The argument vector.
     */
    void InitGlut(int _argc, char* _argv[]) const;

    void InitServer() const;
    void InitClient() const;

    void Display();
    static void DisplayWrapper();

    void Reshape(const int _w, const int _h) const;
    static void ReshapeWrapper(const int _w, const int _h);

    void Keyboard(const unsigned char _chr, const int _x, const int _y);
    static void KeyboardWrapper(unsigned char _chr, int _x, int _y);

    void KeyboardUp(const unsigned char _chr, const int _x, const int _y);
    static void KeyboardUpWrapper(const unsigned char _chr, const int _x, const int _y);

    void KeyboardUpdateState();

    /** @brief Array to track pressed keys. */
    std::array<bool, 256> keysPressed_;

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

#endif // asteroids_glgame_h
