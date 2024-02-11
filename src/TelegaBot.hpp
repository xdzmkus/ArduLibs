/*
 Name:		TelegaBot.hpp
 Created:	29.01.2024 14:58:52
 Author:	xdzmkus
*/

#ifndef _TelegaBot_hpp
#define _TelegaBot_hpp

#include <Arduino.h>
#include <WiFiClientSecure.h>

#include "SerialDebug.hpp"

namespace ArduLibs
{

    const char TELEGRAM_CERTIFICATE[] = R"=EOF=(
-----BEGIN CERTIFICATE-----
MIIDxTCCAq2gAwIBAgIBADANBgkqhkiG9w0BAQsFADCBgzELMAkGA1UEBhMCVVMx
EDAOBgNVBAgTB0FyaXpvbmExEzARBgNVBAcTClNjb3R0c2RhbGUxGjAYBgNVBAoT
EUdvRGFkZHkuY29tLCBJbmMuMTEwLwYDVQQDEyhHbyBEYWRkeSBSb290IENlcnRp
ZmljYXRlIEF1dGhvcml0eSAtIEcyMB4XDTA5MDkwMTAwMDAwMFoXDTM3MTIzMTIz
NTk1OVowgYMxCzAJBgNVBAYTAlVTMRAwDgYDVQQIEwdBcml6b25hMRMwEQYDVQQH
EwpTY290dHNkYWxlMRowGAYDVQQKExFHb0RhZGR5LmNvbSwgSW5jLjExMC8GA1UE
AxMoR28gRGFkZHkgUm9vdCBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkgLSBHMjCCASIw
DQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAL9xYgjx+lk09xvJGKP3gElY6SKD
E6bFIEMBO4Tx5oVJnyfq9oQbTqC023CYxzIBsQU+B07u9PpPL1kwIuerGVZr4oAH
/PMWdYA5UXvl+TW2dE6pjYIT5LY/qQOD+qK+ihVqf94Lw7YZFAXK6sOoBJQ7Rnwy
DfMAZiLIjWltNowRGLfTshxgtDj6AozO091GB94KPutdfMh8+7ArU6SSYmlRJQVh
GkSBjCypQ5Yj36w6gZoOKcUcqeldHraenjAKOc7xiID7S13MMuyFYkMlNAJWJwGR
tDtwKj9useiciAF9n9T521NtYJ2/LOdYq7hfRvzOxBsDPAnrSTFcaUaz4EcCAwEA
AaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYwHQYDVR0OBBYE
FDqahQcQZyi27/a9BUFuIMGU2g/eMA0GCSqGSIb3DQEBCwUAA4IBAQCZ21151fmX
WWcDYfF+OwYxdS2hII5PZYe096acvNjpL9DbWu7PdIxztDhC2gV7+AJ1uP2lsdeu
9tfeE8tTEH6KRtGX+rcuKxGrkLAngPnon1rpN5+r5N9ss4UXnT3ZJE95kTXWXwTr
gIOrmgIttRD02JDHBHNA7XIloKmf7J6raBKZV8aPEjoJpL1E/QYVN8Gb5DKj7Tjo
2GTzLH4U/ALqn83/B2gX2yKQOC16jdFU8WnjXzPKej17CuPKf1855eJ1usV2GDPO
LPAvTK33sefOT6jEm0pUBsV/fdUID+Ic/n4XuKxe9tQWskMJDE32p2u0mYRlynqI
4uJEvlz36hz1
-----END CERTIFICATE-----
)=EOF=";

#define TELEGRAM_HOST "api.telegram.org"
#define TELEGRAM_PORT 443

    class TelegaBot
    {
    public:

        TelegaBot(const char* token, WiFiClientSecure& client);
        virtual ~TelegaBot();

        bool getMe();
        bool sendMessage(const char* chatID, const char* msg);
        bool editMessage(const char* chatID, int32_t msgID, const char* msg);
        bool sendPoll(const char* chatID, const char* question, const char** options, uint8_t optNum);

        int32_t getMsgID();

    protected:

        void bufferInitGET();
        void bufferCopyText(const char* src);
        void bufferCopyURLencodedText(const char* src);
        void bufferAddChar(char ch);
        void bufferAddNumber(int32_t msgID);

        bool sendBuffer();

    protected:

        static const uint8_t RESPONSE_TIMEOUT = 10; // seconds

        static const uint16_t BUF_SIZE = 2048;
        char* _buffer;

        char* _token;
        WiFiClientSecure* const _client;
    };



    TelegaBot::TelegaBot(const char* token, WiFiClientSecure& client) : _client(&client)
    {
        client.setCACert(TELEGRAM_CERTIFICATE); // Add root certificate for api.telegram.org

        uint8_t tSize = strlen(token) + 1;

        _token = static_cast<char*>(malloc(tSize));

        if (_token != NULL)
        {
            strlcpy(_token, token, tSize);
        }

        _buffer = static_cast<char*>(malloc(BUF_SIZE + 1));

        if (_buffer != NULL)
        {
            _buffer[0] = '\0';
            _buffer[BUF_SIZE] = '\0';
        }
    }

    TelegaBot::~TelegaBot()
    {
        if (_token != NULL)
        {
            free(_token);
        }

        if (_buffer != NULL)
        {
            free(_buffer);
        }
    }

    bool TelegaBot::getMe()
    {
        if (_buffer == NULL) return false;

        bufferInitGET();
        bufferCopyText("getMe");

        return sendBuffer();
    }

    bool TelegaBot::sendMessage(const char* chatID, const char* msg)
    {
        if (_buffer == NULL) return false;

        bufferInitGET();
        bufferCopyText("sendMessage?chat_id=");
        bufferCopyText(chatID);
        bufferCopyText("&text=");
        bufferCopyURLencodedText(msg);

        return sendBuffer();
    }

    bool TelegaBot::editMessage(const char* chatID, int32_t msgID, const char* msg)
    {
        if (_buffer == NULL) return false;

        bufferInitGET();
        bufferCopyText("editMessageText?chat_id=");
        bufferCopyText(chatID);
        bufferCopyText("&message_id=");
        bufferAddNumber(msgID);
        bufferCopyText("&text=");
        bufferCopyURLencodedText(msg);

        return sendBuffer();
    }

    bool TelegaBot::sendPoll(const char* chatID, const char* question, const char** options, uint8_t optNum)
    {
        if (_buffer == NULL) return false;

        const char escp = '"';
        const char comma = ',';

        bufferInitGET();
        bufferCopyText("sendPoll?chat_id=");
        bufferCopyText(chatID);
        bufferCopyText("&question=");
        bufferCopyURLencodedText(question);
        bufferCopyText("&options=[");

        uint8_t i = 0;

        while (i < optNum)
        {
            bufferAddChar(escp);
            bufferCopyURLencodedText(options[i]);
            bufferAddChar(escp);
            if (++i < optNum) bufferAddChar(comma);
        }

        bufferAddChar(']');

        return sendBuffer();
    }

    int32_t TelegaBot::getMsgID()
    {
        if (_buffer == NULL) return -1;

        const char* prefix = "\"message_id\":";

        const char* numberStart = strstr(_buffer, prefix);

        if (numberStart == NULL) return -1;

        numberStart += strlen(prefix);

        char* end;
        int32_t number = strtol(numberStart, &end, 10);

        return number;
    }


    void TelegaBot::bufferInitGET()
    {
        _buffer[0] = '\0';
        strlcat(_buffer, "GET /bot", BUF_SIZE);
        if (_token != NULL) strlcat(_buffer, _token, BUF_SIZE);
        strlcat(_buffer, "/", BUF_SIZE);
    }

    void TelegaBot::bufferCopyText(const char* src)
    {
        strlcat(_buffer, src, BUF_SIZE);
    }

    void TelegaBot::bufferCopyURLencodedText(const char* src)
    {
        uint16_t bufEnd = strlen(_buffer);

        uint16_t len = strlen(src);

        for (uint16_t i = 0; i < len && bufEnd < BUF_SIZE; i++)
        {
            char c = src[i];
            if (c == ' ')
            {
                _buffer[bufEnd++] = '+';
            }
            else if (c <= 38 || c == '+')
            {
                _buffer[bufEnd++] = '%';
                _buffer[bufEnd++] = (char)((c >> 4) + (((c >> 4) > 9) ? 87 : 48));
                _buffer[bufEnd++] = (char)((c & 0xF) + (((c & 0xF) > 9) ? 87 : 48));
            }
            else
            {
                _buffer[bufEnd++] = c;
            }
        }

        _buffer[bufEnd] = '\0';
    }

    void TelegaBot::bufferAddChar(char ch)
    {
        uint16_t bufEnd = strlen(_buffer);

        if (bufEnd < BUF_SIZE)
        {
            _buffer[bufEnd++] = ch;
            _buffer[bufEnd] = '\0';
        }
    }

    void TelegaBot::bufferAddNumber(int32_t msgID)
    {
        uint16_t bufEnd = strlen(_buffer);

        snprintf(_buffer + bufEnd, BUF_SIZE - bufEnd, "%d", msgID);
    }


    bool TelegaBot::sendBuffer()
    {
        // Connect with api.telegram.org if not already connected
        if (!_client->connected())
        {
            SerialDebug::println(LOG_LEVEL::DEBUG, String(F("[TELEGABOT] Connecting to server")));

            if (!_client->connect(TELEGRAM_HOST, TELEGRAM_PORT))
            {
                SerialDebug::println(LOG_LEVEL::ERROR, String(F("[TELEGABOT] Conection error: ")));

                _client->lastError(_buffer, BUF_SIZE);

                _client->stop();

                SerialDebug::println(LOG_LEVEL::ERROR, String(_buffer));

                return false;
            }
        }

        if (_client->connected())
        {
            SerialDebug::println(LOG_LEVEL::DEBUG, String(F("[TELEGABOT] Sending: ")) + _buffer);

            _client->print(_buffer);
            _client->println(F(" HTTP/1.1"));
            _client->println(F("Host:" TELEGRAM_HOST));
            _client->println(F("Accept: application/json"));
            _client->println(F("Cache-Control: no-cache"));
            _client->println();

            uint32_t previousMillis = millis();
            while (!_client->available())
            {
                yield();
                uint32_t currentMillis = millis();
                if (currentMillis - previousMillis >= ((int32_t)RESPONSE_TIMEOUT * 1000UL))
                {
                    SerialDebug::println(LOG_LEVEL::ERROR, String(F("[TELEGABOT] Response timeout!")));

                    _client->stop();

                    return false;
                }
            }

            uint32_t receivedPacketLength = 0;

            while (_client->available() && receivedPacketLength < BUF_SIZE)
            {
                _buffer[receivedPacketLength++] = static_cast<char>(_client->read());
            }

            _buffer[receivedPacketLength] = '\0';

            _client->stop();

            SerialDebug::println(LOG_LEVEL::DEBUG, String(F("[TELEGABOT] Response lenth: ")) + receivedPacketLength);

            SerialDebug::println(LOG_LEVEL::DEBUG, String(F("[TELEGABOT] Response: ")) + _buffer);

            return true;
        }

        return false;
    }

}

#endif

