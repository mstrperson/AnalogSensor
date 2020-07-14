#include <AnalogSensor.h>

// the AnalogSensor library has up to 4 arguments:
// Uncomment the Line that suits your needs.

// standard AnalogSensor with a 25 datapoint running average, connected to A0
// outputs data in the same range as the input source [0, 1024)
AnalogSensor ldr(A0);

// Standard 25 datapoint running average, outputs data in the range [0.0, 180.0)
//AnalogSensor ldr(A0, 0, 180);

// Same AnalogSensor on A0, this time with a 50 data point running average.
// Outputs data in the range [0, 1024)
//AnalogSensor ldr(A0, 50);

// Fully customized AnalogSensor.  50 datapoint running average, and outputs [0.0. 180.0)
//AnalogSensor ldr(A0, 50, 0, 180);

void setup() 
{
  Serial.begin(9600);
  
  // calibrate the sensor for 30 seconds
  Serial.println("Calibration: Shine a bright light on the sensor and cover the sensor.");
  ldr.calibrate();

  Serial.println("RawData\tSensorValue");
}

void loop() 
{
  int v = analogRead(A0);
  ldr.update();
  float sensorValue = ldr.getValue();

  Serial.print(v);
  Serial.print("\t");
  Serial.println(sensorValue);

  delay(20);
}
