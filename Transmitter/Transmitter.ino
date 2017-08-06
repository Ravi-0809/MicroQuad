#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

 #include <SPI.h>
 #include "nRF24L01.h"
// #include "RF24.h"

 long int joystick[4];
    RF24 radio(7,8);

 int ledPin = 13;
 int joyPin1 = 0;      //ROLL           
 int joyPin2 = 1;      //PITCH
 int joypin3 = 2;      //THROTTLE
 int joypin4 = 3;      //YAW
 
// long int map10(long int a)
//{
//  long int near10;
//  int d;
//  if(a != 1023)   
//  {
//    d = (a/100);
//    near10 = d*100;   
//  }
//  else
//      near10=1023;
//  a=near10;
//  
//  return a;
//  
//}
              
    const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
    void setup()
    {
    Serial.begin(57600);
    pinMode(ledPin, OUTPUT);              // initializes digital pins 0 to 7 as outputs
 
    radio.begin();
    radio.setRetries(15,15);
    radio.openReadingPipe(1,pipes[1]);
    radio.startListening();
    radio.printDetails();
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
    radio.stopListening();
    }

 
    void loop(void)
    {
      
     joystick[0] = analogRead(joyPin1);               
     joystick[1] = analogRead(joyPin2);   
     joystick[2]=analogRead(joypin3);
     joystick[3]=analogRead(joypin4);

     Serial.print(joystick[0]);
     Serial.print(" ");
     Serial.print(joystick[1]);
     Serial.print(" ");
     Serial.print(joystick[2]);
     Serial.print(" ");
     Serial.println(joystick[3]);
            
     radio.stopListening();
 //    int number = 10; 
     
     radio.write(&joystick,sizeof(joystick));  
 
     radio.startListening();
 
     delay(10);
    }
