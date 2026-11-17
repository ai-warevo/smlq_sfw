# ТЗ: Создать и закодить проект `aw::tobus`

---

## КТО ТЫ

Ты — C++20-разработчик. Пишешь чистый, современный, компилируемый код.
Стек: C++20, CMake, clang-18, libc++.

---

## ЧТО ДЕЛАТЬ

Создать проект `aw::tobus` в директории `/workspace/tobus/`.

`aw::tobus` — это **event bus библиотека для C++20** с transport-agnostic архитектурой.
Модули общаются через единый интерфейс `IEventBus` — не важно, подписчики в том же процессе, в соседнем процессе или на другом сервере.

Проект является частью экосистемы `ai-warevo` (наряду с `aw::tpl` — coroutine library).

---

## ИСПОЛЬЗУЙ ШАБЛОН `template-cpp`

В репозитории есть `dotnet new`-стиль генератор C++20-проектов: `/workspace/template-cpp/`.
**Начинай работу с `aw::tobus` ТОЛЬКО после анализа этого шаблона.**

### ШАГ 1: Проанализируй шаблоны

Открой `/workspace/template-cpp/templates/` и изучи все доступные шаблоны:

| TYPE     | Aliases            | Produces                                         |
|----------|--------------------|--------------------------------------------------|
| `cli`    | `console`, `exe`, `app` | Executable: small core library + `main.cpp`  |
| `static` | `lib`, `library`   | Static library with a public header              |
| `dynamic`| `dll`, `so`, `shared` | Shared library with export macros           |

Изучи также:
- `templates/scaffold.sh` — генератор (list | init | eject)
- `templates/<type>/template.conf` — DESCRIPTION и ALIASES каждого шаблона
- `templates/<type>/files/` — что генерирует каждый шаблон (CMakeLists.txt, include/, src/, tests/)
- `cmake/ProjectConfig.cmake`, `cmake/CoreLibrary.cmake`, `cmake/Tests.cmake` — CMake-модули
- `CMakePresets.json`, `.clang-format`, `.clang-tidy`, `.editorconfig`, `.gitignore` — shared files
- `Makefile` — все таргеты: `init`, `list`, `selftest`, `eject`, `build`, `test`, `lint`, `format`, `clean`, `release`, `changelog`

### ШАГ 2: Выбери подходящий шаблон

Для `aw::tobus` (статическая библиотека, event bus, с тестами и examples) выбери наиболее подходящий шаблон из существующих:
- Скорее всего `static` — это статическая библиотека с заголовками и тестами
- Или `cli` — если нужны examples как исполняемые файлы, линкованные на библиотеку

Используй `make init TYPE=<выбранный> --dry-run` чтобы посмотреть что сгенерируется.

### ШАГ 3: Если нет подходящего — создай новый шаблон

Если ни один из существующих шаблонов не подходит:

1. Создай новый шаблон: `cp -r templates/static templates/<new-type>`
2. Напиши `template.conf` (DESCRIPTION, ALIASES)
3. Адаптируй файлы в `files/` под нужды `aw::tobus`
4. **Запроси моё одобрение** перед использованием нового шаблона
5. После одобрения запусти `make init TYPE=<new-type> NAME=tobus NS=aw::` из `/workspace/template-cpp/` в `/workspace/tobus/`

### ШАГ 4: Сгенерируй проект

После выбора/создания шаблона:

```bash
cd /workspace/template-cpp
make init TYPE=<выбранный> NAME=tobus NS=aw::tobus
```

Это создаст `/workspace/tobus/` с правильной структурой.

### ШАГ 5: Адаптируй под `aw::tobus`

После генерации:
- Адаптируй `CMakeLists.txt` — добавь опции для транспортов (`OPTION(ENABLE_UNIX_SOCKET ...)`, `OPTION(ENABLE_REDIS ...)` и т.д.)
- Добавь `examples/` директорию (не входит ни в один шаблон, добавь вручную)
- Добавь `README.md`, `LICENSE`, `VERSION` (shared files уже копируются из root template-cpp)
- Наполни код (ядро, InProcessBus, TODO-заголовки будущих транспортов)

### Разрешение на модификацию шаблона

Тебе **разрешено** редактировать `/workspace/template-cpp/`:
- Добавлять новые шаблоны в `templates/<type>/`
- Добавлять новые CMake-модули в `cmake/`
- Модифицировать существующие CMake-модули (`ProjectConfig`, `CoreLibrary`, `Tests`)
- Создавать новые shared files

Шаблон предназначен для переиспользования. Если модификация нужна — делай.

---

## ЗАЧЕМ ЭТО НУЖНО

В проекте `smlq_sfw` (AI-персонаж в Telegram) есть 12 модулей, которым нужно общаться:
Telegram Channel ↔ Brain ↔ Personality ↔ Memory ↔ Mood ↔ Proactive ↔ LLM Client ↔ ComfyUI Client ↔ TTS Client.

Сейчас они будут общаться через **event bus внутри одного процесса**.
Но мы хотим, чтобы при росте нагрузки можно было **разнести модули на отдельные процессы** (или даже серверы) **без переписывания кода модулей** — только поменять транспорт в конфиге.

`aw::tobus` — это библиотека, которая это обеспечивает.

---

## ГЛАВНЫЙ ПРИНЦИП

**Модули зависят только от `IEventBus`. Они не знают, какой транспорт используется.**

```
Telegram Channel ──publish──→ IEventBus ──subscribe──→ Brain
                                      ▲
                              "какой транспорт?"
                              in_process / unix_socket / redis / kafka / ...
```

Код модулей НЕ МЕНЯЕТСЯ при переключении транспорта. Меняется только одна строчка в `main()`:

```cpp
// Было:
auto bus = EventBusFactory::create({.transport = "in_process"});
// Стало (при масштабировании):
auto bus = EventBusFactory::create({.transport = "redis", .address = "redis://localhost:6379"});
```

---

## СТРУКТУРА ПРОЕКТА

```
tobus/
├── CMakeLists.txt
├── README.md
├── LICENSE
├── VERSION
├── include/
│   └── aw/
│       └── tobus/
│           ├── tobus.hpp              # umbrella header (include everything)
│           ├── i_event_bus.hpp        # PURE VIRTUAL INTERFACE — ядро проекта
│           ├── event_type.hpp         # EventTypeId
│           ├── serializable_event.hpp # SerializableEvent
│           ├── event_handler.hpp      # EventHandler type
│           ├── event_bus_factory.hpp  # EventBusFactory
│           ├── config.hpp             # EventBusConfig
│           ├── in_process_bus.hpp     # InProcessBus (РЕАЛИЗОВАТЬ)
│           └── transport/             # Опциональные транспорты (ЗАГОЛОВКИ + TODO)
│               ├── unix_socket_bus.hpp
│               ├── tcp_server_bus.hpp
│               ├── http_bus.hpp
│               ├── websocket_bus.hpp
├── src/
│   ├── i_event_bus.cpp
│   ├── event_type.cpp
│   ├── serializable_event.cpp
│   ├── event_handler.cpp
│   ├── event_bus_factory.cpp
│   ├── config.cpp
│   ├── in_process_bus.cpp
│   └── transport/
│       ├── unix_socket_bus.cpp
│       ├── tcp_server_bus.cpp
│       ├── http_bus.cpp
│       └── websocket_bus.cpp
├── examples/
│   ├── 01_in_process.cpp
│   ├── 02_unix_socket.cpp
│   ├── 03_tcp_server.cpp
│   └── 04_redis.cpp
└── tests/
    ├── test_event_bus.cpp
    └── test_in_process_bus.cpp
```

---

## ЧТО РЕАЛИЗОВАТЬ ПРЯМО СЕЙЧАС (MVP)

1. **Ядро библиотеки** (все заголовки + impl) — `IEventBus`, `EventTypeId`, `SerializableEvent`, `EventHandler`, `EventBusFactory`, `EventBusConfig`
2. **InProcessBus** — первая реализация транспорта (vector<callback>, thread-safe)
3. **Пример 01_in_process.cpp** — рабочий пример
4. **Тесты** — базовые тесты на IEventBus и InProcessBus
5. **CMakeLists.txt** — с опциями для всех транспортов
6. **README.md** — описание проекта

## ЧТО ЗАФИКСИРОВАТЬ (ЗАГОЛОВКИ + TODO)

Создать заголовки и cpp-файлы для всех будущих транспортов, но с комментариями `// TODO: implement` и `// Not implemented yet`. Это нужно, чтобы:
- Код компилировался (заголовки существуют)
- Был виден полный список транспортов
- Можно было дописать реализацию позже

Транспорты для фиксации:
- UnixSocketBus (Unix domain socket)
- TCPServerBus (TCP socket)
- HTTPBus (HTTP POST + SSE)
- WebSocketBus (WebSocket)

Опциональные транспорты (только заголовки, без impl):
- RedisBus
- ZeroMQBus
- RabbitMQBus
- KafkaBus

---

## ДЕТАЛИ РЕАЛИЗАЦИИ

### 1. `IEventBus` — чистый интерфейс

```cpp
class IEventBus {
public:
    virtual ~IEventBus() = default;

    // Подписаться на typed event
    template <typename T>
    void subscribe(EventHandler<T> handler);

    // Опубликовать event
    template <typename T>
    void publish(const T& event);

    // Отписаться
    template <typename T>
    void unsubscribe(EventHandler<T> handler);

    // Лайфтайл
    virtual void start() = 0;
    virtual void stop() = 0;
};
```

**Важно:** `subscribe`, `publish`, `unsubscribe` — template-методы (реализация в заголовке). `start`/`stop` — виртуальные (реализация в cpp).

### 2. `EventTypeId` — уникальный идентификатор события

Каждый тип события (MessageReceived, ResponseReady, и т.д.) получает уникальный ID.
Реализация: `std::type_index` или хеш от имени типа.

### 3. `SerializableEvent` — универсальный payload

Базовый класс для сериализации. Поддерживает:
- `to_json()` → строка (JSON)
- `from_json(string)` → объект
- `to_bytes()` → vector<uint8_t>
- `from_bytes(vector<uint8_t>)` → объект

Это нужно транспортам, которым нужна сериализация (network, Redis, Kafka).
Для `InProcessBus` сериализация не нужна — события передаются по значению.

### 4. `EventHandler<T>` — типизированный callback

```cpp
using EventHandler<T> = std::function<void(const T&)>;
```

### 5. `EventBusConfig` — конфигурация транспорта

```cpp
struct EventBusConfig {
    std::string transport = "in_process";  // "in_process", "unix_socket", "tcp_server", "http", "websocket", "redis", "zeromq", "rabbitmq", "kafka"
    std::string address = "";              // "/tmp/smlq_events.sock", "0.0.0.0:5555", "redis://localhost:6379"
    int timeout_ms = 5000;                 // таймаут подключения
    int max_retries = 3;                   // макс. попыток переподключения
};
```

### 6. `EventBusFactory` — фабрика транспортов

```cpp
class EventBusFactory {
public:
    static std::unique_ptr<IEventBus> create(const EventBusConfig& config);
};
```

Внутри: switch по `config.transport` → создаёт нужный транспорт.
Если транспорт не поддерживается — бросает `std::runtime_error`.

### 7. `InProcessBus` — in-process транспорт

Реализация:
- Хранит `std::unordered_map<EventTypeId, std::vector<EventHandlerBase>>`
- `subscribe` добавляет handler в мапу
- `publish` итерирует handlers и вызывает каждый
- Thread-safe: `std::mutex` на мапу, `std::shared_mutex` для read/write
- `start()`/`stop()` — пока ничего не делают (но нужны по интерфейсу)

### 8. Будущие транспорты (заголовки + TODO)

Для каждого транспорта:
- Заголовок `transport/<name>_bus.hpp` с объявлением класса
- Файл `src/transport/<name>_bus.cpp` с `// TODO: implement`
- Добавить в `EventBusFactory::create()` case для этого транспорта
- В README — строка в таблице транспортов со статусом `🔲 Planned`

---

## ЧТО НЕ ДЕЛАТЬ

1. **Не писать реализации будущих транспортов** — только заголовки и TODO
2. **Не добавлять внешние зависимости** — ядро только std
3. **Не делать message broker** — это не RabbitMQ, не Kafka, не Redis. Это просто bus.
4. **Не делать service discovery** — это не задача библиотеки
5. **Не делать protocol definitions** — protobuf, flatbuffers — не здесь
6. **Не делать queues, retries, dead letter queues** — это не messaging framework

---

## ТРЕБОВАНИЯ К КОДУ

1. **C++20** — использовать корутины, концепты, ranges где уместно
2. **CMake** — `cmake_minimum_required(VERSION 3.23)`, `add_library` с `FILE_SET HEADERS`
3. **Namespace** — `aw::tobus`
4. **Include guard** — `#pragma once`
5. **Комментарии** — doxygen-style для публичных API
6. **Компилируется** — код должен компилироваться и линковаться
7. **Тесты** — базовые юнит-тесты через CTest (если GoogleTest недоступен — писать свои минимальные assert-макросы)
8. **Линейка совместима с `aw::tpl`** — тот же CMake-стиль, тот же namespace-паттерн `aw::`

---

## КАК ИСПОЛЬЗУЕТСЯ В smlq_sfw

В `smlq_sfw` `aw::tobus` будет подключён через CMake FetchContent:

```cmake
include(FetchContent)
FetchContent_Declare(
    tobus
    SOURCE_DIR /workspace/tobus
)
FetchContent_MakeAvailable(tobus)

target_link_libraries(smlq_sfw PRIVATE aw::tobus)
```

Пример использования в модуле Telegram Channel:

```cpp
// telegram_channel.hpp
class TelegramChannel {
public:
    explicit TelegramChannel(std::shared_ptr<IEventBus> bus);
    void start();
private:
    void on_td_update(td::TdApi::Object* update);
    // ...
    std::shared_ptr<IEventBus> bus_;
};

void TelegramChannel::on_td_update(td::TdApi::Object* update) {
    // ... разбираем обновление ...
    if (auto* message = dynamic_cast<td::TdApi::Message*>(update.get())) {
        MessageReceived event{
            .chat_id = message->chat_id_,
            .message_id = message->id_,
            .text = std::move(message->text_),
            .sender_name = "Alice"
        };
        bus_->publish(event);  // Все подписчики получат событие
    }
}
```

Пример использования в модуле Brain:

```cpp
// brain.hpp
class Brain {
public:
    explicit Brain(std::shared_ptr<IEventBus> bus);
    void start();
private:
    void on_message(const MessageReceived& event);
    // ...
    std::shared_ptr<IEventBus> bus_;
};

void Brain::start() {
    bus_->subscribe<MessageReceived>([this](const MessageReceived& event) {
        on_message(event);
    });
}
```

---

## ПОРЯДОК ВЫПОЛНЕНИЯ

1. Создать структуру директорий
2. Написать CMakeLists.txt
3. Написать ядро (i_event_bus.hpp/cpp, event_type.hpp/cpp, serializable_event.hpp/cpp, event_handler.hpp/cpp, event_bus_factory.hpp/cpp, config.hpp/cpp)
4. Написать InProcessBus (in_process_bus.hpp/cpp)
5. Написать заголовки+TODO для будущих транспортов
6. Написать примеры
7. Написать тесты
8. Написать README.md
9. Проверить что компилируется

---

## ФИНАЛЬНАЯ ПРОВЕРКА

Перед тем как закончить — проверь:
1. `cmake --build build` проходит без ошибок
2. `ctest --test-dir build` проходит
3. Примеры компилируются
4. Все заголовки будущих транспортов существуют
5. EventBusFactory знает про все транспорты (даже если они TODO)
6. README.md описывает проект, транспорты, quick start
