/**
 * Author: Tevin Poudrier
 * Date: Thur 01 Feb 2024 10:44 AM MDT
 * Description
 *    Display the current value of the VL53L0X Time-of-Flight sensor (TOF) and a status bar showning how far the current value is from the target value.
 *    LCD is connected to I2C1 (Wire1) on teensy 4.1, through the TXB0108 logic level shifter. It is powered by 5V. Recommended to find an I2C safe logic shifter
 *    TOF is connected to I2C (Wire) on teensy 4.1.
 **/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Adafruit_VL53L0X.h"
#include <Arduino.h>
#include "LcdHelper.h"

int getDistance();

// Create 16x2 lcd, LcdHelper contains additional functions for status bar
LcdHelper lcd(0x3F, 16, 2);
uint32_t const SET_VALUE = 150;
uint32_t const ERROR_MARGIN = 10;

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

  //Turn on builtin led to know the board is powered
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Initialize LCD
  lcd.helperInit();
  lcd.setStatusBarArea(10, 14, 0);
  lcd.clear();

  // Boot TOF, halt if failed
  if (!lox.begin())
  {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1)
      ;
  }
  //lox.startRangeContinuous();

  //Print lcd variables to verify setup
  Serial.printf("LCD TOF test \n");
  Serial.printf("start index: %d, end index: %d\n", lcd.getStartIndex(), lcd.getEndIndex());
  Serial.printf("Middle Index: %d", (lcd.getStartIndex() - lcd.getEndIndex()) / 2);
}

void loop()
{

  // Get Measurement and time
  int distance = getDistance();
  sampleTime = millis() - sampleTime;

  //Calculate speed
  float speed = (float) (abs(distance - prevDist) * 100) / (sampleTime);
  Serial.printf("Sample time: %d, Distance: %d, Previous Distance: %d\n", sampleTime, distance, prevDist);

  //Set prev time to current program time before calling LCD functions
  sampleTime = millis();

  // Print to display
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