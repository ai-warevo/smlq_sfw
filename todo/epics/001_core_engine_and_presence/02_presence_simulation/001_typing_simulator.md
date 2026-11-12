# Task: Typing Status Simulator
Implementation:
- Create `src/engine/PresenceSimulator.hpp/cpp`.
- Implement logic to call TDLib's 'sendChatAction' (typing) based on message length.
- Formula: duration = max(2.0, min(10.0, length * 0.15)).

Verification:
- [ ] Send a short message and verify "typing..." appears in the Telegram client for ~2s.
- [ ] Send a very long message and verify "typing..." stays active for up to 10s.

Cleanup:
- [ ] Ensure the timer/thread used for typing is properly cancelled if the bot shuts down or the chat action is interrupted.
- [ ] Check that no multiple "typing" actions are overlapping for the same user.
