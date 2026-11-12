#include "WorkerPool.hpp"

namespace engine {

WorkerPool::WorkerPool(std::size_t num_threads) {
    for (std::size_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back(&WorkerPool::worker_loop, this);
    }
}

WorkerPool::~WorkerPool() {
    stop();
}

void WorkerPool::submit(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        tasks_.push(std::move(task));
    }
    condition_.notify_one();
}

void WorkerPool::stop() {
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        stop_.store(true);
    }
    condition_.notify_all();
    for (std::thread& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void WorkerPool::worker_loop() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            condition_.wait(lock, [this] { return stop_.load() || !tasks_.empty(); });

            if (stop_.load() && tasks_.empty()) {
                return;
            }

            task = std::move(tasks_.front());
            tasks_.pop();
        }

        if (task) {
            task();
        }
    }
}

} // namespace engine
