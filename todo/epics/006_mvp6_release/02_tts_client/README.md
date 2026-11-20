# 02: TTS Client

**Epic:** 006 (MVP 6 Release)
**Phase:** 14

## Requirements

### 1. TTS Client
- HTTP requests to TTS service
- Voice message synthesis from text
- Settings: voice, speed, tone

### 2. TTSSettings
- Voice selection
- Speech speed
- Emotional coloring (based on Mood Engine)

### 3. Brain Integration
- On user request or proactive (morning/evening voice message)
- Sent via Channel as a voice message

## Interface

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. The interface may change.
struct TTSSettings {
    std::string voice;
    float speed;
    std::string emotion;
};

class TTSClient {
public:
    std::string synthesize(const std::string& text, const TTSSettings& settings);
    bool isAvailable() const;
};
```

## Verification

- [ ] The bot generates voice messages
- [ ] Voice matches the settings
