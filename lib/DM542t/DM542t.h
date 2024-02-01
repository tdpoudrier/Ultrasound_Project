/**
 * Author: Tevin Poudrier
 * Date: Thu 01 Feb 2024 11:44 AM MDT
 * Description: Header file for DM542T object
 *      Controls timing and stepping of DM542T Stepper motor controller
 * 
*/

#include <Arduino.h>

//Class for controlling timing of DM542T Stepper motor controller
class DM542T {
public:
    
    //Create object, set the direction and pulse GPIO pins
    DM542T(uint8_t directionPin, uint8_t pulsePin);

    //Initialize outputs
    void begin();

    //Send one pulse to the DM542T, which moves motor one step
    void StepMotor();

    //Set the direction of the DM542T, enter true or false
    void SetDirection(uint8_t direction);
    uint8_t GetDirection();

private:

    //Data members
    uint8_t DirectionPin;
    uint8_t PulsePin;
    uint8_t CurrentDirection;
    uint8_t NewDirection;
};

