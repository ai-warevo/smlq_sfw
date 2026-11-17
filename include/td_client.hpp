#ifndef TD_CLIENT_HPP
#define TD_CLIENT_HPP

#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>
#include "async/task.hpp"
#include <memory>
#include <map>
#include <functional>
#include <atomic>
#include <cstdint>
#include <string>

namespace td_api = td::td_api;

class TdClient {
public:
    TdClient();
    ~TdClient();

    TdClient(const TdClient&) = delete;
    TdClient& operator=(const TdClient&) = delete;

    void run_loop();
    void stop();

    async::Task<td_api::object_ptr<td_api::Object>> send_query(td_api::object_ptr<td_api::Function> f);
    
    using UpdateHandler = std::function<void(td_api::object_ptr<td_api::Object>)>;
    void set_update_handler(UpdateHandler handler);

private:
    struct QueryAwaiter {
        std::uint64_t query_id;
        TdClient& client;

        bool await_ready() const noexcept { return false; }
        void await_suspend(std::coroutine_handle<> h);
        td_api::object_ptr<td_api::Object> await_resume();
    };

    void process_response(td::ClientManager::Response response);
    void process_update(td_api::object_ptr<td_api::Object> update);

    std::unique_ptr<td::ClientManager> client_manager_;
    std::int32_t client_id_{0};
    std::atomic<bool> is_running_{true};

    UpdateHandler update_handler_;

    std::uint64_t next_query_id_{1};
    std::map<std::uint64_t, std::coroutine_handle<>> pending_tasks_;
    std::map<std::uint64_t, td_api::object_ptr<td_api::Object>> results_cache_;
};

#endif // TD_CLIENT_HPP
