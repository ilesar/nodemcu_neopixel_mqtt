#include "../Interface/Kernel.h"

Kernel::Kernel(char* deviceId)
{
    Serial.begin(115200);
    delay(100);

    _deviceId = deviceId;

    _led = new LedStripService(LED_PIN, LED_COUNT);
    _mqtt = new MqttService(MQTT_HOST, MQTT_PORT, _deviceId);
    _firmware = new FirmwareUpdateService(FIRMWARE_PASSWORD);
    _wifi = new WirelessNetworkingService(WIFI_SSID, WIFI_PASSWORD);
}

void Kernel::setup(void (*callback)(char *charTopic, uint8_t *payload, unsigned int length))
{
    _led->connect();
    _led->applyPreset(Solid, true);
    _wifi->connect();
    _mqtt->setup(callback);
    _mqtt->connect();
    _mqtt->install();
    _firmware->setup();
}

void Kernel::loop()
{
    _firmware->waitForUpdate();
    _mqtt->loop();
}