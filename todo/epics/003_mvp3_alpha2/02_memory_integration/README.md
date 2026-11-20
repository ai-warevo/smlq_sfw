# 02: Memory Integration

**Epic:** 003 (MVP 3 Alpha 2)
**Phase:** 5

## Requirements

### 1. Brain Integration
- Brain calls `MemorySystem::saveMessage()` after each flush
- Brain calls `MemorySystem::getRecent()` before LLM request
- Facts are extracted from messages (simple rule: first mentions of names)

### 2. Context for LLM
- Last 10 messages → added to prompt as history
- Format: `user: ... \n assistant: ...`

## Verification

- [ ] Conversation history available to LLM
- [ ] Facts are saved after each conversation
