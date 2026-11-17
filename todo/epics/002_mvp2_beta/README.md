# MVP 2 (Beta): LLM-интеграция

**Цель:** Бот начинает отвечать осмысленно через локальный LLM (Qwen3-VL-8B / llama.cpp). Заменяем заглушку на реальный ответ.

**Статус:** ⬜ Не начато

**Что работает:** Всё из MVP 1 + бот отвечает через Qwen3-VL-8B. Стиль общения базовый (системный промпт).

---

## Подзадачи

| # | Задача | Описание | Статус |
|---|---|---|---|
| 01 | LLM Client | HTTP-запросы к llama.cpp API | ⬜ |
| 02 | System Prompt | Базовый стиль общения | ⬜ |

---

## Интерфейсы

```cpp
class LLMClient {
public:
    std::string generate(const std::string& prompt, int maxTokens = 512);
    bool isAvailable() const;
    void setBaseUrl(const std::string& url); // http://localhost:6969
};
```

## Верификация MVP 2

- [ ] Бот отвечает осмысленно через Qwen3-VL-8B
- [ ] Стиль ответа соответствует системному промпту
- [ ] LLM недоступен → бот не падает
- [ ] Ответ проходит через Presence Simulator перед отправкой
