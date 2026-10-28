
#include <Servo.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);

const int ldrPin1 = A0;
const int ldrPin2 = A1;
const int batteryPin = A2;
const int servoPin = 2;

Servo myServo;

const int stopSpeed = 90;
const int upSpeed = 180;
const int downSpeed = 0;

const int threshold = 80;

const float referenceVoltage = 4.48;
const float R1 = 10000.0;
const float R2 = 10000.0;

void setup() {
  setupLCD();
  Serial.begin(9600);
  myServo.attach(servoPin);
  myServo.write(stopSpeed);
}

void loop() {
  int value1 = analogRead(ldrPin1);
  int value2 = analogRead(ldrPin2);

  int difference = value1 - value2;

  Serial.print("LDR1: ");
  Serial.print(value1);
  Serial.print(", LDR2: ");
  Serial.print(value2);
  Serial.print(", Difference: ");
  Serial.println(difference);

  if (difference > threshold) {
    myServo.write(downSpeed);
  }
  else if (difference < -threshold) {
    myServo.write(upSpeed);
  }
  else {
    myServo.write(stopSpeed);
  }

  float voltage = readBatteryVoltage();
  int percent = constrain(batteryPercent(voltage), 0, 100);
  displayBattery(voltage, percent);

  delay(100);
}

float readBatteryVoltage() {
  int raw = analogRead(batteryPin);

  float voltageA0 = raw * referenceVoltage / 1023.0;
  float batteryVoltage = voltageA0 * (R1 + R2) / R2;

  return batteryVoltage;
}

int batteryPercent(float voltage) {
  if (voltage >= 4.20) return 100;
  if (voltage <= 3.30) return 0;
  if (voltage >= 4.10) return 90 + (voltage - 4.10) * 100;
  if (voltage >= 4.00) return 80 + (voltage - 4.00) * 100;
  if (voltage >= 3.90) return 60 + (voltage - 3.90) * 200;
  if (voltage >= 3.80) return 40 + (voltage - 3.80) * 200;
  if (voltage >= 3.70) return 20 + (voltage - 3.70) * 200;
  return (voltage - 3.30) * 20;
}

void displayBattery(float voltage, int percent) {
  lcd.setCursor(0, 0);
  lcd.print("Voltage: ");
  lcd.print(voltage, 2);
  lcd.print("V     ");

  lcd.setCursor(0, 1);
  lcd.print("Battery: ");
  lcd.print(percent);
  lcd.print("%       ");
}

void setupLCD() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Battery Monitor");
  delay(1500);
  lcd.clear();
}
