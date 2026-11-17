# 01: Telegram Bot (Channel)

**Эpic:** 001 (MVP 1 Alpha)
**Фаза:** 1

## ТЗ

### 1. TelegramBot (Channel Interface)
- Реализовать класс `TelegramChannel`, реализующий `Channel` интерфейс
- Обёртка над TDLib `ClientManager`
- Отправка текста, фото, голосовых через TDLib

### 2. Авторизация (Auth State Machine)
- Поддержка QR-код / номер телефона / код / 2FA
- Session persistence — сохранение сессии в `data/tdlib_db/`
- При перезапуске — пропускать авторизацию, если сессия есть

### 3. Event Loop
- Бесконечный `receive()` с timeout
- Dispatch обновлений в UpdateDispatcher

### 4. Graceful Shutdown
- SIGINT/SIGTERM → stop event loop → close TDLib → flush сессии

### 5. Message Filters
- Игнорировать исходящие сообщения (prevent self-response loops)
- Фильтровать старые (игнорировать старше 10с — backlog при старте)
- Только private chats (игнорировать группы/каналы)
- Извлекать контент: текст, голосовые, фото

## Интерфейс

```cpp
// ⚠️ ПРИМЕР — НЕ СПЕЦИФИКАЦИЯ. Интерфейс может измениться.
class TelegramChannel : public Channel {
public:
    void start() override;
    void stop() override;
    bool isAvailable() const override;
    void sendText(const UserId& user, const std::string& text) override;
    void sendPhoto(const UserId& user, const std::string& path) override;
    void sendVoice(const UserId& user, const std::string& path) override;
    UserId getCurrentUserId() const override;
    std::string getUserName(const UserId& user) const override;
};
```

## Верификация

- [ ] Бот авторизуется по QR / номеру телефона
- [ ] Сессия сохраняется после перезапуска контейнера
- [ ] Входящие сообщения от владельца обрабатываются
- [ ] Исходящие сообщения игнорируются
- [ ] Старые сообщения игнорируются
