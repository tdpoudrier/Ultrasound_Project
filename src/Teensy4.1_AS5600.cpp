//
//    FILE: AS5600_position.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo
//     URL: https://github.com/RobTillaart/AS5600
//
//  Examples may use AS5600 or AS5600L devices.
//  Check if your sensor matches the one used in the example.
//  Optionally adjust the code.


#include "AS5600.h"


AS5600 as5600;   //  use default Wire

const uint32_t MAX_POS = 28490; 
const float MAX_INCH = 24.375;

uint32_t initPos = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("AS5600_LIB_VERSION: ");
  Serial.println(AS5600_LIB_VERSION);

  Wire.begin();

  as5600.begin(4);  //  set direction pin.
  as5600.setDirection(AS5600_CLOCK_WISE);  //  default, just be explicit.

  Serial.println(as5600.getAddress());

  //  as5600.setAddress(0x40);  //  AS5600L only

  int b = as5600.isConnected();
  Serial.print("Connect: ");
  Serial.println(b);

  delay(1000);

  //  set initial position
  initPos = as5600.getCumulativePosition();
  Serial.print(initPos);
  delay(1000);
}


void loop()
{
  static uint32_t lastTime = 0;
  float distance = 0;
  uint32_t position  = 0;

  //  update every 100 ms
  //  should be enough up to ~200 RPM
  if (millis() - lastTime >= 100)
  {
    lastTime = millis();
    position = as5600.getCumulativePosition();
    Serial.print("Cum Position: ");
    Serial.print(position);
    Serial.print("\t");

    if(position < initPos) {
      distance = 0;
    }
    else {
    distance = (position - initPos) * (MAX_INCH)/(MAX_POS - initPos);
    }

    Serial.print("Distance: ");
    Serial.print(distance);

    Serial.print("\t Revolutions: ");
    Serial.println(as5600.getRevolutions());
  }

  //  just to show how reset can be used
  if (as5600.getRevolutions() >= 10)
  {
    as5600.resetPosition();
  }
}


//  -- END OF FILE --
