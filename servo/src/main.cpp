#include <Arduino.h>
#include <Servo.h>
#include <ALKS.h>
static const int servoPin = S4;
static const int potentiometerPin = POT1;

Servo servo1;

void setup() {
    setupAll();
    Serial.begin(115200);
    servo1.attach(servoPin);
}

void loop() {
    int servoPosition = map(analogRead(potentiometerPin), 0, 650, 0, 179);
    servo1.write(servoPosition);
    Serial.println(servoPosition);
    delay(90);
}
