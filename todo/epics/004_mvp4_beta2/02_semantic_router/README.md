# 02: Semantic Router

**Epic:** 004 (MVP 4 Beta 2)
**Phase:** 7

## Requirements

### 1. Cosine Similarity (pure C++)
```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. Interface may change.
float cosineSimilarity(const std::vector<float>& a,
                         const std::vector<float>& b);
```
Dot product / product of norms.

### 2. Semantic Search Flow
1. Current message → embedding via llama.cpp `/api/embed` (or `/v1/embeddings`)
2. Load all embeddings from DB
3. Calculate cosine similarity for each
4. Return TOP-K (5-10) most similar pairs (question → answer from the past)

### 3. Integration
- Called on buffer flush (MVP 1)
- Found pairs → example for system prompt (MVP 4)
- If llama.cpp is unavailable or DB is empty → return empty list, don't crash

## Interface

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. Interface may change.
class SemanticRouter {
public:
    std::vector<SimilarPair> search(const std::vector<float>& queryVec, int topK = 5);
    std::vector<SimilarPair> searchByText(const std::string& text, int topK = 5);
};

struct SimilarPair {
    int messageId;
    std::string question;
    std::string answer;
    float similarity;
};
```

## Verification

- [ ] Typing a phrase from the archive → returns matching historical pairs
- [ ] Empty DB → works without errors
- [ ] llama.cpp unavailable → works without errors
