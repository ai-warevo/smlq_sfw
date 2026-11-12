#pragma once

#include "UpdateListener.hpp"
#include "PresenceSimulator.hpp"
#include "MessageManager.hpp"
#include <td/telegram/td_api.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <numeric>

namespace engine {

/**
 * @brief A listener that buffers messages and echoes them back as a single combined response after a period of silence.
 */
class BufferingEchoListener : public UpdateListener {
public:
    BufferingEchoListener(IQuerySender& sender, PresenceSimulator* simulator, std::chrono::seconds timeout = std::chrono::seconds(3))
        : sender_(sender), 
          simulator_(simulator),
          message_manager_([this](std::int64_t chat_id, std::vector<std::string> messages) {
              this->handle_flush(chat_id, std::move(messages));
          }, timeout) {}

    void onMessage(const MessageUpdate& update) override {
        if (update.text) {
            message_manager_.addMessage(update.chat_id, *update.text);
        }
    }

private:
    void handle_flush(std::int64_t chat_id, std::vector<std::string> messages) {
        if (messages.empty()) return;

        // Склеиваем все сообщения в одно, разделяя их переносом строки
        std::string combined_text;
        for (size_t i = 0; i < messages.size(); ++i) {
            combined_text += messages[i];
            if (i < messages.size() - 1) {
                combined_text += "\n";
            }
        }

        std::cout << "[BufferingEchoListener] Flushing " << messages.size() << " messages as one for chat " << chat_id << "..." << std::endl;

        if (simulator_) {
            // Симулируем ответ на всё объединенное сообщение целиком
            simulator_->simulateResponse(chat_id, combined_text.length(), [this, chat_id, text = std::move(combined_text)]() {
                send_echo(chat_id, text);
            });
        } else {
            // Fallback если нет симулятора
            send_echo(chat_id, combined_text);
        }
    }

    void send_echo(std::int64_t chat_id, const std::string& text) {
        auto send_message = td::td_api::make_object<td::td_api::sendMessage>();
        send_message->chat_id_ = chat_id;

        auto text_content = td::td_api::make_object<td::td_api::inputMessageText>();
        text_content->text_ = td::td_api::make_object<td::td_api::formattedText>(text, std::vector<td::td_api::object_ptr<td::td_api::textEntity>>{});

        send_message->input_message_content_ = std::move(text_content);
        sender_.send_query(std::move(send_message));
    }

    IQuerySender& sender_;
    PresenceSimulator* simulator_;
    MessageManager message_manager_;
};

} // namespace engine
