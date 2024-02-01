/**
 * 
 * 
*/

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class LcdHelper : public LiquidCrystal_I2C {
public:
    //Use default Constructor
    using LiquidCrystal_I2C::LiquidCrystal_I2C;

    //Display an indicator bar on the LCD. The indicator bar shows the user where the target value needs to be and how many error margins the current value is off
    //setStatusBarArea must be called first to define where the status bar goes
    void updateStatusBar(uint32_t, int, int); 
    void setStatusBarArea (uint8_t startIndex, uint8_t endIndex, uint8_t lineToPrint);

    int getStartIndex ();
    int getEndIndex ();
    void helperInit();

private:

    //Data members
    uint8_t StartIndex, EndIndex, MiddleIndex, LineToPrint;
};