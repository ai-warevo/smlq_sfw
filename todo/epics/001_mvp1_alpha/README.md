# MVP 1 (Alpha): Telegram Bot + Presence + Buffer

**Goal:** Bot connected to Telegram, simulates human behavior, waits for the other person to finish their thought. Can chat, but responds with a stub.

**Status:** ⬜ Not started

**What works:** Bot authenticates, reads messages, behaves like a human (pauses, "typing"), merges message "drops". Brain not connected yet.

---

## Subtasks

| # | Task | Description | Status |
|---|---|---|---|
| 01 | Telegram Bot (Channel) | TDLib, authentication, event loop, filters | ⬜ |
| 02 | Presence Simulation | Reading delays, typing simulator | ⬜ |
| 03 | Message Buffer | Silence window 10s, message merging | ⬜ |

---

## Interfaces

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. The interface may change.
// Channel interface
class Channel {
public:
    virtual ~Channel() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool isAvailable() const = 0;
    virtual void sendText(const UserId& user, const std::string& text) = 0;
    virtual void sendPhoto(const UserId& user, const std::string& path) = 0;
    virtual void sendVoice(const UserId& user, const std::string& path) = 0;
};

// Telegram implementation
class TelegramChannel : public Channel { /* ... */ };

// Human-like behavior
class PresenceSimulator { /* ... */ };

// Message pipeline
class MessageBuffer { /* ... */ };

// Brain stub — stub for now
class Brain {
public:
    std::string process(const std::string& input); // returns a stub
};
```

## MVP 1 Verification

- [ ] Bot authenticates via QR / phone number
- [ ] Session persists after restart
- [ ] Reading pause 1.5–3s before response
- [ ] "Typing" status is dynamic (depends on response length)
- [ ] Message "drops" are merged
- [ ] Bot doesn't crash, runs stably
