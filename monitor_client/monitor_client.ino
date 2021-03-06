#include "GPRS_Shield_Arduino.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include "Suli.h"

//I2C Port expander and LCDs
#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x20, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//LCD address can be read in the program I2CScanner

/*
--- I2C Bus Scanner Test ---
starting scanning of I2C bus from 1 to 127...

addr: 32     HEX: 0x20   found!

--- I2C Bus Scanner Complete ---

*/

//DHT11
#include "DHT11.h"
#define DHT11PIN 2
dht11 DHT11;

//DSM501A
int pin = 3;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 30000;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

//SIM900
GPRS gprs(7, 8, 9600,"cmnet");
char buffer[128];
//Debug in PC
#define is_debug true

void setup(){

    Serial.begin(9600);

    //initialize DSM501A
    pinMode(3,INPUT);
    starttime = millis();

    //initialize I2C LCD-1602
    lcd.init();
    lcd.backlight();
    //initialize gprs
    if(!is_debug){
        gprs.init();
        // attempt DHCP
        while(false == gprs.join()) {
            Serial.println("gprs join network error");
            delay(2000);
        }
    
        // successful DHCP
        Serial.print("IP Address is ");
        Serial.println(gprs.getIPAddress());
    }
}

void loop(){
    DHT11.read(DHT11PIN);
/*
    Serial.print("Temperature (oC): ");
    Serial.println((float)DHT11.temperature, 2);
    Serial.print("Humidity (%): ");
    Serial.println((float)DHT11.humidity, 2);
*/
    duration = pulseIn(pin, LOW);
    lowpulseoccupancy = lowpulseoccupancy+duration;
      
    if ((millis()-starttime) > sampletime_ms)
    {
        ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
        concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
        lowpulseoccupancy = 0;
        starttime = millis();
     
  
    //snprintf(http_cmd,sizeof(http_cmd),"GET monitor.ourjnu.com/submit.php?device_index=13726247339&longitude=113.540718&latitude=22.256467&temperature=%03d&humidity=%03d&particulate_matter=25 HTTP/1.0  \r\n\r\n",DHT11.temperature,DHT11.humidity);
    //sprintf(http_cmd,"GET /submit.php?device_index=13726247339&longitude=113.540718&latitude=22.256467&temperature=%03d&humidity=%03d&particulate_matter=25 HTTP/1.0  \r\n\r\n",DHT11.temperature,DHT11.humidity);
    //Serial.println(http_cmd);
    if(!is_debug){
        //Use rewrited url to prevent memory overflow
        char target_url[68];
        snprintf(target_url, sizeof(target_url), "GET /13726247339-113.540718-22.256467-%03d-%03d-%07.2f HTTP/1.0  \r\n\r\n",DHT11.temperature, DHT11.humidity, ratio);
        Serial.println(target_url);
        
        if(false == gprs.connect(TCP,"monitor.ourjnu.com", 80)) {
            Serial.println("connect error");
            }else{
            Serial.println("connect monitor.ourjnu.com success");
        }
    
        Serial.println("waiting to fetch...");
        gprs.send(target_url, sizeof(target_url)-1);
        while (true) {
            int ret = gprs.recv(buffer, sizeof(buffer)-1);
            if (ret <= 0){
                Serial.println("fetch over...");
                break;
            }
            buffer[ret] = '\0';
            Serial.print("Recv: ");
            Serial.print(ret);
            Serial.print(" bytes: ");
            Serial.println(buffer);
        }
        //gprs.close();
        //gprs.disconnect();
    }else{
        //Serial.print(lowpulseoccupancy);
        // Serial.print(",");
        Serial.print(ratio);
        Serial.print(",");
        //Serial.println(concentration);
        Serial.print((float)DHT11.temperature, 2);
        Serial.print(",");
        Serial.println((float)DHT11.humidity, 2);

        lcd.clear();
        //lcd.backlight();
        lcd.print(ratio);
        lcd.setCursor(8,0);
        lcd.print((float)DHT11.temperature, 2);
        lcd.setCursor(0,1);
        lcd.print((float)DHT11.humidity, 2);
    }
    
    }
    /*
    while(!gprs.available()) {
    gprs.write("AT+CGATT?");
    delay(2000);
    }
    Serial.println(gprs.read());
    */
}
