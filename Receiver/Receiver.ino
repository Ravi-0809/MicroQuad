#include <SPI.h>
    #include "nRF24L01.h"
    #include "RF24.h"
 
    RF24 radio(3,10);

    #define CHANNEL_NUMBER 4
    #define CHANNEL_DEFAULT_VALUE 1200
    #define FRAME_LENGTH 22500
    #define PULSE_LENGTH 300
    #define onState 1
    #define sigPin 3
    
    int ppm[CHANNEL_NUMBER];
     
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

    cli();
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;
  
  OCR1A = 100;  // compare match register, change this
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();
    
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

    for(int j=0;j<4;j++)
      {
        ppm[j]=map(joystick[j],0,1023,1000,2000);
      }
    
    delay(20);
    }
    }

    ISR(TIMER1_COMPA_vect){  //leave this alone
  static boolean state = true;
  
  TCNT1 = 0;
  
  if (state) {  //start pulse
    digitalWrite(sigPin, onState);
    OCR1A = PULSE_LENGTH * 2;
    state = false;
  } else{  //end pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;
  
    digitalWrite(sigPin, !onState);
    state = true;

    if(cur_chan_numb >= CHANNEL_NUMBER){
      cur_chan_numb = 0;
      calc_rest = calc_rest + PULSE_LENGTH;// 
      OCR1A = (FRAME_LENGTH - calc_rest) * 2;
      calc_rest = 0;
    }
    else{
      OCR1A = (ppm[cur_chan_numb] - PULSE_LENGTH) * 2;
      calc_rest = calc_rest + ppm[cur_chan_numb];
      cur_chan_numb++;      
    }     
  }
}
