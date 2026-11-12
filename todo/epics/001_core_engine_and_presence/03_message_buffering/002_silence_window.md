# Task: Silence Window Logic
Implementation:
- Implement a timer that triggers when no new messages are received from a specific `user_id`.
- Upon timeout (10s), 'flush' the buffer to the next stage in the pipeline.

Verification:
- [x] Send two messages with 5s gap $\rightarrow$ verify buffer is NOT flushed.
- [x] Send a message, wait 11s $\rightarrow$ verify buffer IS flushed and timer resets correctly.

Cleanup:
- [x] Ensure timers are properly cancelled when a user sends a new message to prevent multiple simultaneous flushes for the same user.
- [x] Check that the flush operation is thread-safe and doesn't race with incoming messages.
