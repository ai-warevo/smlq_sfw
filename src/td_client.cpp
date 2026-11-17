#include "td_client.hpp"
#include <iostream>

TdClient::TdClient() {
    td::ClientManager::execute(td_api::make_object<td_api::setLogVerbosityLevel>(1));
    client_manager_ = std::make_unique<td::ClientManager>();
    client_id_ = client_manager_->create_client_id();
}

TdClient::~TdClient() {
    stop();
}

void TdClient::stop() {
    is_running_ = false;
}

void TdClient::run_loop() {
    while (is_running_) {
        process_response(client_manager_->receive(10));
    }
}

void TdClient::set_update_handler(UpdateHandler handler) {
    update_handler_ = std::move(handler);
}

async::Task<td_api::object_ptr<td_api::Object>> TdClient::send_query(td_api::object_ptr<td_api::Function> f) {
    std::uint64_t query_id = next_query_id_++;
    client_manager_->send(client_id_, query_id, std::move(f));

    co_return co_await QueryAwaiter{query_id, *this};
}

void TdClient::process_response(td::ClientManager::Response response) {
    if (!response.object) return;

    if (response.request_id == 0) {
        process_update(std::move(response.object));
        return;
    }

    auto it = pending_tasks_.find(response.request_id);
    if (it != pending_tasks_.end()) {
        results_cache_[response.request_id] = std::move(response.object);
        auto handle = it->second;
        pending_tasks_.erase(it);
        handle.resume();
    }
}

void TdClient::process_update(td_api::object_ptr<td_api::Object> update) {
    if (update_handler_) {
        update_handler_(std::move(update));
    }
}

void TdClient::QueryAwaiter::await_suspend(std::coroutine_handle<> h) {
    client.pending_tasks_[query_id] = h;
}

td_api::object_ptr<td_api::Object> TdClient::QueryAwaiter::await_resume() {
    auto it = client.results_cache_.find(query_id);
    if (it != client.results_cache_.end()) {
        auto res = std::move(it->second);
        client.results_cache_.erase(it);
        return res;
    }
    return nullptr;
}
