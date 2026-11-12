#include "UpdateDispatcher.hpp"
#include <iostream>

namespace engine {

// Helper for downcasting in TDLib
namespace detail {
template <class... Fs>
struct overload;

template <class F>
struct overload<F> : public F {
  explicit overload(F f) : F(f) {}
};
template <class F, class... Fs>
struct overload<F, Fs...>
    : public overload<F>, public overload<Fs...> {
  overload(F f, Fs... fs) : overload<F>(f), overload<Fs...>(fs...) {}
  using overload<F>::operator();
  using overload<Fs...>::operator();
};
} // namespace detail

template <class... Fs>
auto overloaded(Fs... f) {
  return detail::overload<Fs...>(f...);
}

UpdateDispatcher::UpdateDispatcher(td::ClientManager& client_manager)
    : client_manager_(client_manager), start_time_(std::chrono::system_clock::now()) {}

UpdateDispatcher::~UpdateDispatcher() {
  stop();
}

void UpdateDispatcher::registerListener(std::shared_ptr<UpdateListener> listener) {
  std::lock_guard<std::mutex> lock(listeners_mutex_);
  listeners_.push_back(listener);
}

void UpdateDispatcher::setQueryResponseHandler(QueryResponseHandler handler) {
  std::lock_guard<std::mutex> lock(handler_mutex_);
  query_response_handler_ = std::move(handler);
}

void UpdateDispatcher::start() {
  if (running_.exchange(true)) return;
  worker_thread_ = std::thread(&UpdateDispatcher::runLoop, this);
}

void UpdateDispatcher::stop() {
  if (!running_.exchange(false)) return;
  if (worker_thread_.joinable()) {
    worker_thread_.join();
  }
}

void UpdateDispatcher::runLoop() {
  while (running_) {
    auto response = client_manager_.receive(100);
    if (!response.object) continue;

    if (response.request_id == 0) {
      handleUpdate(std::move(response.object));
    } else {
      QueryResponseHandler handler;
      {
        std::lock_guard<std::mutex> lock(handler_mutex_);
        handler = query_response_handler_;
      }
      if (handler) {
        handler(std::move(response));
      }
    }
  }
}

void UpdateDispatcher::handleUpdate(td_api::object_ptr<td_api::Object> update) {
  td_api::downcast_call(*update, overloaded(
      [this](td_api::updateNewChat& update_new_chat) {
        if (auto chat = update_new_chat.chat_->get_as<td_api::Chat>()) {
          std::lock_guard<std::mutex> lock(chat_types_mutex_);
          chat_types_[chat->id_] = std::move(chat->type_);
        }
      },
      [this](td_api::updateNewMessage& update_new_message) {
        auto& message = *update_new_message.message_;

        // 1. Ignore Outgoing
        if (message.is_outgoing_) return;

        // 2. Filter Stale Messages
        int64_t start_time_unix = std::chrono::duration_cast<std::chrono::seconds>(
            start_time_.time_since_epoch()).count();
        if (message.date_ < start_time_unix - 10) return;

        // 3. Chat Type Filter
        bool is_private = false;
        {
          std::lock_guard<std::mutex> lock(chat_types_mutex_);
          auto it = chat_types_.find(message.chat_id_);
          if (it != chat_types_.end()) {
            if (it->second->get_id() == td_api::chatTypePrivate::ID) {
              is_private = true;
            }
          } else {
            // If we don't know the chat type yet, we skip it to be safe.
            return;
          }
        }

        if (!is_private) return;

        // 4. Content Extraction
        MessageUpdate msg_update;
        msg_update.chat_id = message.chat_id_;
        msg_update.message_id = message.id_;

        // Sender ID extraction
        td_api::downcast_call(*message.sender_id_, overloaded(
            [this, &msg_update](td_api::messageSenderUser& user) {
              msg_update.sender_id = user.user_id_;
            },
            [this, &msg_update](td_api::messageSenderChat& chat) {
              msg_update.sender_id = chat.chat_id_;
            },
            [](auto&) {}
        ));

        // Content extraction
        if (auto text_content = message.content_->get_as<td_api::messageText>()) {
          msg_update.text = text_content->text_->text_;
        } else if (auto voice_content = message.content_->get_as<td_api::messageVoiceNote>()) {
          if (auto vn = voice_content->voice_note_->get_as<td_api::VoiceNote>()) {
            msg_update.voice = VoiceMetadata{
                static_cast<int32_t>(vn->duration_),
                MediaMetadata{vn->mime_type_, vn->file_size_}
            };
          }
        } else if (auto video_content = message.content_->get_as<td_api::messageVideo>()) {
           msg_update.media = MediaMetadata{
               video_content->mime_type_,
               video_content->file_size_
           };
        } else if (auto photo_content = message.content_->get_as<td_api::messagePhoto>()) {
           // Photo doesn't have mime_type directly in messageContent, 
           // but we can assume image/jpeg or similar.
           msg_update.media = MediaMetadata{"image/jpeg", 0}; 
        }

        // Dispatch to listeners
        std::lock_guard<std::mutex> lock(listeners_mutex_);
        for (auto& listener : listeners_) {
          listener->onMessage(msg_update);
        }
      },
      [](auto&) {}
  ));
}

} // namespace engine
