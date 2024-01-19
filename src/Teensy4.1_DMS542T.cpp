/*
  Stepper motor driver controller test
  buttons are used for user to move the motor
  direction and pulse pins are connected to DMS542T through a 3.3V to 5V level shifter

  Limit switches are used to prevent the linear rail from hitting stoppers and from trying to move past stoppers, consuming current

*/

#include <Arduino.h>

//Define GPIO pins
#define DIRECTION_PIN 2
#define PULSE_PIN 3
#define button1 4
#define button2 5
#define limitSwitch1 11
#define limitSwitch2 12

//Function declarations
void StepAwayFromMotor(uint8_t, uint8_t);
void StepTowardMotor(uint8_t, uint8_t);
void DM542T_begin();

void setup() 
{
  //Initialize GPIO
  pinMode(DIRECTION_PIN, OUTPUT);
  pinMode(PULSE_PIN, OUTPUT);
  pinMode(button1,INPUT); 
  pinMode(button2,INPUT);
  pinMode(limitSwitch1, INPUT);
  pinMode(limitSwitch1, INPUT);

  DM542T_begin();
} 

void loop() 
{
  bool button1_value = digitalRead(button1);
  bool button2_value = digitalRead(button2);

  bool limit1_value = digitalRead(limitSwitch1);
  bool limit2_value = digitalRead(limitSwitch2);

  if(button1_value == HIGH && limit1_value != HIGH)
    StepAwayFromMotor(DIRECTION_PIN, PULSE_PIN);
  
  else if (button2_value == LOW && limit2_value != HIGH)
    StepTowardMotor(DIRECTION_PIN, PULSE_PIN);
}

void DM542T_begin()
{
  digitalWrite(PULSE_PIN, HIGH); //Set pulse pin high 
}

void StepTowardMotor(uint8_t directionPin, uint8_t pulsePin)
{
  //direction leads pulse by >5us
  digitalWrite(directionPin, LOW);
  delayMicroseconds(500);

  //pulse width is >2.5us
  digitalWrite(pulsePin, LOW);
  delayMicroseconds(500);

  digitalWrite(pulsePin, HIGH);
  delayMicroseconds(500);
}

void StepAwayFromMotor(uint8_t directionPin, uint8_t pulsePin)
{
  //direction leads pulse by >5us
  digitalWrite(directionPin, HIGH);
  delayMicroseconds(500);

  //pulse width is >2.5us
  digitalWrite(pulsePin, LOW);
  delayMicroseconds(500);

  digitalWrite(pulsePin, HIGH);
  delayMicroseconds(500);
}