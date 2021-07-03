/*
  NTC Temperature Sensor using Franzininho, NTC and SSD1306
  by Anderson Costa with ❤ for the Wokwi community
  Visit https://wokwi.com to learn about the Wokwi Simulator
  Visit https://franzininho.com.br to learn about the Franzininho
*/
#include <TinyWireM.h>
#include <Tiny4kOLED.h>

#define NTC_PIN A3

const float BETA = 3950; // Deve corresponder ao coeficiente beta do termistor

const uint8_t img_thermometer[] PROGMEM = {
  0x00, 0xfe, 0x03, 0xfe, 0x50,
  0x00, 0xff, 0x00, 0xff, 0x55,
  0x60, 0x9f, 0x80, 0x9f, 0x65,
};

void setup() {
  // Inicializa o display oled
  oled.begin();

  // Duas fontes são fornecidas com esta biblioteca, FONT6X8 e FONT8X16
  oled.setFont(FONT6X8);

  // Limpa toda a memória do display
  oled.clear();

  splash();
  delay(3000);

  initDisplay();
}

void loop() {
  static unsigned long startTime = 0;
  unsigned long currentTime;

  // Retorna o número de milissegundos passados
  // desde que começou a executar o programa atual
  currentTime = millis();

  // Verifica se passou 1 segundo
  if ((currentTime - startTime) >= 1000) {
    // Reseta o temporizador
    startTime = currentTime;
    // Atualiza o display
    updateDisplay();
  }
}

void initDisplay()
{
  oled.clear();
  oled.begin();

  oled.setCursor(7, 1);
  oled.print(F("Franzininho+SSD1306"));
  oled.setCursor(12, 2);
  oled.print(F("sensor temperatura"));

  oled.bitmap(105, 4, 110, 7, img_thermometer);
  oled.setCursor(46, 4);
  oled.print(F("0.0C"));
}

void updateDisplay()
{
  // Atualiza a temperatura
  float temperature = getTemperature();

  // Posiciona o cursor
  oled.setCursor(46, 4);

  // Imprime a temperatura no display
  oled.print(temperature, 1);
  oled.print("C   ");

  // Posiciona o cursor
  oled.setCursor(32, 5);
  oled.print("          ");

  oled.bitmap(105, 4, 110, 7, img_thermometer);
}

void splash()
{
  oled.clear();
  oled.setCursor(7, 1);
  oled.print(F("Franzininho+SSD1306"));

  oled.setCursor(42, 3);
  oled.print(F("Exemplo"));

  oled.setCursor(32, 5);
  oled.print(F("NTC Sensor"));

  oled.setCursor(35, 7);
  oled.print(F("wokwi.com"));
}

float getTemperature()
{
  int analogValue = analogRead(NTC_PIN);
  float celsius = 1 / (log(1 / ((1023.0 / analogValue) - 1)) / BETA + 1.0 / 298.15) - 273.15;
  return celsius;
}
