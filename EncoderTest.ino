#include <ESP32Encoder.h>

ESP32Encoder encoder;

#define ENCODER_A 0
#define ENCODER_B 4

void setup() {
  Serial.begin(115200);
  
  // Включение подтягивающих резисторов (НОВЫЙ СИНТАКСИС)
  encoder.attachHalfQuad(ENCODER_A, ENCODER_B);
  encoder.setCount(0);
  
  // Явно включаем подтяжку к 3.3V
  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  
  Serial.println("Энкодер готов");
}

void loop() {
  Serial.print("Счет: ");
  Serial.println(encoder.getCount());
  delay(100);
}
