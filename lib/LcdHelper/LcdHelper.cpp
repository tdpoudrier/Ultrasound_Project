/**
 * Author: Tevin Poudrier
 * Date: Thu 01 Feb 2024 11:14 AM MDT
 * Description: Source file for LcdHelper object
 *  this object stores additonal status bar functions for LiquidCrystal_I2C
*/

#include <LcdHelper.h>

// Custom Characters
byte bar[] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B11111};
byte box[] = {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111};
byte checkMark[] = {B00000, B00001, B00001, B00010, B00010, B10100, B10100, B01000};

void LcdHelper::helperInit()
{
    init();
    backlight();
    createChar(0, checkMark);
    createChar(1, box);
    createChar(6, bar);
}

void LcdHelper::setStatusBarArea(uint8_t startIndex, uint8_t endIndex, uint8_t lineToPrint)
{
    StartIndex = startIndex;
    EndIndex = endIndex;
    MiddleIndex = (EndIndex + StartIndex) / 2;

    LineToPrint = lineToPrint;
}

int LcdHelper::getStartIndex ()
{
    return StartIndex;
}

int LcdHelper::getEndIndex ()
{
    return EndIndex;
}

int prevIndex = 0;
void LcdHelper::updateStatusBar(uint32_t targetValue, int currentValue, int errorMargin)
{
    uint8_t indexToPrint;

    //Calculate error and number of error margins off from target value
    signed int error = targetValue - currentValue;
    signed int numErrorMarginOff = error / errorMargin;

    //Calculate index to print at
    //numErrorMarginOff is signed, so it gives error in both directions
    indexToPrint = MiddleIndex - numErrorMarginOff;

    //If error is greater than status area, print at last avaiable index
    if (indexToPrint < StartIndex)
        indexToPrint = StartIndex;
    if (indexToPrint > EndIndex)
        indexToPrint = EndIndex;

    //print/update status bar only if it has changed
    if (indexToPrint != prevIndex)
    {
        // Reset status bar
        for (int i = StartIndex; i <= EndIndex; i++)
        {
            setCursor(i, LineToPrint);
            write(6);
        }
        
        // Print check mark if within error margin
        if (indexToPrint == MiddleIndex)
        {
            setCursor(MiddleIndex, LineToPrint);
            write(0);
        }

        //Print box
        else
        {
            setCursor(indexToPrint, LineToPrint);
            write(1);
            setCursor(MiddleIndex, LineToPrint);
            print("0");
        }
        prevIndex = indexToPrint;
    }
}
