#define trigPin 1
#define echoPin 0// Example pin

void setup() 
{
  long duration;
}

void loop() {
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  Serial.println(duration);
}