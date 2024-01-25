/**
 * 
 * 
*/

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

//Custom Characters
    byte bar[] = { B00000, B00000, B00000, B00000, B00000, B00000, B00000, B11111};
    byte box[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111};
    byte checkMark[] = { B00000, B00001, B00001, B00010, B00010, B10100, B10100, B01000 };

class LcdHelper : public LiquidCrystal_I2C {
public:
    //Use default Constructor
    using LiquidCrystal_I2C::LiquidCrystal_I2C;

    //Display an indicator bar on the LCD. The indicator bar shows the user where the target value needs to be and how many error margins the current value is off
    //Ex: Value is 3 error margins greater than target, X represents a box: ____[]__X__
    void updateStatusBar(uint32_t, int, int, uint8_t); 

    void helperInit();

private:

    //Data members
    //contants to avoid magic numbers and improve code legibility
    uint8_t const LCD_GREATER_SHIFT = 8;
    uint8_t const LCD_EQUAL_SHIFT = 7;
    uint8_t const LCD_LESS_SHIFT = 7;
    uint8_t const LCD_STATUS_BAR_WIDTH = 7;
    uint8_t const LCD_CUST_LINE =  6;
    uint8_t const LCD_CUST_BOX = 1;
    uint8_t const LCD_CUST_CHECK = 0;
};