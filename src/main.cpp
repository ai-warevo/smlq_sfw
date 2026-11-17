#include "td_client.hpp"
#include "auth_manager.hpp"
#include "data_cache.hpp"
#include <iostream>
#include <csignal>
#include <cstdlib>
#include <string>

TdClient* g_client = nullptr;

void signal_handler(int signal) {
    if (g_client) {
        std::cout << "\n[SYSTEM] Signal " << signal << " received. Stopping bot..." << std::endl;
        g_client->stop();
    }
}

async::Task<void> run_bot_logic(TdClient& client, AuthManager& auth, DataCache& cache) {
    try {
        std::cout << "[BOT] Starting initialization sequence..." << std::endl;

        auto me_obj = co_await client.send_query(td_api::make_object<td_api::getMe>());
        if (me_obj) {
            auto me = td::move_tl_object_as<td_api::user>(me_obj);
            std::cout << "[BOT] Authenticated as: " << me->first_name_ << " " << me->last_name_ << std::endl;
        } else {
            std::cerr << "[ERROR] Failed to get 'Me' info." << std::endl;
            co_return;
        }

        std::cout << "[BOT] Fetching recent chats..." << std::endl;
        auto chats_obj = co_await client.send_query(td_api::make_object<td_api::getChats>(nullptr, 5));
        if (chats_obj) {
            auto chats = td::move_tl_object_as<td_api::chats>(chats_obj);
            std::cout << "[BOT] Found " << chats->chat_ids_.size() << " recent chats." << std::endl;
        }

        std::cout << "[BOT] Initialization complete. Listening for messages..." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "[CRITICAL ERROR] Bot logic failed: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "[SYSTEM] Starting SMLQ Bot Process..." << std::endl;

    TdClient client;
    g_client = &client;

    AuthManager auth(client);
    DataCache cache;

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    // Single Dispatcher: One update -> One destination.
    // This avoids ownership issues with unique_ptr.
    client.set_update_handler([&](td_api::object_ptr<td_api::Object> update) {
        if (!update) return;

        // 1. Check if it's an authorization state update
        if (auto state = td::move_tl_object_as<td_api::authorization_state>(update.get())) {
            auth.handle_update(std::move(update));
        } 
        // 2. Check if it's a user update
        else if (auto user = td::move_tl_object_as<td_api::user>(update.get())) {
            cache.update_user(std::move(update));
        }
        // 3. Check if it's a chat update
        else if (auto chat = td::move_tl_object_as<td_api::chat>(update.get())) {
            cache.update_chat(std::move(update));
        }
        // 4. Otherwise, just pass to auth as fallback or ignore
        else {
            auth.handle_update(std::move(update));
        }
    });

    auth.set_on_auth_complete([&](bool success) {
        if (success) {
            std::cout << "[SYSTEM] Auth successful! Launching bot logic..." << std::endl;
            auto task = run_bot_logic(client, auth, cache);
            task.handle.resume();
        }
    });

    client.run_loop();

    std::cout << "[SYSTEM] Bot process finished." << std::endl;
    return 0;
}
