#include <Servo.h>

const int ldrPin1 = A0;
const int ldrPin2 = A1;
const int servoPin = 2;

Servo myServo;

// สำหรับ Continuous Rotation Servo
const int stopSpeed = 90;
const int upSpeed = 180;
const int downSpeed = 0;

const int threshold = 20;

void setup() {
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

  delay(100);
}
