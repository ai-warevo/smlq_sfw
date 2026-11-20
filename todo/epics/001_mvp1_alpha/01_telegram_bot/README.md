# 01: Telegram Bot (Channel)

**Epic:** 001 (MVP 1 Alpha)
**Phase:** 1

## Requirements

### 1. TelegramBot (Channel Interface)
- Implement `TelegramChannel` class implementing the `Channel` interface
- Wrapper around TDLib `ClientManager`
- Send text, photos, voice messages via TDLib

### 2. Authentication (Auth State Machine)
- Support QR code / phone number / verification code / 2FA
- Session persistence — save session to `data/tdlib_db/`
- On restart — skip authentication if session exists

### 3. Event Loop
- Infinite `receive()` with timeout
- Dispatch updates to UpdateDispatcher

### 4. Graceful Shutdown
- SIGINT/SIGTERM → stop event loop → close TDLib → flush sessions

### 5. Message Filters
- Ignore outgoing messages (prevent self-response loops)
- Filter old messages (ignore messages older than 10s — backlog at startup)
- Only private chats (ignore groups/channels)
- Extract content: text, voice messages, photos

## Interface

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. The interface may change.
class TelegramChannel : public Channel {
public:
    void start() override;
    void stop() override;
    bool isAvailable() const override;
    void sendText(const UserId& user, const std::string& text) override;
    void sendPhoto(const UserId& user, const std::string& path) override;
    void sendVoice(const UserId& user, const std::string& path) override;
    UserId getCurrentUserId() const override;
    std::string getUserName(const UserId& user) const override;
};
```

## Verification

- [ ] Bot authenticates via QR / phone number
- [ ] Session persists after container restart
- [ ] Incoming messages from owner are processed
- [ ] Outgoing messages are ignored
- [ ] Old messages are ignored
