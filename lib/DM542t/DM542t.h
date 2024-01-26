/**
 * 
 * 
*/

#include <Arduino.h>

class DM542T {
public:
    //Constructor
    DM542T(uint8_t directionPin, uint8_t pulsePin);

    //Function Prototypes
    void begin();
    void StepMotor();
    void SetDirection(uint8_t direction);
    uint8_t GetDirection();

private:

    //Data members
    uint8_t DirectionPin;
    uint8_t PulsePin;
    uint8_t CurrentDirection;
    uint8_t NewDirection;
};

