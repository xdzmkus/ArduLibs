#include "SerialDebug.hpp"

#include "WiFiMQTT_HA.hpp"

#include "ArduinoDebounceButton.h"

using namespace ArduLibs;

#define BTN_PIN D0

ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::VCC, BUTTON_NORMAL::OPEN);

const char* const WLAN_AP_SSID = "ArduLibs-Sensor";
const char* const WLAN_AP_PASS = "5b385136";
const char* const WLAN_HOSTNAME = "ArduLibs";

const char* const DEVICE_UNIQUE_ID = "ArduLibs-Sensor-36c";

const char* const DEVICE_manufacturer = "ArduLibs";
const char* const DEVICE_model = "WiFiMQTT_HA";
const char* const DEVICE_name = "ArduLibs WiFiMQTT_HA_Sensor";
const char* const DEVICE_sw_version = "1.0.0";

const char* const TEMPERATURE_SENSOR_UNIQUE_ID = "ArduLibs-36c-temperature";
const char* const HUMIDITY_SENSOR_UNIQUE_ID = "ArduLibs-36c-humidity";
const char* const PRESSURE_SENSOR_UNIQUE_ID = "ArduLibs-36c-pressure";

const char* const DISCOVERY_TOPIC_TEMPERATURE = "homeassistant/sensor/ardulibs-WiFiMQTT_HA_Sensor/temperature/config";
const char* const DISCOVERY_TOPIC_HUMIDITY = "homeassistant/sensor/ardulibs-WiFiMQTT_HA_Sensor/humidity/config";
const char* const DISCOVERY_TOPIC_PRESSURE = "homeassistant/sensor/ardulibs-WiFiMQTT_HA_Sensor/pressure/config";

const char* const COMMON_STATE_TOPIC = "homeassistant/sensor/ardulibs-WiFiMQTT_HA_Sensor/state";
const char* const AVAIL_STATUS_TOPIC = "homeassistant/sensor/ardulibs-WiFiMQTT_HA_Sensor/status";

class WiFiMQTT_Sensor : public WiFiMQTT_HA
{
public:

	WiFiMQTT_Sensor() 
		: WiFiMQTT_HA(WLAN_AP_SSID, WLAN_AP_PASS, WLAN_HOSTNAME, DEVICE_UNIQUE_ID, AVAIL_STATUS_TOPIC)
	{
	}

	void publishState()
	{
		JsonDocument doc;

		doc["temperature"] = random(-20, 50);
		doc["humidity"] = random(0, 100);
		doc["pressure"] = random(900, 1100);

		publishJson(COMMON_STATE_TOPIC, doc);
	}

protected:

	void discover() override
	{
		JsonDocument deviceDoc;

		deviceDoc["platform"] = "mqtt";
		deviceDoc["enabled_by_default"] = true;
		deviceDoc["state_class"] = "measurement";
		deviceDoc["state_topic"] = COMMON_STATE_TOPIC;
		deviceDoc["json_attributes_topic"] = COMMON_STATE_TOPIC;
		deviceDoc["availability"][0]["topic"] = AVAIL_STATUS_TOPIC;

		JsonObject device = deviceDoc["device"].to<JsonObject>();
		device["identifiers"] = DEVICE_UNIQUE_ID;
		device["manufacturer"] = DEVICE_manufacturer;
		device["model"] = DEVICE_model;
		device["name"] = DEVICE_name;
		device["sw_version"] = DEVICE_sw_version;

		JsonDocument doc;

		doc = deviceDoc;
		doc["name"] = "Temperature";
		doc["device_class"] = "temperature";
		doc["unit_of_measurement"] = "°C";
		doc["value_template"] = "{{ value_json.temperature }}";
		doc["unique_id"] = TEMPERATURE_SENSOR_UNIQUE_ID;

		publishJson(DISCOVERY_TOPIC_TEMPERATURE, doc, true);

		doc = deviceDoc;
		doc["name"] = "Humidity";
		doc["device_class"] = "humidity";
		doc["unit_of_measurement"] = "%";
		doc["value_template"] = "{{ value_json.humidity }}";
		doc["unique_id"] = HUMIDITY_SENSOR_UNIQUE_ID;

		publishJson(DISCOVERY_TOPIC_HUMIDITY, doc, true);

		doc = deviceDoc;
		doc["name"] = "Pressure";
		doc["device_class"] = "pressure";
		doc["unit_of_measurement"] = "hPa";
		doc["value_template"] = "{{ value_json.pressure }}";
		doc["unique_id"] = PRESSURE_SENSOR_UNIQUE_ID;

		publishJson(DISCOVERY_TOPIC_PRESSURE, doc, true);
	}

	void subscribe() override
	{
		// NOT USED
	}

	void receiveMessage(char* topic, uint8_t* payload, unsigned int length) override
	{
		WiFiMQTT_HA::receiveMessage(topic, payload, length);
	}
};

WiFiMQTT_Sensor wifiMqttSensor;

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
	delay(wifiMqttSensor.TIMEOUT);

	bool f_setupMode = btn.getCurrentState();

	if (f_setupMode)
		SerialDebug::println(LOG_LEVEL::WARN, F("BUTTON PRESSED - RECONFIGURE WIFI"));

	wifiMqttSensor.init(f_setupMode);

	SerialDebug::println(LOG_LEVEL::WARN, String(F("Device restarted")));

	tickerMQTT.attach(MQTT_PUBLISH_INTERVAL, callback_publishMQTT);
}

void loop()
{
	btn.check();

	wifiMqttSensor.process();

	if (doPublish)
	{
		doPublish = false;

		wifiMqttSensor.publishState();
	}
}
