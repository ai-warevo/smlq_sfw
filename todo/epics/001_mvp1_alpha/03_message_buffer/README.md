# 03: Message Buffer

**Epic:** 001 (MVP 1 Alpha)
**Фаза:** 3

## ТЗ

### 1. In-Memory Buffer
- Словарь `user_id → std::vector<std::string>` (сообщения)
- Словарь `user_id → Timer` (silence window)

### 2. Silence Window 10 сек
- Если за 10 секунд нет нового сообщения от user_id — буфер flush-ится
- Новое сообщение сбрасывает таймер 10с

### 3. Склейка
- Сообщения склеиваются через `\n`
- Мозг получает один логический блок, отвечает одним блоком

## Интерфейс

```cpp
class MessageBuffer {
public:
    void addMessage(UserId userId, const std::string& text);
    void onSilence(UserId userId, std::function<void(std::vector<std::string>)> callback);
    void flush(UserId userId);
};
```

## Верификация

- [ ] Одиночное сообщение → flush через 10с → передача Мозгу
- [ ] "Капли" (3 сообщения с интервалом 3-5с) → один склеенный блок через 10с после последнего
- [ ] Таймер сбрасывается при новом сообщении
