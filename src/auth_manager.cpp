#include "auth_manager.hpp"
#include "td_client.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

AuthManager::AuthManager(TdClient& client) : client_(client) {}

bool AuthManager::check_auth() {
    return is_authorized_.load();
}

void AuthManager::set_on_auth_complete(AuthCallback callback) {
    on_auth_complete_ = std::move(callback);
}

void AuthManager::handle_update(td_api::object_ptr<td_api::Object>&& update) {
    auto state = td::move_tl_object_as<td_api::authorization_state>(std::move(update));
    if (state) {
        process_authorization_state(std::move(state));
    }
}

void AuthManager::handle_wait_params() {
    std::cout << "[AUTH] Waiting for TDLib parameters..." << std::endl;
    const char* env_id = std::getenv("TG_API_ID");
    const char* env_hash = std::getenv("TG_API_HASH");
    if (!env_id || !env_hash) {
        std::cerr << "[ERROR] TG_API_ID or TG_API_HASH not set in environment!" << std::endl;
        return;
    }

    auto request = td_api::make_object<td_api::setTdlibParameters>();
    request->database_directory_ = "/app/tdlib_db";
    request->use_message_database_ = true;
    request->use_secret_chats_ = true;
    request->api_id_ = std::stoi(env_id);
    request->api_hash_ = env_hash;
    request->system_language_code_ = "en";
    request->device_model_ = "Desktop";
    request->application_version_ = "1.0";
    
    auto task = client_.send_query(std::move(request));
    task.handle.resume();
}

void AuthManager::handle_wait_qr(td_api::authorizationStateWaitOtherDeviceConfirmation& s) {
    std::cout << "\nScan this QR code in Telegram (Settings -> Devices -> Link Device):\n" << std::endl;
    std::string cmd = "qrencode -t ANSIUTF8 \"" + s.link_ + "\"";
    std::system(cmd.c_str());
}

void AuthManager::process_authorization_state(td_api::object_ptr<td_api::AuthorizationState> state) {
    if (auto s = td::move_tl_object_as<td_api::authorization_state_wait_tdlib_parameters>(state.get())) {
        handle_wait_params();
    }
    else if (auto s = td::move_tl_object_as<td_api::authorization_state_wait_phone_number>(state.get())) {
        std::cout << "Requesting QR code for login..." << std::endl;
        auto task = client_.send_query(td_api::make_object<td_api::requestQrCodeAuthentication>());
        task.handle.resume();
    }
    else if (auto s = td::move_tl_object_as<td_api::authorization_state_wait_other_device_confirmation>(state.get())) {
        handle_wait_qr(*s);
    }
    else if (auto s = td::move_tl_object_as<td_api::authorization_state_waiting_for_phone_number>(state.get())) {
        std::cout << "[AUTH] Please enter your phone number: ";
        std::string phone;
        if (!(std::cin >> phone)) return;
        auto task = client_.send_query(td_api::make_object<td_api::setAuthenticationPhoneNumber>(phone));
        task.handle.resume();
    } 
    else if (auto s = td::move_tl_object_as<td_api::authorization_state_waiting_for_code>(state.get())) {
        std::cout << "[AUTH] Please enter the code: ";
        std::string code;
        if (!(std::cin >> code)) return;
        auto task = client_.send_query(td_api::make_object<td_api::checkAuthenticationCode>(code));
        task.handle.resume();
    } 
    else if (auto s = td::move_tl_object_as<td_api::authorization_state_waiting_for_password>(state.get())) {
        std::cout << "[AUTH] Please enter your 2FA password: ";
        std::string password;
        if (!(std::cin >> password)) return;
        auto task = client_.send_query(td_api::make_object<td_api::checkAuthenticationPassword>(password));
        task.handle.resume();
    }
    else if (auto s = td::move_tl_object_as<td_api::authorization_state_ready>(state.get())) {
        std::cout << "[AUTH] Authorization ready!" << std::endl;
        is_authorized_ = true;
        if (on_auth_complete_) on_auth_complete_(true);
    }
    else if (auto s = td::move_tl_object_as<td_api::authorization_state_deleted>(state.get())) {
        std::cout << "[AUTH] Authorization deleted." << std::endl;
        is_authorized_ = false;
    }
    else if (auto s = td::move_tl_object_as<td_api::authorization_state_logging_out>(state.get())) {
        std::cout << "[AUTH] Logging out..." << std::endl;
    }
}
