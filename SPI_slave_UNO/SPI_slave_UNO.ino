#include <SPI.h>
uint16_t receviedval;

void setup() {
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCK, INPUT);
  pinMode(SS, INPUT);
  SPCR |= _BV(SPE);
  SPI.begin();
  SPI.beginTransaction(SPISettings(8000000,LSBFIRST,SPI_MODE0));
  Serial.begin(9600);
}

void loop() {
  receviedval = SPI.transfer16(uint16_t data);

}
