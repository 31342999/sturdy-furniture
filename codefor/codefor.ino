"""

The code is made so that 2 buttons and a led can be connected to the port 2, 3 and 13 respectively 
The port 2 button is used to turn the led on or off
The port 3 button is used to turn the function of the button 2 on or off
The led is connected to the port 13 which is also connected to the built in led

"""

const byte LEDPIN = 13;
const byte interruptPIN = 2;
const byte interruptPIN1 = 3;
volatile bool x = false;
volatile byte STATE = LOW;

void setup() {
  pinMode(LEDPIN, OUTPUT);
  pinMode(interruptPIN, INPUT_PULLUP);
  pinMode(interruptPIN1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPIN), blink, FALLING);
  attachInterrupt(digitalPinToInterrupt(interruptPIN1), detach, FALLING);
}

void loop() {
  digitalWrite(LEDPIN, STATE);
  if(x==true)
  {
    detachInterrupt(digitalPinToInterrupt(interruptPIN));
  }
  if(x==false)
  {
    attachInterrupt(digitalPinToInterrupt(interruptPIN), blink, FALLING);
  }
}

void blink()
{
  STATE =! STATE;
}

void detach()
{
  x =! x;
}