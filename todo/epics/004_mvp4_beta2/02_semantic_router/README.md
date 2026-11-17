# 02: Semantic Router

**Epic:** 004 (MVP 4 Beta 2)
**Фаза:** 7

## ТЗ

### 1. Cosine Similarity (чистый C++)
```cpp
float cosineSimilarity(const std::vector<float>& a,
                        const std::vector<float>& b);
```
Скалярное произведение / произведение нормалей.

### 2. Semantic Search Flow
1. Текущее сообщение → эмбеддинг через Ollama `/api/embeddings`
2. Загрузить все эмбеддинги из БД
3. Рассчитать cosine similarity для каждого
4. Вернуть ТОП-K (5-10) наиболее похожих пар (вопрос → ответ из прошлого)

### 3. Интеграция
- Вызывается при flush буфера (MVP 1)
- Найденные пары → пример для системного промпта (MVP 4)
- Если Ollama недоступен или база пуста → пустой список, не падать

## Интерфейс

```cpp
class SemanticRouter {
public:
    std::vector<SimilarPair> search(const std::vector<float>& queryVec, int topK = 5);
    std::vector<SimilarPair> searchByText(const std::string& text, int topK = 5);
};

struct SimilarPair {
    int messageId;
    std::string question;
    std::string answer;
    float similarity;
};
```

## Верификация

- [ ] Написание фразы из архива → возвращаются соответствующие исторические пары
- [ ] Пустая база → работает без ошибок
- [ ] Ollama недоступен → работает без ошибок
