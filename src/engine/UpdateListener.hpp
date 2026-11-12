#pragma once

#include <string>
#include <variant>
#include <optional>
#include <cstdint>

namespace engine {

struct MediaMetadata {
    std::string mime_type;
    int64_t file_size;
};

struct VoiceMetadata {
    int32_t duration;
    MediaMetadata media;
};

struct MessageUpdate {
    int64_t chat_id;
    int64_t message_id;
    int64_t sender_id;
    std::optional<std::string> text;
    std::optional<VoiceMetadata> voice;
    std::optional<MediaMetadata> media;
};

class UpdateListener {
public:
    virtual ~UpdateListener() = default;
    virtual void onMessage(const MessageUpdate& update) = 0;
};

} // namespace engine
