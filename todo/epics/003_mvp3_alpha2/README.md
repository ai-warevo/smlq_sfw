# MVP 3 (Alpha 2): Память (SQLite)

**Цель:** Мозг помнит историю диалогов. Данные сохраняются между сессиями в SQLite. Бот начинает "помнить" имя пользователя, прошлые разговоры.

**Статус:** ⬜ Не начато

**Что работает:** Всё из MVP 2 + история диалогов сохраняется в SQLite, бот может подсмотреть последние сообщения.

---

## Подзадачи

| # | Задача | Описание | Статус |
|---|---|---|---|
| 01 | SQLite Repository | Таблицы messages, facts | ⬜ |
| 02 | Memory Integration | Запись/чтение из Brain | ⬜ |

---

## Интерфейсы

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

## Верификация MVP 3

- [ ] Мозг помнит имя пользователя из прошлого диалога
- [ ] История сохраняется после перезапуска контейнера
- [ ] Факты сохраняются и извлекаются
- [ ] SQLite-запросы не блокируют event loop
