# MVP 1 (Alpha): Telegram-бот + Presence + Buffer

**Цель:** Бот подключён к Telegram, имитирует человеческое поведение, ждёт пока собеседник допишет мысль. Может общаться, но отвечает заглушкой.

**Статус:** ⬜ Не начато

**Что работает:** Бот авторизуется, читает сообщения, ведёт себя как человек (паузы, "печатает"), склеивает "капли" сообщений. Мозг пока не подключён.

---

## Подзадачи

| # | Задача | Описание | Статус |
|---|---|---|---|
| 01 | Telegram Bot (Channel) | TDLib, авторизация, event loop, filters | ⬜ |
| 02 | Presence Simulation | Reading delays, typing simulator | ⬜ |
| 03 | Message Buffer | Silence window 10с, склейка сообщений | ⬜ |

---

## Интерфейсы

```cpp
// ⚠️ ПРИМЕР — НЕ СПЕЦИФИКАЦИЯ. Интерфейс может измениться.
// Channel interface
class Channel {
public:
    virtual ~Channel() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool isAvailable() const = 0;
    virtual void sendText(const UserId& user, const std::string& text) = 0;
    virtual void sendPhoto(const UserId& user, const std::string& path) = 0;
    virtual void sendVoice(const UserId& user, const std::string& path) = 0;
};

// Telegram implementation
class TelegramChannel : public Channel { /* ... */ };

// Human-like behavior
class PresenceSimulator { /* ... */ };

// Message pipeline
class MessageBuffer { /* ... */ };

// Brain stub — пока заглушка
class Brain {
public:
    std::string process(const std::string& input); // возвращает заглушку
};
```

## Верификация MVP 1

- [ ] Бот авторизуется по QR / номеру телефона
- [ ] Сессия сохраняется после перезапуска
- [ ] Пауза чтения 1.5–3с перед ответом
- [ ] Статус "печатает" динамический (зависит от длины ответа)
- [ ] "Капли" сообщений склеиваются
- [ ] Бот не падает, работает стабильно
