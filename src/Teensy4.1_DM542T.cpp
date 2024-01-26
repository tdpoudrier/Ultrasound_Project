/*
  Stepper motor driver controller test
  buttons are used for user to move the motor
  direction and pulse pins are connected to DM542T through a 3.3V to 5V level shifter

  Limit switches are used to prevent the linear rail from hitting stoppers and from trying to move past stoppers, consuming current

*/

#include <Arduino.h>
#include <DM542T.h>

//Define GPIO pins
#define DIRECTION_PIN 2
#define PULSE_PIN 3
#define button1 4
#define button2 5
#define limitSwitch1 11
#define limitSwitch2 12

//Function declarations
void MoveMotor();

IntervalTimer myTimer;
DM542T motor(DIRECTION_PIN, PULSE_PIN);

void setup() 
{
  //Initialize GPIO
  pinMode(DIRECTION_PIN, OUTPUT);
  pinMode(PULSE_PIN, OUTPUT);
  pinMode(button1,INPUT); 
  pinMode(button2,INPUT);
  pinMode(limitSwitch1, INPUT);
  pinMode(limitSwitch1, INPUT);

  //turn on LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  motor.begin();

  myTimer.begin(MoveMotor, 500);
  myTimer.priority(20);
} 

void loop() {}

void MoveMotor()
{
  bool button1_value = digitalRead(button1);
  bool button2_value = digitalRead(button2);
  bool limit1_value = digitalRead(limitSwitch1);
  bool limit2_value = digitalRead(limitSwitch2);

  if(button1_value == HIGH && limit1_value != HIGH)
  {
    motor.SetDirection(HIGH);
    motor.StepMotor();
  }
  else if (button2_value == HIGH && limit2_value != HIGH)
  {
    motor.SetDirection(LOW);
    motor.StepMotor();
  }
}