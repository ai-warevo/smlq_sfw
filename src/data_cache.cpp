#include "data_cache.hpp"

void DataCache::update_user(td_api::object_ptr<td_api::user> user) {
    if (!user) return;
    std::lock_guard lock(mutex_);
    std::string name = user->first_name_;
    if (!user->last_name_.empty()) {
        name += " " + user->last_name_;
    }
    users_[user->id_] = name;
}

void DataCache::update_chat(td_api::object_ptr<td_api::chat> chat) {
    if (!chat) return;
    std::lock_guard lock(mutex_);
    chats_[chat->id_] = chat->title_;
}

std::string DataCache::get_user_name(int64_t user_id) const {
    std::lock_guard lock(mutex_);
    auto it = users_.find(user_id);
    return (it != users_.end()) ? it->second : std::to_string(user_id);
}

std::string DataCache::get_chat_title(int64_t chat_id) const {
    std::lock_guard lock(mutex_);
    auto it = chats_.find(chat_id);
    return (it != chats_.end()) ? it->second : std::to_string(chat_id);
}
