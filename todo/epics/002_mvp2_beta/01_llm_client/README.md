# 01: LLM Client

**Epic:** 002 (MVP 2 Beta)
**Phase:** 4

## Requirements

### 1. LLM Client
- Async POST request to llama.cpp API (`/api/chat`)
- Request format:
  ```json
  {
    "model": "qwen3-vl-8b",
    "messages": [{"role": "user", "content": "<TEXT>"}],
    "stream": false
  }
  ```
- Parse SSE response / JSON

### 2. System Prompt (basic)
- Short communication style rule (conversational, no capitals, parentheses instead of emojis)
- Full prompt will be assembled in Phase 8

### 3. Error Handling
- If LLM is unavailable — log, don't crash
- Retry with exponential backoff (optional)

### 4. Channel Integration
- Brain builds prompt → LLM Client → response → Channel (Presence Simulator → send)

## Interface

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. The interface may change.
class LLMClient {
public:
    std::string generate(const std::string& prompt, int maxTokens = 512);
    bool isAvailable() const;
    void setBaseUrl(const std::string& url); // http://localhost:6969
};
```

## Verification

- [ ] Brain sends text → receives meaningful response
- [ ] Response style matches the system prompt
- [ ] LLM unavailable → bot doesn't crash
