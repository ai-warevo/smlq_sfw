#pragma once

#include "UpdateListener.hpp"
#include "PresenceSimulator.hpp" // For IQuerySender and PresenceSimulator
#include <td/telegram/td_api.hpp>
#include <iostream>
#include <vector>
#include <string>

namespace engine {

/**
 * @brief A simple listener that echoes back any text messages received in private chats.
 */
class EchoListener : public UpdateListener {
public:
    explicit EchoListener(IQuerySender& sender, PresenceSimulator* simulator = nullptr) 
        : sender_(sender), simulator_(simulator) {}

    void onMessage(const MessageUpdate& update) override {
        // We only echo text messages in private chats (the dispatcher already filters for private chats)
        if (update.text) {
            std::string text = *update.text;
            std::int64_t chat_id = update.chat_id;

            if (simulator_) {
                std::cout << "[EchoListener] Received message, simulating response for chat " << chat_id << "..." << std::endl;
                
                simulator_->simulateResponse(chat_id, text.length(), [this, chat_id, text]() {
                    std::cout << "[EchoListener] Executing delayed echo to chat " << chat_id << ": " << text << std::endl;

                    auto send_message = td::td_api::make_object<td::td_api::sendMessage>();
                    send_message->chat_id_ = chat_id;

                    auto text_content = td::td_api::make_object<td::td_api::inputMessageText>();
                    text_content->text_ = td::td_api::make_object<td::td_api::formattedText>(text, std::vector<td::td_api::object_ptr<td::td_api::textEntity>>{});

                    send_message->input_message_content_ = std::move(text_content);
                    sender_.send_query(std::move(send_message));
                });
            } else {
                // Fallback if no simulator
                std::cout << "[EchoListener] Echoing (no simulator): " << text << " to chat " << chat_id << std::endl;
                auto send_message = td::td_api::make_object<td::td_api::sendMessage>();
                send_message->chat_id_ = chat_id;

                auto text_content = td::td_api::make_object<td::td_api::inputMessageText>();
                text_content->text_ = td::td_api::make_object<td::td_api::formattedText>(text, std::vector<td::td_api::object_ptr<td::td_api::textEntity>>{});

                send_message->input_message_content_ = std::move(text_content);
                sender_.send_query(std::move(send_message));
            }
        }
    }

private:
    IQuerySender& sender_;
    PresenceSimulator* simulator_;
};

} // namespace engine
