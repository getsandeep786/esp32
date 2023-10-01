#include <ESP32Servo.h>

Servo myservo;  // create servo object to control a servo
static const int servoPin = 18; // Servo GPIO pin

void setup() {
  // put your setup code here, to run once:
  myservo.attach(servoPin);
  // Start serial
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    int angle = Serial.parseInt();
    myservo.write(angle);
  }
  delay(20);
}
