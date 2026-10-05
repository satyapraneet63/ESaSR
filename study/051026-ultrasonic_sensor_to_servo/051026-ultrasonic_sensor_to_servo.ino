#include <Servo.h>
Servo myServo;

const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 8;

const int minAngle = 15;
const int maxAngle = 165;

int currentAngle = minAngle;
int sweepDirection = 1;

unsigned long lastStepTime = 0;
unsigned long lastSensorRead = 0;

int stepDelay = 30;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  myServo.attach(servoPin);
  myServo.write(currentAngle);
}

int readDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000);
  if (duration == 0) {
    return 400;
  }
  return duration * 0.034 / 2;
}

void loop() {
  unsigned long now = millis();
  if (now - lastSensorRead >= 50) {
    lastSensorRead = now;
    int distance = readDistanceCM();

    distance = constrain(distance, 5, 50);

    stepDelay = map(distance, 5, 50, 4, 30);

    Serial.print("Distance ");
    Serial.print(distance);
    Serial.print(" cm | Step Delay: ");
    Serial.println(" ms");
  }

  if (now - lastStepTime >= (unsigned long)stepDelay) {
    lastStepTime = now;

    currentAngle += sweepDirection;
    myServo.write(currentAngle);

    if (currentAngle >= maxAngle) {
      currentAngle = maxAngle;
      sweepDirection = -1;
    } else if (currentAngle <= minAngle) {
      currentAngle = minAngle;
      sweepDirection = 1;
    }
  }
}
