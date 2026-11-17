# Spec: `tobus` — transport-agnostic event bus for C++20

---

## OVERALL ROADMAP

The full project evolution:

```
smlq_sfw (current state)
  ├── needs → aw::tpl (coroutine support for async event handling)
  └── needs → aw::tobus (event bus)

Final state:
  smlq_sfw
    ├── aw::tpl  (coroutines)
    └── aw::tobus (event bus, depends on tpl)
```

**Your job:** phases 0–3 below. Phase 4 (smlq_sfw refactoring) is handled separately — you will NOT do it.

---

## PHASE 0: `aw::tpl` analysis and modification

Read `/workspace/tpl/` carefully. Understand its API (Task<T>, co_spawn, executors, etc.).

**Report:**
1. What `aw::tpl` provides
2. What works for `tobus` as-is
3. What needs changes — propose specific modifications
4. **Ask for approval** before modifying `tpl`

You **may** edit `/workspace/tpl/` to add what `tobus` needs.

**STOP and wait for approval before modifying `tpl`.**

---

## PHASE 1: `template-cpp` analysis and modification

Read `/workspace/template-cpp/` carefully. Understand the generator (scaffold.sh, templates, cmake modules).

**Report:**
1. What works for `tobus` as-is
2. What needs changes — propose specific modifications
3. **Ask for approval** before modifying `template-cpp`

You **may** edit `/workspace/template-cpp/` to add/modify templates, cmake modules, shared files.

**STOP and wait for approval before modifying `template-cpp`.**

---

## PHASE 2: Architecture proposal (no code)

After tpl and template-cpp are approved, propose `tobus` architecture.

**Do not write code. Propose:**

1. **Directory structure** — `include/`, `src/`, `tests/`, `examples/` layout
2. **Core design** — IEventBus facade (NVI vs hybrid), EventBusFactory dispatch, SerializableEvent serialization, InProcessBus avoiding serialization
3. **CMake structure** — tpl via FetchContent, transport options (`TOBUS_ENABLE_UNIX_SOCKET`, etc.), library + examples wiring, Catch2 via FetchContent
4. **Async strategy** — EventHandler = `std::function<Task<void>(const T&)>`, fire-and-forget publish, co_spawn for Task<void> dispatch
5. **Transport stub plan** — which transports get TODO bodies vs header-only

**No code.** Design, rationale, tradeoffs.

I'll review, ask questions, then you implement (Phase 3).

---

## PHASE 3: Implementation

After architecture approval:

1. Modify `tpl` (if approved in Phase 0)
2. Modify `template-cpp` (if approved in Phase 1)
3. Generate `tobus` via `make init` from `/workspace/template-cpp/` into `/workspace/tobus/`
4. Implement `tobus`:

### MVP deliverables:
- **Core library** — `IEventBus` (pure interface), `EventTypeId`, `SerializableEvent`, `EventHandler`, `EventBusConfig`, `EventBusFactory`
- **InProcessBus** — first transport impl (in-process, callback vector, thread-safe)
- **One example** — working InProcessBus demo
- **Tests** — basic unit tests
- **Stub headers** — TODO-headers for future transports (UnixSocketBus, TCPServerBus, HTTPBus, WebSocketBus, RedisBus, ZeroMQBus, RabbitMQBus, KafkaBus) — only declarations, bodies with `// TODO: implement`

Other transports will be implemented later **using InProcessBus as reference**.

### Generation steps:
```bash
mkdir -p /workspace/tobus
cd /workspace/template-cpp
make init TYPE=<picked> NAME=tobus NS=aw::tobus --out /workspace/tobus
```

The scaffold renders into an **existing** directory — it does not create one.

---

## CONTEXT

`tobus` is an **event bus library** (namespace `aw::tobus`).

`/workspace/smlq_sfw/` is a Telegram AI persona project (~12 modules) that will consume `tobus` via FetchContent.
You are **NOT** tasked with modifying `smlq_sfw`. **Do not read its source code.**

`/workspace/tpl/` is a coroutine library in the same `aw::` ecosystem. `tobus` depends on it.

`tobus` is consumed via CMake FetchContent:
```cmake
include(FetchContent)
FetchContent_Declare(
    tobus
    GIT_REPOSITORY https://github.com/ai-warevo/tobus.git
    GIT_TAG main
)
FetchContent_MakeAvailable(tobus)
target_link_libraries(smlq_sfw PRIVATE aw::tobus)
```

---

## CONSTRAINTS

- **C++20**, CMake 3.23+, clang-18, libc++
- **Depends on `aw::tpl`** — read `/workspace/tpl/` locally. In production, `tobus` pulls `tpl` via FetchContent from `https://github.com/ai-warevo/tpl`
- Tests: Catch2 v3 via FetchContent
- CMake style: similar to `ai-warevo` / `aw::tpl` ecosystem
- Core API: modules depend only on `IEventBus`, transport-agnostic

```
Module ──publish──→ IEventBus ──subscribe──→ Module
                      ▲
              "which transport?"
              in_process / unix_socket / redis / kafka / ...
```

Module code doesn't change when transport changes — only config.

---

## KEY DESIGN CONSTRAINTS

### IEventBus: type-safe facade over type-erased dispatch

`IEventBus` must be a pure virtual interface (modules depend on it). Template methods cannot be virtual.

**Propose how to reconcile this.** Options include:
- **NVI + type-erasure bridge**: template `publish<T>()` on a facade forwards to a virtual `publish_raw(EventTypeId, std::any)` or `publish_raw(EventTypeId, std::span<const std::byte>)` on the impl.
- **Hybrid**: template methods on the interface itself (not virtual), with a type-erased factory that returns `std::shared_ptr<IEventBus>`.

### Async / coroutine integration

`EventHandler<T>` is `std::function<Task<void>(const T&)>` where `Task<T>` comes from `aw::tpl`.

`publish<T>()` is **fire-and-forget** — dispatches to all subscribers and returns immediately. Does NOT wait for handlers. Handlers run concurrently.

Network transports serialize and send — no handler waiting. InProcessBus dispatches via `co_spawn` from `aw::tpl`.

### Serialization boundary

InProcessBus must **avoid serialization** — events pass by value in memory. Network transports serialize. Clarify how your design achieves this.

---

## DO NOT

1. Implement future transports — only TODO stubs
2. Add external deps beyond `aw::tpl` (tpl + std only)
3. Build a message broker — not RabbitMQ/Kafka/Redis
4. Service discovery
5. Protocol definitions (protobuf, flatbuffers)
6. Queues, retries, dead letter queues
7. Refactor `smlq_sfw` — it's handled separately
8. Read `smlq_sfw` source code
