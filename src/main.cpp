#include <ESP8266WiFi.h>
#include "config.h"
#include "wifi.h"
#include "mqtt.h"
#include "led.h"


void setup() {
  Serial.begin(9600);
  ledSetup();
  connectToWifi();
  connecttoMQTT();
}


void loop() {
  mqttLoop();
  ledLoop();
}
