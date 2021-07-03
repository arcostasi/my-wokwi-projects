/*
  Temperature and Humidity using Franzininho, DHT22 and SSD1306
  by Anderson Costa with ❤ for the Wokwi community
  Visit https://wokwi.com to learn about the Wokwi Simulator
  Visit https://franzininho.com.br to learn about the Franzininho
*/
#include <dht.h>
#include <TinyWireM.h>
#include <Tiny4kOLED.h>

#define DHT22_PIN PB3

const uint8_t img_thermometer[] PROGMEM = {
  0x00, 0xfe, 0x03, 0xfe, 0x50,
  0x00, 0xff, 0x00, 0xff, 0x55,
  0x60, 0x9f, 0x80, 0x9f, 0x65,
};

dht DHT;

void setup() {
  pinMode(DHT22_PIN, INPUT);

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

  // Retorna os valores da humidade e temperatura
  DHT.read22(DHT22_PIN);

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
  oled.setCursor(3, 2);
  oled.print(F("temperatura|humidade"));

  oled.bitmap(105, 4, 110, 7, img_thermometer);
  oled.setCursor(57, 4);
  oled.print(F("24.0C"));
  oled.setCursor(57, 5);
  oled.print(F("40.0%"));
}

void updateDisplay()
{
  // Atualiza a temperatura
  float temperature = getTemperature();

  // Posiciona o cursor
  oled.setCursor(45, 4);

  // Fix negative temperature
  if (temperature < 0) {
    oled.print("-");
    temperature = (uint8_t)temperature - 52;
  }

  // Imprime a temperatura no display
  oled.print(temperature, 1);
  oled.print("C  ");

  // Atualiza a humidade
  float humidity = getHumidity();

  // Posiciona o cursor
  oled.setCursor(45, 5);

  // Imprime a humidade no display
  oled.print(humidity, 1);
  oled.print("%  ");

  oled.bitmap(105, 4, 110, 7, img_thermometer);
}

void splash()
{
  oled.clear();
  oled.setCursor(7, 1);
  oled.print(F("Franzininho+SSD1306"));

  oled.setCursor(42, 3);
  oled.print(F("Exemplo"));

  oled.setCursor(48, 5);
  oled.print(F("DHT22"));

  oled.setCursor(35, 7);
  oled.print(F("wokwi.com"));
}

float getTemperature()
{
  return DHT.temperature;
}

float getHumidity()
{
  return DHT.humidity;
}
