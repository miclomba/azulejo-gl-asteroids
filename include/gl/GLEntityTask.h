/**
 * @file GLEntityTask.h
 * @brief Declaration of the GLEntityTask struct for handling asynchronous tasks related to GLEntity.
 */

#ifndef asteroids_glentitytask_h
#define asteroids_glentitytask_h

#include <functional>
#include <future>
#include <memory>

namespace asteroids {

class GLEntity;

/**
 * @struct GLEntityTask
 * @brief A struct for managing asynchronous tasks that produce GLEntity instances.
 *
 * This struct wraps an asynchronous task and provides mechanisms to execute and retrieve the result.
 */
struct GLEntityTask
{
    /**
     * @brief Constructor for GLEntityTask.
     * @param lambda A function returning a pointer to a GLEntity instance.
     */
    GLEntityTask(std::function<GLEntity*()> lambda);

    /**
     * @brief Execute the stored task.
     */
    void operator()();

    /**
     * @brief Get the future result of the task.
     * @return A future object holding the GLEntity pointer result.
     */
    std::future<GLEntity*> GetFuture();

private:
    /** @brief A packaged task encapsulating the asynchronous operation. */
    std::shared_ptr<std::packaged_task<GLEntity*()>> task_;
};

} // namespace asteroids

#endif // asteroids_glentitytask_h
