// ard-gymk-big7deg_servo-diplay

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

boolean chars[11][7] = {
{1,1,1,1,1,1,0}, //0
{0,1,1,0,0,0,0}, //1
{1,1,0,1,1,0,1}, //2
{1,1,1,1,0,0,1}, //3
{0,1,1,0,0,1,1}, //4
{1,0,1,1,0,1,1}, //5
{1,0,1,1,1,1,1}, //6
{1,1,1,0,0,0,0}, //7
{1,1,1,1,1,1,1}, //8
{1,1,1,1,0,1,1}, //9
{0,0,0,0,0,0,0} //10 pusty  
};

const byte numChars = 32;
byte receivedBytes[numChars];
byte sendBytes[numChars];
boolean newData = false;
int rcvLenght = 0;
int cyfra = 10;

const int pinA = 9; 
const int pinB = 8; 
const int pinC = 7; 
const int pinD = 6; 
const int pinE = 5; 
const int pinF = 4; 
const int pinG = 3; 

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinE, OUTPUT);
  pinMode(pinF, OUTPUT);
  pinMode(pinG, OUTPUT);
}

void loop() {
  receive();
  if (newData == true){
  naLED();
    send();
    newData = false;
  }
}

void receive() {
  static boolean receiveInProgress = false;
  static byte ndx = 0;
  byte startMarker = 60; //znak <
  byte endMarker = 62; //znam >
  byte rc;
  
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();
    if (receiveInProgress == true)
    {
      if (rc != endMarker)
      {
        receivedBytes[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {ndx = numChars -1;}       
      }
      else
      {
        receiveInProgress = false;
        cyfra = receivedBytes[ndx-1]-48;
        rcvLenght = ndx;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc == startMarker)
    {
      receiveInProgress = true; 
      for (int i=0; i<numChars;i++){
      receivedBytes[i] = 0;
      sendBytes[i] = 0;
    }
      rcvLenght = 0;      
    }
  }
}

void send() {
  if (rcvLenght>1) {
  sendBytes[0] = 60;
  for(int i=0; i<rcvLenght; i++){
    sendBytes[i+1] = receivedBytes[i];
  }
  sendBytes[rcvLenght]=62;
  }
  else if (rcvLenght=1){
  sendBytes[0] = 60;
  sendBytes[1] = 58;
  sendBytes[2]= 62;
  }
  mySerial.write((char*)sendBytes);
}

void naLED() {
digitalWrite(pinA, chars[cyfra][0]);
digitalWrite(pinB, chars[cyfra][1]);
digitalWrite(pinC, chars[cyfra][2]);
digitalWrite(pinD, chars[cyfra][3]);
digitalWrite(pinE, chars[cyfra][4]);
digitalWrite(pinF, chars[cyfra][5]);
digitalWrite(pinG, chars[cyfra][6]);  
}
