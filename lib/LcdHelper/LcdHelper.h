/**
 * Author: Tevin Poudrier
 * Date: Thu 01 Feb 2024 11:13 AM MDT
 * Description: header file for LcdHelper object
 *  this object stores additonal status bar functions for LiquidCrystal_I2C
*/

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

//Class that stores additonal status bar functions for LiquidCrystal_I2C
class LcdHelper : public LiquidCrystal_I2C {
public:
    //Use default Constructor
    using LiquidCrystal_I2C::LiquidCrystal_I2C;

    //Display an indicator bar on the LCD. The indicator bar shows the user where the target value needs to be and how many error margins the current value is off
    //setStatusBarArea must be called first to define where the status bar goes
    void updateStatusBar(uint32_t, int, int); 
    
    //Define the size and location of the status bar
    void setStatusBarArea (uint8_t startIndex, uint8_t endIndex, uint8_t lineToPrint);

    int getStartIndex ();
    int getEndIndex ();
    
    //Init LCD and create custom characters
    void helperInit();

private:

    //Data members
    uint8_t StartIndex, EndIndex, MiddleIndex, LineToPrint;
};