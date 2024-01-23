/**
 * 
*/
#include <DM542t.h>

DM542T::DM542T(uint8_t directionPin, uint8_t pulsePin)
{

}

//Start timer and initalize outputs
void DM542T::begin()
{

}

void DM542T::MoveMotor(uint8_t direction)
{

}

void DM542T::SetDelay(uint32_t delay)
{
    MicrosecondDelay = delay;
}

uint32_t DM542T::GetDelay()
{
    return MicrosecondDelay;
}