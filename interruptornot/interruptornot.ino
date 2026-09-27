"""

The programe is used to check if the input pin through the serial is a vaild interrupt pin or not.

"""
int pin = 2;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()>0)
  {
    pin = Serial.parseInt();
  }
  int checkPin = digitalPinToInterrupt(pin);
  if (checkPin == -1) {
    Serial.println("Not a valid interrupt pin!");
  } else {
    Serial.println("Valid interrupt pin.");
  }
  delay(500);
}