#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "SerialDebug.hpp"

#include "TelegaBot.hpp"

#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASS"
#define BOT_TOKEN "bot:token"
#define CHAT_ID "@channel"

using namespace ArduLibs;

// the setup function runs once when you press reset or power the board
void setup()
{
    SerialDebug::begin(115200);

    // attempt to connect to Wifi network:
    SerialDebug::println(LOG_LEVEL::INFO, "Connecting to Wifi SSID " WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        SerialDebug::print(LOG_LEVEL::NONE, ".");
        delay(500);
    }
    SerialDebug::println(LOG_LEVEL::NONE, "");
    SerialDebug::println(LOG_LEVEL::INFO, String("WiFi connected. IP address: ") + WiFi.localIP().toString());

    WiFiClientSecure client;

    TelegaBot bot(BOT_TOKEN, client);

    bot.getMe();

    bot.sendMessage(CHAT_ID, "0 - Андрей\n1 - Василий");

    int32_t id = bot.getMsgID();

    const char* options[] = { "Андрей К.", "Василий Н." };

    bot.sendPoll(CHAT_ID, "И кто же победит?", options, 2);

    if (id > 0) bot.editMessage(CHAT_ID, id, "1 - Андрей\n1 - Василий");
}


// the loop function runs over and over again until power down or reset
void loop()
{

}

