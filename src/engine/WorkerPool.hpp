#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

namespace engine {

/**
 * @brief A simple thread pool for offloading asynchronous tasks.
 */
class WorkerPool {
public:
    explicit WorkerPool(std::size_t num_threads);
    ~WorkerPool();

    // Disable copy and move
    WorkerPool(const WorkerPool&) = delete;
    WorkerPool& operator=(const WorkerPool&) = delete;
    WorkerPool(WorkerPool&&) = delete;
    WorkerPool& operator=(WorkerPool&&) = delete;

    /**
     * @brief Submits a task to the pool for execution.
     * 
     * @param task A function representing the work to be done.
     */
    void submit(std::function<void()> task);

    /**
     * @brief Stops all worker threads and waits for them to finish current tasks.
     */
    void stop();

private:
    void worker_loop();

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;

    std::mutex queue_mutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_{false};
};

} // namespace engine
