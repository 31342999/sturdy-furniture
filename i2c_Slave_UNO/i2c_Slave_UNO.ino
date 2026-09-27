#include <Wire.h>

void setup() {
  Wire.setClock(2000000);
  Serial.begin(9600);
  Wire.begin(0x08);
  Wire.onRequest(requestEvent);
  Wire.onReceive(reciveevent);
}

void loop() {

  delay(200);

}

void requestEvent()
{
  Wire.write(100);
}

void reciveevent()
{
  if(Wire.available()>0)
  {
    byte c = Wire.read();
    Serial.println(c);
  }
}
