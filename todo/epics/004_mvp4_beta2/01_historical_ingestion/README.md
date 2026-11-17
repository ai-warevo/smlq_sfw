# 01: Historical Ingestion

**Epic:** 004 (MVP 4 Beta 2)
**Фаза:** 6

## ТЗ

### 1. JSON Parser
- Парсер экспорта Telegram Desktop (`result.json`)
- Sessionization: сообщения одного автора с интервалом < 60с = один блок
- Разрыв сессии: пауза > 30мин
- Фильтры: игнорировать медиа-заглушки, системные сообщения, пересланные

### 2. Embedding Pipeline
- Для каждого `user_message` → POST к Ollama `/api/embeddings`
- Сохранение вектора в таблицу `embeddings`

### 3. Таблица `embeddings`
- `id` (INTEGER PRIMARY KEY)
- `message_id` (INTEGER REFERENCES messages(id))
- `vector` (BLOB — бинарный массив float)
- `created_at` (DATETIME)

### 4. Факты
- Извлечение именованных сущностей (имена, даты, места)
- Сохранение в таблицу `facts`

### 5. Прогресс
- Вывод счётчика в консоль (обработано N сообщений)

## Интерфейс

```cpp
class HistoricalIngestion {
public:
    void parseJSON(const std::string& filePath);
    void generateEmbeddings();
    void saveToDatabase();
};
```

## Верификация

- [ ] Парсер обрабатывает большой JSON без падений
- [ ] В БД записаны сообщения с векторами
- [ ] Ollama возвращает корректные эмбеддинги
