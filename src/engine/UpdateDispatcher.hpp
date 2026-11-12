#pragma once

#include "UpdateListener.hpp"
#include <td/telegram/Client.h>
#include <td/telegram/td_api.hpp>
#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <map>
#include <functional>

namespace engine {

class UpdateDispatcher {
public:
    using QueryResponseHandler = std::function<void(td::ClientManager::Response)>;

    explicit UpdateDispatcher(td::ClientManager& client_manager);
    ~UpdateDispatcher();

    // Disable copy and move for safety with thread management
    UpdateDispatcher(const UpdateDispatcher&) = delete;
    UpdateDispatcher& operator=(const UpdateDispatcher&) = delete;
    UpdateDispatcher(UpdateDispatcher&&) = delete;
    UpdateDispatcher& operator=(UpdateDispatcher&&) = delete;

    void registerListener(std::shared_ptr<UpdateListener> listener);
    void setQueryResponseHandler(QueryResponseHandler handler);
    void start();
    void stop();

private:
    void runLoop();
    void handleUpdate(td_api::object_ptr<td_api::Object> update);

    td::ClientManager& client_manager_;
    std::vector<std::shared_ptr<UpdateListener>> listeners_;
    std::mutex listeners_mutex_;
    
    std::thread worker_thread_;
    std::atomic<bool> running_{false};
    std::chrono::system_clock::time_point start_time_;

    std::map<int64_t, td_api::object_ptr<td_api::ChatType>> chat_types_;
    std::mutex chat_types_mutex_;

    QueryResponseHandler query_response_handler_;
    std::mutex handler_mutex_;
};

} // namespace engine
