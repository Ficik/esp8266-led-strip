#include "led.h"
#define COLOR_TAIL 16
#define COLOR_STEP 16 // COLOR_TAIL * COLOR_STEP >= 255

CHSV currentColor = CHSV(0, 0, 255);
CHSV lastColor = currentColor;
CHSV previousColor = CHSV(0, 0, 0);
char state = 1;
int changePosition = 0;
int isChromaOn = 1;
int hue = 0;

CRGBArray<NUM_LEDS> leds;

// ws2811

void approxColor(int i, CHSV lastColor, CHSV currentColor, int step) {
    fract8 frac = min(255, COLOR_STEP * (step + 1));
    CHSV color = CHSV(0, 0, 0);
    color.hue = lerp8by8(lastColor.hue, currentColor.hue, frac);
    color.sat = lerp8by8(lastColor.sat, currentColor.sat, frac);
    color.val = lerp8by8(lastColor.val, currentColor.val, frac);
    //  Serial.print("Color ");
    // Serial.print(color.hue);
    // Serial.print(", ");
    // Serial.print(color.sat);
    // Serial.print(", ");
    // Serial.print(color.val);
    // Serial.print(" step ");
    // Serial.print(step);
    //  Serial.print(" frac ");
    //  Serial.print(frac);
    //  Serial.print(" to ");
    //  Serial.println(i);
    leds[i] = color;
}

void ledSetup() {
    FastLED.addLeds<WS2811, LED_DATA_PIN, BRG>(leds, NUM_LEDS)
        .setCorrection(TypicalSMD5050);
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
};

void changeColor() {
    if (changePosition == (NUM_LEDS/2) + 1 + COLOR_TAIL) {
        previousColor = currentColor;
        return;
    }
    int middle = NUM_LEDS / 2;
    for (int i=0;i < min(COLOR_TAIL, changePosition+1); i++)  {
        approxColor(min(NUM_LEDS - 1, middle + changePosition - i ), previousColor, currentColor, i);
        approxColor(max(0, middle - changePosition + i), previousColor, currentColor, i);
    }
    changePosition += 1;
    FastLED.setTemperature(Candle);
    FastLED.show();
    FastLED.delay(15);
}

void ledLoop() {
    if (isChromaOn) {
        leds.fill_rainbow(hue, 255 / NUM_LEDS);
        hue += 1;
        FastLED.delay(5);
        FastLED.show();
    } else {
        changeColor();
    }
}

void setLedState(int val) {
    state = val;
    if (state == 0) {
        currentColor = CHSV(currentColor.hue, currentColor.sat, 0);
    }
    if (state == 1) {
        currentColor = lastColor;
    }
    changePosition = 0;
    isChromaOn = 0;
}

void setColor(int h, int s, int v) {
    currentColor = CHSV((uint8_t)(h * 255/360.0), (uint8_t)(s * 255.0/100), (uint8_t)(v * 255.0/100));
    lastColor = currentColor;
    changePosition = 0;
    isChromaOn = 0;
}

void setLedBrightness(int val) {
    currentColor = CHSV(currentColor.hue, currentColor.sat, (uint8_t)(val * 255.0/100));
    if (currentColor.value > 0) {
        lastColor = currentColor;
    }
    changePosition = 0;
    isChromaOn = 0;
}

void setChroma() {
    isChromaOn = 1;
}