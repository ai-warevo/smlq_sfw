# MVP 4 (Beta 2): Semantic Router + Prompt Builder

**Goal:** The brain searches its past for similar situations and assembles full context for the LLM. Responses become more personalized.

**Status:** ⬜ Not started

**What works:** Everything from MVP 3 + bot searches for similar historical pairs, assembles "Prompt Pie" from system + semantics + history.

---

## Subtasks

| # | Task | Description | Status |
|---|---|---|---|
| 01 | Historical Ingestion | Import Telegram JSON export | ⬜ |
| 02 | Semantic Router | Cosine similarity, search for similar pairs | ⬜ |
| 03 | Prompt Builder | Assemble full prompt (Prompt Pie) | ⬜ |

---

## Interfaces

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. Interface may change.
class HistoricalIngestion {
public:
    void parseJSON(const std::string& filePath);
    void generateEmbeddings();
    void saveToDatabase();
};

class SemanticRouter {
public:
    std::vector<SimilarPair> search(const std::vector<float>& queryVec, int topK = 5);
    std::vector<SimilarPair> searchByText(const std::string& text, int topK = 5);
};

class PromptBuilder {
public:
    std::string buildFullPrompt(const Context& ctx);
    int estimateTokens(const std::string& prompt);
};
```

## MVP 4 Verification

- [ ] Bot's response sounds personal (style from past + context)
- [ ] Semantic examples influence response style
- [ ] Context doesn't bloat (limit: system + TOP-2 pairs + 10 messages)
