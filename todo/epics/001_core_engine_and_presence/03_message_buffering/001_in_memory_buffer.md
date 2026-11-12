# Task: In-Memory Message Buffer
Implementation:
- Create `src/engine/MessageBuffer.hpp/cpp`.
- Implement a structure to hold `std::vector<string>` keyed by `user_id`.

Verification:
- [ ] Unit test the buffer: add strings, retrieve them, and ensure they are correctly stored and ordered per user.

Cleanup:
- [ ] Ensure memory is cleared when a buffer is flushed or a user session ends.
- [ ] Check for thread-safety if multiple threads access the buffer.
