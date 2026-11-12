# Task: Telegram JSON Parser
Implementation:
- Create src/tools/Parser.hpp/cpp.
- Implement logic to parse exported .json files from Telegram, extracting message text, sender, and timestamps.
- Apply sessionization rule: messages by same author with <60s gap are one block.
