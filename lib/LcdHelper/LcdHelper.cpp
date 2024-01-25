#include <LcdHelper.h>


void LcdHelper::helperInit ()
{
  init();
  backlight();
  createChar(0, checkMark);
  createChar(1, box);
  createChar(6, bar);
}

void LcdHelper::updateStatusBar(uint32_t targetValue, int currentValue, int errorMargin, uint8_t lineToPrint) 
{
  signed int error = targetValue - currentValue;
  signed int numErrorMarginOff = error / errorMargin;

  Serial.printf("Error: %d, divError: %d, abs divError: %d \n", error, numErrorMarginOff, abs(numErrorMarginOff));

  //Print a line
  setCursor(0, lineToPrint);
  for (int i = 0; i <= 15; i++) {
    write(6);
  }
  
  //Current value is greater than target, iterate though each location and check if a box needs to be displayed
  for (int i = LCD_GREATER_SHIFT + 1; i <= 15; i++) 
  {
    //Print box away from center valid range based on the amount of error margins the current value is from the target value
    if (abs(numErrorMarginOff) + LCD_GREATER_SHIFT == i && numErrorMarginOff < 0 && abs(numErrorMarginOff) <= LCD_STATUS_BAR_WIDTH) 
    {
      setCursor(i, lineToPrint);
      write(LCD_CUST_BOX);
      Serial.printf("Greater than target, printing at %d \n", i);
    } 
    
    //Print box at edge of status bar if current value is out of displayable range.
    else if (abs(numErrorMarginOff) > LCD_STATUS_BAR_WIDTH && numErrorMarginOff < 0) 
    {
      setCursor(15, lineToPrint);
      write(LCD_CUST_BOX);
      break;
    }
  }
  
  //Current value is less than Target , iterate though each location and check if a box needs to be displayed
  for (int i = 1; i <= LCD_LESS_SHIFT; i++) 
  {
    //Print box away from center valid range based on the amount of error margins the current value is from the target value
    if (abs(numErrorMarginOff) == i && numErrorMarginOff > 0 && abs(numErrorMarginOff) <= LCD_STATUS_BAR_WIDTH) 
    {
      setCursor(LCD_LESS_SHIFT - i, lineToPrint);
      write(LCD_CUST_BOX);
    } 

    //Print box at edge of status bar if current value is out of displayable range.
    else if (abs(numErrorMarginOff) > LCD_STATUS_BAR_WIDTH && numErrorMarginOff > 0) 
    {
      setCursor(0, lineToPrint);
      write(LCD_CUST_BOX);
      break;
    }
  }

  //Print check mark if current value is within error margin, else print []
  setCursor(LCD_EQUAL_SHIFT, lineToPrint);
  if (error > errorMargin * -1 && error < errorMargin) 
  {
    write(LCD_CUST_CHECK);
    write(LCD_CUST_CHECK);
  } 
  else 
  {
    print("[]");
  }
}
