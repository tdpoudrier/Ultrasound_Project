/**
 * Display the current value of the VL53L0X Time-of-Flight sensor (TOF) and a status bar showning how far the current value is from the target value
 * LCD and TOF are connected via I2C1 on teensy 4.1
**/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Adafruit_VL53L0X.h"
#include <Arduino.h>
#include "LcdHelper.h"

//Create lcd
LcdHelper lcd(0x3F, 16, 2);
uint32_t SET_VALUE = 150;
uint32_t ERROR_MARGIN = 10;

//Time of flight sensor
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
IntervalTimer tofTimer;
volatile double distance;
volatile boolean distanceStatus;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  //Initialize LCD
  lcd.helperInit();

  //check if TOF is bootted
  if (!lox.begin()) 
  {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  
  tofTimer.begin(getDistance, 10000);
  tofTimer.priority(30);

  Serial.printf("LCD TOF test \n");
}

void loop() {
  String dataString = "";
  
  if (distanceStatus != true)  // phase failures have incorrect data
  {
    dataString += String((distance));
  } 
  else
  {
    dataString += String("error");
  }

  //Print to display
  lcd.clear();
  lcd.updateStatusBar(SET_VALUE, distance, ERROR_MARGIN, 1);
  lcd.setCursor(0, 0);
  lcd.print(dataString);

  // 20ms delay and blink LED
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(75);                        // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(75);
}

void getDistance ()
{
  distanceStatus = true;
  int numAverages = 50;
  int sumMeasurements = 0;

  for (int i; i < numAverages; i++)
  {
    //Create and initialize TOF object
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);  // pass in 'true' to get debug data printout!

    //Get measurement if data is valid and in range (30mm - 2000mm)
    sumMeasurements += measure.RangeMilliMeter;
    if (measure.RangeStatus == 4)  // phase failures have incorrect data
    {
      distanceStatus = false;
    } 
  }

  distance = (double) sumMeasurements / numAverages;
}

