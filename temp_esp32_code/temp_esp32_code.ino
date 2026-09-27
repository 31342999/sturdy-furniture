uint8_t a[5] = {1, 2, 3, 4, 5};
uint8_t data = 0xAA;
void setup() {
  Serial2.begin(115200);
}

void loop() {
  Serial.write(data);
  for(int i = 0;i<5;i++)
  {
    Serial2.write(a[i]);
  }
  delay(10);
}
