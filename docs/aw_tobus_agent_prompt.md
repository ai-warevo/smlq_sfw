# Spec: Create and code the `aw::tobus` project

---

## WHO YOU ARE

You are a C++20 developer. You write clean, modern, compilable code.
Tech stack: C++20, CMake, clang-18, libc++.

---

## WHAT TO DO

Create the `aw::tobus` project in the `/workspace/tobus/` directory.

`aw::tobus` is a **C++20 event bus library** with a transport-agnostic architecture.
Modules communicate through a single `IEventBus` interface — it doesn't matter whether subscribers are in the same process, a neighboring process, or on a different server.

The project is part of the `ai-warevo` ecosystem (alongside `aw::tpl` — a coroutine library).

---

## USE THE `template-cpp` TEMPLATE

The repository includes a `dotnet new`-style C++20 project generator: `/workspace/template-cpp/`.
**Start working on `aw::tobus` ONLY after analyzing this template.**

### STEP 1: Analyze the Templates

Open `/workspace/template-cpp/templates/` and study all available templates:

| TYPE     | Aliases            | Produces                                         |
|----------|--------------------|--------------------------------------------------|
| `cli`    | `console`, `exe`, `app` | Executable: small core library + `main.cpp`  |
| `static` | `lib`, `library`   | Static library with a public header              |
| `dynamic`| `dll`, `so`, `shared` | Shared library with export macros           |

Also study:
- `templates/scaffold.sh` — the generator (list | init | eject)
- `templates/<type>/template.conf` — DESCRIPTION and ALIASES for each template
- `templates/<type>/files/` — what each template generates (CMakeLists.txt, include/, src/, tests/)
- `cmake/ProjectConfig.cmake`, `cmake/CoreLibrary.cmake`, `cmake/Tests.cmake` — CMake modules
- `CMakePresets.json`, `.clang-format`, `.clang-tidy`, `.editorconfig`, `.gitignore` — shared files
- `Makefile` — all targets: `init`, `list`, `selftest`, `eject`, `build`, `test`, `lint`, `format`, `clean`, `release`, `changelog`

### STEP 2: Choose the Right Template

For `aw::tobus` (static library, event bus, with tests and examples), choose the most suitable template from the existing ones:
- Most likely `static` — this is a static library with headers and tests
- Or `cli` — if examples are needed as executables linked to the library

Use `make init TYPE=<chosen> --dry-run` to see what will be generated.

### STEP 3: If No Suitable Template Exists — Create a New One

If none of the existing templates fit:

1. Create a new template: `cp -r templates/static templates/<new-type>`
2. Write `template.conf` (DESCRIPTION, ALIASES)
3. Adapt files in `files/` for `aw::tobus` needs
4. **Request my approval** before using the new template
5. After approval, run `make init TYPE=<new-type> NAME=tobus NS=aw::` from `/workspace/template-cpp/` into `/workspace/tobus/`

### STEP 4: Generate the Project

After selecting/creating the template:

```bash
cd /workspace/template-cpp
make init TYPE=<chosen> NAME=tobus NS=aw::tobus
```

This will create `/workspace/tobus/` with the correct structure.

### STEP 5: Adapt for `aw::tobus`

After generation:
- Adapt `CMakeLists.txt` — add transport options (`OPTION(ENABLE_UNIX_SOCKET ...)`, `OPTION(ENABLE_REDIS ...)`, etc.)
- Add an `examples/` directory (not included in any template, add manually)
- Add `README.md`, `LICENSE`, `VERSION` (shared files are already copied from root template-cpp)
- Populate the code (core, InProcessBus, TODO headers for future transports)

### Template Modification Authorization

You are **allowed** to edit `/workspace/template-cpp/`:
- Add new templates to `templates/<type>/`
- Add new CMake modules to `cmake/`
- Modify existing CMake modules (`ProjectConfig`, `CoreLibrary`, `Tests`)
- Create new shared files

The template is designed for reuse. If modification is needed — go ahead.

---

## WHY THIS IS NEEDED

The `smlq_sfw` project (AI persona in Telegram) has 12 modules that need to communicate:
Telegram Channel ↔ Brain ↔ Personality ↔ Memory ↔ Mood ↔ Proactive ↔ LLM Client ↔ ComfyUI Client ↔ TTS Client.

Right now they will communicate through an **event bus within a single process**.
But we want to be able to **spread modules across separate processes** (or even servers) as load grows **without rewriting module code** — just change the transport in the config.

`aw::tobus` is the library that makes this possible.

---

## KEY PRINCIPLE

**Modules depend only on `IEventBus`. They don't know which transport is being used.**

```
Telegram Channel ──publish──→ IEventBus ──subscribe──→ Brain
                                      ▲
                              "which transport?"
                              in_process / unix_socket / redis / kafka / ...
```

Module code DOES NOT CHANGE when switching transport. Only one line in `main()` changes:

```cpp
// Was:
auto bus = EventBusFactory::create({.transport = "in_process"});
// Now (at scale):
auto bus = EventBusFactory::create({.transport = "redis", .address = "redis://localhost:6379"});
```

---

## PROJECT STRUCTURE

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
│           ├── i_event_bus.hpp        # PURE VIRTUAL INTERFACE — project core
│           ├── event_type.hpp         # EventTypeId
│           ├── serializable_event.hpp # SerializableEvent
│           ├── event_handler.hpp      # EventHandler type
│           ├── event_bus_factory.hpp  # EventBusFactory
│           ├── config.hpp             # EventBusConfig
│           ├── in_process_bus.hpp     # InProcessBus (IMPLEMENT)
│           └── transport/             # Optional transports (HEADERS + TODO)
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

## WHAT TO IMPLEMENT NOW (MVP)

1. **Library Core** (all headers + impl) — `IEventBus`, `EventTypeId`, `SerializableEvent`, `EventHandler`, `EventBusFactory`, `EventBusConfig`
2. **InProcessBus** — first transport implementation (vector<callback>, thread-safe)
3. **Example 01_in_process.cpp** — working example
4. **Tests** — basic tests for IEventBus and InProcessBus
5. **CMakeLists.txt** — with options for all transports
6. **README.md** — project description

## WHAT TO LOCK IN (HEADERS + TODO)

Create header and cpp files for all future transports, but with `// TODO: implement` and `// Not implemented yet` comments. This is needed so:
- The code compiles (headers exist)
- The full list of transports is visible
- Implementations can be added later

Transports to lock in:
- UnixSocketBus (Unix domain socket)
- TCPServerBus (TCP socket)
- HTTPBus (HTTP POST + SSE)
- WebSocketBus (WebSocket)

Optional transports (headers only, no impl):
- RedisBus
- ZeroMQBus
- RabbitMQBus
- KafkaBus

---

## IMPLEMENTATION DETAILS

### 1. `IEventBus` — Pure Interface

```cpp
class IEventBus {
public:
    virtual ~IEventBus() = default;

    // Subscribe to typed event
    template <typename T>
    void subscribe(EventHandler<T> handler);

    // Publish event
    template <typename T>
    void publish(const T& event);

    // Unsubscribe
    template <typename T>
    void unsubscribe(EventHandler<T> handler);

    // Lifecycle
    virtual void start() = 0;
    virtual void stop() = 0;
};
```

**Important:** `subscribe`, `publish`, `unsubscribe` — template methods (implementation in header). `start`/`stop` — virtual (implementation in cpp).

### 2. `EventTypeId` — Unique Event Identifier

Each event type (MessageReceived, ResponseReady, etc.) gets a unique ID.
Implementation: `std::type_index` or hash of the type name.

### 3. `SerializableEvent` — Universal Payload

Base class for serialization. Supports:
- `to_json()` → string (JSON)
- `from_json(string)` → object
- `to_bytes()` → vector<uint8_t>
- `from_bytes(vector<uint8_t>)` → object

This is needed for transports that require serialization (network, Redis, Kafka).
For `InProcessBus` serialization is not needed — events are passed by value.

### 4. `EventHandler<T>` — Typed Callback

```cpp
using EventHandler<T> = std::function<void(const T&)>;
```

### 5. `EventBusConfig` — Transport Configuration

```cpp
struct EventBusConfig {
    std::string transport = "in_process";  // "in_process", "unix_socket", "tcp_server", "http", "websocket", "redis", "zeromq", "rabbitmq", "kafka"
    std::string address = "";              // "/tmp/smlq_events.sock", "0.0.0.0:5555", "redis://localhost:6379"
    int timeout_ms = 5000;                 // connection timeout
    int max_retries = 3;                   // max reconnection attempts
};
```

### 6. `EventBusFactory` — Transport Factory

```cpp
class EventBusFactory {
public:
    static std::unique_ptr<IEventBus> create(const EventBusConfig& config);
};
```

Internally: switch on `config.transport` → creates the appropriate transport.
If transport is not supported — throws `std::runtime_error`.

### 7. `InProcessBus` — In-Process Transport

Implementation:
- Stores `std::unordered_map<EventTypeId, std::vector<EventHandlerBase>>`
- `subscribe` adds handler to the map
- `publish` iterates handlers and calls each one
- Thread-safe: `std::mutex` on the map, `std::shared_mutex` for read/write
- `start()`/`stop()` — currently do nothing (but required by interface)

### 8. Future Transports (Headers + TODO)

For each transport:
- Header `transport/<name>_bus.hpp` with class declaration
- File `src/transport/<name>_bus.cpp` with `// TODO: implement`
- Add a case for this transport in `EventBusFactory::create()`
- In README — a row in the transports table with status `🔲 Planned`

---

## WHAT NOT TO DO

1. **Don't implement future transports** — only headers and TODO
2. **Don't add external dependencies** — core is std only
3. **Don't build a message broker** — this is not RabbitMQ, Kafka, or Redis. It's just a bus.
4. **Don't do service discovery** — not the library's job
5. **Don't do protocol definitions** — protobuf, flatbuffers — not here
6. **Don't do queues, retries, dead letter queues** — not a messaging framework

---

## CODE REQUIREMENTS

1. **C++20** — use coroutines, concepts, ranges where appropriate
2. **CMake** — `cmake_minimum_required(VERSION 3.23)`, `add_library` with `FILE_SET HEADERS`
3. **Namespace** — `aw::tobus`
4. **Include guard** — `#pragma once`
5. **Comments** — doxygen-style for public APIs
6. **Compiles** — code must compile and link
7. **Tests** — basic unit tests via CTest (if GoogleTest is unavailable — write your own minimal assert macros)
8. **Compatible with `aw::tpl` lineup** — same CMake style, same `aw::` namespace pattern

---

## HOW IT'S USED IN smlq_sfw

In `smlq_sfw`, `aw::tobus` will be included via CMake FetchContent:

```cmake
include(FetchContent)
FetchContent_Declare(
    tobus
    SOURCE_DIR /workspace/tobus
)
FetchContent_MakeAvailable(tobus)

target_link_libraries(smlq_sfw PRIVATE aw::tobus)
```

Usage example in the Telegram Channel module:

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
    // ... parse the update ...
    if (auto* message = dynamic_cast<td::TdApi::Message*>(update.get())) {
        MessageReceived event{
            .chat_id = message->chat_id_,
            .message_id = message->id_,
            .text = std::move(message->text_),
            .sender_name = "Alice"
        };
        bus_->publish(event);  // All subscribers receive the event
    }
}
```

Usage example in the Brain module:

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

## EXECUTION ORDER

1. Create directory structure
2. Write CMakeLists.txt
3. Write core (i_event_bus.hpp/cpp, event_type.hpp/cpp, serializable_event.hpp/cpp, event_handler.hpp/cpp, event_bus_factory.hpp/cpp, config.hpp/cpp)
4. Write InProcessBus (in_process_bus.hpp/cpp)
5. Write headers+TODO for future transports
6. Write examples
7. Write tests
8. Write README.md
9. Verify it compiles

---

## FINAL CHECK

Before finishing — verify:
1. `cmake --build build` passes without errors
2. `ctest --test-dir build` passes
3. Examples compile
4. All future transport headers exist
5. EventBusFactory knows about all transports (even if they're TODO)
6. README.md describes the project, transports, quick start
