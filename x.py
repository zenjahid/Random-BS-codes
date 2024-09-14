import os
import logging
from dotenv import load_dotenv
from telegram import (Update, BotCommand, InlineKeyboardButton, InlineKeyboardMarkup)
from telegram.ext import (Application, CommandHandler, MessageHandler, filters, ContextTypes, CallbackQueryHandler)
from utility import Utility
from file_info_retriever import FileInfoRetriever
from typing import Final
from datetime import datetime

# Load environment variables
load_dotenv()

# Enable logging to a file
logging.basicConfig(
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    level=logging.INFO,
    handlers=[
        logging.FileHandler("bot_log.txt"),
        logging.StreamHandler()
    ]
)
logger = logging.getLogger(__name__)

# Credentials
TOKEN: Final = os.getenv("TOKEN")
BOT_USERNAME: Final = os.getenv("BOT_USERNAME")
CHANNEL_USERNAME: Final = os.getenv("CHANNEL_USERNAME")

utility = Utility()
file_info_retriever = FileInfoRetriever()

# Membership Check
async def check_channel_membership(update: Update, context: ContextTypes.DEFAULT_TYPE) -> bool:
    """Checks if the user is a member of the specified Telegram channel."""
    user_id = update.message.from_user.id
    user_name = update.message.from_user.username

    try:
        # Check if the user is a member of the channel
        member = await context.bot.get_chat_member(chat_id=CHANNEL_USERNAME, user_id=user_id)

        # If user is creator, administrator, or member
        if member.status in ['creator', 'administrator', 'member']:
            return True
        else:
            await update.message.reply_text(f"⚠️ Please join our channel first: {CHANNEL_USERNAME} 🔗")
            return False
    except Exception as e:
        logger.error(f"Error when checking membership for user {user_name}: {e}")
        await update.message.reply_text(f"⚠️ Please join our channel first: {CHANNEL_USERNAME} 🔗")
        return False

# Command Handlers
async def start_command(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    """Starts the bot and provides a welcome message."""
    if await check_channel_membership(update, context):
        await update.message.reply_text(
            "👋 <b>Welcome to the TeraBox Downloader Bot!</b> 🤖\n\n"
            "📋 Use /help to see how to use this bot.\n\n"
            "🔗 <b>Official Telegram channel:</b> @teraboxdownloader_by_anoxmax\n"
            "📞 <b>Developer contact:</b> @AnoxmaxBot",
            parse_mode="HTML"
        )

async def help_command(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    """Provides help information."""
    if await check_channel_membership(update, context):
        await update.message.reply_text(
            "ℹ️ <b>How to use the bot:</b>\n\n"
            "1️⃣ Send a TeraBox file link using the /link command followed by the link.\n"
            "Example: <code>/link https://teraboxapp.com/s/XXXXXXXXXXXX</code>\n\n"
            "🔗 <b>Official Telegram channel:</b> @teraboxdownloader_by_anoxmax\n"
            "📞 <b>Developer contact:</b> @AnoxmaxBot",
            parse_mode="HTML"
        )

def modify_link(link: str) -> str:
    """Ensures the provided link starts with http."""
    if not link.startswith("http"):
        link = "https://" + link
    return link

async def link_command(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    """Processes the provided TeraBox link and returns file information."""
    if await check_channel_membership(update, context):
        if context.args:
            link = context.args[0]
            modified_link = modify_link(link)
            current_user = update.message.chat_id
            username = update.message.from_user.username
            first_name = update.message.from_user.first_name
            last_name = update.message.from_user.last_name
            full_name = f"{first_name} {last_name}".strip()

            logger.info(f"User {username} ({full_name}, {current_user}) sent a link: {modified_link}")

            download_file = file_info_retriever.get_file_info(modified_link, str(current_user))

            if download_file and download_file[4]:
                download_link_logging = f"Filename = {download_file[0]}\nFile Size = {download_file[1]}\nDownload link = {download_file[2]}"
                response_str = (
                    f"📂 <b>Filename:</b> <code>{download_file[0]}</code>\n"
                    f"📦 <b>File Size:</b> <code>{download_file[1]}</code>\n"
                    f"📢 <b>Official Channel:</b> @teraboxdownloader_by_anoxmax\n"
                )
                if download_file[6]:
                    keyboard = [
                        [InlineKeyboardButton("⬇️ Download", url=str(download_file[5]))],
                        [InlineKeyboardButton("🎥 Stream Link", url=str(download_file[6]))]
                    ]
                else:
                    keyboard = [
                        [InlineKeyboardButton("⬇️ Download", url=str(download_file[5]))]
                    ]
                reply_markup = InlineKeyboardMarkup(keyboard)
                logger.info(download_link_logging)
                await update.message.reply_text(response_str, reply_markup=reply_markup, parse_mode="HTML")
            else:
                await update.message.reply_text("❌ File not found!")
        else:
            await update.message.reply_text("⚠️ Please provide a valid link after the command, e.g., /link https://teraboxapp.com/s/XXXXXXXXXXXX")

# Responses
async def handle_message(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    """Handles messages and logs user interactions."""
    if await check_channel_membership(update, context):
        message_type = update.message.chat.type
        text = update.message.text
        chat_id = update.message.chat_id
        username = update.message.from_user.username
        first_name = update.message.from_user.first_name
        last_name = update.message.from_user.last_name
        full_name = f"{first_name} {last_name}".strip()
        message_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        user_info = f"⏰ Time: {message_time} | 👤 User: {username} ({full_name}, {chat_id}) | 💬 Message: {text}"

        logger.info(user_info)

        if message_type in ["group", "supergroup"] and BOT_USERNAME in text:
            new_text = text.replace(BOT_USERNAME, "").trim()
            response = 'ℹ️ Use /help for more information.'
        else:
            response = 'ℹ️ Use /help for more information.'

        await update.message.reply_text(response)

# Error Handler
async def error(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    """Logs errors caused by updates."""
    logger.error(f"⚠️ Update {update} caused error {context.error}")

def main(token: str):
    print("🚀 Starting bot...")
    bot_app = Application.builder().token(token).build()

    # Setup commands
    commands = [
        BotCommand("start", "🚀 Start the bot"),
        BotCommand("help", "ℹ️ Get help information"),
        BotCommand("link", "🔗 Return the provided link")
    ]
    bot_app.bot.set_my_commands(commands)

    # Handlers
    bot_app.add_handler(CommandHandler("start", start_command))
    bot_app.add_handler(CommandHandler("help", help_command))
    bot_app.add_handler(CommandHandler("link", link_command, filters=filters.ChatType.PRIVATE))
    bot_app.add_handler(MessageHandler(filters.TEXT & ~filters.COMMAND, handle_message))
    bot_app.add_error_handler(error)

    print("🔄 Polling...")
    bot_app.run_polling(poll_interval=4)

# Main function
if __name__ == "__main__":
    main(TOKEN)
