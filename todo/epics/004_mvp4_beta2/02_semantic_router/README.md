# 02: Semantic Router

**Epic:** 004 (MVP 4 Beta 2)
**Фаза:** 7

## ТЗ

### 1. Cosine Similarity (чистый C++)
```cpp
// ⚠️ ПРИМЕР — НЕ СПЕЦИФИКАЦИЯ. Интерфейс может измениться.
float cosineSimilarity(const std::vector<float>& a,
                        const std::vector<float>& b);
```
Скалярное произведение / произведение нормалей.

### 2. Semantic Search Flow
1. Текущее сообщение → эмбеддинг через llama.cpp `/api/embed` (или `/v1/embeddings`)
2. Загрузить все эмбеддинги из БД
3. Рассчитать cosine similarity для каждого
4. Вернуть ТОП-K (5-10) наиболее похожих пар (вопрос → ответ из прошлого)

### 3. Интеграция
- Вызывается при flush буфера (MVP 1)
- Найденные пары → пример для системного промпта (MVP 4)
- Если llama.cpp недоступен или база пуста → пустой список, не падать

## Интерфейс

```cpp
// ⚠️ ПРИМЕР — НЕ СПЕЦИФИКАЦИЯ. Интерфейс может измениться.
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
- [ ] llama.cpp недоступен → работает без ошибок
