# 03: Message Buffer

**Epic:** 001 (MVP 1 Alpha)
**Phase:** 3

## Requirements

### 1. In-Memory Buffer
- Dictionary `user_id → std::vector<std::string>` (messages)
- Dictionary `user_id → Timer` (silence window)

### 2. Silence Window 10s
- If no new message from user_id within 10 seconds — buffer is flushed
- New message resets the 10s timer

### 3. Merging
- Messages are merged via `\n`
- Brain receives one logical block, responds with one block

## Interface

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. The interface may change.
class MessageBuffer {
public:
    void addMessage(UserId userId, const std::string& text);
    void onSilence(UserId userId, std::function<void(std::vector<std::string>)> callback);
    void flush(UserId userId);
};
```

## Verification

- [ ] Single message → flush after 10s → sent to Brain
- [ ] "Drops" (3 messages with 3–5s interval) → one merged block, 10s after the last
- [ ] Timer resets on new message
