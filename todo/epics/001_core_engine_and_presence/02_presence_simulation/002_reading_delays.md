# Task: Reading Delay Logic
Implementation:
- Implement random "read" delays (1.5 - 3.0s) in `UpdateDispatcher` before a message is processed.

Verification:
- [ ] Send a message and observe the delay between the message arriving in Telegram and the bot's next action (e.g., typing or responding). Verify it falls within the 1.5-3.0s range.

Cleanup:
- [ ] Ensure delays are non-blocking to the main event loop.
- [ ] Check that multiple concurrent "read" delays for different users don't interfere with each other.
