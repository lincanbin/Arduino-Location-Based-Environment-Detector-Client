//Below program comes from http://www.geek-workshop.com/thread-287-1-1.html
//According to that page I have fiexed some bugs :
// 1. " rc = twi_writeTo(addr, &data, 0, 1);"
// 2. addr = addr<<1;  This line should be removed
// I have tested below code with my I2C LCD.

#include "Wire.h"
extern "C" {
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}

byte start_address = 1;
byte end_address = 127;

// Scan the I2C bus between addresses from_addr and to_addr.
// On each address, call the callback function with the address and result.
// If result==0, address was found, otherwise, address wasn't found
// (can use result to potentially get other status on the I2C bus, see twi.c)
// Assumes Wire.begin() has already been called
void scanI2CBus(byte from_addr, byte to_addr, 
                void(*callback)(byte address, byte result) ) 
{
  byte rc;
  byte data = 0; // not used, just an address to feed to twi_writeTo()
  for( byte addr = from_addr; addr <= to_addr; addr++ ) {
    rc = twi_writeTo(addr,&data,0,1,1);
    if(rc==0) callback( addr, rc );
  }
}

// Called when address is found in scanI2CBus()
// Feel free to change this as needed
// (like adding I2C comm code to figure out what kind of I2C device is there)
void scanFunc( byte addr, byte result ) {
  Serial.print("addr: ");
  Serial.print(addr,DEC);
  //ZT Debug addr = addr<<1;
  Serial.print("\t HEX: 0x");
  Serial.print(addr,HEX);
  Serial.println( (result==0) ? "\t found!":"   ");
//  Serial.print( (addr%4) ? "\t":"\n");
}

// standard Arduino setup()
void setup()
{
    pinMode(13,OUTPUT);
    Wire.begin();

    Serial.begin(9600);
    Serial.println("--- I2C Bus Scanner Test ---");
    Serial.print("starting scanning of I2C bus from ");
    Serial.print(start_address,DEC);
    Serial.print(" to ");
    Serial.print(end_address,DEC);
    Serial.println("...");
    Serial.println();

    // start the scan, will call "scanFunc()" on result from each address
    scanI2CBus( start_address, end_address, scanFunc );

    Serial.print("\n");
    Serial.println("--- I2C Bus Scanner Complete ---");

    digitalWrite(13,HIGH); delay(500); digitalWrite(13,LOW); delay(500);
    digitalWrite(13,HIGH); delay(500); digitalWrite(13,LOW); delay(500);
    digitalWrite(13,HIGH); delay(500); digitalWrite(13,LOW); delay(500);
}

void loop() 
{
}
