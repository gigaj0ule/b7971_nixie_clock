#include <SPI.h>              // We use this library, so it must be called here.
#include <MCP23S17.h>         // Here is the new class to make using the MCP23S17 easy.
#include <ctype.h>
#include <TimeLib.h>
#include "TinyGPS++.h"

#define PIN_digit_one     14
#define PIN_digit_two     15
#define PIN_digit_three   16
#define PIN_digit_four    17

#define FLAG_clock_mode          true
#define FLAG_display_seconds_too false
#define FLAG_underscore_seconds  true
#define FLAG_use_gps_clock       true

#define UTC_OFFSET -8
#define MINUTE 20
#define HOUR   24

// Header tag for serial time sync message
#define TIME_HEADER  "T"   

// GPS
#define GPS_BAUD          9600
#define GPS_PERIPHERAL    Serial1
TinyGPSPlus gps_;

// Instantiate an object called "mcp" on an MCP23S17 device at address 0
MCP mcp(0);            
int ascii[255] = {};

String message_1     = "abcdefghijklmnopqrstuvwxyz";
String message_2     = "";
String inputString   = "";         // a string to hold incoming data
String statement;

boolean stringComplete = false;  // whether the string is complete

// Because we want non negative mod results
inline int mod(int a, int b) {int ret = a%b; return ret>=0? ret: ret+b; }

void setup() {
  setSyncProvider(getTeensy3Time);

  Serial.begin(115200);
  GPS_PERIPHERAL.begin(GPS_BAUD);

  if(FLAG_clock_mode && FLAG_use_gps_clock){

    Serial.print("Looking for GPS device on Serial1");
    while(!GPS_PERIPHERAL.available()){
      Serial.print(".");
    }
    Serial.println("\nFound!");
    Serial.flush();
    GPS_PERIPHERAL.flush();
  }
   
  inputString.reserve(200);

  pinMode(PIN_digit_one,   OUTPUT);
  pinMode(PIN_digit_two,   OUTPUT);
  pinMode(PIN_digit_three, OUTPUT);
  pinMode(PIN_digit_four,  OUTPUT);
  
  make_font_table();
            
  delay(150);

  byte i = 0;
  
  while( i < 10 ){ // No I really mean init
    delay(50);
    NIXIE_WRITE_broadcast_begin();
    mcp.byteWrite(0x0A, 0b00000000);  ///ICONA , NO HW Addressing
    NIXIE_WRITE_broadcast_end();
    i++;
  };
  
  NIXIE_WRITE_broadcast_begin();
  mcp.pinMode(0x0000);
  NIXIE_WRITE_broadcast_end();
  
  ANIMATION_init_test(); 
  
  if(FLAG_clock_mode){    
    //setTime(12,0,0, 0,0,0);
    //hourFormat12();
  }
};



void loop() {
  if(!FLAG_clock_mode){
    if(Serial.available()){
      boolean firstchar_not_dumped = true;
      
      while (Serial.available()) {
        char inChar = (char)Serial.read(); 
        if(firstchar_not_dumped){
           firstchar_not_dumped = false;
           inChar = (char)Serial.read(); 
        }
  
        if (inChar == '\n') {
           Serial.println(inputString); 
           message_1 = inputString;
           inputString = "";
        }
        inputString += inChar;
      }
    }
    NIXIE_WRITE_scroll_message(message_1, 200);
    delay(1000);
  }
  
  else
  {
    if(FLAG_use_gps_clock)
    {
      if(GPS_PERIPHERAL.available())
      {
        // Make sure read buffer is full
        delay(25);
        while(GPS_PERIPHERAL.available())
        {
          char c = GPS_PERIPHERAL.read();
          //Serial.print(c);
          gps_.encode(c);
        }
        GPS_PERIPHERAL.flush();
      }
     
      if(gps_.location.isValid())
      {     
        // Pass GPS into system state
        //state.lastKnownHour = gps_.time.hour();
        //state.lastKnownMinute = gps_.time.minute();
        //state.lastKnownSecond = gps_.time.second();  
        //state.lastKnownDay = gps_.date.day();
        //state.lastKnownMonth = gps_.date.month();
        //state.lastKnownYear = gps_.date.year();

        Serial.print("Got GPS Fix, I see ");        
        Serial.print(gps_.satellites.value());
        Serial.println(" satellites");
        Serial.print("UTC time is ");  
        Serial.print(gps_.time.hour());
        Serial.print(":");        
        Serial.print(gps_.time.minute());              
        Serial.print(":");        
        Serial.println(gps_.time.second());
        
        int local_hour = mod((gps_.time.hour() + UTC_OFFSET), 12);

        if(minute() != gps_.time.minute() || hour() != local_hour)
        {
          setTime(local_hour, gps_.time.minute(), gps_.time.second(), gps_.date.day(), gps_.date.month(), gps_.date.year());                  
        }

        Serial.print("Local time is ");  
        Serial.print(local_hour);
        Serial.print(":");        
        Serial.print(gps_.time.minute());              
        Serial.print(":");        
        Serial.println(gps_.time.second());      
      }
      else
      {
        Serial.print("No GPS fix, I see ");
        Serial.print(gps_.satellites.value());
        Serial.println(" satellites");      
      }

      Serial.println();
    } 
      
    if(Serial.available())
    {   
      statement = '0';    
      while(Serial.available())
      {
        char i = Serial.read();
            
        if(!isdigit(i))
        {
          Serial.println("NACK");
          break;
        };
        statement += i;
      }       
      if(statement.toInt() < 2500)
      {
        int hrs  = statement.toInt()/100;
        int mins = statement.toInt()%100;
          
        setTime(hrs, mins, second(),0,0,0);
          
        Serial.println(statement);      
      }  
    }
        
    if (Serial.available())
    {
      time_t t = processSyncMessage();
      if (t != 0)
      {
        Teensy3Clock.set(t); // set the RTC
        setTime(t);
      }
    }
    
    int hourModified = hour();
      
    if(hourModified > 12)
    {
      hourModified -= 12;
    };
      
    if(hourModified == 0)
    {
      hourModified = 12;
    }
      
    int Hour_Ones = (hourModified%10);   
    int Hour_Tens = ((hourModified/10)%10)%12;
    
    int Min_Ones = (minute()%10);
    int Min_Tens = ((minute()/10)%10);
        
    int Sec_Ones = (second()%10);
    int Sec_Tens = ((second()/10)%10);
      
    byte underscore_byte = 0b00000000;
      
    if(FLAG_underscore_seconds)
    {
      switch(Sec_Ones)
      {
        case 1:
          underscore_byte = 0b00001000;
          break;
        case 2:
          underscore_byte = 0b00000100;
          break;
        case 3:
          underscore_byte = 0b00000010;
          break;
        case 4:
          underscore_byte = 0b00000001;
          break; 
        case 5:
          underscore_byte = 0b00000000;
          break;
        case 6:
          underscore_byte = 0b00000001;
          break;
        case 7:
          underscore_byte = 0b00000010;
          break;
        case 8:
          underscore_byte = 0b00000100;
          break;
        case 9:
          underscore_byte = 0b00001000;
          break; 
        case 0:
          underscore_byte = 0b00000000;
          break;        
      }
    };
    
    NIXIE_WRITE_letter(1, Hour_Tens+48, underscore_byte & 0b1000);
    NIXIE_WRITE_letter(2, Hour_Ones+48, underscore_byte & 0b0100);
  
    NIXIE_WRITE_letter(3, Min_Tens+48, underscore_byte  & 0b0010);
    NIXIE_WRITE_letter(4, Min_Ones+48, underscore_byte  & 0b0001);    
      
    delay(750);  
    
    if(FLAG_display_seconds_too)
    {
      NIXIE_WRITE_letter(3, ' ', 0);
      NIXIE_WRITE_letter(4, ' ', 0);
    
      delay(25);

      NIXIE_WRITE_letter(3, Sec_Tens+48, 0);
      NIXIE_WRITE_letter(4, Sec_Ones+48, 0);
    
      delay(200);
    
      NIXIE_WRITE_letter(3, ' ', 0);
      NIXIE_WRITE_letter(4, ' ', 0);
   
      delay(25);
    }
    else
    {
      delay(250);
    }
  }
};

