#include "MessageManager.hpp"

namespace engine {

MessageManager::MessageManager(FlushCallback on_flush, std::chrono::seconds timeout)
    : on_flush_(std::move(on_flush)), 
      silence_window_([this](std::int64_t user_id) { this->handleTimeout(user_id); }, timeout) {}

void MessageManager::addMessage(std::int64_t user_id, const std::string& text) {
    buffer_.add_message(user_id, text);
    silence_window_.notifyActivity(user_id);
}

void MessageManager::handleTimeout(std::int64_t user_id) {
    auto messages = buffer_.flush_messages(user_id);
    if (!messages.empty() && on_flush_) {
        on_flush_(user_id, std::move(messages));
    }
}

} // namespace engine
