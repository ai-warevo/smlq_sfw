# 01: Repository — storing messages and facts

**Epic:** 003 (MVP 3 Alpha 2)
**Phase:** 5

## Requirements

### 1. DB
- **Engine:** TBD — SQLite by default, PostgreSQL / other possible
- Selection is made at MVP 3 start, before implementation

### 2. `messages` table
- `id` (PRIMARY KEY)
- `chat_id` (BIGINT)
- `user_id` (BIGINT)
- `role` (TEXT: 'user' / 'assistant')
- `content` (TEXT)
- `timestamp` (DATETIME)
- `sentiment` (REAL)
- `embedding_id` (INTEGER)

### 3. `facts` table
- `id` (PRIMARY KEY)
- `category` (TEXT)
- `key` (TEXT)
- `value` (TEXT)
- `confidence` (REAL)
- `created_at` (DATETIME)
- `updated_at` (DATETIME)

### 4. Writing
- After buffer flush → write incoming message to DB
- After LLM response → write response to DB
- Extracted facts → `facts` table

### 5. Context for LLM
- Before LLM request: last 10 messages from DB → conversation history

### 6. Docker
- Volume for DB data (`./db_data`)

### 7. Asynchrony
- DB queries in background thread, not blocking the event loop

## Interface

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. Interface may change.
class MemorySystem {
public:
    void saveMessage(const Message& msg);
    std::vector<Message> getRecent(int chatId, int count);
    void saveFact(const std::string& category, const std::string& key,
                   const std::string& value, float confidence);
    std::vector<Fact> getFacts(const std::string& category);
};
```

## Verification

- [ ] The brain remembers the user's name from a past conversation
- [ ] History persists after container restart
- [ ] Facts are saved and retrievable
