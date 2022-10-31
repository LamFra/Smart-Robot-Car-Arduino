#include <Wire.h>
#include <SoftwareSerial.h>

//definisco pin RX e TX da Arduino verso modulo BT
#define BT_TX_PIN 12
#define BT_RX_PIN 11

//L298 Connection   
const int LF = 5;  // Pin  in1 of L298
const int LB = 6;  // Pin  in2 of L298
const int RB = 9; // Pin in3 of L298
const int RF = 10;  // Pin in4 of L298
const int RESEARCHTIME = 1000;

int cruiseSpeed = 40;     // Default speed, from 0 to 255
int manouvreSpeed = 60;

bool lf = false, rf = false, lb = false, rb = false;
SoftwareSerial bt =  SoftwareSerial(BT_RX_PIN, BT_TX_PIN);

const int IN_A0 = A0; // analog input
const int IN_D0 = 8; // digital input
long time;

void setup() {
  pinMode (IN_A0, INPUT);
  pinMode (IN_D0, INPUT);
  pinMode(LF, OUTPUT);
  pinMode(LB, OUTPUT);
  pinMode(RB, OUTPUT);
  pinMode(RF, OUTPUT);
  Serial.begin(9600);
}

int value_A0;
bool value_D0;
char state = '*';

void loop() {

  value_A0 = analogRead(IN_A0); // reads the analog input from the IR distance sensor
  value_D0 = digitalRead(IN_D0);// reads the digital input from the IR distance sensor

  if(value_D0)
  {
    changeState('w');
  }
  else
  {
    changeState('*');
    time = millis();
    changeState('d');
    while(!digitalRead(IN_D0) && (millis() - time) < RESEARCHTIME);
    changeState('*');
    if(!digitalRead(IN_D0))
    {
      changeState('a');
      while(!digitalRead(IN_D0));
      changeState('*');
    }
  }
  
 // Serial.print("AN:");
 // Serial.println(value_A0); // prints analog value on the LCD module
  Serial.print("DIG:");
  Serial.println(value_D0); // prints digital value on the LCD module
  
}
void motorManage(bool rotate)
{
  int vSpeed = rotate ? manouvreSpeed : cruiseSpeed;
  //Serial.println(vSpeed);
  analogWrite(LF, lf ? vSpeed : 0); 
  analogWrite(LB, lb ? vSpeed : 0);
  analogWrite(RB, rb ? vSpeed : 0);      
  analogWrite(RF, rf ? vSpeed : 0);
}

void changeState(char c)
{
  if(c == 'w')
  {
    if(state == '*')
    {
      lf = rf = true;
      lb = rb = false;
      motorManage(false);
      state = 'w';
    }
    else if(state == 's')
    {
      lf = rf = lb = rb = false;
      motorManage(false);
      state = '*';
    }
  }
  else if(c == 's')
  {
    if(state == '*')
    {
      lb = rb = true;
      lf = rf = false;
      motorManage(false);
      state = 's';
    }
    else if(state == 'w')
    {
      lf = rf = lb = rb = false;
      motorManage(false);
      state = '*';
    }
  }
  else if(c == 'a')
  {
    if(state == '*')
    {
      rf = lb = true; 
      rb = lf = false;
      motorManage(true);
      state = 'a';
    }
    else if(state == 'd')
    {
      lf = rf = lb = rb = false;
      motorManage(true);
      state = '*';
    }
  }
  else if(c == 'd')
  {
    if(state == '*')
    {
      lf = rb = true;
      lb = rf = false;
      motorManage(true);
      state = 'd';
    }
    else if(state == 'a')
    {
      lf = rf = lb = rb = false;
      motorManage(true);
      state = '*';
    }
  }
  else if(c == '*') 
  {
    lf = rf = lb = rb = false;
    motorManage(true);
    state = '*';
  }
  else if(c == '+')
  {
    cruiseSpeed = cruiseSpeed != 255 ? cruiseSpeed + 5 : 255;
    motorManage(false);
  }
  else if (c == '-')
  {
    cruiseSpeed = cruiseSpeed != 60 ? cruiseSpeed - 5 : 60;
    motorManage(false);
  }
}


