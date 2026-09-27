int S0 = 11;
int S1 = 12;
int S2 = 9;
int S3 = 10;
int OUT = 8;

int redPW = 0;
int greenPW = 0;
int bluePW = 0;

int redMin = 32;
int redMax = 595;
int greenMax = 762;
int greenMin = 46;
int blueMax = 583;
int blueMin = 38;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  Serial.begin(9600);
}

void loop() {
  redPW = getRed();
  delay(200);
  greenPW = getGreen();
  delay(200);
  bluePW = getBlue();
  delay(200);
  redPW = map(redPW, redMin, redMax, 255, 0);
  greenPW = map(greenPW, greenMin, greenMax, 255, 0);
  bluePW = map(bluePW, blueMin, blueMax, 255, 0);
  Serial.print("Red = ");
  Serial.println(redPW);
  Serial.print("Green = ");
  Serial.println(greenPW);
  Serial.print("Blue = ");
  Serial.println(bluePW);
}

int getRed()
{
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  int PW;
  PW = pulseIn(OUT, LOW, 100000);
  return PW;
}

int getBlue()
{
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);

  int PW;
  PW = pulseIn(OUT, LOW, 100000);
  return PW;
}

int getGreen()
{
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);

  int PW;
  PW = pulseIn(OUT, LOW, 100000);
  return PW;
}
