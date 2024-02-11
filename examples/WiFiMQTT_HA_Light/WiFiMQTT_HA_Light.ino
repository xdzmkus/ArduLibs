#include "SerialDebug.hpp"

#include "WiFiMQTT_HA.hpp"

#define BTN_PIN D0

#include "ArduinoDebounceButton.h"

using namespace ArduLibs;

ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::GND, BUTTON_NORMAL::OPEN);

const char* const WLAN_AP_SSID = "ArduLibs-Light";
const char* const WLAN_AP_PASS = "5b385136";
const char* const WLAN_HOSTNAME = "ArduLibs";

const char* const DEVICE_UNIQUE_ID = "ArduLibs-Light-36c";

const char* const DEVICE_manufacturer = "ArduLibs";
const char* const DEVICE_model = "WiFiMQTT_HA";
const char* const DEVICE_name = "ArduLibs WiFiMQTT_HA_Light";
const char* const DEVICE_sw_version = "1.0.0";

const char* const DISCOVERY_TOPIC = "homeassistant/light/ardulibs-WiFiMQTT_HA_Light/config";
const char* const AVAIL_STATUS_TOPIC = "homeassistant/light/ardulibs-WiFiMQTT_HA_Light/status";
const char* const STATE_TOPIC = "homeassistant/light/ardulibs-WiFiMQTT_HA_Light/state";
const char* const COMMAND_TOPIC = "homeassistant/light/ardulibs-WiFiMQTT_HA_Light/set";
const char* const BRIGHTNESS_COMMAND_TOPIC = "homeassistant/light/ardulibs-WiFiMQTT_HA_Light/brightness/set";
const char* const EFFECT_COMMAND_TOPIC = "homeassistant/light/ardulibs-WiFiMQTT_HA_Light/effect/set";


class WiFiMQTT_HA_Light : public WiFiMQTT_HA
{
public:

	WiFiMQTT_HA_Light() 
		: WiFiMQTT_HA(WLAN_AP_SSID, WLAN_AP_PASS, WLAN_HOSTNAME, DEVICE_UNIQUE_ID, AVAIL_STATUS_TOPIC)
	{
	}

	void publishState()
	{
		static bool state = false;
		static uint8_t brightness = 127;

		state = !state;
		brightness += 5;

		JsonDocument doc;

		doc["state"] = state ? "ON" : "OFF";
		doc["brightness"] = brightness;
		doc["effect"] = "RAINBOW";

		publishJson(STATE_TOPIC, doc);
	}

protected:

	void discover() override
	{
		JsonDocument doc;

		doc["name"] = "LED";
		doc["unique_id"] = DEVICE_UNIQUE_ID;
		doc["platform"] = "mqtt";
		doc["availability"][0]["topic"] = AVAIL_STATUS_TOPIC;
		doc["command_topic"] = COMMAND_TOPIC;
		doc["state_topic"] = STATE_TOPIC;
		doc["state_value_template"] = "{{ value_json.state }}";
		doc["brightness_state_topic"] = STATE_TOPIC;
		doc["brightness_command_topic"] = BRIGHTNESS_COMMAND_TOPIC;
		doc["brightness_value_template"] = "{{ value_json.brightness }}";
		doc["effect_state_topic"] = STATE_TOPIC;
		doc["effect_command_topic"] = EFFECT_COMMAND_TOPIC;
		doc["effect_value_template"] = "{{ value_json.effect }}";
		doc["effect_list"][0] = "COLORS";
		doc["effect_list"][1] = "RAINBOW";


		JsonObject device = doc["device"].to<JsonObject>();
		device["identifiers"] = DEVICE_UNIQUE_ID;
		device["manufacturer"] = DEVICE_manufacturer;
		device["model"] = DEVICE_model;
		device["name"] = DEVICE_name;
		device["sw_version"] = DEVICE_sw_version;

		publishJson(DISCOVERY_TOPIC, doc, true);
	}

	void subscribe() override
	{
		mqtt.subscribe(COMMAND_TOPIC);
		mqtt.subscribe(BRIGHTNESS_COMMAND_TOPIC);
		mqtt.subscribe(EFFECT_COMMAND_TOPIC);
	}

	void receiveMessage(char* topic, uint8_t* payload, unsigned int length) override
	{
		WiFiMQTT_HA::receiveMessage(topic, payload, length);
	}
};

WiFiMQTT_HA_Light wifiMqttLight;

#include <Ticker.h>
Ticker tickerMQTT;

#define MQTT_PUBLISH_INTERVAL	30.0F   // 30 seconds

volatile bool doPublish = false;

void callback_publishMQTT()
{
	doPublish = true;
}

void setup(void)
{
	SerialDebug::begin(115200);

	btn.initPin();

	// wait for button to reconfigure WiFi
	delay(wifiMqttLight.TIMEOUT);

	bool f_setupMode = btn.getCurrentState();

	if (f_setupMode)
		SerialDebug::println(LOG_LEVEL::WARN, F("BUTTON PRESSED - RECONFIGURE WIFI"));

	wifiMqttLight.init(f_setupMode);

	SerialDebug::println(LOG_LEVEL::WARN, String(F("Device restarted")));

	tickerMQTT.attach(MQTT_PUBLISH_INTERVAL, callback_publishMQTT);
}

void loop()
{
	btn.check();

	wifiMqttLight.process();

	if (doPublish)
	{
		doPublish = false;

		wifiMqttLight.publishState();
	}
}
