#include <Wire.h>
#include <LiquidCrystal.h>

#define DS1307_ADDRESS 0x68

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

// Arrays for creating segments and customizing numbers
byte segment[8][8] =
{
  { B01111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111 },
  { B11111,  B11111,  B11111,  B00000,  B00000,  B00000,  B00000,  B00000 },
  { B11110,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111 },
  { B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B01111 },
  { B00000,  B00000,  B00000,  B00000,  B00000,  B11111,  B11111,  B11111 },
  { B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11110 },
  { B11111,  B11111,  B11111,  B00000,  B00000,  B00000,  B11111,  B11111 },
  { B11111,  B00000,  B00000,  B00000,  B00000,  B11111,  B11111,  B11111 }
};

byte zero = 0x00;

int col = 0;

int number, lastMin1, lastMin2, lastHour1, lastHour2;

void setup()
{
  Wire.begin();
  // Initializes the LCD
  lcd.begin(16, 2);

  // Associates each created segment with a number
  for (byte i = 0; i < 8; i++) {
    lcd.createChar(i, segment[i]);
  }

  // The line below can be removed after setting the date and time
  // setDateTime();
}

void loop()
{
  // Extracts the time from the DS1307 and shows it on the LCD
  showClock();
  delay(1000);
}

byte convertToBCD(byte val) { // Converts the decimal number to BCD
  return ( (val / 10 * 16) + (val % 10) );
}

byte convertToDecimal(byte val) { // Converts from BCD to decimal
  return ( (val / 16 * 10) + (val % 16) );
}

void digit0() // Set the segments to the number 0
{
  lcd.setCursor(col, 0); // Select the top line
  lcd.write((byte)0);  // Segment 0 selected
  lcd.write(1);  // Segment 1 selected
  lcd.write(2);
  lcd.setCursor(col, 1); // Select the bottom line
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
}

void digit1() // Set the segments to the number 1
{
  lcd.setCursor(col, 0);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(col + 1, 1);
  lcd.write(5);
}

void digit2() // Set the segments to the number 2
{
  lcd.setCursor(col, 0);
  lcd.write(6);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(col, 1);
  lcd.write(3);
  lcd.write(7);
  lcd.write(7);
}

void digit3()  // Set the segments to the number 3
{
  lcd.setCursor(col, 0);
  lcd.write(6);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(col, 1);
  lcd.write(7);
  lcd.write(7);
  lcd.write(5);
}

void digit4()  // Set the segments to the number 4
{
  lcd.setCursor(col, 0);
  lcd.write(3);
  lcd.write(4);
  lcd.write(2);
  lcd.setCursor(col + 2, 1);
  lcd.write(5);
}

void digit5()  // Set the segments to the number 5
{
  lcd.setCursor(col, 0);
  lcd.write((byte)0);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(col, 1);
  lcd.write(7);
  lcd.write(7);
  lcd.write(5);
}

void digit6()  // Set the segments to the number 6
{
  lcd.setCursor(col, 0);
  lcd.write((byte)0);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(col, 1);
  lcd.write(3);
  lcd.write(7);
  lcd.write(5);
}

void digit7() // Set the segments to the number 7
{
  lcd.setCursor(col, 0);
  lcd.write(1);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(col + 1, 1);
  lcd.write((byte)0);
}

void digit8()  // Set the segments to the number 8
{
  lcd.setCursor(col, 0);
  lcd.write((byte)0);
  lcd.write((byte)6);
  lcd.write(2);
  lcd.setCursor(col, 1);
  lcd.write(3);
  lcd.write(7);
  lcd.write(5);
}

void digit9() // Set the segments to the number 9
{
  lcd.setCursor(col, 0);
  lcd.write((byte)0);
  lcd.write((byte)6);
  lcd.write((byte)2);
  lcd.setCursor(col + 2, 1);
  lcd.write((byte)5);
}

void showDigit() // Shows the number in the position defined by "X"
{
  switch (number) {
    case 0: digit0();
      break;
    case 1: digit1();
      break;
    case 2: digit2();
      break;
    case 3: digit3();
      break;
    case 4: digit4();
      break;
    case 5: digit5();
      break;
    case 6: digit6();
      break;
    case 7: digit7();
      break;
    case 8: digit8();
      break;
    case 9: digit9();
      break;
  }
}

void showClock()
{
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDRESS, 7);

  int seconds = convertToDecimal(Wire.read());
  int minutes = convertToDecimal(Wire.read());
  int hours = convertToDecimal(Wire.read() & 0b111111);
  int dayWeek = convertToDecimal(Wire.read());
  int dayMonth = convertToDecimal(Wire.read());
  int month = convertToDecimal(Wire.read());
  int year = convertToDecimal(Wire.read());

  if (seconds & 1) {
    lcd.setCursor(8, 0);
    lcd.print(".");
    lcd.setCursor(8, 1);
    lcd.print(".");
  } else {
    lcd.setCursor(8, 0);
    lcd.print(" ");
    lcd.setCursor(8, 1);
    lcd.print(" ");
  }

  col = 0;
  number = hours / 10;

  // Conditional to avoid overlapping characters
  if (lastHour1 != minutes % 10) {
    lcd.setCursor(0, 0);
    lcd.print("   ");
    lcd.setCursor(0, 1);
    lcd.print("   ");
    lastHour1 = minutes % 10;
  }

  showDigit(); // Show the customized number on the display

  col = 4;
  number = hours % 10;

  if (lastHour2 != minutes % 10) {
    lcd.setCursor(4, 0);
    lcd.print("   ");
    lcd.setCursor(4, 1);
    lcd.print("   ");
    lastHour2 = minutes % 10;
  }

  showDigit();

  col = 9;
  number = minutes / 10;

  if (lastMin1 != minutes % 10) {
    lcd.setCursor(9, 0);
    lcd.print("   ");
    lcd.setCursor(9, 1);
    lcd.print("   ");
    lastMin1 = minutes % 10;
  }

  showDigit();

  col = 13;
  number = minutes % 10;

  if (lastMin2 != minutes % 10) {
    lcd.setCursor(13, 0);
    lcd.print("   ");
    lcd.setCursor(13, 1);
    lcd.print("   ");
    lastMin2 = minutes % 10;
  }

  showDigit();
}

void setDateTime()   // Set the date and time of the DS1307
{
  byte seconds = 00; // Values ​​from 0 to 59
  byte minutes = 00; // Values ​​from 0 to 59
  byte hours = 00; // Values ​​from 0 to 23
  byte dayWeek = 1; // Values ​​from 0 to 6, where 0 = Domgino, 1 = Monday
  byte dayMonth = 17; // Values ​​from 1 to 31
  byte month = 5; // Values ​​from 1 to 12
  byte year = 21; // Values ​​from 0 to 99

  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero); // Stop at CI so that it can receive data

  // The lines below write the values ​​of date and
  // time they were placed in the variables above
  Wire.write(convertToBCD(seconds));
  Wire.write(convertToBCD(minutes));
  Wire.write(convertToBCD(hours));
  Wire.write(convertToBCD(dayWeek));
  Wire.write(convertToBCD(dayMonth));
  Wire.write(convertToBCD(month));
  Wire.write(convertToBCD(year));
  Wire.write(zero); // Start at CI
  Wire.endTransmission();
}
