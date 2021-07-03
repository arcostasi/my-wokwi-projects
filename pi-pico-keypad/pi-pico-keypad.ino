#include <Keypad.h>

const uint8_t LEDS = 12;
const uint8_t ROWS = 4;
const uint8_t COLS = 4;

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

// Pins connected to LED1, LED2, LED3, ...LED12
uint8_t ledPins[LEDS] = { 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 28, 27 };
uint8_t rowPins[ROWS] = { 26, 22, 21, 20 }; // Pins connected to R1, R2, R3, R4
uint8_t colPins[COLS] = { 19, 18, 17, 16 }; // Pins connected to C1, C2, C3, C4

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

bool state[LEDS] = { false };

void toggleWrite(uint8_t pos)
{
  bool value = state[pos - 1] = !state[pos - 1];
  digitalWrite(ledPins[pos - 1], value ? HIGH : LOW);
}

void rangeWrite(uint8_t i, uint8_t e, bool high)
{
  for (uint8_t l = i; l <= e; l++) {
    state[l - 1] = high;
    digitalWrite(ledPins[l - 1], high ? HIGH : LOW);
  }
}

void setup() {
  for (uint8_t l = 0; l < LEDS; l++) {
    pinMode(ledPins[l], OUTPUT);
    digitalWrite(ledPins[l], LOW);
  }
}

void loop() {
  char key = keypad.getKey();

  if (key != NO_KEY) {
    switch (key) {
      case 'A': toggleWrite(9);
        break;
      case 'B': toggleWrite(10);
        break;
      case 'C': toggleWrite(11);
        break;
      case 'D': toggleWrite(12);
        break;
      case '0': rangeWrite(1, 8, false);
        break;
      case '9': rangeWrite(1, 8, true);
        break;
      case '*': rangeWrite(9, 12, true);
        break;
      case '#': rangeWrite(9, 12, false);
        break;
      default: toggleWrite(atoi(&key));
        break;
    }
  }
}
