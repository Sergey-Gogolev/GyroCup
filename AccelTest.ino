#include <Wire.h>
#include <SparkFun_MMA8452Q.h>

MMA8452Q accel;

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  
  // Пробуем оба адреса
  byte addr = 0x1D;
  if (!accel.begin(addr)) {
    addr = 0x1C;
    if (!accel.begin(addr)) {
      Serial.println("Датчик не найден!");
      while(1);
    }
  }
  Serial.print("Датчик найден на адресе 0x");
  Serial.println(addr, HEX);
  
  // ЭТИ ДВЕ КОМАНДЫ АКТИВИРУЮТ ДАТЧИК
  accel.setScale(SCALE_2G);     // Установка диапазона активирует датчик
  accel.setDataRate(ODR_100);   // Установка частоты тоже активирует
  
  // Проверяем, активен ли датчик
  uint8_t whoami = accel.readRegister(0x0D); // WHO_AM_I регистр
  Serial.print("WHO_AM_I: 0x");
  Serial.println(whoami, HEX);
  
  delay(50);
  Serial.println("Готово");
}

void loop() {
  if (accel.available()) {
    accel.read();
    
    Serial.print("X:");
    Serial.print(accel.x);
    Serial.print(" Y:");
    Serial.print(accel.y);
    Serial.print(" Z:");
    Serial.println(accel.z);
    
    delay(100);
  } else {
    // Если данные не готовы, проверяем регистр STATUS
    byte status = accel.readRegister(0x00);
    Serial.print("STATUS: 0x");
    Serial.println(status, HEX);
    delay(500);
  }
}
