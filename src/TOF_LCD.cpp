/**
 * Display the current value of the VL53L0X Time-of-Flight sensor (TOF) and a status bar showning how far the current value is from the target value
 * LCD and TOF are connected via I2C1 on teensy 4.1
 **/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Adafruit_VL53L0X.h"
#include <Arduino.h>
#include "LcdHelper.h"

int getDistance();

// Create lcd
LcdHelper lcd(0x3F, 16, 2);
uint32_t SET_VALUE = 150;
uint32_t ERROR_MARGIN = 10;

// Time of flight sensor
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

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Initialize LCD
  lcd.helperInit();
  lcd.setStatusBarArea(10, 14, 0);
  lcd.clear();

  // check if TOF is bootted
  if (!lox.begin())
  {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1)
      ;
  }

  Serial.printf("LCD TOF test \n");
  Serial.printf("start index: %d, end index: %d\n", lcd.getStartIndex(), lcd.getEndIndex());
  Serial.printf("Middle Index: %d", (lcd.getStartIndex() - lcd.getEndIndex()) / 2);
}

void loop()
{

  // Get Measurement
  int distance = getDistance();
  sampleTime = millis() - sampleTime;
  float speed = (float) (abs(distance - prevDist) * 100) / (sampleTime);
  Serial.printf("Sample time: %.2f, Distance: %d, Previous Distance: %d\n", speed, distance, prevDist);

  prevDist = distance;
  sampleTime = millis();

  // Print to display
  lcd.updateStatusBar(SET_VALUE, distance, ERROR_MARGIN);
  lcd.setCursor(0, 0);
  lcd.print(distance);
  lcd.print("  ");
  lcd.setCursor(0, 1);
  lcd.print(speed);
  lcd.print(" mm/s  ");

  // 20ms delay and blink LED
  digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
  // delay(75);                       // wait for a second
  // digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
  // delay(75);
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