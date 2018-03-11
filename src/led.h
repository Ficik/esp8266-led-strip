// #define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ALLOW_INTERRUPTS 0

#include "config.h"
#include "FastLED.h"


void ledSetup();
void ledLoop();
void setColor(int r, int g, int b);
void setLedState(int val);
void setLedBrightness(int val);
void setChroma();