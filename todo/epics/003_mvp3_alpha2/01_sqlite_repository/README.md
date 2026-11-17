# 01: SQLite Repository

**Epic:** 003 (MVP 3 Alpha 2)
**Фаза:** 5

## ТЗ

### 1. SQLite Repository
- Таблица `messages`:
  - `id` (INTEGER PRIMARY KEY)
  - `chat_id` (BIGINT)
  - `user_id` (BIGINT)
  - `role` (TEXT: 'user' / 'assistant')
  - `content` (TEXT)
  - `timestamp` (DATETIME)
  - `sentiment` (REAL)
  - `embedding_id` (INTEGER)

- Таблица `facts`:
  - `id` (INTEGER PRIMARY KEY)
  - `category` (TEXT)
  - `key` (TEXT)
  - `value` (TEXT)
  - `confidence` (REAL)
  - `created_at` (DATETIME)
  - `updated_at` (DATETIME)

### 2. Запись
- После flush буфера → записать входящее сообщение в БД
- После ответа LLM → записать ответ в БД
- Извлечённые факты → таблица `facts`

### 3. Контекст для LLM
- Перед запросом к LLM: последние 10 сообщений из БД → история диалога

### 4. Docker
- Volume `./db_data:/app/db_data` для сохранения БД

### 5. Асинхронность
- SQLite-запросы в фоновом потоке, не блокируя event loop

## Интерфейс

```cpp
class MemorySystem {
public:
    void saveMessage(const Message& msg);
    std::vector<Message> getRecent(int chatId, int count);
    void saveFact(const std::string& category, const std::string& key,
                  const std::string& value, float confidence);
    std::vector<Fact> getFacts(const std::string& category);
};
```

## Верификация

- [ ] Мозг помнит имя пользователя из прошлого диалога
- [ ] История сохраняется после перезапуска контейнера
- [ ] Факты сохраняются и извлекаются
