/*
  Stepper motor driver controller test
  buttons are used for user to move the motor
  direction and pulse pins are connected to DMS542T through a 3.3V to 5V level shifter

  Limit switches are used to prevent the linear rail from hitting stoppers and from trying to move past stoppers, consuming current

*/

#include <Arduino.h>

#define directionPin 2
#define pulsePin 3
#define button1 4
#define button2 5

#define limitSwitch1 11
#define limitSwitch2 12

void setup() 
{
  pinMode(pulsePin, OUTPUT);  // move motor
  pinMode(directionPin, OUTPUT);// direction

  pinMode(button1,INPUT); // clockwise
  pinMode(button2,INPUT); // anti clockwise

  pinMode(limitSwitch1, INPUT);
  pinMode(limitSwitch1, INPUT);
} 

void loop() 
{
  bool button1_value = digitalRead(button1);
  bool button2_value = digitalRead(button2);

  bool limit1_value = digitalRead(limitSwitch1);
  bool limit2_value = digitalRead(limitSwitch2);
  
  digitalWrite(pulsePin, HIGH);
  
  //Move linear rail away from motor
  if (button1_value == HIGH && limit1_value != HIGH)
  { 
    digitalWrite(directionPin, HIGH);
    delayMicroseconds(500);

    digitalWrite(pulsePin, LOW);
    delayMicroseconds(500); 
  }

  //Move linear rail towards motor
  else if (button2_value == HIGH && limit2_value != HIGH)
  {
    digitalWrite(directionPin, LOW);
    delayMicroseconds(500);

    digitalWrite(pulsePin, LOW);
    delayMicroseconds(500); 
  }
  else
    digitalWrite(pulsePin, HIGH);

}