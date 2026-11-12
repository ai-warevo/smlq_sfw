#include "SilenceWindow.hpp"

namespace engine {

SilenceWindow::SilenceWindow(FlushCallback callback, std::chrono::seconds timeout)
    : callback_(std::move(callback)), timeout_(timeout) {
    monitor_thread_ = std::thread(&SilenceWindow::runMonitor, this);
}

SilenceWindow::~SilenceWindow() {
    stop();
}

void SilenceWindow::notifyActivity(std::int64_t user_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    last_activity_[user_id] = std::chrono::steady_clock::now();
}

void SilenceWindow::stop() {
    running_.store(false);
    if (monitor_thread_.joinable()) {
        monitor_thread_.join();
    }
}

void SilenceWindow::runMonitor() {
    while (running_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Check every 500ms

        auto now = std::chrono::steady_clock::now();
        std::vector<std::int64_t> users_to_flush;

        {
            std::lock_guard<std::mutex> lock(mutex_);
            for (auto it = last_activity_.begin(); it != last_activity_.end(); ) {
                if (now - it->second >= timeout_) {
                    users_to_flush.push_back(it->first);
                    it = last_activity_.erase(it); // Remove from tracking after timeout
                } else {
                    ++it;
                }
            }
        }

        for (auto user_id : users_to_flush) {
            if (callback_) {
                callback_(user_id);
            }
        }
    }
}

} // namespace engine
