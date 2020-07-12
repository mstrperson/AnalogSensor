#include <AnalogSensor.h>

// the AnalogSensor library has up to 4 arguments:
// see AnalogSensor.h for explanation.
AnalogSensor ldr(A0);

void setup() 
{
  Serial.being(9600);
  Serial.println("Here we go!");
}

void loop() 
{
  ldr.update();
  float sensorValue = ldr.getValue();

  Serial.println(sensorValue);
}
