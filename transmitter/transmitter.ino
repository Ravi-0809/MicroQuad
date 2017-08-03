#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
//long int current_throt;
long int joystick[4];
RF24 radio(9,10);

 int ledPin = 13;
 int joyPin1 = 0;                 
 int joyPin2 = 1;      
 int joypin3 = 2;
 int joypin4 = 3; 

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
    //Serial.begin(9600);
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
 
     radio.write(joystick , sizeof(joystick));
     
    
       
     radio.startListening();
 
     delay(10);
    }
