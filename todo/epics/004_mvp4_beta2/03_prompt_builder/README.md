# 03: Prompt Builder

**Epic:** 004 (MVP 4 Beta 2)
**Phase:** 8

## Requirements

### 1. Prompt Pie (prompt structure)

```
═══════════════════════════════════════════
│ SYSTEM PROMPT (static constants)          │
│  - Name, age, appearance                  │
│  - Communication style                    │
│  - Trigger word: smlq_sfw                 │
│  - Forbidden topics                       │
═══════════════════════════════════════════
│ SEMANTIC EXAMPLES (Semantic Router)       │
│  - TOP-K similar historical pairs         │
│  - Question → Answer from the past        │
═══════════════════════════════════════════
│ RECENT HISTORY (Memory System)            │
│  - Last N messages of current conversation│
═══════════════════════════════════════════
│ CURRENT INPUT (incoming message)          │
│  - Text / voice / photo                   │
═══════════════════════════════════════════
```

### 2. Prompt Builder
- System Prompt Builder — static
- Semantic Example Fetcher — query to Semantic Router
- History Fetcher — query to Memory System
- Assembly — stitch all parts into a single prompt
- Token estimation — length estimation (don't exceed LLM context limit)

### 3. Integration
- Assembled prompt → LLM Client → response → Channel → send

## Interface

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. Interface may change.
class PromptBuilder {
public:
    std::string buildFullPrompt(const Context& ctx);
    int estimateTokens(const std::string& prompt);
};
```

## Verification

- [ ] Bot's response sounds personal (style from past + context)
- [ ] Semantic examples influence response style
- [ ] Context doesn't bloat (limit: system + TOP-2 pairs + 10 messages)
