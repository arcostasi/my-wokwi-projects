/*
  Display Letreiro com Franzininho e MAX7219 4 em 1
  by Anderson Costa with ❤ for the Wokwi community
  Visit https:// wokwi.com to learn about the Wokwi Simulator
  Visit https:// franzininho.com.br to learn about the Franzininho
  Credits: https://www.youtube.com/watch?v=bcdMu1MvvLE
*/
#include <dht.h>
#include <LedControl.h>
#include <TinyDebug.h>
#include <TinyWireM.h>
#include "font5x7.h"

#define DEBUG       false
#define DS1307_ADDR 0x68       // Endereço I2C do DS1307
#define DHT22_PIN   1          // Define o pino do sensor de temperatura e humidade
#define CLK         3          // Define o pino de clock
#define DIN         4          // Define o pino de entrada de dados
#define CS          5          // Define o pino seletor de transferência

const uint8_t numDevices = 4;  // Define o número de MAX7219 utilizados
const long scrollDelay = 40;   // Ajusta a velocidade de rolagem

const unsigned char scrollText[] PROGMEM = {
  "         Seja Bem-vindo         Welcome         Bienvenue          Bienvenidos          "
};

unsigned long buffer[12] = {0};
unsigned char scrollData[50];

uint8_t wireRet = 0;
uint8_t seconds;
uint8_t minutes;
uint8_t hours;
uint8_t dayOfWeek;
uint8_t day;
uint8_t month;
uint16_t year;

dht DHT;

LedControl lc = LedControl(DIN, CLK, CS, numDevices);

volatile float temperature;
volatile float humidity;

volatile uint16_t timer1 = 1000;

ISR(TIMER0_COMPA_vect) {        // Requisição dos dados por interrupção
  // Verifica se chegou em zero
  if (!timer1) {
    cli();
    timer1 = 1000;
    sei();

    // Retorna os valores da data e hora
    requestDateTime();

    // Retorna os valores da humidade e temperatura
    DHT.read22(DHT22_PIN);

    // Atualiza a temperatura
    temperature = DHT.temperature;

    // Atualiza a humidade
    humidity = DHT.humidity;

    // Corrige a temperatura negativa
    if (temperature < 0) {
      temperature = -((uint8_t)temperature - 52);
    }

#if DEBUG
    // Imprime no monitor serial a data e hora, temperatura e humidade
    Debug.print(formatDate(day, month, year));
    Debug.print(" ");
    Debug.print(formatTime(hours, minutes));
    Debug.print(" ");
    Debug.print(temperature, 1);
    Debug.print("°C ");
    Debug.print(humidity, 1);
    Debug.println("%");
#endif
  }

  // Se tem valor, decrementa
  if (timer1)
    timer1--;
}

void setup() {
#if DEBUG
  Debug.begin();
#endif
  TinyWireM.begin();

  // Configura a porta do sensor como entrada
  pinMode(DHT22_PIN, INPUT);

  TCCR0A = (1 << WGM01);        // Define o modo CTC
  OCR0A = 125;                  // Define ORC0A

  TIMSK = (1 << OCIE0A);        // Timer/Counter0 Output Compare Match A Interrupt Enable

  TCCR0B |= (1 << CS01);        // Ajusta o relógio do prescale em 1/64
  TCCR0B |= (1 << CS00);
  sei();                        // Habilita a interrupção

  delay(1000);

  for (uint8_t x = 0; x < lc.getDeviceCount(); x++) {
    lc.shutdown(x, false);      // Configura o MAX72XX para o modo de economia de energia
    lc.setIntensity(x, 8);      // Define o brilho para o valor padrão
    lc.clearDisplay(x);         // Limpa o display
  }
}

void loop() {
  scrollMessage(scrollText, true);
  // Formata os dados do RTC e DHT22
  sprintf(scrollData, "%s   %s   %s   %s  ",
          formatDate(day, month, year), formatTime(hours, minutes),
          formatTemperature(temperature), formatHumidity(humidity));
  scrollMessage(scrollData, false);
}

void requestDateTime() {
  TinyWireM.beginTransmission(DS1307_ADDR);      // Redefine endereço de dados do DS1307
  TinyWireM.send(0x00);
  wireRet = TinyWireM.endTransmission();

#if DEBUG
  if (wireRet) {                                 // Reporta qualquer erro de envio
    Debug.print("SendError: ");
    Debug.println(wireRet, DEC);
  }
#endif

  // Solicita 7 bytes do DS1307
  TinyWireM.requestFrom(DS1307_ADDR, 7);
  seconds = bcdToDec(TinyWireM.receive() & 0x7F);
  minutes = bcdToDec(TinyWireM.receive());
  hours = bcdToDec(TinyWireM.receive());
  dayOfWeek = TinyWireM.receive();
  day = bcdToDec(TinyWireM.receive());
  month = bcdToDec(TinyWireM.receive());
  year = bcdToDec(TinyWireM.receive()) + 2000;
}

// Rolagem de texto
void scrollMessage(char *messageString, bool isProgMem) {
  uint8_t counter = 0;
  char myChar = 0;
  do {
    // Verifica se a mensagem está na memória flash
    if (isProgMem)
      myChar = pgm_read_byte_near(messageString + counter);
    else
      myChar = messageString[counter];
    // Se existe um caractere válido, continua lendo do buffer
    if (myChar != '\0') {
      loadBuffer(myChar);
    }
    counter++;
  }
  while (myChar != '\0');
}

// Carrega o caractere no buffer de rolagem
void loadBuffer(char ascii) {
  if (ascii >= 0x20 && ascii <= 0x7f) {
    for (uint8_t a = 0; a < 7; a++) {         // Repete 7 vezes para uma fonte 5x7
      // Índice na tabela de caracteres para obter a linha de dados
      unsigned long c = pgm_read_byte_near(font5x7 + ((ascii - 0x20) * 8) + a);
      unsigned long x = buffer[a * 2];        // Carregar o buffer de rolagem atual
      x = x | c;                              // OU o novo caractere no final do atual
      buffer[a * 2] = x;                      // Armazena no buffer
    }

    // Índice na tabela de caracteres para ajustar o espaçamento entre caracteres (kerning)
    uint8_t count = pgm_read_byte_near(font5x7 + ((ascii - 0x20) * 8) + 7);

    for (uint8_t x = 0; x < count; x++) {
      rotateBuffer();
      printBuffer();
      delay(scrollDelay);
    }
  }
}

// Rotaciona o buffer
void rotateBuffer() {
  for (int a = 0; a < 7; a++) {             // Repete 7 vezes para uma fonte 5x7
    unsigned long x = buffer[a * 2];        // Obtem a entrada baixa do buffer
    uint8_t b = bitRead(x, 31);             // Copia o bit alto que se perde na rotação
    x = x << 1;                             // Rotaciona um bit para a esquerda
    buffer[a * 2] = x;                      // Armazena o nível baixo no buffer
    x = buffer[a * 2 + 1];                  // Obtem a entrada alta do buffer
    x = x << 1;                             // Rotaciona um bit para a esquerda
    bitWrite(x, 0, b);                      // Armazena o bit salvo
    buffer[a * 2 + 1] = x;                  // Armazena o nível alto no buffer
  }
}

// Buffer de exibição na matriz de LED
void printBuffer() {
  for (uint8_t a = 0; a < 7; a++) {         // Repete 7 vezes para uma fonte 5x7
    // Obtem a entrada do buffer
    unsigned long x = buffer[a * 2 + 1];
    uint8_t y = x;                          // Máscara do primeiro caractere
    lc.setRow(0, 6 - a, y);                 // Envia a linha para o chip MAX7219
    x = buffer[a * 2];                      // Obtem a entrada baixa do buffer
    y = (x >> 24);                          // Máscara do segundo caractere
    lc.setRow(1, 6 - a, y);                 // Envia a linha para o chip MAX7219
    y = (x >> 16);                          // Máscara do terceiro caractere
    lc.setRow(2, 6 - a, y);                 // Envia a linha para o chip MAX7219
    y = (x >> 8);                           // Máscara do quarto caractere
    lc.setRow(3, 6 - a, y);                 // Envia a linha para o chip MAX7219
  }
}

// Converte decimais codificados em binários para números decimais normais
uint8_t bcdToDec(uint8_t value) {
  return ((value / 16 * 10) + (value % 16));
}

// Formata os dados da data do RTC (dd/mm/yyyy)
char* formatDate(int8_t d, int8_t m, int16_t y) {
  static char date[11] = {'\0'};
  snprintf(date, sizeof(date), "\%02d/\%02d/\%04d", d, m, y);
  return date;
}

// Formata os dados da hora do RTC (hh:mm:ss)
char* formatTime(int8_t hour, int8_t minute) {
  static char time[6] = {'\0'};
  snprintf(time, sizeof(time), "\%02d:\%02d", hour, minute);
  return time;
}

// Formata os dados da temperatura (Temp. 00.0C)
char* formatTemperature(float temp) {
  static char temp_char[13] = {'\0'};
  snprintf(temp_char, sizeof(temp_char), "Temp. %02d.%01dC",
           (int)temp, (int)(temp * 100) % 100);
  return temp_char;
}

// Formata os dados da humidade (HR 00.0%)
char* formatHumidity(float hum) {
  static char hum_char[11] = {'\0'};
  snprintf(hum_char, sizeof(hum_char), "HR %02d.%01d%%",
           (int)hum, (int)(hum * 100) % 100);
  return hum_char;
}
