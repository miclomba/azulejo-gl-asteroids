#ifndef asteroids_glentitytask_h
#define	asteroids_glentitytask_h

#include <functional>
#include <future>
#include <memory>

namespace entity {
class Entity;
}

namespace asteroids {

struct GLEntityTask
{
	GLEntityTask(std::function<entity::Entity*()> lambda);

	void operator()();

	std::future<entity::Entity*> GetFuture();

private:
	std::shared_ptr<std::packaged_task<entity::Entity*()>> task_;
};

} // asteroids

#endif // asteroids_glentitytask
