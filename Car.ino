//Includo libreria SoftwareSerial
#include <SoftwareSerial.h>

//definisco pin RX e TX da Arduino verso modulo BT
#define BT_TX_PIN 12
#define BT_RX_PIN 11

#define BLUETOOTH_MODE false
#define FOLLOW_LINE_MODE true
#define BUTTON_PIN 2

#define LF 5
#define LB 6
#define RB 9
#define RF 10

#define IN_A0 A0  
#define IN_D0 8    

#define IN_A1 A1   
#define IN_D1 4    

#define PROX 3

bool mode = BLUETOOTH_MODE;
bool buttonState;

int value_A0, value_A1;
bool valueL, valueR;

int cruiseSpeed = 100;     // Default speed, from 0 to 255
int manouvreSpeed = 100;
char c;
char state = '*';

bool lf = false, rf = false, lb = false, rb = false;

//istanzio oggetto SoftwareSerial (il nostro futuro bluetooth)
SoftwareSerial bt =  SoftwareSerial(BT_RX_PIN, BT_TX_PIN);

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
    lf = rf = true;
    lb = rb = false;
    motorManage(false);
    state = 'w';
  }
  else if(c == 's')
  {
    lb = rb = true;
    lf = rf = false;
    motorManage(false);
    state = 's';
  }
  else if(c == 'a')
  {
    rf = lb = true; 
    rb = lf = false;
    motorManage(true);
    state = 'a';
  }
  else if(c == 'd')
  {
    lf = rb = true;
    lb = rf = false;
    motorManage(true);
    state = 'd';
  }
  else if(c == '*') 
  {
    lf = rf = lb = rb = false;
    motorManage(true);
    state = '*';
  }
}

void bluetooth_mode(){
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
          changeState('w');
        }
        else if(state == 's')
          changeState('*');
      }
      else if(c == 's')
      {
        if(state == '*')
        {
          changeState('s');
        }
        else if(state == 'w')
          changeState('*');
      }
      else if(c == 'a')
      {
        if(state == '*')
        {
          changeState('a');
        }
        else if(state == 'd')
          changeState('*');
      }
      else if(c == 'd')
      {
        if(state == '*')
        {
          changeState('d');
        }
        else if(state == 'a')
          changeState('*');
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

void tracking_line_mode(){
  value_A0 = analogRead(IN_A0); // reads the analog input from the IR distance sensor
  valueR = digitalRead(IN_D0);// reads the digital input from the IR distance sensor
  value_A1 = analogRead(IN_A1); // reads the analog input from the IR distance sensor
  valueL = digitalRead(IN_D1);// reads the digital input from the IR distance sensor
  
  if(digitalRead(PROX)){
    if(!valueL & !valueR)
      changeState('w');
    else if(valueL)
      changeState('a');
    else if(valueR)
      changeState('d');
  }
  else
    changeState('*');
}

void setup()
{
  // Set pins as outputs:
  pinMode(LF, OUTPUT);
  pinMode(LB, OUTPUT);
  pinMode(RB, OUTPUT);
  pinMode(RF, OUTPUT);

  //defining bluethoot pin mode
  pinMode(BT_RX_PIN, INPUT);
  pinMode(BT_TX_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  //defining sensors pin
  pinMode(PROX,INPUT);
  pinMode(IN_A0, INPUT);
  pinMode(IN_D0, INPUT);
  pinMode(IN_A1, INPUT);
  pinMode(IN_D1, INPUT);

  //inizializzo comunicazione Bluetooth
  bt.begin(9600);
  Serial.begin(9600);
}

void loop()
{
  buttonState = digitalRead(BUTTON_PIN);
  if(buttonState == HIGH)
  {
    mode = mode ? false : true;
    while(digitalRead(BUTTON_PIN) == HIGH);
    changeState('*');
    while(bt.available()) bt.read();
    delay(500);
  }
  if(mode == BLUETOOTH_MODE){
    Serial.println("mode bluetooth");
    bluetooth_mode();
  }  
  else{
    Serial.println("mode tracking line");
    tracking_line_mode();
  }
}