#pragma once

#include <td/telegram/td_api.hpp>
#include <cstdint>
#include <memory>
#include <map>
#include <mutex>
#include <thread>
#include <atomic>
#include <functional>

namespace engine {

/**
 * @brief Interface for sending TDLib queries without depending on the full client implementation.
 */
class IQuerySender {
public:
    virtual ~IQuerySender() = default;
    virtual void send_query(td::td_api::object_ptr<td::td_api::Function> f) = 0;
};

/**
 * @brief Simulates a user typing status in Telegram chats.
 */
class PresenceSimulator {
public:
    explicit PresenceSimulator(IQuerySender& sender);
    ~PresenceSimulator();

    // Disable copy and move
    PresenceSimulator(const PresenceSimulator&) = delete;
    PresenceSimulator& operator=(const PresenceSimulator&) = delete;
    PresenceSimulator(PresenceSimulator&&) = delete;
    PresenceSimulator& operator=(PresenceSimulator&&) = delete;

    /**
     * @brief Simulates typing for a given chat based on the length of the message to be sent.
     * 
     * @param chat_id The ID of the chat.
     * @param message_length The length of the message (in characters).
     */
    void simulateTyping(std::int64_t chat_id, std::size_t message_length);

    /**
     * @brief Simulates typing and then executes a callback after a realistic delay.
     * 
     * @param chat_id The ID of the chat.
     * @param message_length The length of the message (in characters).
     * @param callback The action to perform after the simulation.
     */
    void simulateResponse(std::int64_t chat_id, std::size_t message_length, std::function<void()> callback);

    /**
     * @brief Stops all active typing simulations and cleans up resources.
     */
    void stop();

private:
    void run_typing(std::int64_t chat_id, double duration, std::shared_ptr<std::atomic<bool>> cancelled);
    void run_response(std::int64_t chat_id, std::size_t message_length, std::function<void()> callback, std::shared_ptr<std::atomic<bool>> cancelled);

    struct TypingTask {
        std::shared_ptr<std::atomic<bool>> cancelled;
        std::thread thread;
    };

    IQuerySender& sender_;
    std::mutex tasks_mutex_;
    std::map<std::int64_t, std::unique_ptr<TypingTask>> active_tasks_;
    std::atomic<bool> running_{true};
};

} // namespace engine
