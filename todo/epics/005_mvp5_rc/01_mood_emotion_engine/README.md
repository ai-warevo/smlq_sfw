# 01: Mood & Emotion Engine

**Epic:** 005 (MVP 5 RC)
**Phase:** 12

## Requirements

### 1. Mood Model
- Mood: `positive`, `neutral`, `negative`
- Emotion: `happy`, `sad`, `angry`, `excited`, `calm`, `worried`
- State: `energetic`, `tired`, `distracted`

### 2. Updates
- Sentiment analysis of incoming messages
- Context of the last N messages
- Time of day (morning/day/evening/night)

### 3. Integration with Prompt Builder
- Mood modifiers are added to the system prompt
- Emotion affects response style

## Interface

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. The interface may change.
class MoodEngine {
public:
    Mood getCurrentMood() const;
    void updateMood(const std::string& input, float sentiment);
    std::string getMoodModifiers(); // for Prompt Builder
};
```

## Verification

- [ ] Mood changes depending on context
- [ ] Emotional coloring affects responses
