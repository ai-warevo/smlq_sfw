# 01: Mood & Emotion Engine

**Epic:** 005 (MVP 5 RC)
**Фаза:** 12

## ТЗ

### 1. Mood Model
- Настроение: `positive`, `neutral`, `negative`
- Эмоция: `happy`, `sad`, `angry`, `excited`, `calm`, `worried`
- Состояние: `energetic`, `tired`, `distracted`

### 2. Обновление
- Анализ тональности входящего сообщения
- Контекст последних N сообщений
- Время суток (утро/день/вечер/ночь)

### 3. Интеграция с Prompt Builder
- Mood modifiers добавляются в системный промпт
- Эмоция влияет на стиль ответа

## Интерфейс

```cpp
// ⚠️ ПРИМЕР — НЕ СПЕЦИФИКАЦИЯ. Интерфейс может измениться.
class MoodEngine {
public:
    Mood getCurrentMood() const;
    void updateMood(const std::string& input, float sentiment);
    std::string getMoodModifiers(); // для Prompt Builder
};
```

## Верификация

- [ ] Настроение меняется в зависимости от контекста
- [ ] Эмоциональная окраска влияет на ответы
