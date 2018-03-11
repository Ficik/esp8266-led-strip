#include "mqtt.h"

WiFiClient wifiClient;
PubSubClient client(MQTT_SERVER, 1883, onMQTTMessage, wifiClient);

char mqttMessageBuffer[50];

void onMQTTMessage(char* topic, byte* payload, unsigned int length) {
  int i = 0;
  if (String(topic).equals(MQTT_SUBSCRIBE)) {
    for (int i=0;i<length;i++) {
      mqttMessageBuffer[i] = payload[i];
    }
    mqttMessageBuffer[length] = '\0';
    //Serial.println("Message arrived:  topic: " + String(topic));
    //Serial.println("Length: " + String(length, DEC));
    String stringPayload = String((char *)mqttMessageBuffer);
    if (stringPayload.equals((char*)"ON")) {
      // state to on
      Serial.println("Turning on");
      setLedState(1);
      client.publish(MQTT_PUBLISH_STATE, "ON");
    } else if (stringPayload.equals((char*)"OFF")) {
      setLedState(0);
      Serial.println("Turning off");
      client.publish(MQTT_PUBLISH_STATE, "OFF");
    } else if (stringPayload.equals((char*)"chroma")) {
      setChroma();
    } else if (length <= 3) {
      int brightness = stringPayload.toInt();
      Serial.print("Setting brightness to ");
      Serial.println(brightness);
      setLedState(1);
      setLedBrightness(brightness);
      if ((int)brightness > 0) {
        client.publish(MQTT_PUBLISH_STATE, "ON");
      } else {
        client.publish(MQTT_PUBLISH_STATE, "OFF");
      }
      client.publish(MQTT_PUBLISH_BRIGHTNESS, mqttMessageBuffer);
    } else {
      char color = 'h';
      char pos = 0;
      char hs[] = {0,0,0,0}; // hue
      char ss[] = {0,0,0,0}; // sat
      char vs[] = {0,0,0,0}; // val
      for (int i=0;i<length;i++) {
        if (payload[i] == ',') {
          color = color == 'h' ? 's' :
                  color == 's' ? 'v' : 'h';
          pos = 0;
        } else {
          if (color == 'h') hs[pos] = payload[i];
          if (color == 's') ss[pos] = payload[i];
          if (color == 'v') vs[pos] = payload[i];
          pos += 1;
        }
      }
      int h = String(hs).toInt();
      int s = String(ss).toInt();
      int v = String(vs).toInt();
      setColor(h,s,v);
      setLedState(1);
      if (v > 0) {
        client.publish(MQTT_PUBLISH_STATE, "ON");
      } else {
        client.publish(MQTT_PUBLISH_STATE, "OFF");
      }
      client.publish(MQTT_PUBLISH_BRIGHTNESS, vs);
      client.publish(MQTT_PUBLISH_COLOR, mqttMessageBuffer);
    }
    Serial.println("Message: " + stringPayload);
  }
}


void connecttoMQTT() {
  if (!client.connected()) {
    Serial.print("Connecting to ");
    Serial.print(MQTT_SERVER);
    Serial.println("");
    char myTopic[100];

    int is_conn = 0;
    while (is_conn == 0) {
      if (client.connect((char*) MQTT_CLIENT_NAME, MQTT_USER, MQTT_PASS)) {
        Serial.println("Connected to MQTT broker");
        if (client.subscribe(MQTT_SUBSCRIBE)) {
          Serial.print("Successfully subscribed to ");
          Serial.println(MQTT_SUBSCRIBE);
        }
        is_conn = 1;
      }
      else {
        Serial.print("MQTT connect failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
  }
}

void mqttLoop() {
  connecttoMQTT();
  client.loop();
}
