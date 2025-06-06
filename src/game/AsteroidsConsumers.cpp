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

namespace
{
	const std::string ASTEROIDS_CONSUMERS_KEY = "AsteroidsConsumers";
} // end namespace

AsteroidsConsumers::AsteroidsConsumers(std::shared_ptr<Asteroids> asteroids) : Entity()
{
	SetKey(ASTEROIDS_CONSUMERS_KEY);

	leftArrowConsumer_ = std::make_shared<EventConsumer<void(void)>>([asteroids]()
																	 { asteroids->RotateLeft(); });
	rightArrowConsumer_ = std::make_shared<EventConsumer<void(void)>>([asteroids]()
																	  { asteroids->RotateRight(); });
	thrustConsumer_ = std::make_shared<EventConsumer<void(void)>>([asteroids]()
																  { asteroids->Thrust(); });
	fireConsumer_ = std::make_shared<EventConsumer<void(void)>>([asteroids]()
																{ asteroids->Fire(); });
	resetConsumer_ = std::make_shared<EventConsumer<void(void)>>([asteroids]()
																 { asteroids->ResetGame(); });
	drawConsumer_ = std::make_shared<
		EventConsumer<
			void(GLint w_, GLint h_, const std::array<GLfloat, 16> &projOrtho_, const std::array<GLfloat, 16> &projPerspective_)>>([asteroids](GLint w_, GLint h_, const std::array<GLfloat, 16> &projOrtho_, const std::array<GLfloat, 16> &projPerspective_)
																																   { asteroids->Draw(w_, h_, projOrtho_, projPerspective_); });
	runConsumer_ = std::make_shared<EventConsumer<void(void)>>([asteroids]()
															   { asteroids->Run(); });
	serializeConsumer_ = std::make_shared<EventConsumer<void(void)>>([asteroids]()
																	 { asteroids->Serialize(); });
	deserializeConsumer_ = std::make_shared<EventConsumer<void(void)>>([asteroids]()
																	   { asteroids->Deserialize(); });
}

AsteroidsConsumers::~AsteroidsConsumers() = default;

std::shared_ptr<EventConsumer<void(void)>> AsteroidsConsumers::GetLeftArrowConsumer()
{
	return leftArrowConsumer_;
}

std::shared_ptr<EventConsumer<void(void)>> AsteroidsConsumers::GetRightArrowConsumer()
{
	return rightArrowConsumer_;
}

std::shared_ptr<EventConsumer<void(void)>> AsteroidsConsumers::GetThrustConsumer()
{
	return thrustConsumer_;
}

std::shared_ptr<EventConsumer<void(void)>> AsteroidsConsumers::GetFireConsumer()
{
	return fireConsumer_;
}

std::shared_ptr<EventConsumer<void(void)>> AsteroidsConsumers::GetResetConsumer()
{
	return resetConsumer_;
}

std::shared_ptr<
	EventConsumer<
		void(GLint w_, GLint h_, const std::array<GLfloat, 16> &projOrtho_, const std::array<GLfloat, 16> &projPerspective_)>>
AsteroidsConsumers::GetDrawConsumer()
{
	return drawConsumer_;
}

std::shared_ptr<EventConsumer<void(void)>> AsteroidsConsumers::GetRunConsumer()
{
	return runConsumer_;
}

std::shared_ptr<EventConsumer<void(void)>> AsteroidsConsumers::GetSerializeConsumer()
{
	return serializeConsumer_;
}

std::shared_ptr<EventConsumer<void(void)>> AsteroidsConsumers::GetDeserializeConsumer()
{
	return deserializeConsumer_;
}