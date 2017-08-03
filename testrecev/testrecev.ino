#include <SPI.h>
    #include "nRF24L01.h"
    #include "RF24.h"
 
    RF24 radio(9,10);
 
    const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
 
    typedef enum { role_ping_out = 1, role_pong_back } role_e;
 
    const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};
 
    role_e role = role_pong_back;
 
    void setup(void)
    {
    Serial.begin(57600);
    radio.begin();
    radio.setRetries(15,15);
    radio.openReadingPipe(1,pipes[1]);
    radio.startListening();
    radio.printDetails();
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
    radio.startListening();
    
    } 
 
    void loop(void)
    {
 
    if ( radio.available() )
    {
      
    long int joystick[4];
    radio.read( &joystick, sizeof(joystick) );
    //radio.read( &joystick, sizeof(joystick[1]) );
    Serial.print("T : ");
    Serial.print(joystick[0]);                                 
    Serial.print(" ");
    Serial.print("Y : ");
    Serial.print(joystick[1]);
    Serial.print(" ");
    Serial.print("P : ");
    Serial.print(joystick[2]);
    Serial.print(" ");
    Serial.print("R : ");
    Serial.println(joystick[3]);
    //keep joystick pins pointing towards pilot
    // order of reading is : throttle , yaw , pitch ,roll 
    delay(20);
    }
    }
