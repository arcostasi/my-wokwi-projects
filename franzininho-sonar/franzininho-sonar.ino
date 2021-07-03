/*
  Ultrasonic Sensor using Franzininho, HC-SR04 and 74HC595
  by Anderson Costa with ❤ for the Wokwi community
  Visit https://wokwi.com to learn about the Wokwi Simulator
  Visit https://franzininho.com.br to learn about the Franzininho
*/

#define TRIG_PIN  A0
#define ECHO_PIN  A3

#define DATA_PIN  0  // Pino conectado ao DS do 74HC595
#define LATCH_PIN 1  // Pino conectado ao STCP do 74HC595
#define CLOCK_PIN 2  // Pino conectado ao SHCP do 74HC595

// Quantos registros de deslocamento
#define NUM_SHIFT_REGS 1

const uint8_t numOfRegisterPins = NUM_SHIFT_REGS * 8;

bool registers[numOfRegisterPins];

uint16_t duration = 0;
uint32_t interval = 0;

float distance = 0;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  clearRegisters();
  writeRegisters();
}

void loop() {
  // Verifica a cada 100 milissegundos
  if ((millis() - interval) >= 100) {
    interval = millis();

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Tempo de leitura do trigger ao echo
    duration = pulseIn(ECHO_PIN, HIGH);

    // Calcula a distância
    distance = (duration / 2) / 29;

    // Retorna o valor da distância e converte para o número de bits do registrador
    uint8_t value = map(distance, 2, 400, 0, numOfRegisterPins);

    for (uint8_t i = 0; i < numOfRegisterPins; i++) {
      if (i >= value) {
        setRegisterPin(i, HIGH);
      } else {
        setRegisterPin(i, LOW);
      }
    }
    writeRegisters();
  }
}

void clearRegisters() {
  // Coloca em nível baixo todos os pinos do registrador
  for (int i = numOfRegisterPins - 1; i >= 0; i--) {
    registers[i] = LOW;
  }
}

void setRegisterPin(int index, int value) {
  // Define um pino individual como HIGH ou LOW
  registers[index] = value;
}

void writeRegisters() {
  // Define e exibe os registros
  digitalWrite(LATCH_PIN, LOW);

  for (int i = numOfRegisterPins - 1; i >= 0; i--) {
    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(DATA_PIN, registers[i]);
    digitalWrite(CLOCK_PIN, HIGH);
  }

  digitalWrite(LATCH_PIN, HIGH);
}
