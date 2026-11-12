#pragma once

#include "MessageBuffer.hpp"
#include "SilenceWindow.hpp"
#include <functional>
#include <string>
#include <vector>
#include <cstdint>

namespace engine {

/**
 * @brief Manages message buffering and silence window detection.
 */
class MessageManager {
public:
    using FlushCallback = std::function<void(std::int64_t, std::vector<std::string>)>;

    explicit MessageManager(FlushCallback on_flush, std::chrono::seconds timeout = std::chrono::seconds(10));
    ~MessageManager() = default;

    // Disable copy and move
    MessageManager(const MessageManager&) = delete;
    MessageManager& operator=(const MessageManager&) = delete;
    MessageManager(MessageManager&&) = delete;
    MessageManager& operator=(MessageManager&&) = delete;

    /**
     * @brief Adds a message to the buffer and resets the silence timer for the user.
     * 
     * @param user_id The ID of the user.
     * @param text The message content.
     */
    void addMessage(std::int64_t user_id, const std::string& text);

private:
    void handleTimeout(std::int64_t user_id);

    MessageBuffer buffer_;
    SilenceWindow silence_window_;
    FlushCallback on_flush_;
};

} // namespace engine
