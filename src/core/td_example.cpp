#include "td_example.hpp"

TdExample::TdExample() {
  td::ClientManager::execute(td_api::make_object<td_api::setLogVerbosityLevel>(1));
  client_manager_ = std::make_unique<td::ClientManager>();
  client_id_ = client_manager_->create_client_id();

  dispatcher_ = std::make_shared<engine::UpdateDispatcher>(*client_manager_);
  logger_ = std::make_shared<LoggerListener>();
  dispatcher_->registerListener(logger_);
  dispatcher_->setQueryResponseHandler([this](auto resp) { this->process_response(std::move(resp)); });

  send_query(td_api::make_object<td_api::getOption>("version"), {});
}

void TdExample::loop() {
  while (true) {
    if (need_restart_) {
      restart();
    } else if (!are_authorized_) {
      process_response(client_manager_->receive(10));
    } else {
      std::cout << "Enter action [q] quit [c] show chats [m <chat_id> "
                   "<text>] send message [me] show self [l] logout: "
                 << std::endl;
      std::string line;
      std::getline(std::cin, line);
      std::istringstream ss(line);
      std::string action;
      if (!(ss >> action)) {
        continue;
      }
      if (action == "q") {
        return;
      }
      if (action == "close") {
        std::cout << "Closing..." << std::endl;
        send_query(td_api::make_object<td_api::close>(), {});
      } else if (action == "me") {
        send_query(td_api::make_object<td_api::getMe>(),
                   [this](Object object) { std::cout << to_string(object) << std::endl; });
      } else if (action == "l") {
        std::cout << "Logging out..." << std::endl;
        send_query(td_api::make_object<td_api::logOut>(), {});
      } else if (action == "m") {
        std::int64_t chat_id;
        ss >> chat_id;
        ss.get();

        std::cout << "Sending message to chat " << chat_id << "..." << std::endl;
        auto send_message = td_api::make_object<td_api::sendMessage>();
        send_message->chat_id_ = chat_id;

        // 1. Сначала создаём объект самого голосового файла (inputVoiceNote)
        auto voice_note = td_api::make_object<td_api::inputVoiceNote>();
        voice_note->voice_note_ = td_api::make_object<td_api::inputFileLocal>("/app/user/hi.ogg");
        voice_note->duration_ = 4; // Длительность в секундах
        voice_note->waveform_ = ""; // Волну (можно оставить пустой)

        // 2. Передаём его внутрь контента сообщения (inputMessageVoiceNote)
        auto voice_content = td_api::make_object<td_api::inputMessageVoiceNote>();
        voice_content->voice_note_ = std::move(voice_note);

        send_message->input_message_content_ = std::move(voice_content);
        send_query(std::move(send_message), {});

      } else if (action == "c") {
        std::cout << "Loading chat list..." << std::endl;
        send_query(td_api::make_object<td_api::getChats>(nullptr, 20), [this](Object object) {
          if (object->get_id() == td_api::error::ID) {
            return;
          }
          auto chats = td::move_tl_object_as<td_api::chats>(object);
          for (auto chat_id : chats->chat_ids_) {
            std::cout << "[chat_id:" << chat_id << "] [title:" << chat_title_[chat_id] << "]" << std::endl;
          }
        });
      }
    }
  }
}

void TdExample::restart() {
  client_manager_.reset();
  *this = TdExample();
}

void TdExample::send_query(td_api::object_ptr<td_api::Function> f, std::function<void(Object)> handler) {
  auto query_id = next_query_id();
  if (handler) {
    handlers_.emplace(query_id, std::move(handler));
  }
  client_manager_->send(client_id_, query_id, std::move(f));
}

void TdExample::process_response(td::ClientManager::Response response) {
  if (!response.object) {
    return;
  }
  //std::cout << response.request_id << " " << to_string(response.object) << std::endl;
  if (response.request_id == 0) {
    return process_update(std::move(response.object));
  }
  auto it = handlers_.find(response.request_id);
  if (it != handlers_.end()) {
    it->second(std::move(response.object));
    handlers_.erase(it);
  }
}

std::string TdExample::get_user_name(std::int64_t user_id) const {
  auto it = users_.find(user_id);
  if (it == users_.end()) {
    return "unknown user";
  }
  return it->second->first_name_ + " " + it->second->last_name_;
}

std::string TdExample::get_chat_title(std::int64_t chat_id) const {
  auto it = chat_title_.find(chat_id);
  if (it == chat_title_.end()) {
    return "unknown chat";
  }
  return it->second;
}

void TdExample::process_update(td_api::object_ptr<td_api::Object> update) {
  td_api::downcast_call(
      *update, overloaded(
                   [this](td_api::updateAuthorizationState &update_authorization_state) {
                     authorization_state_ = std::move(update_authorization_state.authorization_state_);
                     on_authorization_state_update();
                   },
                   [this](td_api::updateNewChat &update_new_chat) {
                     chat_title_[update_new_chat.chat_->id_] = update_new_chat.chat_->title_;
                   },
                   [this](td_api::updateChatTitle &update_chat_title) {
                     chat_title_[update_chat_title.chat_id_] = update_chat_title.title_;
                   },
                   [this](td_api::updateUser &update_user) {
                     auto user_id = update_user.user_->id_;
                     users_[user_id] = std::move(update_user.user_);
                   },
                   [this](td_api::updateNewMessage &update_new_message) {
                     auto chat_id = update_new_message.message_->chat_id_;
                     std::string sender_name;
                     td_api::downcast_call(*update_new_message.message_->sender_id_,
                                           overloaded(
                                               [this, &sender_name](td_api::messageSenderUser &user) {
                                                 sender_name = get_user_name(user.user_id_);
                                               },
                                               [this, &sender_name](td_api::messageSenderChat &chat) {
                                                 sender_name = get_chat_title(chat.chat_id_);
                                               }));
                     std::string text;
                     if (update_new_message.message_->content_->get_id() == td_api::messageText::ID) {
                       text = static_cast<td_api::messageText &>(*update_new_message.message_->content_).text_->text_;
                     }
                     std::cout << "Receive message: [chat_id:" << chat_id << "] [from:" << sender_name << "] ["
                               << text << "]" << std::endl;
                   },
                   [](auto &update) {}));
}

auto TdExample::create_authentication_query_handler() {
  return [this, id = authentication_query_id_](Object object) {
    if (id == authentication_query_id_) {
      check_authentication_error(std::move(object));
    }
  };
}

void TdExample::on_authorization_state_update() {
  authentication_query_id_++;
  td_api::downcast_call(*authorization_state_,
                       overloaded(
                            [this](td_api::authorizationStateReady &) {
                              are_authorized_ = true;
                              std::cout << "Authorization is completed" << std::endl;
                              dispatcher_->start();
                            },
                            [this](td_api::authorizationStateLoggingOut &) {
                              are_authorized_ = false;
                              std::cout << "Logging out" << std::endl;
                            },
                            [this](td_api::authorizationStateClosing &) { std::cout << "Closing" << std::endl; },
                            [this](td_api::authorizationStateClosed &) {
                              are_authorized_ = false;
                              need_restart_ = true;
                              std::cout << "Terminated" << std::endl;
                            },
                            [this](td_api::authorizationStateWaitPhoneNumber &) {
      std::cout << "Запрашиваем QR-код для входа..." << std::endl;
      send_query(td_api::make_object<td_api::requestQrCodeAuthentication>(),
                 create_authentication_query_handler());
                                 // std::cout << "Enter phone number: " << std::flush;
                                 // std::string phone_number;
                                 // std::cin >> phone_number;
                                 // send_query(
                                 //     td_api::make_object<td_api::setAuthenticationPhoneNumber>(phone_number, nullptr),
                                 //     create_authentication_query_handler());
                            },
                            [this](td_api::authorizationStateWaitPremiumPurchase &) {
                              std::cout << "Telegram Premium subscription is required" << std::endl;
                            },
                            [this](td_api::authorizationStateWaitEmailAddress &) {
                              std::cout << "Enter email address: " << std::flush;
                              std::string email_address;
                              std::cin >> email_address;
                              send_query(td_api::make_object<td_api::setAuthenticationEmailAddress>(email_address),
                                         create_authentication_query_handler());
                            },
                            [this](td_api::authorizationStateWaitEmailCode &) {
                              std::cout << "Enter email authentication code: " << std::flush;
                              std::string code;
                              std::cin >> code;
                              send_query(td_api::make_object<td_api::checkAuthenticationEmailCode>(
                                             td_api::make_object<td_api::emailAddressAuthenticationCode>(code)),
                                         create_authentication_query_handler());
                            },
                            [this](td_api::authorizationStateWaitCode &wait_code) {
                              std::cout << "Delivery method: " << to_string(wait_code.code_info_->type_) << std::endl;
                              std::cout << "Enter authentication code: " << std::flush;
                              std::string code;
                              std::cin >> code;
                              if (code == "r") {
                                  send_query(td_api::make_object<td_api::resendAuthenticationCode>(nullptr),
                                          create_authentication_query_handler());
                              } else {
                                  send_query(td_api::make_object<td_api::checkAuthenticationCode>(code),
                                          create_authentication_query_handler());
                              }
                            },
                            [this](td_api::authorizationStateWaitRegistration &) {
                              std::string first_name;
                              std::string last_name;
                              std::cout << "Enter your first name: " << std::flush;
                              std::cin >> first_name;
                              std::cout << "Enter your last name: " << std::flush;
                              std::cin >> last_name;
                              send_query(td_api::make_object<td_api::registerUser>(first_name, last_name, false),
                                         create_authentication_query_handler());
                            },
                            [this](td_api::authorizationStateWaitPassword &) {
                              std::cout << "Enter authentication password: " << std::flush;
                              std::string password;
                              std::getline(std::cin, password);
                              send_query(td_api::make_object<td_api::checkAuthenticationPassword>(password),
                                         create_authentication_query_handler());
                            },
                            [this](td_api::authorizationStateWaitOtherDeviceConfirmation &state) {
      std::cout << "\nОтсканируй этот QR-код в Telegram (Настройки -> Устройства -> Подключить устройство):\n" << std::endl;
      std::string cmd = "qrencode -t ANSIUTF8 \"" + state.link_ + "\"";
      std::system(cmd.c_str());
    },
                            //   [this](td_api::authorizationStateWaitOtherDeviceConfirmation &state) {
                            //     std::cout << "Confirm this login link on another device: " << state.link_ << std::endl;
                            //   },
                            [this](td_api::authorizationStateWaitTdlibParameters &) {
   const char* env_id = std::getenv("TG_API_ID");
   const char* env_hash = std::getenv("TG_API_HASH");

   if (!env_id || !env_hash) {
     std::cerr << "[ОШИБКА] Не заданы TG_API_ID или TG_API_HASH в окружении!" << std::endl;
     std::exit(1);
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
                                   send_query(std::move(request), create_authentication_query_handler());
                             }));
}

void TdExample::check_authentication_error(Object object) {
  if (object->get_id() == td_api::error::ID) {
    auto error = td::move_tl_object_as<td_api::error>(object);
    std::cout << "Error: " << to_string(error) << std::flush;
    on_authorization_state_update();
  }
}

std::uint64_t TdExample::next_query_id() {
  return ++current_query_id_;
}
