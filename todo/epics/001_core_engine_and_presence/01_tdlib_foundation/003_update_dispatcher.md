# Task: Event-Driven Update Dispatcher & Message Filtering
Implementation:
- Create `src/engine/UpdateDispatcher.hpp/cpp`.
- Implement an infinite loop that polls TDLib updates using `receive()` with a timeout.
- **Implement Strict Message Filters:**
    1. **Ignore Outgoing:** Check `is_outgoing_` flag; skip if true to prevent self-response loops.
    2. **Filter Stale Messages:** Compare message `date_` with program start time; ignore messages older than 10s (prevents processing backlog on startup).
    3. **Chat Type Filter:** Only process `chatTypePrivate` (or as configured) to ensure the bot stays in DMs.
    4. **Content Extraction:** Extract text, voice, or media metadata from the update payload.
- Dispatch valid updates to registered listeners (Presence Simulator, Message Buffer).

Verification:
- [ ] Run the bot and send a message from another account.
- [ ] Verify in logs that `UpdateDispatcher` received the update and dispatched it to at least one listener.
- [ ] Test "stale" messages by sending an old message via TDLib (if possible) or simulating time jump, ensuring they are ignored.

Cleanup:
- [ ] Ensure no memory leaks occur during continuous polling (check for unhandled updates or dangling pointers).
- [ ] Check that dispatcher doesn't block the main thread if a listener is slow (use async/threads).
