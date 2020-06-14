#ifndef asteroids_glentitytask_h
#define	asteroids_glentitytask_h

#include <functional>
#include <future>
#include <memory>

namespace asteroids {

class GLEntity;

struct GLEntityTask
{
	GLEntityTask(std::function<GLEntity*()> lambda);

	void operator()();

	std::future<GLEntity*> GetFuture();

private:
	std::shared_ptr<std::packaged_task<GLEntity*()>> task_;
};

} // asteroids

#endif // asteroids_glentitytask
