# MVP 3 (Alpha 2): Память

**Цель:** Мозг помнит историю диалогов. Данные сохраняются между сессиями. Бот начинает "помнить" имя пользователя, прошлые разговоры.

**БД:** TBD — SQLite по умолчанию, возможен PostgreSQL / другой (выбор при старте MVP 3)

**Статус:** ⬜ Не начато

**Что работает:** Всё из MVP 2 + история диалогов сохраняется в БД, бот может подсмотреть последние сообщения.

---

## Подзадачи

| # | Задача | Описание | Статус |
|---|---|---|---|
| 01 | Repository | Таблицы messages, facts | ⬜ |
| 02 | Memory Integration | Запись/чтение из Brain | ⬜ |

---

## Интерфейсы

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

## Верификация MVP 3

- [ ] Мозг помнит имя пользователя из прошлого диалога
- [ ] История сохраняется после перезапуска контейнера
- [ ] Факты сохраняются и извлекаются
- [ ] Запросы к БД не блокируют event loop
