#include "gl/GLEntityTask.h"

#include <functional>
#include <future>

#include "gl/GLEntity.h"

using asteroids::GLEntity;
using asteroids::GLEntityTask;

GLEntityTask::GLEntityTask(std::function<GLEntity *()> lambda) : task_(std::make_shared<std::packaged_task<GLEntity *()>>(lambda))
{
}

void GLEntityTask::operator()()
{
	return (*task_)();
}

std::future<GLEntity *> GLEntityTask::GetFuture()
{
	return task_->get_future();
}
