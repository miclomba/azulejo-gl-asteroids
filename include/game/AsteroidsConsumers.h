/**
 * @file AsteroidsConsumers.h
 * @brief Declaration of the AsteroidsConsumers class which handles event consumption for the game.
 */

#ifndef asteroids_asteroids_consumers_h
#define asteroids_asteroids_consumers_h

#include <array>
#include <memory>
#include <string>

#include "Entities/Entity.h"
#include "Events/EventConsumer.h"
#include "configuration/config.h"
#include "game/Asteroids.h"

namespace asteroids
{

    /**
     * @class AsteroidsConsumers
     * @brief A class that manages event consumers for various actions in the Asteroids game.
     *
     * This class provides mechanisms to handle events triggered by user input and other actions,
     * allowing the game instance to respond accordingly.
     */
    class ASTEROIDS_DLL_EXPORT AsteroidsConsumers : public entity::Entity
    {
    public:
        /**
         * @brief Constructs a AsteroidsConsumers object with a given Asteroids instance.
         * @param asteroids A shared pointer to the Asteroids instance.
         */
        AsteroidsConsumers(std::shared_ptr<Asteroids> asteroids);

        /**
         * @brief Destructor for AsteroidsConsumers.
         */
        ~AsteroidsConsumers();

        AsteroidsConsumers(const AsteroidsConsumers &) = delete;
        AsteroidsConsumers(AsteroidsConsumers &&) = delete;
        AsteroidsConsumers &operator=(const AsteroidsConsumers &) = delete;
        AsteroidsConsumers &operator=(AsteroidsConsumers &&) = delete;

        /**
         * @brief Getter for the left arrow event consumer.
         * @return An event consumer.
         */
        std::shared_ptr<events::EventConsumer<void(void)>> GetLeftArrowConsumer();

        /**
         * @brief Getter for the right arrow event consumer.
         * @return An event consumer.
         */
        std::shared_ptr<events::EventConsumer<void(void)>> GetRightArrowConsumer();

        /**
         * @brief Getter for the thrust action event consumer.
         * @return An event consumer.
         */
        std::shared_ptr<events::EventConsumer<void(void)>> GetThrustConsumer();

        /**
         * @brief Getter for the fire event consumer.
         * @return An event consumer.
         */
        std::shared_ptr<events::EventConsumer<void(void)>> GetFireConsumer();

        /**
         * @brief Getter for the reset event consumer.
         * @return An event consumer.
         */
        std::shared_ptr<events::EventConsumer<void(void)>> GetResetConsumer();

        /**
         * @brief Getter for the draw event consumer.
         * @return An event consumer.
         */
        std::shared_ptr<events::EventConsumer<void()>> GetDrawConsumer();

        /**
         * @brief Getter for the run event consumer.
         * @return An event consumer.
         */
        std::shared_ptr<events::EventConsumer<void(void)>> GetRunConsumer();

        /**
         * @brief Getter for the serialize event consumer.
         * @return An event consumer.
         */
        std::shared_ptr<events::EventConsumer<void(void)>> GetSerializeConsumer();

        /**
         * @brief Getter for the deserialize event consumer.
         * @return An event consumer.
         */
        std::shared_ptr<events::EventConsumer<void(void)>> GetDeserializeConsumer();

    private:
        std::shared_ptr<events::EventConsumer<void(void)>> leftArrowConsumer_;
        std::shared_ptr<events::EventConsumer<void(void)>> rightArrowConsumer_;
        std::shared_ptr<events::EventConsumer<void(void)>> thrustConsumer_;
        std::shared_ptr<events::EventConsumer<void(void)>> fireConsumer_;
        std::shared_ptr<events::EventConsumer<void(void)>> resetConsumer_;
        std::shared_ptr<events::EventConsumer<void(void)>> runConsumer_;
        std::shared_ptr<events::EventConsumer<void(void)>> serializeConsumer_;
        std::shared_ptr<events::EventConsumer<void(void)>> deserializeConsumer_;
        std::shared_ptr<events::EventConsumer<void()>> drawConsumer_;
    };

} // end namespace asteroids

#endif // asteroids_asteroids_consumers_h
