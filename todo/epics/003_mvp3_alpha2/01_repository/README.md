# 01: Repository — хранение сообщений и фактов

**Epic:** 003 (MVP 3 Alpha 2)
**Фаза:** 5

## ТЗ

### 1. БД
- **Движок:** TBD — SQLite по умолчанию, возможен PostgreSQL / другой
- Выбор делается на старте MVP 3, до реализации

### 2. Таблица `messages`
- `id` (PRIMARY KEY)
- `chat_id` (BIGINT)
- `user_id` (BIGINT)
- `role` (TEXT: 'user' / 'assistant')
- `content` (TEXT)
- `timestamp` (DATETIME)
- `sentiment` (REAL)
- `embedding_id` (INTEGER)

### 3. Таблица `facts`
- `id` (PRIMARY KEY)
- `category` (TEXT)
- `key` (TEXT)
- `value` (TEXT)
- `confidence` (REAL)
- `created_at` (DATETIME)
- `updated_at` (DATETIME)

### 4. Запись
- После flush буфера → записать входящее сообщение в БД
- После ответа LLM → записать ответ в БД
- Извлечённые факты → таблица `facts`

### 5. Контекст для LLM
- Перед запросом к LLM: последние 10 сообщений из БД → история диалога

### 6. Docker
- Volume для данных БД (`./db_data`)

### 7. Асинхронность
- Запросы к БД в фоновом потоке, не блокируя event loop

## Интерфейс

```cpp
// ⚠️ ПРИМЕР — НЕ СПЕЦИФИКАЦИЯ. Интерфейс может измениться.
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
