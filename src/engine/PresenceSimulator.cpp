#include "PresenceSimulator.hpp"
#include <algorithm>
#include <cmath>
#include <chrono>
#include <random>

namespace engine {

PresenceSimulator::PresenceSimulator(IQuerySender& sender)
    : sender_(sender) {}

PresenceSimulator::~PresenceSimulator() {
    stop();
}

void PresenceSimulator::simulateTyping(std::int64_t chat_id, std::size_t message_length) {
    if (!running_.load()) return;

    double duration = std::max(2.0, std::min(10.0, static_cast<double>(message_length) * 0.15));

    std::lock_guard<std::mutex> lock(tasks_mutex_);

    // If there is an existing task for this chat, cancel it
    auto it = active_tasks_.find(chat_id);
    if (it != active_tasks_.end()) {
        it->second->cancelled->store(true);
        if (it->second->thread.joinable()) {
            it->second->thread.detach();
        }
    }

    // Create a new task
    auto cancelled_flag = std::make_shared<std::atomic<bool>>(false);
    auto task = std::make_unique<TypingTask>();
    task->cancelled = cancelled_flag;
    
    task->thread = std::thread(&PresenceSimulator::run_typing, this, chat_id, duration, cancelled_flag);

    active_tasks_[chat_id] = std::move(task);
}

void PresenceSimulator::simulateResponse(std::int64_t chat_id, std::size_t message_length, std::function<void()> callback) {
    if (!running_.load()) return;

    std::lock_guard<std::mutex> lock(tasks_mutex_);

    // If there is an existing task for this chat, cancel it
    auto it = active_tasks_.find(chat_id);
    if (it != active_tasks_.end()) {
        it->second->cancelled->store(true);
        if (it->second->thread.joinable()) {
            it->second->thread.detach();
        }
    }

    // Create a new task
    auto cancelled_flag = std::make_shared<std::atomic<bool>>(false);
    auto task = std::make_unique<TypingTask>();
    task->cancelled = cancelled_flag;
    
    task->thread = std::thread(&PresenceSimulator::run_response, this, chat_id, message_length, callback, cancelled_flag);

    active_tasks_[chat_id] = std::move(task);
}

void PresenceSimulator::stop() {
    running_.store(false);
    std::lock_guard<std::mutex> lock(tasks_mutex_);
    for (auto& pair : active_tasks_) {
        pair.second->cancelled->store(true);
        if (pair.second->thread.joinable()) {
            pair.second->thread.detach();
        }
    }
    active_tasks_.clear();
}

void PresenceSimulator::run_typing(std::int64_t chat_id, double duration, std::shared_ptr<std::atomic<bool>> cancelled) {
    // 1. Send typing action
    auto send_chat_action = td::td_api::make_object<td::td_api::sendChatAction>();
    send_chat_action->chat_id_ = chat_id;
    send_chat_action->topic_id_ = nullptr;
    send_chat_action->business_connection_id_ = "";
    send_chat_action->action_ = td::td_api::make_object<td::td_api::chatActionTyping>();

    sender_.send_query(std::move(send_chat_action));

    // 2. Sleep in increments to check for cancellation
    auto start = std::chrono::steady_clock::now();
    auto duration_ms = static_cast<long long>(duration * 1000);
    
    while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(duration_ms)) {
        if (cancelled->load()) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void PresenceSimulator::run_response(std::int64_t chat_id, std::size_t message_length, std::function<void()> callback, std::shared_ptr<std::atomic<bool>> cancelled) {
    // 1. Random reading delay (1.5 - 3.0s)
    static thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> dis(1.5, 3.0);
    double reading_delay = dis(gen);

    auto start_reading = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - start_reading < std::chrono::milliseconds(static_cast<long long>(reading_delay * 1000))) {
        if (cancelled->load()) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // 2. Typing simulation
    double typing_duration = std::max(2.0, std::min(10.0, static_cast<double>(message_length) * 0.15));
    run_typing(chat_id, typing_duration, cancelled);

    // 3. Execute callback
    if (!cancelled->load() && callback) {
        callback();
    }
}

} // namespace engine
