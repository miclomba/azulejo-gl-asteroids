/**
 * @file GL.h
 * @brief Declaration of the GL class for managing the underlying graphics library.
 */

#ifndef asteroids_gl_h
#define asteroids_gl_h

#include <array>
#include <memory>

#include <QOpenGLFunctions>

#include "configuration/config.h"

namespace asteroids
{

    /**
     * @class GL
     * @brief A class responsible for managing the underlying graphics library.
     *
     * This class initializes OpenGL and handles rendering.
     */
    class ASTEROIDS_DLL_EXPORT GL : protected QOpenGLFunctions
    {
    public:
        /**
         * @brief Singleton Get function.
         * @return the GL singleton reference.
         */
        static GL &Get();

        /**
         * @brief Destructor for GL.
         */
        virtual ~GL();

        GL(const GL &) = delete;
        GL(GL &&) = delete;
        GL &operator=(const GL &) = delete;
        GL &operator=(GL &&) = delete;

        /**
         * @brief Display clear function.
         */
        void DisplayClear();

        /**
         * @brief Display flush function.
         */
        void DisplayFlush();

        /**
         * @brief Reshape function.
         * @param _w Window width.
         * @param _h Window height.
         */
        void Reshape(const int _w, const int _h);

        /**
         * @brief Initialize OpenGL Function pointers.
         */
        void InitOpenGLFunctions();

    private:
        /**
         * @brief Constructor for GL.
         */
        GL();

        /**
         * @brief Initialize the server component.
         */
        void InitServer();

        /**
         * @brief Initialize the client component.
         */
        void InitClient() const;

        // Members
        static std::unique_ptr<GL> instance_;
    };

} // end asteroids

#endif // asteroids_gl_h
