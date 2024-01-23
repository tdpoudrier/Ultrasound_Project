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
    void MoveMotor(uint8_t direction);
    void SetDelay(uint32_t delay);
    uint32_t GetDelay();

private:

    //Data members
    uint32_t MicrosecondDelay = 500;
    uint8_t DirectionPin;
    uint8_t PulsePin;
    IntervalTimer timer;
};

