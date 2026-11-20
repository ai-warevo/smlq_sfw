# MVP 6 (Release): Multi-modal Bot

**Goal:** A full-featured bot with photo generation, voice messages, and web scraping. Samantha can send photos, speak with voice, and discuss news.

**Status:** ⬜ Not started

**What works:** Everything from MVP 5 + the bot generates photos, speaks with voice, reads public channels and news.

---

## Subtasks

| # | Task | Description | Status |
|---|---|---|---|
| 01 | ComfyUI Client | Photo generation from prompt | ⬜ |
| 02 | TTS Client | Voice message synthesis | ⬜ |
| 03 | Web Scraper | Parsing public channels, RSS, news | ⬜ |

---

## Interfaces

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. The interface may change.
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

## MVP 6 Verification

- [ ] The bot generates Samantha photos on request
- [ ] The bot sends voice messages
- [ ] The bot can discuss current news
- [ ] All components work stably
