# 03: Web Scraper

**Epic:** 006 (MVP 6 Release)
**Фаза:** 11

## ТЗ

### 1. Web Scraper
- RSS-ленты (новости, паблики)
- Парсинг страниц
- Поиск по запросам

### 2. News Processing
- Извлечение заголовков, текста, ссылок
- Фильтрация по релевантности (интересы пользователя)
- Краткий пересказ для отправки в чат

### 3. Интеграция с Brain
- Proactive: "Смотри что нашлась!"
- По запросу: "Погугли ..."
- Контекст для ответов: "Читала сегодня ..."

## Интерфейс

```cpp
struct NewsItem {
    std::string title;
    std::string content;
    std::string url;
    std::time_t published;
};

class WebScraper {
public:
    std::vector<NewsItem> scrapeRSS(const std::string& url);
    std::string scrapePage(const std::string& url);
    std::vector<NewsItem> search(const std::string& query);
};
```

## Верификация

- [ ] Бот может обсудить текущие новости
- [ ] RSS-ленты парсятся корректно
