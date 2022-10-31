//Includo libreria SoftwareSerial
#include <SoftwareSerial.h>

//definisco pin RX e TX da Arduino verso modulo BT
#define BT_TX_PIN 12
#define BT_RX_PIN 11

//L298 Connection   
const int LF = 5;  // Pin  in1 of L298
const int LB = 6;  // Pin  in2 of L298
const int RB = 9; // Pin in3 of L298
const int RF = 10;  // Pin in4 of L298

int cruiseSpeed = 100;     // Default speed, from 0 to 255
int manouvreSpeed = 80;
char c;
char state = '*';

bool lf = false, rf = false, lb = false, rb = false;

//istanzio oggetto SoftwareSerial (il nostro futuro bluetooth)
SoftwareSerial bt =  SoftwareSerial(BT_RX_PIN, BT_TX_PIN);

void setup() {
  // Set pins as outputs:
  pinMode(LF, OUTPUT);
  pinMode(LB, OUTPUT);
  pinMode(RB, OUTPUT);
  pinMode(RF, OUTPUT);

  // Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  //defining bluethoot pin mode
  pinMode(BT_RX_PIN, INPUT);
  pinMode(BT_TX_PIN, OUTPUT);

  //inizializzo comunicazione Bluetooth
  bt.begin(9600);

}

void loop() {

  if(Serial.available() > 0 || bt.available() > 0)
  {
    if(bt.available() > 0)
      c = bt.read();
    else if(Serial.available() > 0)
      c = Serial.read();
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
    else if(c == '+'){
      cruiseSpeed = cruiseSpeed != 255 ? cruiseSpeed + 5 : 255;
      motorManage(false);
    }
    else if (c == '-'){
      cruiseSpeed = cruiseSpeed != 60 ? cruiseSpeed - 5 : 60;
      motorManage(false);
    }
  }  
}

void motorManage(bool rotate)
{
  int vSpeed = rotate ? manouvreSpeed : cruiseSpeed;
  Serial.println(vSpeed);
  analogWrite(LF, lf ? vSpeed : 0); 
  analogWrite(LB, lb ? vSpeed : 0);
  analogWrite(RB, rb ? vSpeed : 0);      
  analogWrite(RF, rf ? vSpeed : 0);
}
