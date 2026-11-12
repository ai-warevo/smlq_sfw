//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>

namespace detail {
template <class... Fs>
struct overload;

template <class F>
struct overload<F> : public F {
  explicit overload(F f) : F(f) {}
};
template <class F, class... Fs>
struct overload<F, Fs...>
    : public overload<F>
    , public overload<Fs...> {
  overload(F f, Fs... fs) : overload<F>(f), overload<Fs...>(fs...) {}
  using overload<F>::operator();
  using overload<Fs...>::operator();
};
}  // namespace detail

template <class... F>
auto overloaded(F... f) {
  return detail::overload<F...>(f...);
}

namespace td_api = td::td_api;

class TdExample {
 public:
  TdExample() {
    std::cout << "[INFO] Initializing TDLib Client..." << std::endl;
    td::ClientManager::execute(td_api::make_object<td_api::setLogVerbosityLevel>(1));
    client_manager_ = std::make_unique<td::ClientManager>();
    std::cout << "[INFO] ClientManager created." << std::endl;
    client_id_ = client_manager_->create_client_id();
    std::cout << "[INFO] Client ID: " << client_id_ << std::endl;
    send_query(td_api::make_object<td_api::getOption>("version"), [](Object object) {
        std::cout << "[INFO] TDLib version query successful." << std::endl;
    });
  }

  void loop() {
    std::cout << "[INFO] Entering main event loop..." << std::endl;
    while (true) {
      process_response(client_manager_->receive(10));
    }
  }

 private:
  using Object = td_api::object_ptr<td_api::Object>;
  std::unique_ptr<td::ClientManager> client_manager_;
  std::int32_t client_id_{0};

  td_api::object_ptr<td_api::AuthorizationState> authorization_state_;
  bool are_authorized_{false};
  std::uint64_t current_query_id_{0};
  std::uint64_t authentication_query_id_{0};

  std::map<std::uint64_t, std::function<void(Object)>> handlers_;

  void send_query(td_api::object_ptr<td_api::Function> f, std::function<void(Object)> handler) {
    auto query_id = next_query_id();
    if (handler) {
      handlers_.emplace(query_id, std::move(handler));
    }
    client_manager_->send(client_id_, query_id, std::move(f));
  }

  void process_response(td::ClientManager::Response response) {
    if (!response.object) {
        return;
    }

    if (response.request_id == 0) {
      process_update(std::move(response.object));
      return;
    }

    auto it = handlers_.find(response.request_id);
    if (it != handlers_.end()) {
      it->second(std::move(response.object));
      handlers_.erase(it);
    }
  }

  void process_update(td_api::object_ptr<td_api::Object> update) {
    td_api::downcast_call(*update, overloaded(
        [this](td_api::updateAuthorizationState &u) { handleAuthUpdate(u); },
        [this](td_api::updateNewMessage &u) { handleNewMessage(u); },
        [](auto &) {}
    ));
  }

  void handleAuthUpdate(td_api::updateAuthorizationState &u) {
    std::cout << "[INFO] Authorization state update received." << std::endl;
    authorization_state_ = std::move(u.authorization_state_);
    on_authorization_state_update();
  }

  void handleNewMessage(td_api::updateNewMessage &u) {
    auto chat_id = u.message_->chat_id_;
    std::string text;

    if (u.message_->content_->get_id() == td_api::messageText::ID) {
      text = static_cast<td_api::messageText &>(*u.message_->content_).text_->text_;
    }

    if (!text.empty()) {
      std::cout << "[BOT] Received message: '" << text << "' in chat " << chat_id << ". Echoing..." << std::endl;
      auto send_msg = td_api::make_object<td_api::sendMessage>();
      send_msg->chat_id_ = chat_id;
      auto content = td_api::make_object<td_api::inputMessageText>();
      content->text_ = td_api::make_object<td_api::formattedText>();
      content->text_->text_ = text;
      send_msg->input_message_content_ = std::move(content);
      send_query(std::move(send_msg), {});
    }
  }

  void handleReady() {
    are_authorized_ = true;
    std::cout << "[SUCCESS] Authorization is completed! Bot is now active." << std::endl;
  }

  void check_authentication_error(Object object) {
    if (object->get_id() == td_api::error::ID) {
      auto error = td::move_tl_object_as<td_api::error>(object);
      std::cerr << "[AUTH ERROR] " << to_string(error) << std::endl;
      on_authorization_state_update();
    }
  }

  auto create_authentication_query_handler() {
    return [this, id = authentication_query_id_](Object object) {
      if (id == authentication_query_id_) {
        check_authentication_error(std::move(object));
      }
    };
  }

  void on_authorization_state_update() {
    authentication_query_id_++;
    // Log the state name for debugging
    std::cout << "[DEBUG] Current State: " << to_string(*authorization_state_) << std::endl;

    td_api::downcast_call(*authorization_state_, overloaded(
        [this](td_api::authorizationStateReady &) { handleReady(); },
        [this](td_api::authorizationStateWaitTdlibParameters &) { handleWaitParams(); },
        [this](td_api::authorizationStateWaitOtherDeviceConfirmation &s) { handleWaitQr(s); },
        [this](td_api::authorizationStateWaitPhoneNumber &) { 
            std::cout << "Запрашиваем QR-код для входа..." << std::endl;
            send_query(td_api::make_object<td_api::requestQrCodeAuthentication>(),
                       create_authentication_query_handler());
        },
        [](auto &state) { 
            std::cout << "[INFO] Received unhandled state: " << to_string(state) << std::endl; 
        }
    ));
  }

  void handleWaitParams() {
    std::cout << "[AUTH] Waiting for TDLib parameters..." << std::endl;
    const char* env_id = std::getenv("TG_API_ID");
    const char* env_hash = std::getenv("TG_API_HASH");
    if (!env_id || !env_hash) {
      std::cerr << "[ERROR] TG_API_ID or TG_API_HASH not set in environment!" << std::endl;
      return;
    }

    auto request = td_api::make_object<td_api::setTdlibParameters>();
    request->database_directory_ = "tdlib";
    request->use_message_database_ = true;
    request->use_secret_chats_ = true;
    request->api_id_ = std::stoi(env_id);
    request->api_hash_ = env_hash;
    request->system_language_code_ = "en";
    request->device_model_ = "Desktop";
    request->application_version_ = "1.0";
    send_query(std::move(request), create_authentication_query_handler());
  }

  void handleWaitQr(td_api::authorizationStateWaitOtherDeviceConfirmation &s) {
    std::cout << "\nОтсканируй этот QR-код в Telegram (Настройки -> Устройства -> Подключить устройство):\n" << std::endl;
    std::string cmd = "qrencode -t ANSIUTF8 \"" + s.link_ + "\"";
    std::system(cmd.c_str());
  }

  std::uint64_t next_query_id() {
    return ++current_query_id_;
  }
};

int main() {
  std::cout << "[SYSTEM] Starting Bot Process..." << std::endl;
  TdExample example;
  example.loop();
  return 0;
}
