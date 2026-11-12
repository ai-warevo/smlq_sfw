import asyncio
import os
from dotenv import load_dotenv
from pyrogram import Client, filters

# Load environment variables from .env file
load_dotenv()

# Load credentials
API_ID = os.getenv("TG_API_ID")
API_HASH = os.getenv("TG_API_HASH")
SESSION_NAME = os.getenv("TG_SESSION", "session")

async def main():
    if not all([API_ID, API_HASH]):
        print("Error: TG_API_ID and TG_API_HASH must be set in the .env file.")
        return

    # Initialize the Client
    try:
        app = Client(SESSION_NAME, api_id=API_ID, api_hash=API_HASH)
    except Exception as e:
        print(f"Error initializing Pyrogram client: {e}")
        return

    # Start the client
    await app.start()
    print("Userbot started successfully. Waiting for messages...")

    # Define the echo handler
    @app.on_message(filters.text & ~filters.private) # Only echo in private chats
    async def echo_message(client, message):
        """Echoes the received message text."""
        if message.text:
            text = message.text
            print(f"Received message from {message.chat.id}: {text[:50]}...")
            await message.reply_text(text)
        else:
             print(f"Received non-text message from {message.chat.id}.")

    # Keep the client running until interrupted
    try:
        await asyncio.Future()  # Runs forever
    except asyncio.CancelledError:
        pass
    finally:
        await app.stop()
        print("Userbot stopped.")


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\nUserbot stopped by keyboard interrupt.")