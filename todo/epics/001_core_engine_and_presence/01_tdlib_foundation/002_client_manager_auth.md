# Task: ClientManager & Auth State Machine
Implementation:
- Create `src/core/ClientManager.hpp/cpp`.
- Implement TDLib authorization state machine (WaitPhoneNumber, WaitCode, etc.).
- Ensure session data is written to the persistent volume mapped in Docker.

Verification:
- [ ] Run the container in interactive mode (`docker compose run --rm userbot`).
- [ ] Verify that you can successfully enter phone number and code.
- [ ] Stop the container and restart it; verify that it skips auth and goes straight to `authorizationStateReady` (session persistence check).

Cleanup:
- [ ] Ensure no hardcoded API keys or credentials are left in the source code.
- [ ] Verify all logs related to auth are properly leveled (INFO/DEBUG) and not spamming stdout.
