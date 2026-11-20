# 03: Proactive Engine

**Epic:** 005 (MVP 5 RC)
**Phase:** 9

## Requirements

### 1. Triggers
- Time since last message
- Context (missed day, special dates)
- Mood (if negative → show care)

### 2. Message Types
- "How are you?"
- "Miss you"
- "Look what I found" (with photo)
- News / interesting topic

### 3. Schedule
- Morning: "Good morning!", question about plans
- Day: "How are you?", check-in
- Evening: "How was your day?", "Good night"
- Rarely: initiative at any time

## Interface

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. The interface may change.
class ProactiveEngine {
public:
    void scheduleCheck();
    std::optional<ProactiveMessage> shouldInitiate();
};
```

## Verification

- [ ] The bot initiates contact after a long silence
- [ ] Initiative matches the schedule
- [ ] No spamming (initiative coefficient < 3 per day)
