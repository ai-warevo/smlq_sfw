# MVP 2 (Beta): LLM Integration

**Goal:** Bot begins responding meaningfully via a local LLM (Qwen3-VL-8B / llama.cpp). Replaces the stub with a real response.

**Status:** ⬜ Not started

**What works:** Everything from MVP 1 + bot responds via Qwen3-VL-8B. Basic communication style (system prompt).

---

## Subtasks

| # | Task | Description | Status |
|---|---|---|---|
| 01 | LLM Client | HTTP requests to llama.cpp API | ⬜ |
| 02 | System Prompt | Basic communication style | ⬜ |

---

## Interfaces

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. The interface may change.
class LLMClient {
public:
    std::string generate(const std::string& prompt, int maxTokens = 512);
    bool isAvailable() const;
    void setBaseUrl(const std::string& url); // http://localhost:6969
};
```

## MVP 2 Verification

- [ ] Bot responds meaningfully via Qwen3-VL-8B
- [ ] Response style matches the system prompt
- [ ] LLM unavailable → bot doesn't crash
- [ ] Response passes through Presence Simulator before sending
