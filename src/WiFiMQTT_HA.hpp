#ifndef _WIFIMQTT_HA_H_
#define _WIFIMQTT_HA_H_

#include <WiFiManager.h>

#include <WiFiClient.h>

#include <PubSubClient.h>

#include <ArduinoJson.h>

#include "SerialDebug.hpp"

#include "ClockTimer.hpp"

#include "EEPROMHelper.hpp"

namespace ArduLibs
{

	class WiFiMQTT_HA
	{
	public:

		static const uint16_t TIMEOUT = 5000UL;
		static const uint16_t RECONNECT_TIMEOUT = 30000UL;

	private:

		const char* const _WLAN_AP_SSID;
		const char* const _WLAN_AP_PASS;
		const char* const _WLAN_HOSTNAME;

		const char* const _DEVICE_UNIQUE_ID;
		const char* const _AVAIL_STATUS_TOPIC;

		MillisTimer reconnectTimer;

		bool shouldSaveConfig = false;

	protected:

		WiFiClient client;

		PubSubClient mqtt;

		char mqtt_host[64] = {};
		char mqtt_port[6] = {};
		char mqtt_user[32] = {};
		char mqtt_pass[32] = {};

	public:

		WiFiMQTT_HA(const char* wlan_ap_ssid, const char* wlan_ap_pass, const char* wlan_hostname, const char* device_unique_id, const char* availability_status_topic);

		void init(bool reconfigure = false);

		void process();

	protected:

		virtual void loadConfig();

		virtual void saveConfig();

		virtual void discover() = 0;

		virtual void subscribe() = 0;

		virtual void receiveMessage(char* topic, uint8_t* payload, unsigned int length) = 0;

		bool publishJson(const char* const topic, JsonDocument& payload, bool retain = false);

	private:

		// callback notifying us of the need to save config
		void saveConfigCallback()
		{
			SerialDebug::println(LOG_LEVEL::INFO, F("Configuration changed. Need to save config."));
			shouldSaveConfig = true;
		}
	};


	WiFiMQTT_HA::WiFiMQTT_HA(const char* wlan_ap_ssid, const char* wlan_ap_pass, const char* wlan_hostname, const char* device_unique_id, const char* availability_status_topic) :
		_WLAN_AP_SSID(wlan_ap_ssid), _WLAN_AP_PASS(wlan_ap_pass), _WLAN_HOSTNAME(wlan_hostname),
		_DEVICE_UNIQUE_ID(device_unique_id), _AVAIL_STATUS_TOPIC(availability_status_topic),
		reconnectTimer(WiFiMQTT_HA::RECONNECT_TIMEOUT)
	{
		mqtt.setClient(client);
		mqtt.setKeepAlive(60);
		mqtt.setBufferSize(256);
		mqtt.setCallback(std::bind(&WiFiMQTT_HA::receiveMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}

	void WiFiMQTT_HA::init(bool reconfigure)
	{
		loadConfig();

		WiFiManager wm;

		if (reconfigure)
		{
			WiFiManagerParameter custom_mqtt_host("host", "MQTT hostname", mqtt_host, 64);
			WiFiManagerParameter custom_mqtt_port("port", "MQTT port", mqtt_port, 6);
			WiFiManagerParameter custom_mqtt_user("user", "MQTT user", mqtt_user, 32, " maxlength=31");
			WiFiManagerParameter custom_mqtt_pass("pass", "MQTT pass", mqtt_pass, 32, " maxlength=31 type='password'");

			wm.addParameter(&custom_mqtt_host);
			wm.addParameter(&custom_mqtt_port);
			wm.addParameter(&custom_mqtt_user);
			wm.addParameter(&custom_mqtt_pass);

			wm.setSaveConfigCallback(std::bind(&WiFiMQTT_HA::saveConfigCallback, this));

			wm.setConfigPortalTimeout(180);
			wm.setConfigPortalBlocking(true);

			wm.startConfigPortal(_WLAN_AP_SSID, _WLAN_AP_PASS);

			strcpy(mqtt_host, custom_mqtt_host.getValue());
			strcpy(mqtt_port, custom_mqtt_port.getValue());
			strcpy(mqtt_user, custom_mqtt_user.getValue());
			strcpy(mqtt_pass, custom_mqtt_pass.getValue());

			if (shouldSaveConfig)
				saveConfig();

			SerialDebug::println(LOG_LEVEL::INFO, F("WiFi Reconfigured! Rebooting..."));

			delay(TIMEOUT);
			ESP.restart();
		}

		// Set the ESP8266 to be a WiFi-client
		WiFi.mode(WIFI_STA);
		WiFi.hostname(_WLAN_HOSTNAME);

		SerialDebug::println(LOG_LEVEL::INFO, _WLAN_HOSTNAME);

		wm.setEnableConfigPortal(false);

		if (!wm.autoConnect())
		{
			SerialDebug::println(LOG_LEVEL::ERROR, F("Connection Failed! Rebooting..."));
			delay(TIMEOUT);
			ESP.restart();
		}

		SerialDebug::println(LOG_LEVEL::INFO, F("WiFi connected"));
		SerialDebug::println(LOG_LEVEL::INFO, String(F("IP address: ")) + WiFi.localIP().toString());

		mqtt.setServer(mqtt_host, atoi(mqtt_port));
	}

	void WiFiMQTT_HA::process()
	{
		if (!mqtt.connected())
		{
			if (reconnectTimer.isActive() && !reconnectTimer.isReady())
				return;

			SerialDebug::println(LOG_LEVEL::INFO, F("Connecting to MQTT... "));

			if (!mqtt.connect(_DEVICE_UNIQUE_ID, mqtt_user, mqtt_pass, _AVAIL_STATUS_TOPIC, 0, true, "offline"))
			{
				reconnectTimer.start();

				SerialDebug::println(LOG_LEVEL::ERROR, String(F("Connect error: ")) + mqtt.state());

				mqtt.disconnect();

				return;
			}
			else
			{
				SerialDebug::println(LOG_LEVEL::INFO, F("MQTT Connected!"));

				reconnectTimer.stop();

				mqtt.publish(_AVAIL_STATUS_TOPIC, "online", true);

				discover();

				subscribe();
			}
		}

		mqtt.loop();
	}

	void WiFiMQTT_HA::loadConfig()
	{
		EEPROMHelper::begin(134);

		EEPROMHelper::readEEPROM(0, 64).toCharArray(mqtt_host, 64);   // 0-63
		EEPROMHelper::readEEPROM(64, 6).toCharArray(mqtt_port, 6);    // 64-69
		EEPROMHelper::readEEPROM(70, 32).toCharArray(mqtt_user, 32);  // 70-101
		EEPROMHelper::readEEPROM(102, 32).toCharArray(mqtt_pass, 32); // 102-133
	}

	void WiFiMQTT_HA::saveConfig()
	{
		EEPROMHelper::writeEEPROM(0, 64, mqtt_host);   // 0-63
		EEPROMHelper::writeEEPROM(64, 6, mqtt_port);   // 64-69
		EEPROMHelper::writeEEPROM(70, 32, mqtt_user);  // 70-101
		EEPROMHelper::writeEEPROM(102, 32, mqtt_pass); // 102-133

		EEPROMHelper::commit();
	}

	void WiFiMQTT_HA::receiveMessage(char* topic, uint8_t* payload, unsigned int length)
	{
		String payloadString;
		for (uint8_t i = 0; i < length; i++) {
			payloadString.concat((char)payload[i]);
		}

		SerialDebug::println(LOG_LEVEL::DEBUG, String(F("MQTT Message arrived!")));
		SerialDebug::println(LOG_LEVEL::DEBUG, topic);
		SerialDebug::println(LOG_LEVEL::DEBUG, payloadString);
	}

	bool WiFiMQTT_HA::publishJson(const char* const topic, JsonDocument& payload, bool retain)
	{
		if (reconnectTimer.isActive()) return false;

		String jsonString;

		serializeJson(payload, jsonString);

		SerialDebug::println(LOG_LEVEL::DEBUG, String(F("Publish message: ")));
		SerialDebug::println(LOG_LEVEL::DEBUG, jsonString);

		if (mqtt.beginPublish(topic, jsonString.length(), retain))
		{
			mqtt.print(jsonString);
			if (mqtt.endPublish()) return true;
		}

		SerialDebug::println(LOG_LEVEL::ERROR, String(F("Publish failed: ")) + mqtt.state());

		return false;
	}

}

#endif

