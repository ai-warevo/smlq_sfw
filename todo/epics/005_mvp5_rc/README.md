# MVP 5 (RC): Brain Core — Настроение, Личность, Proactive

**Цель:** Мозг обретает "душу" — настроение, эмоции, инициативу. Бот не просто отвечает, а проявляет интерес, помнит контекст эмоционально.

**Статус:** ⬜ Не начато

**Что работает:** Всё из MVP 4 + бот имеет настроение, проявляет инициативу, отвечает с эмоциональной окраской.

---

## Подзадачи

| # | Задача | Описание | Статус |
|---|---|---|---|
| 01 | Mood & Emotion Engine | Настроение, эмоции, состояние | ⬜ |
| 02 | Personality Engine | System prompt, digital twin | ⬜ |
| 03 | Proactive Engine | Инициатива, расписание, триггеры | ⬜ |

---

## Интерфейсы

```cpp
// ⚠️ ПРИМЕР — НЕ СПЕЦИФИКАЦИЯ. Интерфейс может измениться.
class MoodEngine {
public:
    Mood getCurrentMood() const;
    void updateMood(const std::string& input, float sentiment);
    std::string getMoodModifiers(); // для Prompt Builder
};

class PersonalityEngine {
public:
    std::string getPersonalityProfile();
    void learnFromHistory(const std::vector<Message>& history);
};

class ProactiveEngine {
public:
    void scheduleCheck();
    std::optional<ProactiveMessage> shouldInitiate();
};
```

## Верификация MVP 5

- [ ] Бот меняет настроение в зависимости от контекста
- [ ] Бот первым пишет (proactive)
- [ ] Ответы эмоционально окрашены
- [ ] Личность согласована с digital twin
