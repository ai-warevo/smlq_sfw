# MVP 5 (RC): Brain Core — Mood, Personality, Proactive

**Goal:** The brain gains a "soul" — mood, emotions, and initiative. The bot doesn't just respond; it shows interest and remembers context emotionally.

**Status:** ⬜ Not started

**What works:** Everything from MVP 4 + the bot has a mood, shows initiative, and responds with emotional coloring.

---

## Subtasks

| # | Task | Description | Status |
|---|---|---|---|
| 01 | Mood & Emotion Engine | Mood, emotions, state | ⬜ |
| 02 | Personality Engine | System prompt, digital twin | ⬜ |
| 03 | Proactive Engine | Initiative, schedule, triggers | ⬜ |

---

## Interfaces

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. The interface may change.
class MoodEngine {
public:
    Mood getCurrentMood() const;
    void updateMood(const std::string& input, float sentiment);
    std::string getMoodModifiers(); // for Prompt Builder
};

class PersonalityEngine {
public:
    std::string getPersonalityProfile();
    void learnFromHistory(const std::vector<Message>& history);
};

class ProactiveEngine {
public:
    void scheduleCheck();
    std::optional<ProactiveMessage> shouldInitiate();
};
```

## MVP 5 Verification

- [ ] The bot changes its mood depending on context
- [ ] The bot initiates contact (proactive)
- [ ] Responses are emotionally colored
- [ ] Personality is consistent with the digital twin
