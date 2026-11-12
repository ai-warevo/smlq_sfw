# Task: In-Memory Message Buffer
Implementation:
- Create `src/engine/MessageBuffer.hpp/cpp`.
- Implement a structure to hold `std::vector<string>` keyed by `user_id`.

Verification:
- [x] Unit test the buffer: add strings, retrieve them, and ensure they are correctly stored and ordered per user.

Cleanup:
- [x] Ensure memory is cleared when a buffer is flushed or a user session ends.
- [x] Check for thread-safety if multiple threads access the buffer.
