#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "config.h"
#include "led.h"

void onMQTTMessage(char* topic, byte* payload, unsigned int length);
void connecttoMQTT();
void mqttLoop();
