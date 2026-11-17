# MVP 6 (Release): Мульти-модальный бот

**Цель:** Полноценный бот с генерацией фото, голосовыми сообщениями и веб-скрапингом. Samantha может кидать фото, говорить голосом, обсуждать новости.

**Статус:** ⬜ Не начато

**Что работает:** Всё из MVP 5 + бот генерирует фото, говорит голосом, читает паблики и новости.

---

## Подзадачи

| # | Задача | Описание | Статус |
|---|---|---|---|
| 01 | ComfyUI Client | Генерация фото по запросу | ⬜ |
| 02 | TTS Client | Синтез голосовых сообщений | ⬜ |
| 03 | Web Scraper | Парсинг пабликов, RSS, новостей | ⬜ |

---

## Интерфейсы

```cpp
class ComfyUIClient {
public:
    std::string generatePhoto(const std::string& prompt);
    bool isAvailable() const;
};

class TTSClient {
public:
    std::string synthesize(const std::string& text, const TTSSettings& settings);
    bool isAvailable() const;
};

class WebScraper {
public:
    std::vector<NewsItem> scrapeRSS(const std::string& url);
    std::string scrapePage(const std::string& url);
    std::vector<NewsItem> search(const std::string& query);
};
```

## Верификация MVP 6

- [ ] Бот генерирует фото Samantha по запросу
- [ ] Бот отправляет голосовые сообщения
- [ ] Бот может обсудить текущие новости
- [ ] Все компоненты работают стабильно
