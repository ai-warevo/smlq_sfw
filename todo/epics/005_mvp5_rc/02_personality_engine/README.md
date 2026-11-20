# 02: Personality Engine

**Epic:** 005 (MVP 5 RC)
**Phase:** 12

## Requirements

### 1. System Prompt Builder
- Extended prompt with full Samantha specification
- Name, age, appearance, style, word triggers
- Forbidden topics

### 2. Digital Twin
- Analysis of the owner's real conversations
- Copying style, mannerisms, expressions
- Integration with Historical Ingestion (MVP 4)

## Interface

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. The interface may change.
class PersonalityEngine {
public:
    std::string getPersonalityProfile();
    void learnFromHistory(const std::vector<Message>& history);
};
```

## Verification

- [ ] The bot responds in Samantha's style
- [ ] Style is consistent with the owner's digital twin
