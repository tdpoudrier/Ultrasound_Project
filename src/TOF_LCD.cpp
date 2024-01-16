/**
 * Display the current value of the VL53L0X Time-of-Flight sensor (TOF) and a status bar showning how far the current value is from the target value
 * LCD and TOF are connected via I2C1 on teensy 4.1
**/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Adafruit_VL53L0X.h"
#include <Arduino.h>

void updateStatusBar(uint32_t, uint32_t, int, uint8_t); 

//Custom Characters
byte bar[] = { B00000, B00000, B00000, B00000, B00000, B00000, B00000, B11111};
byte box[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111};
byte checkMark[] = { B00000, B00001, B00001, B00010, B00010, B10100, B10100, B01000 };

//Create objects
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
LiquidCrystal_I2C lcd(0x3F, 16, 2);

uint32_t SET_VALUE = 150;
uint32_t ERROR_MARGIN = 10;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  //Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, checkMark);
  lcd.createChar(1, box);
  lcd.createChar(6, bar);

  //check if TOF is bootted
  if (!lox.begin()) 
  {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  
  Serial.printf("LCD TOF test \n");
}

void loop() {
  String dataString = "";

  //Create and initialize TOF object
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);  // pass in 'true' to get debug data printout!

  //Get measurement if data is valid and in range (30mm - 2000mm)
  uint32_t measurement = measure.RangeMilliMeter;
  if (measure.RangeStatus != 4)  // phase failures have incorrect data
  {
    Serial.print("Distance (mm): ");
    Serial.println(measurement);
    dataString += String((measurement));
  } 
  else
  {
    dataString += String("error");
  }

  //Print to display
  lcd.clear();
  updateStatusBar(SET_VALUE, measurement, ERROR_MARGIN, 1);
  lcd.setCursor(0, 0);
  lcd.print(dataString);

  // 20ms delay and blink LED
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(75);                        // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(75);
}

//contants to avoid magic numbers and improve code legibility
uint8_t LCD_GREATER_SHIFT = 8;
uint8_t LCD_EQUAL_SHIFT = 7;
uint8_t LCD_LESS_SHIFT = 7;
uint8_t LCD_STATUS_BAR_WIDTH = 7;
uint8_t LCD_CUST_LINE =  6;
uint8_t LCD_CUST_BOX = 1;
uint8_t LCD_CUST_CHECK = 0;

//Display an indicator bar on the LCD. The indicator bar shows the user where the target value needs to be and how many error margins the current value is off
//Ex: Value is 3 error margins greater than target, X represents a box: ____[]__X__
void updateStatusBar(uint32_t targetValue, uint32_t currentValue, int errorMargin, uint8_t lineToPrint) 
{
  signed int error = targetValue - currentValue;
  signed int numErrorMarginOff = error / errorMargin;

  Serial.printf("Error: %d, divError: %d, abs divError: %d \n", error, numErrorMarginOff, abs(numErrorMarginOff));

  //Print a line
  lcd.setCursor(0, lineToPrint);
  for (int i = 0; i <= 15; i++) {
    lcd.write(6);
  }
  
  //Current value is greater than target, iterate though each location and check if a box needs to be displayed
  for (int i = LCD_GREATER_SHIFT + 1; i <= 15; i++) 
  {
    //Print box away from center valid range based on the amount of error margins the current value is from the target value
    if (abs(numErrorMarginOff) + LCD_GREATER_SHIFT == i && numErrorMarginOff < 0 && abs(numErrorMarginOff) <= LCD_STATUS_BAR_WIDTH) 
    {
      lcd.setCursor(i, lineToPrint);
      lcd.write(LCD_CUST_BOX);
      Serial.printf("Greater than target, printing at %d \n", i);
    } 
    
    //Print box at edge of status bar if current value is out of displayable range.
    else if (abs(numErrorMarginOff) > LCD_STATUS_BAR_WIDTH && numErrorMarginOff < 0) 
    {
      lcd.setCursor(15, lineToPrint);
      lcd.write(LCD_CUST_BOX);
      break;
    }
  }
  
  //Current value is less than Target , iterate though each location and check if a box needs to be displayed
  for (int i = 1; i <= LCD_LESS_SHIFT; i++) 
  {
    //Print box away from center valid range based on the amount of error margins the current value is from the target value
    if (abs(numErrorMarginOff) == i && numErrorMarginOff > 0 && abs(numErrorMarginOff) <= LCD_STATUS_BAR_WIDTH) 
    {
      lcd.setCursor(LCD_LESS_SHIFT - i, lineToPrint);
      lcd.write(LCD_CUST_BOX);
    } 

    //Print box at edge of status bar if current value is out of displayable range.
    else if (abs(numErrorMarginOff) > LCD_STATUS_BAR_WIDTH && numErrorMarginOff > 0) 
    {
      lcd.setCursor(0, lineToPrint);
      lcd.write(LCD_CUST_BOX);
      break;
    }
  }

  //Print check mark if current value is within error margin, else print []
  lcd.setCursor(LCD_EQUAL_SHIFT, lineToPrint);
  if (error > errorMargin * -1 && error < errorMargin) 
  {
    lcd.write(LCD_CUST_CHECK);
    lcd.write(LCD_CUST_CHECK);
  } 
  else 
  {
    lcd.print("[]");
  }
}
