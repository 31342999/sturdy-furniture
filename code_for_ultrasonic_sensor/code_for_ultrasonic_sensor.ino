int TrigPin = 3;
int EchoPin = 4;
void setup() {
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  float duration = pulseIn(EchoPin, HIGH);
  float distance =duration*0.034/2;
  Serial.print("Distance: ");
  Serial.println(distance);

}
