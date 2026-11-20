# 02: Presence Simulation

**Epic:** 001 (MVP 1 Alpha)
**Phase:** 2

## Requirements

### 1. Reading Delays
- After receiving a message — random delay 1.5–3.0s before processing
- Does not block the event loop (asynchronously via coroutines or thread pool)
- Randomization for naturalness

### 2. Typing Simulator
- Send `sendChatAction("typing")` via TDLib
- Duration: `max(2s, min(10s, response_length * 0.15))`
- Automatically send `sendChatAction("cancelled")` after sending a message

### 3. Human-like Variations (optional)
- Sometimes "start typing", stop, and continue
- Rarely — "forgot" to reply immediately (simulates distraction)

## Interface

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. The interface may change.
class PresenceSimulator {
public:
    void simulateReading(UserId userId, int messageLength);
    void simulateTyping(UserId userId, int responseLength);
    void cancelTyping(UserId userId);
};
```

## Verification

- [ ] Short message: "typing" ~2s
- [ ] Long text: "typing" longer, proportional to length
- [ ] No overlapping typing statuses
- [ ] Reading pause 1.5–3s before processing
