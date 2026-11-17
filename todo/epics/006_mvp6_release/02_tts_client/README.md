# 02: TTS Client

**Epic:** 006 (MVP 6 Release)
**Фаза:** 14

## ТЗ

### 1. TTS Client
- HTTP-запросы к TTS-сервису
- Синтез голосовых сообщений из текста
- Настройки: голос, скорость, тон

### 2. TTSSettings
- Выбор голоса
- Скорость речи
- Эмоциональная окраска (на основе Mood Engine)

### 3. Интеграция с Brain
- По запросу пользователя или proactive (утреннее/вечернее сообщение голосом)
- Отправка через Channel как голосовое сообщение

## Интерфейс

```cpp
// ⚠️ ПРИМЕР — НЕ СПЕЦИФИКАЦИЯ. Интерфейс может измениться.
struct TTSSettings {
    std::string voice;
    float speed;
    std::string emotion;
};

class TTSClient {
public:
    std::string synthesize(const std::string& text, const TTSSettings& settings);
    bool isAvailable() const;
};
```

## Верификация

- [ ] Бот генерирует голосовые сообщения
- [ ] Голос соответствует настройкам
