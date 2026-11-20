# MVP 3 (Alpha 2): Memory

**Goal:** The brain remembers conversation history. Data persists between sessions. The bot starts to "remember" the user's name, past conversations.

**DB:** TBD — SQLite by default, PostgreSQL / other possible (selected at MVP 3 start)

**Status:** ⬜ Not started

**What works:** Everything from MVP 2 + conversation history is saved to DB, bot can retrieve recent messages.

---

## Subtasks

| # | Task | Description | Status |
|---|---|---|---|
| 01 | Repository | messages, facts tables | ⬜ |
| 02 | Memory Integration | Write/read to Brain | ⬜ |

---

## Interfaces

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

## MVP 3 Verification

- [ ] The brain remembers the user's name from a past conversation
- [ ] History persists after container restart
- [ ] Facts are saved and retrievable
- [ ] DB queries don't block the event loop
