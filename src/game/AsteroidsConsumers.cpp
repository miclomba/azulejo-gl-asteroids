#include "game/AsteroidsConsumers.h"

#include <array>
#include <memory>
#include <string>

#include "Entities/Entity.h"
#include "Events/EventConsumer.h"

#include "configuration/config.h"
#include "configuration/filesystem.h"
#include "game/Asteroids.h"

using asteroids::Asteroids;
using asteroids::AsteroidsConsumers;
using entity::Entity;
using events::EventConsumer;
using Consumer = EventConsumer<void(void)>;

namespace
{
	const std::string ASTEROIDS_CONSUMERS_KEY = "AsteroidsConsumers";
} // end namespace

AsteroidsConsumers::AsteroidsConsumers(
	std::shared_ptr<Asteroids> asteroids) : Entity(),
											leftArrowConsumer_(
												std::make_shared<Consumer>(
													[asteroids]()
													{ asteroids->RotateLeft(); })),
											rightArrowConsumer_(
												std::make_shared<Consumer>(
													[asteroids]()
													{ asteroids->RotateRight(); })),
											thrustConsumer_(
												std::make_shared<Consumer>(
													[asteroids]()
													{ asteroids->Thrust(); })),
											fireConsumer_(
												std::make_shared<Consumer>(
													[asteroids]()
													{ asteroids->Fire(); })),
											resetConsumer_(
												std::make_shared<Consumer>(
													[asteroids]()
													{ asteroids->ResetGame(); })),
											drawConsumer_(
												std::make_shared<Consumer>(
													[asteroids]()
													{ asteroids->Draw(); })),
											runConsumer_(
												std::make_shared<Consumer>(
													[asteroids]()
													{ asteroids->Run(); })),
											serializeConsumer_(
												std::make_shared<Consumer>(
													[asteroids]()
													{ asteroids->Serialize(); })),
											deserializeConsumer_(
												std::make_shared<Consumer>(
													[asteroids]()
													{ asteroids->Deserialize(); }))
{
	SetKey(ASTEROIDS_CONSUMERS_KEY);
}

AsteroidsConsumers::~AsteroidsConsumers() = default;

std::shared_ptr<Consumer> AsteroidsConsumers::GetLeftArrowConsumer()
{
	return leftArrowConsumer_;
}

std::shared_ptr<Consumer> AsteroidsConsumers::GetRightArrowConsumer()
{
	return rightArrowConsumer_;
}

std::shared_ptr<Consumer> AsteroidsConsumers::GetThrustConsumer()
{
	return thrustConsumer_;
}

std::shared_ptr<Consumer> AsteroidsConsumers::GetFireConsumer()
{
	return fireConsumer_;
}

std::shared_ptr<Consumer> AsteroidsConsumers::GetResetConsumer()
{
	return resetConsumer_;
}

std::shared_ptr<EventConsumer<void()>> AsteroidsConsumers::GetDrawConsumer()
{
	return drawConsumer_;
}

std::shared_ptr<Consumer> AsteroidsConsumers::GetRunConsumer()
{
	return runConsumer_;
}

std::shared_ptr<Consumer> AsteroidsConsumers::GetSerializeConsumer()
{
	return serializeConsumer_;
}

std::shared_ptr<Consumer> AsteroidsConsumers::GetDeserializeConsumer()
{
	return deserializeConsumer_;
}