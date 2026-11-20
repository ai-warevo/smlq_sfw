# 01: ComfyUI Client

**Epic:** 006 (MVP 6 Release)
**Phase:** 10

## Requirements

### 1. ComfyUI Client
- HTTP requests to ComfyUI API
- Workflow management (portrait, dataset, utilities)
- Photo generation from prompt

### 2. Brain Integration
- On user request or on schedule
- Generation prompt is formed based on context
- Sent via Channel

## Interface

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. The interface may change.
class ComfyUIClient {
public:
    std::string generatePhoto(const std::string& prompt);
    bool isAvailable() const;
};
```

## Verification

- [ ] The bot generates photos on request
- [ ] Photo is sent via Channel
