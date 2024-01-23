/**
 * 
*/
#include <DM542t.h>

DM542T::DM542T(uint8_t directionPin, uint8_t pulsePin)
{
    DirectionPin = directionPin;
    PulsePin = pulsePin;
    MicrosecondDelay = DEFAULT_DELAY;
    CurrentDirection = LOW;
}

//Initalize outputs
void DM542T::begin()
{
    digitalWrite(PulsePin, HIGH);
    digitalWrite(DirectionPin, CurrentDirection);
}

void DM542T::StepMotor()
{
    if(CurrentDirection != NewDirection)
    {
        //direction leads pulse by >5us
        digitalWrite(DirectionPin, NewDirection);
        CurrentDirection = NewDirection;
        delayMicroseconds(6);
    }

    //pulse width is >2.5us
    digitalWrite(PulsePin, LOW);
    delayMicroseconds(5);
    digitalWrite(PulsePin, HIGH);
}

void DM542T::SetDelay(uint32_t delay)
{
    MicrosecondDelay = delay;
}

uint32_t DM542T::GetDelay()
{
    return MicrosecondDelay;
}

//Define direction with HIGH or LOW
void DM542T::SetDirection(uint8_t direction)
{
    NewDirection = direction;
}

uint8_t DM542T::GetDirection()
{
    return CurrentDirection;
}