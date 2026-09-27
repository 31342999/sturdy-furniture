#include <SPI.h>

uint16_t receviedVal16;
uint16_t val16 = 0x0004;
const int SC_pin = 53;

void setup() {
  SPISettings mysetting(8000000,LSBFIRST,SPI_MODE0);
  SPI.begin();
  Serial.begin(9600);
  pinMode(SC_pin, OUTPUT);
  digitalWrite(SC_pin, HIGH); // when this pin is set to high that means that the slave is not selected
}

void loop() {
  SPI.beginTransaction(mysetting);
  digitalWrite(SS, LOW);
  receviedVal16 = SPI.transfer16(val16);
  digitalWrite(SS, HIGH);
  SPI.endTransaction();
  Serial.print("Recived byte = 0x")
  Serial.print(receviedVal16, HEX);

  delay(1000);
  
}
