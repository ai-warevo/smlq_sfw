# Task: Graceful Shutdown & Signal Handling
Implementation:
- Implement a global atomic boolean flag is_running to control the main loop.
- Set up signal handlers for SIGTERM and SIGINT (Ctrl+C) to set is_running = false.
- **Shutdown Sequence:**
    1. Stop the main event loop.
    2. Call ClientManager::close() to trigger TDLib's shutdown.
    3. Wait for uthorizationStateClosed to ensure all session data and SQLite buffers are flushed to disk before exiting.
