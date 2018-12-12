#include <MeanFilter.h>
MeanFilter errFilter(0.75);

#define leftMotorDirPin 8
#define leftMotorPwmPin 9
#define rightMotorDirPin 7
#define rightMotorPwmPin 6
#define distSensorPin A0
int distThresholdValue = 0;
float koefProp = 1.1;

void setup() {
  Serial.begin(9600);
  pinMode(rightMotorPwmPin, OUTPUT);
  pinMode(rightMotorDirPin, OUTPUT);
  pinMode(leftMotorPwmPin, OUTPUT);
  pinMode(leftMotorDirPin, OUTPUT);
  pinMode(distSensorPin, INPUT);
  Serial.println("Calibration...");
  calibrationSensor();
  delay(2000);
  Serial.print("Calibration dist:\t");
  Serial.println(distThresholdValue);
  Serial.println("Starting balance...");
  delay(2000);
}

void loop() {
  balance();
}

void balance() {
  int distSensorValue = analogRead(distSensorPin);
  int err = distSensorValue - distThresholdValue;
  int up = koefProp * err;
  up = int(constrain(up, -255, 255));
  Serial.println(up);
  if (err <= 0) {
    motor(1, 1, up);
    motor(-1, 1, up);
  }else {
    motor(1, -1, up);
    motor(-1, -1, up);
  }
  delay(5);
}

// motor (side(сторона мотора: 1 левый, -1 правый), dir (направление движения: 1 прямо, -1 назад), pwmValue)
void motor(int side, int dir, byte pwmValue) {
  switch (side) {
    case 1:
      switch (dir) {
        // движение вперед
        case 1:
          digitalWrite(leftMotorDirPin, LOW);
          analogWrite(leftMotorPwmPin, pwmValue);
          break;

        // движение назад
        case -1:
          digitalWrite(leftMotorDirPin, HIGH);
          analogWrite(leftMotorPwmPin, pwmValue);
          break;
      }
      break;

    case -1:
      switch (dir) {
        // движение вперед
        case 1:
          digitalWrite(rightMotorDirPin, LOW);
          analogWrite(rightMotorPwmPin, pwmValue);
          break;

        // движение назад
        case -1:
          digitalWrite(rightMotorDirPin, HIGH);
          analogWrite(rightMotorPwmPin, pwmValue);
          break;
      }
      break;
  }
}

void calibrationSensor() {
  int var = 0;
  for (int i = 0; i < 10; i++) {
    var += analogRead(distSensorPin);
    delay(15);
  }
  var /= 10;
  distThresholdValue = var;
}

