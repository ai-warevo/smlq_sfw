#pragma once

#include <td/telegram/td_api.h>
#include <map>
#include <string>
#include <mutex>

namespace td_api = td::td_api;

class DataCache {
public:
    void update_user(td_api::object_ptr<td_api::user> user);
    void update_chat(td_api::object_ptr<td_api::chat> chat);

    std::string get_user_name(int64_t user_id) const;
    std::string get_chat_title(int64_t chat_id) const;

private:
    mutable std::mutex mutex_;
    std::map<int64_t, std::string> users_; // id -> name
    std::map<int64_t, std::string> chats_; // id -> title
};
