#include "MessageBuffer.hpp"

namespace engine {

void MessageBuffer::add_message(std::int64_t user_id, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    buffers_[user_id].push_back(message);
}

std::vector<std::string> MessageBuffer::flush_messages(std::int64_t user_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = buffers_.find(user_id);
    if (it != buffers_.end()) {
        std::vector<std::string> messages = std::move(it->second);
        buffers_.erase(it);
        return messages;
    }
    return {};
}

void MessageBuffer::clear_for_user(std::int64_t user_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    buffers_.erase(user_id);
}

void MessageBuffer::clear_all() {
    std::lock_guard<std::mutex> lock(mutex_);
    buffers_.clear();
}

} // namespace engine
