# 02: Personality Engine

**Epic:** 005 (MVP 5 RC)
**Фаза:** 12

## ТЗ

### 1. System Prompt Builder
- Расширенный промпт с полной спецификацией Samantha
- Имя, возраст, внешность, стиль, триггер word
- Запрещённые темы

### 2. Digital Twin
- Анализ реальных переписок владельца
- Копирование стиля, манеры, выражений
- Интеграция с Historical Ingestion (MVP 4)

## Интерфейс

```cpp
class PersonalityEngine {
public:
    std::string getPersonalityProfile();
    void learnFromHistory(const std::vector<Message>& history);
};
```

## Верификация

- [ ] Бот отвечает в стиле Samantha
- [ ] Стиль согласован с digital twin владельца
