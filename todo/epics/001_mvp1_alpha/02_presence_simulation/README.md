# 02: Presence Simulation

**Epic:** 001 (MVP 1 Alpha)
**Фаза:** 2

## ТЗ

### 1. Reading Delays
- После получения сообщения — рандомная задержка 1.5–3.0 сек перед обработкой
- Не блокирует event loop (асинхронно через корутины или thread pool)
- Рандомизация для естественности

### 2. Typing Simulator
- Отправка `sendChatAction("typing")` через TDLib
- Длительность: `max(2с, min(10с, длина_ответа * 0.15))`
- Автоматическая отправка `sendChatAction("cancelled")` после отправки сообщения

### 3. Human-like Variations (опционально)
- Иногда "начать печатать", остановиться и продолжить
- Редко — "забыл" ответить сразу (имитация отвлечения)

## Интерфейс

```cpp
class PresenceSimulator {
public:
    void simulateReading(UserId userId, int messageLength);
    void simulateTyping(UserId userId, int responseLength);
    void cancelTyping(UserId userId);
};
```

## Верификация

- [ ] Короткое сообщение: "печатает" ~2с
- [ ] Длинный текст: "печатает" дольше, пропорционально длине
- [ ] Нет перекрывающихся typing-статусов
- [ ] Пауза чтения 1.5–3с перед обработкой
