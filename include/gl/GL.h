/**
 * @file GL.h
 * @brief Declaration of the GL class for managing the underlying graphics library.
 */

#ifndef asteroids_gl_h
#define asteroids_gl_h

#include <array>
#include <memory>

#include "configuration/config.h"
#include "gl/GLProjectionInfo.h"

namespace asteroids
{

    /**
     * @class GL
     * @brief A class responsible for managing the underlying graphics library.
     *
     * This class initializes GLUT and handles rendering.
     */
    class ASTEROIDS_DLL_EXPORT GL
    {
    public:
        /**
         * @brief Singleton Get function.
         * @param _argc Number of command-line arguments.
         * @param _argv Command-line argument values.
         */
        static GL &Get(int _argc = 0, char *_argv[] = nullptr);

        /**
         * @brief Destructor for GL.
         */
        virtual ~GL();

        GL(const GL &) = delete;
        GL(GL &&) = delete;
        GL &operator=(const GL &) = delete;
        GL &operator=(GL &&) = delete;

        /**
         * @brief GLUT display clear function.
         */
        void DisplayClear();

        /**
         * @brief GLUT display flush function.
         */
        void DisplayFlush();

        /**
         * @brief GLUT reshape function.
         * @param _w Window width.
         * @param _h Window height.
         */
        void Reshape(const int _w, const int _h);

        /**
         * @brief Start the main game loop.
         */
        void Run();

        /**
         * @brief Get the game window instance.
         * @return Reference to the GLProjectionInfo instance.
         */
        GLProjectionInfo &GetGameProjectionInfo();

    private:
        /**
         * @brief Constructor for GL.
         * @param _argc Number of command-line arguments.
         * @param _argv Command-line argument values.
         */
        GL(int _argc, char *_argv[]);

        /**
         * @brief Timer callback function for periodic updates.
         * @param _idx Timer index.
         */
        static void TimerCallback(int _idx);

        // GLUT Initialization
        /**
         * @brief Initialize GLUT with command-line arguments.
         * @param _argc Number of command-line arguments.
         * @param _argv Command-line argument values.
         */
        void InitGlut(int _argc, char *_argv[]) const;

        /**
         * @brief Initialize the server component.
         */
        void InitServer() const;

        /**
         * @brief Initialize the client component.
         */
        void InitClient() const;

        // GLUT callbacks
        /**
         * @brief Register GLUT callbacks.
         */
        void RegisterCallbacks() const;

        // Members
        static std::unique_ptr<GL> instance_;
        GLProjectionInfo gameProjectionInfo_; /**< Handles window properties. */
    };

} // end asteroids

#endif // asteroids_gl_h
