# 03: Web Scraper

**Epic:** 006 (MVP 6 Release)
**Phase:** 11

## Requirements

### 1. Web Scraper
- RSS feeds (news, public channels)
- Page parsing
- Search by queries

### 2. News Processing
- Extracting titles, text, links
- Filtering by relevance (user interests)
- Brief summary for sending to chat

### 3. Brain Integration
- Proactive: "Look what I found!"
- On request: "Google ..."
- Context for responses: "Read today ..."

## Interface

```cpp
// ⚠️ EXAMPLE — NOT A SPECIFICATION. The interface may change.
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

## Verification

- [ ] The bot can discuss current news
- [ ] RSS feeds are parsed correctly
