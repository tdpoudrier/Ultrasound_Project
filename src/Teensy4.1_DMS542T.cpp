/*
  Stepper motor driver controller test
  buttons are used for user to move the motor
  direction and pulse pins are connected to DMS542T through a 3.3V to 5V level shifter

*/

#include <Arduino.h>

#define directionPin 2
#define pulsePin 3
#define button1 4
#define button2 5

void setup() 
{
  pinMode(pulsePin, OUTPUT);  // move motor
  pinMode(directionPin, OUTPUT);// direction

  pinMode(button1,INPUT); // clockwise
  pinMode(button2,INPUT); // anti clockwise
} 

void loop() 
{
  bool button1_value = digitalRead(button1);
  bool button2_value = digitalRead(button2);
  
  digitalWrite(pulsePin, HIGH);
  
  if (button1_value == HIGH)
  { 
    digitalWrite(directionPin, HIGH);
    delayMicroseconds(500);

    digitalWrite(pulsePin, LOW);
    delayMicroseconds(500); 
  }

  if (button2_value == HIGH)
  {
    digitalWrite(directionPin, LOW);
    delayMicroseconds(500);

    digitalWrite(pulsePin, LOW);
    delayMicroseconds(500); 
  }

}