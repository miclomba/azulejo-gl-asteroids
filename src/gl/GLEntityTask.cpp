#include "gl/GLEntityTask.h"

#include <functional>
#include <future>

#include "gl/GLEntity.h"

using asteroids::GLEntity;
using asteroids::GLEntityTask;

GLEntityTask::GLEntityTask(
	std::function<std::shared_ptr<GLEntity>()> lambda) : task_(std::make_shared<std::packaged_task<std::shared_ptr<GLEntity>()>>(lambda))
{
}

void GLEntityTask::operator()()
{
	return (*task_)();
}

std::future<std::shared_ptr<GLEntity>> GLEntityTask::GetFuture()
{
	return task_->get_future();
}
