# Task: Echo to LLM Transition
Implementation:
- Modify UpdateDispatcher or the message processing pipeline to call OllamaClient::generate_response instead of returning a hardcoded string.
- Ensure the response is sent back via TDLib's sendMessage.
