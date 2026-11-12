# Task: Worker Thread Pool & Task Offloading
Implementation:
- Create `src/engine/WorkerPool.hpp/cpp`.
- Implement a thread pool to handle asynchronous tasks (e.g., calling Ollama, processing media).
- **Integration:** The `UpdateDispatcher` or `MessageBuffer` should submit heavy tasks to this pool instead of executing them inline.
- Ensure the `ClientManager::send()` method is called from these background threads safely (TDLib's client is thread-safe for sending).

Verification:
- [ ] Submit a dummy long-running task to the pool and verify it doesn't block the main loop.
- [ ] Verify that multiple tasks can run in parallel up to the pool limit.

Cleanup:
- [ ] Ensure all threads are joined/terminated gracefully during shutdown.
- [ ] Check for potential race conditions when accessing shared resources from worker threads.
