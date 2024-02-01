/**
 * Author: Tevin Poudrier
 * Date: Thu 01 Feb 11:00 AM MDT
 * Description: Main program for ultrasound project, uses LCD, TOF, DM54T motor controller
 * 
*/
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Adafruit_VL53L0X.h"
#include <DM542T.h>
#include "LcdHelper.h"

/* ----GPIO Pinout---- */
//DM54T Motor Controller
#define DIRECTION 2
#define PULSE 3
//User control
#define BUTTON_1 4
#define BUTTON_2 5
#define LIM_SWITCH_1 11
#define LIM_SWITCH_2 12


//Function prototypes
void MoveMotor();
int getDistance();

// Create 16x2 lcd, LcdHelper contains additional functions for status bar
LcdHelper lcd(0x3F, 16, 2);
uint32_t SET_VALUE = 300;
uint32_t ERROR_MARGIN = 10;

//Create Motor controller object and timer
DM542T motor(DIRECTION, PULSE);
IntervalTimer myTimer;

// Create Time-of-flight (TOF) object and variables associated with it
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
int distance;
int const SIZE = 7;
int buffer[SIZE] = {0};
int count = 0;
int prevDist = 0;
int sampleTime = 1;

void setup() 
{
  Serial.begin(9600);
  
  // Boot TOF, halt if failed
  if (!lox.begin()) 
  {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  Serial.println(F("bootted VL53L0X"));

  //Initalize GPIO Pins
  pinMode(PULSE, OUTPUT);
  pinMode(DIRECTION, OUTPUT);
  pinMode(BUTTON_1,INPUT);
  pinMode(BUTTON_2,INPUT);
  pinMode(LIM_SWITCH_1, INPUT);
  pinMode(LIM_SWITCH_2, INPUT);
 
 //turn on led for power indicator
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  //Initialize LCD
  lcd.helperInit();
  lcd.setStatusBarArea(9, 15, 0);
  lcd.clear();
  Serial.println(F("booted LCD"));

  //initalize stepper motor controller, start timer
  motor.begin();
  myTimer.begin(MoveMotor, 500);
  myTimer.priority(20);
} 

void loop() 
{
  // Get Measurement and time
  int distance = getDistance();
  sampleTime = millis() - sampleTime;

  //Calculate speed
  float speed = (float) (abs(distance - prevDist) * 100) / (sampleTime);
  Serial.printf("Sample time(ms): %d, Distance(mm): %d, Previous Distance(mm): %d\n", sampleTime, distance, prevDist);

  //Set prev time to current program time before calling LCD functions
  sampleTime = millis();

  // Print to display only if a change has occured
  lcd.updateStatusBar(SET_VALUE, distance, ERROR_MARGIN);
  if (prevDist != distance)
  {
    lcd.setCursor(0, 0);
    lcd.print(distance);
    lcd.print("  ");
    lcd.setCursor(0, 1);
    lcd.print(speed);
    lcd.print(" mm/s  ");
  }

  //save previous distance
  prevDist = distance;
}

void MoveMotor ()
{
  //Read button states
  bool button1_value = digitalRead(BUTTON_1);
  bool button2_value = digitalRead(BUTTON_2);
  bool limit1_value = digitalRead(LIM_SWITCH_1);
  bool limit2_value = digitalRead(LIM_SWITCH_2);

  //Move linear rail away from motor
  if(button1_value == HIGH && limit1_value != HIGH)
  {
    motor.SetDirection(HIGH);
    motor.StepMotor();
  }

  //Move linear rail towards motor
  else if (button2_value == HIGH && limit2_value != HIGH)
  {
    motor.SetDirection(LOW);
    motor.StepMotor();
  }
}

//Ring buffer implementation
int getDistance ()
{
  //Get measurement
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  //Save valid data into ring buffer 
  if (measure.RangeStatus != 4)
  {
    buffer[count] = measure.RangeMilliMeter;
    count = (count + 1) % SIZE;
  }

  //Calculate total
  int total = 0;
  for (int i = 0; i < SIZE; i++)
  {
    total += buffer[i];
  }

  //Calculate and return average
  int average = total / SIZE;
  return average;
}
