# 01: Historical Ingestion

**Epic:** 004 (MVP 4 Beta 2)
**Phase:** 6

## Requirements

### 1. JSON Parser
- Telegram Desktop export parser (`result.json`)
- Sessionization: messages from the same author with interval < 60s = one block
- Session break: pause > 30min
- Filters: ignore media placeholders, system messages, forwarded messages

### 2. Embedding Pipeline
- For each `user_message` → POST to llama.cpp `/api/embed` (or `/v1/embeddings`)
- Save vector to `embeddings` table

### 3. `embeddings` table
- `id` (INTEGER PRIMARY KEY)
- `message_id` (INTEGER REFERENCES messages(id))
- `vector` (BLOB — binary float array)
- `created_at` (DATETIME)

### 4. Facts
- Named entity extraction (names, dates, places)
- Save to `facts` table

### 5. Progress
- Print counter to console (N messages processed)

## Interface

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. Interface may change.
class HistoricalIngestion {
public:
    void parseJSON(const std::string& filePath);
    void generateEmbeddings();
    void saveToDatabase();
};
```

## Verification

- [ ] Parser handles large JSON without crashes
- [ ] Messages with vectors written to DB
- [ ] llama.cpp returns correct embeddings
