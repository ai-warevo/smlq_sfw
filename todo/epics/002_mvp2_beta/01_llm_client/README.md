# 01: LLM Client

**Epic:** 002 (MVP 2 Beta)
**Фаза:** 4

## ТЗ

### 1. LLM Client
- Асинхронный POST-запрос к llama.cpp API (`/api/chat`)
- Формат запроса:
  ```json
  {
    "model": "qwen3-vl-8b",
    "messages": [{"role": "user", "content": "<ТЕКСТ>"}],
    "stream": false
  }
  ```
- Парсинг SSE-ответа / JSON

### 2. Системный промпт (базовый)
- Короткое правило стиля общения (разговорный, без заглавных, скобочки вместо смайлик)
- Полный промпт будет собираться в Фазе 8

### 3. Error Handling
- Если LLM недоступен — логировать, не падать
- Retry с exponential backoff (опционально)

### 4. Интеграция с каналом
- Мозг формирует промпт → LLM Client → ответ → Channel (Presence Simulator → отправка)

## Интерфейс

```cpp
// ⚠️ ПРИМЕР — НЕ СПЕЦИФИКАЦИЯ. Интерфейс может измениться.
class LLMClient {
public:
    std::string generate(const std::string& prompt, int maxTokens = 512);
    bool isAvailable() const;
    void setBaseUrl(const std::string& url); // http://localhost:6969
};
```

## Верификация

- [ ] Мозг отправляет текст → получает осмысленный ответ
- [ ] Стиль ответа соответствует системному промпту
- [ ] LLM недоступен → бот не падает
