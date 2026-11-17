# 01: ComfyUI Client

**Epic:** 006 (MVP 6 Release)
**Фаза:** 10

## ТЗ

### 1. ComfyUI Client
- HTTP-запросы к ComfyUI API
- Управление воркфлоу (портрет, датасет, утилиты)
- Генерация фото по промпту

### 2. Интеграция с Brain
- По запросу пользователя или по расписанию
- Промпт для генерации формируется на основе контекста
- Отправка через Channel

## Интерфейс

```cpp
// ⚠️ ПРИМЕР — НЕ СПЕЦИФИКАЦИЯ. Интерфейс может измениться.
class ComfyUIClient {
public:
    std::string generatePhoto(const std::string& prompt);
    bool isAvailable() const;
};
```

## Верификация

- [ ] Бот генерирует фото по запросу
- [ ] Фото отправляется через Channel
