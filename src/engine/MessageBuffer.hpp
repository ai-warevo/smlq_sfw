#pragma once

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <cstdint>

namespace engine {

/**
 * @brief A thread-safe in-memory buffer for storing messages per user.
 */
class MessageBuffer {
public:
    MessageBuffer() = default;
    ~MessageBuffer() = default;

    // Disable copy and move
    MessageBuffer(const MessageBuffer&) = delete;
    MessageBuffer& operator=(const MessageBuffer&) = delete;
    MessageBuffer(MessageBuffer&&) = delete;
    MessageBuffer& operator=(MessageBuffer&&) = delete;

    /**
     * @brief Adds a message to the buffer for a specific user.
     * 
     * @param user_id The ID of the user.
     * @param message The message text.
     */
    void add_message(std::int64_t user_id, const std::string& message);

    /**
     * @brief Retrieves all messages for a specific user and clears the buffer for that user.
     * 
     * @param user_id The ID of the user.
     * @return A vector of messages.
     */
    std::vector<std::string> flush_messages(std::int64_t user_id);

    /**
     * @brief Clears all messages for a specific user.
     * 
     * @param user_id The ID of the user.
     */
    void clear_for_user(std::int64_t user_id);

    /**
     * @brief Clears all messages in the buffer.
     */
    void clear_all();

private:
    std::mutex mutex_;
    std::map<std::int64_t, std::vector<std::string>> buffers_;
};

} // namespace engine
