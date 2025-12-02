#include <BMI160Gen.h>
#include <Wire.h>
#include <cmath>

float accelScale = 0.0;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  
  BMI160.begin(BMI160GenClass::I2C_MODE, Wire, 0x68);
  BMI160.setAccelerometerRange(4); // 4g
  accelScale = 4.0 / 32768.0 * 9.81; 
  
}

void loop() {
  int rawAx, rawAy, rawAz;
  
  BMI160.readAccelerometer(rawAx, rawAy, rawAz);
  
  float ax_ms2 = rawAx * accelScale;
  float ay_ms2 = rawAy * accelScale; 
  float az_ms2 = rawAz * accelScale;
  float g = sqrt(ax_ms2 * ax_ms2 + ay_ms2 * ay_ms2 + az_ms2 * az_ms2);

  Serial.print("g = ");
  Serial.println(g);
  
  delay(500);
}
