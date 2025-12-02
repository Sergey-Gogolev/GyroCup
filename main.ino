#include <Wire.h>
#include <cmath>
#include <BMI160Gen.h>
#include <ESP32Encoder.h>
#include <QuickPID.h>


#define BMI160_SCL 21
#define BMI160_SAO 22
#define DC_OUT_IN1 0
#define DC_OUT_IN2 4
#define ENC_C1 2
#define ENC_C2 15
#define MAX_ANGLE 45.0


float setpoint = 0.0;
float input, output;
float P = 1.0, I = 0.0, D = 0.01, RealI = 0.005;
float accelScale = 0.0;
float encoderScale = 360.0 / 660.0;
float angle = 0.0;
float g = 0.0;
int MesurmentTime = 30;


QuickPID myPID(&input, &output, &setpoint);
ESP32Encoder encoder;


void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.print("Setuping Serial... \t Sucsses!\n");

  Serial.print("Setuping Pinouts... \t");
  Wire.begin(BMI160_SCL, BMI160_SAO);
  pinMode(DC_OUT_IN1, OUTPUT);
  pinMode(DC_OUT_IN2, OUTPUT);
  digitalWrite(DC_OUT_IN1, 0);
  digitalWrite(DC_OUT_IN2, 0);
  Serial.print("Sucsses!\n");

  Serial.print("Setuping Encoder... \t");
  encoder.attachHalfQuad(ENC_C1, ENC_C2);
  encoder.setCount(0);
  pinMode(ENC_C1, INPUT_PULLUP);
  pinMode(ENC_C2, INPUT_PULLUP);
  Serial.print("Sucsses!\n");
  
  Serial.print("Setuping BMI160... \t");
  BMI160.begin(BMI160GenClass::I2C_MODE, Wire, 0x68);
  BMI160.setAccelerometerRange(2); // ±2g
  accelScale = 2.0 / 32768.0 * 9.81; // для ±2g
  Serial.print("Sucsses!\n");
  
  int rawAx, rawAy, rawAz;
  BMI160.readAccelerometer(rawAx, rawAy, rawAz);
  g = sqrt(rawAx * rawAx + rawAy * rawAy + rawAz * rawAz); 

  Serial.print("Setuping PID-regulator... \t");
  myPID.SetTunings(P, I, D);
  myPID.SetOutputLimits(-255, 255);
  myPID.SetSampleTimeUs(10000); // 10ms в микросекундах
  myPID.SetMode(QuickPID::Control::automatic);
  Serial.print("Sucsses!\n");
  

  
}



void loop() {
  float timestamp = millis();
  
  input = calcInput(MesurmentTime);
  myPID.Compute();

  motorControl(output - RealI * encoder.getCount(), DC_OUT_IN1, DC_OUT_IN2);  
  Serial.print("Setpoint: ");
  Serial.print(setpoint);
  Serial.print(" | Input: ");
  Serial.print(input);
  Serial.print(" | Output: ");
  Serial.println(output);
  
  while(millis() - timestamp < 50){};
  
}



bool motorControl(float val, byte IN1, byte IN2) {
  bool dir = (val > 0);
  int speed = dir ? (int)(155 * abs(val) + 50) : - (int)(155 * abs(val) + 50);
  

  if (speed > 0) { 
    analogWrite(IN1, speed);
    digitalWrite(IN2, 0);
  } else if (speed < 0){
    analogWrite(IN1, 255 + speed);
    digitalWrite(IN2, 1);
  } else{
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 0);
  }
  
  return true;
}

float calcInput(int MesTime){
  int rawAx, rawAy, rawAz;
  int a = 0;
  int i = 0;
  int timestamp = millis();
  while (millis() - timestamp < MesTime){
    BMI160.readAccelerometer(rawAx, rawAy, rawAz);
    a -= rawAy;
    i ++;
  }
  return (float)a / i / g;
}
