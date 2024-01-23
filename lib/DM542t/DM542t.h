/**
 * 
 * 
*/

#include <Arduino.h>

#define DEFAULT_DELAY 500;

class DM542T {
public:
    //Constructor
    DM542T(uint8_t directionPin, uint8_t pulsePin);

    //Function Prototypes
    void begin();
    void StepMotor();
    void SetDelay(uint32_t delay);
    uint32_t GetDelay();
    void SetDirection(uint8_t direction);
    uint8_t GetDirection();

private:

    //Data members
    uint32_t MicrosecondDelay;
    uint8_t DirectionPin;
    uint8_t PulsePin;
    uint8_t CurrentDirection;
    uint8_t NewDirection;
};

