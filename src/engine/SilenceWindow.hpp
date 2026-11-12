#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>

namespace engine {

/**
 * @brief Manages a silence window for users, triggering a callback after a period of inactivity.
 */
class SilenceWindow {
public:
    using FlushCallback = std::function<void(std::int64_t)>;

    explicit SilenceWindow(FlushCallback callback, std::chrono::seconds timeout = std::chrono::seconds(10));
    ~SilenceWindow();

    // Disable copy and move
    SilenceWindow(const SilenceWindow&) = delete;
    SilenceWindow& operator=(const SilenceWindow&) = delete;
    SilenceWindow(SilenceWindow&&) = delete;
    SilenceWindow& operator=(SilenceWindow&&) = delete;

    /**
     * @brief Notifies the window that activity was detected for a user, resetting their timer.
     * 
     * @param user_id The ID of the user.
     */
    void notifyActivity(std::int64_t user_id);

    /**
     * @brief Stops the background monitoring thread.
     */
    void stop();

private:
    void runMonitor();

    FlushCallback callback_;
    std::chrono::seconds timeout_;
    
    std::mutex mutex_;
    std::map<std::int64_t, std::chrono::steady_clock::time_point> last_activity_;
    
    std::atomic<bool> running_{true};
    std::thread monitor_thread_;
};

} // namespace engine
