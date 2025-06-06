/**
 * @file GLBackendEmitters.h
 * @brief Declaration of the GLBackendEmitters class which manages input event emitters for Asteroids.
 */

#ifndef asteroids_gl_backend_emitters_h
#define asteroids_gl_backend_emitters_h

#include <array>
#include <memory>
#include <string>

#include "Entities/Entity.h"
#include "Events/EventEmitter.h"
#include "configuration/config.h"

namespace asteroids
{

    /**
     * @class GLBackendEmitters
     * @brief A class for managing input event emitters in the Asteroids game.
     *
     * This class provides various event emitters for handling user input and graphical operations,
     * including keyboard, mouse, and rendering events.
     */
    class ASTEROIDS_DLL_EXPORT GLBackendEmitters : public entity::Entity
    {
    public:
        /**
         * @brief Default constructor.
         */
        GLBackendEmitters();

        /**
         * @brief Destructor.
         */
        virtual ~GLBackendEmitters();

        GLBackendEmitters(const GLBackendEmitters &) = delete;
        GLBackendEmitters(GLBackendEmitters &&) = delete;
        GLBackendEmitters &operator=(const GLBackendEmitters &) = delete;
        GLBackendEmitters &operator=(GLBackendEmitters &&) = delete;

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

        /**
         * @brief Get the event emitter for the thrust input.
         * @return Shared pointer to the event emitter.
         */
        std::shared_ptr<events::EventEmitter<void(void)>> GetThrustEmitter();

        /**
         * @brief Get the event emitter for the fire input.
         * @return Shared pointer to the event emitter.
         */
        std::shared_ptr<events::EventEmitter<void(void)>> GetFireEmitter();

        /**
         * @brief Get the event emitter for the reset input.
         * @return Shared pointer to the event emitter.
         */
        std::shared_ptr<events::EventEmitter<void(void)>> GetResetEmitter();

        /**
         * @brief Get the event emitter for the draw input.
         * @return Shared pointer to the event emitter.
         */
        std::shared_ptr<events::EventEmitter<
            void(GLint w_, GLint h_, const std::array<GLfloat, 16> &projOrtho_, const std::array<GLfloat, 16> &projPerspective_)>>
        GetDrawEmitter();

        /**
         * @brief Get the event emitter for the run action.
         * @return Shared pointer to the event emitter.
         */
        std::shared_ptr<events::EventEmitter<void(void)>> GetRunEmitter();

        /**
         * @brief Get the event emitter for the serialize input.
         * @return Shared pointer to the event emitter.
         */
        std::shared_ptr<events::EventEmitter<void(void)>> GetSerializeEmitter();

        /**
         * @brief Get the event emitter for the deserialize input.
         * @return Shared pointer to the event emitter.
         */
        std::shared_ptr<events::EventEmitter<void(void)>> GetDeserializeEmitter();

    private:
        /** @brief Event emitters for various game actions. */
        std::shared_ptr<events::EventEmitter<void(void)>> leftArrowEmitter_;
        std::shared_ptr<events::EventEmitter<void(void)>> rightArrowEmitter_;
        std::shared_ptr<events::EventEmitter<void(void)>> thrustEmitter_;
        std::shared_ptr<events::EventEmitter<void(void)>> fireEmitter_;
        std::shared_ptr<events::EventEmitter<void(void)>> resetEmitter_;
        std::shared_ptr<events::EventEmitter<void(void)>> runEmitter_;
        std::shared_ptr<events::EventEmitter<void(void)>> serializeEmitter_;
        std::shared_ptr<events::EventEmitter<void(void)>> deserializeEmitter_;

        std::shared_ptr<events::EventEmitter<
            void(GLint w_, GLint h_, const std::array<GLfloat, 16> &projOrtho_, const std::array<GLfloat, 16> &projPerspective_)>>
            drawEmitter_;
    };

} // end namespace asteroids

#endif // asteroids_gl_backend_emitters_h
