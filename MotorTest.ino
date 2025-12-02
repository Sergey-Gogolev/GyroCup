#define DC_OUT_IN1 0
#define DC_OUT_IN2 4


void setup(){
  Serial.begin(115200);
  while(!Serial);
  
  pinMode(DC_OUT_IN1, OUTPUT);
  pinMode(DC_OUT_IN2, OUTPUT);
  digitalWrite(DC_OUT_IN1, 0);
  digitalWrite(DC_OUT_IN2, 0);
  Serial.print("Setup was completed successfully! \n");
}

int PWM_val = 0;

void loop(){
  analogWrite(DC_OUT_IN1, 0);
  digitalWrite(DC_OUT_IN2, 1);
}


bool motorControl(int val, byte IN1, byte IN2){
  if (val > 0){
    analogWrite(IN1, val);
    digitalWrite(IN2, 0);
  } else if (val < 0) {
    analogWrite(IN1, 255 + val);
    digitalWrite(IN2, 1);
  } else {
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 0);
  }

  return true;
}
