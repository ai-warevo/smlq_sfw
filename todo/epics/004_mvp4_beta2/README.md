# MVP 4 (Beta 2): Semantic Router + Prompt Builder

**Цель:** Мозг ищет в своём прошлом похожие ситуации и собирает полный контекст для LLM. Ответы становятся более персонализированными.

**Статус:** ⬜ Не начато

**Что работает:** Всё из MVP 3 + бот ищет похожие исторические пары, собирает "Prompt Pie" из системы + семантики + истории.

---

## Подзадачи

| # | Задача | Описание | Статус |
|---|---|---|---|
| 01 | Historical Ingestion | Импорт JSON-экспорта Telegram | ⬜ |
| 02 | Semantic Router | Cosine similarity, поиск похожих пар | ⬜ |
| 03 | Prompt Builder | Сборка полного промпта (Prompt Pie) | ⬜ |

---

## Интерфейсы

```cpp
// ⚠️ ПРИМЕР — НЕ СПЕЦИФИКАЦИЯ. Интерфейс может измениться.
class HistoricalIngestion {
public:
    void parseJSON(const std::string& filePath);
    void generateEmbeddings();
    void saveToDatabase();
};

class SemanticRouter {
public:
    std::vector<SimilarPair> search(const std::vector<float>& queryVec, int topK = 5);
    std::vector<SimilarPair> searchByText(const std::string& text, int topK = 5);
};

class PromptBuilder {
public:
    std::string buildFullPrompt(const Context& ctx);
    int estimateTokens(const std::string& prompt);
};
```

## Верификация MVP 4

- [ ] Ответ бота звучит как собственный (стиль из прошлого + контекст)
- [ ] Семантические примеры влияют на манеру ответа
- [ ] Контекст не раздувается (ограничение: системный + ТОП-2 пары + 10 сообщений)
