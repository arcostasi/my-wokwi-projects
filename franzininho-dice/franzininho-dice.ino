/*
  Dado eletrônico com Franzininho DIY e um módulo matriz de LED 8x8 com MAX7219
  by Anderson Costa with ❤ for the Wokwi community
  Visit https://wokwi.com to learn about the Wokwi Simulator
  Visit https://franzininho.com.br to learn about the Franzininho
*/

#include <LedControl.h>
#include <TinyDebug.h>

#define CLK_PIN PB0
#define CS_PIN  PB1
#define DIN_PIN PB2
#define BTN_PIN PB3
#define RND_PIN A0

#define MAX_SEG 1

// Define os padrões binários para cada número do dado (1 a 6) e nenhum
uint8_t dice[7][8] = {
  { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 }, // Zero
  { B00000000, B00000000, B00000000, B00011000, B00011000, B00000000, B00000000, B00000000 }, // Um
  { B00000000, B01100000, B01100000, B00000000, B00000000, B00000110, B00000110, B00000000 }, // Dois
  { B11000000, B11000000, B00000000, B00011000, B00011000, B00000000, B00000011, B00000011 }, // Três
  { B00000000, B01100110, B01100110, B00000000, B00000000, B01100110, B01100110, B00000000 }, // Quatro
  { B11000011, B11000011, B00000000, B00011000, B00011000, B00000000, B11000011, B11000011 }, // Cinco
  { B00000000, B11011011, B11011011, B00000000, B00000000, B11011011, B11011011, B00000000 }  // Seis
};

// Define os pinos da matriz de LEDs
LedControl led = LedControl(DIN_PIN, CLK_PIN, CS_PIN, MAX_SEG); // MAX7219

bool waitDice = false;
uint8_t numberDice = 0;

void setup() {
  Debug.begin();
  delay(100);
  Debug.println("Clique no botão para jogar o dado...");

  pinMode(BTN_PIN, INPUT_PULLUP);

  led.shutdown(0, false);

  // Para o random não repetir a sequência
  randomSeed(analogRead(RND_PIN));

  // Ajusta o brilho da matriz de LEDs para intensidade média
  led.setIntensity(0, 7);

  // Limpa o display
  led.clearDisplay(0);
}

void loop() {
  // Verifica o pressionamento do botão
  if (!digitalRead(BTN_PIN) && !waitDice) {
    waitDice = true; // Trava o botão até o final da rolagem
    rollsDice();     // Rola o dado
  }
}

void showDice(uint8_t number) {
  // Percorre a matriz e seta o número
  for (uint8_t i = 0; i <= 7; i++) {
    led.setRow(0, i, dice[number][i]);
  }
}

void rollsDice() {
  uint8_t rollingTime = random(10, 15);

  for (uint8_t i = 0; i < rollingTime; i++) {
    // A variável number vai assumir um valor entre 1 e 6
    numberDice = random(1, 7);
    showDice(numberDice);
    Debug.print(numberDice);
    if (i < rollingTime - 1)
      Debug.print(", ");
    else
      Debug.print(" ");
    delay(100 + i * 10);
  }

  showDice(0);
  delay(500);

  showDice(numberDice);
  delay(250);

  Debug.print("=> ");
  Debug.println(numberDice);

  waitDice = false; // Libera o botão para jogar novamente
}
