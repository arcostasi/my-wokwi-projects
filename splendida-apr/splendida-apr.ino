/*
  Splendida AquaPurple Rotate
  Author: Anderson Costa
  Date: 2021-03-06
*/
#include <FastLED.h>

// LED settings
#define NUM_LEDS 256
#define CHIPSET  WS2812B
#define DATA_PIN 3

// LED brightness
#define BRIGHTNESS 255

CRGB leds[NUM_LEDS];

CRGB colorBlack = CRGB::Black;
CRGB colorAqua = CHSV(HUE_AQUA, 255, 255);
CRGB colorPurple = CHSV(HUE_PURPLE, 255, 255);

CRGBPalette16 currentPalette(colorBlack);
CRGBPalette16 targetPalette = CRGBPalette16(
                                colorAqua, colorAqua,
                                colorBlack, colorBlack,
                                colorPurple, colorPurple,
                                colorBlack, colorBlack,
                                colorAqua, colorAqua,
                                colorBlack, colorBlack,
                                colorPurple, colorPurple,
                                colorBlack, colorBlack
                              );

TBlendType currentBlending = LINEARBLEND; // or NOBLEND

bool switchPallete = false;

void animateLEDsFromPalette(uint8_t colorIndex, uint8_t speed, uint8_t slice) {
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t colorIndex = (millis() / speed) + (i * slice);
    if (colorIndex > 128) colorIndex = 0;
    leds[i] = ColorFromPalette(currentPalette, colorIndex, 255);
  }
}

void setup() {
  FastLED.addLeds<CHIPSET, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  static uint8_t startIndex = 0;
  static uint8_t sliceIndex = 1;

  EVERY_N_MILLISECONDS(60) {
    nblendPaletteTowardPalette(currentPalette, targetPalette, 16);
  }

  EVERY_N_MILLISECONDS(20) {
    animateLEDsFromPalette(startIndex++, 1, sliceIndex);
  }

  EVERY_N_SECONDS(10) {
    sliceIndex++;
    if (sliceIndex > 6) sliceIndex = 1;
  }

  FastLED.show();
}
