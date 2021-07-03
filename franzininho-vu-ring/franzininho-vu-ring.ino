/*
  VU ring with sensitivity adjustment in Franzinho DIY
  by Anderson Costa with ❤ for the Wokwi community
  Visit https://wokwi.com to learn about the Wokwi Simulator
  Visit https://franzininho.com.br to learn about the Franzininho
*/
#include <Adafruit_NeoPixel.h>

#define PIN_MIC    A3
#define PIN_POT    A0
#define PIN_RING   PB4
#define NUMLEDS    18
#define OFFSET_MIN 450
#define OFFSET_MAX 500

Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUMLEDS, PIN_RING, NEO_GRB + NEO_KHZ800);

// Mapa de cores (RGB) da intensidade
uint8_t rgb_int[NUMLEDS][5] = {
  {15,   0,   0,   0, 0},
  {20,   1,   0, 255, 0},
  {20,   2,   0, 255, 0},
  {25,   3,  50, 255, 0},
  {30,   4, 100, 255, 0},
  {35,   5, 120, 255, 0},
  {45,   6, 180, 200, 0},
  {45,   7, 200, 180, 0},
  {50,   8, 200, 150, 0},
  {55,   9, 255, 120, 0},
  {60,  10, 255, 100, 0},
  {65,  11, 255,  80, 0},
  {70,  12, 255,  50, 0},
  {75,  13, 255,   0, 0},
  {80,  14, 255,   0, 0},
  {85,  15, 255,   0, 0},
  {90,  16, 255,   0, 0},
  {100, 17, 255,   0, 0},
};

void setup() {
  pinMode(PIN_MIC, INPUT);
  pinMode(PIN_POT, INPUT);
  leds.begin();
  leds.setBrightness(200);
}

void loop() {
  uint16_t intens = analogRead(PIN_MIC);
  uint16_t offset = map(analogRead(PIN_POT), 0, 1023, OFFSET_MIN, OFFSET_MAX);

  clear();

  // Percorre os LEDs
  for (uint8_t i = 0; i < NUMLEDS; i++) {
    // Altera a cor dos LEDs conforme a intensidade do microfone
    led(intens, offset + rgb_int[i][0], rgb_int[i][1],
        rgb_int[i][2], rgb_int[i][3], rgb_int[i][4]);
  }
}

void led(uint16_t intensity, uint16_t offset, uint8_t nled, uint8_t r, uint8_t g, uint8_t b) {
  if (intensity > offset) {
    leds.setPixelColor(nled, leds.Color(r, g, b));
    leds.show();
    delay(1);
  }
}

void clear() {
  for (uint8_t x = 0; x < NUMLEDS; x++) {
    leds.setPixelColor(x, leds.Color(0, 0, 0));
  }
}
