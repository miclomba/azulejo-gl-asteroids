#include "GLEntityTask.h"

#include <functional>
#include <future>

#include "Entities/Entity.h"

using asteroids::GLEntityTask;
using entity::Entity;

GLEntityTask::GLEntityTask(std::function<entity::Entity*()> lambda)
{
	task_ = std::make_shared<std::packaged_task<Entity*()>>(lambda);
}

void GLEntityTask::operator()()
{
	return (*task_)();
}

std::future<entity::Entity*> GLEntityTask::GetFuture()
{
	return task_->get_future();
}
