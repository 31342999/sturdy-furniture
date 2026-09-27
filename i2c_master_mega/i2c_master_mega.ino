#include <Wire.h>

byte val = 1;

void setup() {
  Wire.begin();
  Wire.setClock(2000000);
  Serial.begin(9600);

}

void loop() {
  Wire.beginTransmission(0x08);
  Wire.write(val);
  byte error = Wire.endTransmission();
  Serial.print("Byte sent : ");
  Serial.println(val);
  Serial.print("Error : ");
  Serial.println(error);
  Wire.requestFrom(0x08, 1);
  if(Wire.available())
  {
    byte recevied = Wire.read();
    Serial.println(recevied);
  }


  val++;
  delay(200);
}
