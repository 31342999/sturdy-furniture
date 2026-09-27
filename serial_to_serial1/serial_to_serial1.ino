/*
  This code is used send signal from 1st mega and read it through the 2nd mega and vice-versa
  1st Mega:
  Computer → Serial → Mega → Serial1 → external device
  2nd Mega:
  External device → Serial1 → Mega → Serial → Computer
*/


char incommingbyte;
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  if(Serial.available()>0)
  {
    incommingbyte = Serial.read();
    Serial1.print(incommingbyte);
  }
  if(Serial1.available()>0)
  {
    incommingbyte = Serial1.read();
    Serial.println(incommingbyte);
  }

}
