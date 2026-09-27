int S0 = 11;
int S1 = 12;
int S2 = 9;
int S3 = 10;
int OUT = 8;

int redPW = 0;
int greenPW = 0;
int bluePW = 0;

int maxred = 0;
int minred = 1000;
int maxblue = 0;
int minblue = 1000;
int maxgreen = 0;
int mingreen = 1000;



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
  redPW = getRedPW();
  if(maxred<redPW)
  {
    maxred = redPW;
  }
  if(minred>redPW)
  {
    minred = redPW;
  }
  delay(200);
  greenPW = getGreenPW();
  if(maxgreen<greenPW)
  {
    maxgreen = greenPW;
  }
  if(mingreen>greenPW)
  {
    mingreen = greenPW;
  }
  delay(200);
  bluePW = getBluePW();
  if(maxblue<bluePW)
  {
    maxblue = bluePW;
  }
  if(minblue>bluePW)
  {
    minblue = bluePW;
  }
  delay(200);
  Serial.print("Red PW = ");
  Serial.println(redPW);
  Serial.print("Blue PW = ");
  Serial.println(bluePW);
  Serial.print("green PW = ");
  Serial.println(greenPW);
  Serial.print("red Max = ");
  Serial.println(maxred);
  Serial.print("red Min = ");
  Serial.println(minred);
  Serial.print("green Max = ");
  Serial.println(maxgreen);
  Serial.print("green Min = ");
  Serial.println(mingreen);
  Serial.print("blue Max = ");
  Serial.println(maxblue);
  Serial.print("blue Min = ");
  Serial.println(minblue);
}

int getRedPW()
{
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  int PW;
  PW = pulseIn(OUT, LOW,100000);
  return PW;
}

int getGreenPW()
{
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  int PW;
  PW = pulseIn(OUT, LOW,100000);
  return PW;
}

int getBluePW()
{
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  int PW;
  PW = pulseIn(OUT, LOW,100000);
  return PW;
}
