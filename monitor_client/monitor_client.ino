#include <sim900_Suli.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "Suli.h"
#include "DHT11.h"
#define DHT11PIN 2
const int pin_tx = 7;
const int pin_rx = 8;
SoftwareSerial gprs(pin_tx,pin_rx);//TX,RX

//char http_cmd[162];
dht11 DHT11;
void setup(){
  Serial.begin(9600);
  sim900_init(&gprs, -1, 9600);
  int a = DHT11.read(DHT11PIN);

  Serial.print("Temperature (oC): ");
  Serial.println((float)DHT11.temperature, 2);
  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);
  //snprintf(http_cmd,sizeof(http_cmd),"GET monitor.ourjnu.com/submit.php?device_index=13726247339&longitude=113.540718&latitude=22.256467&temperature=%03d&humidity=%03d&particulate_matter=25 HTTP/1.0\r\n\r\n",DHT11.temperature,DHT11.humidity);
  //sprintf(http_cmd,"GET /submit.php?device_index=13726247339&longitude=113.540718&latitude=22.256467&temperature=%03d&humidity=%03d&particulate_matter=25 HTTP/1.0\r\n\r\n",DHT11.temperature,DHT11.humidity);
  //Serial.println(http_cmd);
  while(!gprs.available()) {
      sim900_send_cmd("AT+CGATT?");
      delay(2000);
  }
  Serial.println(gprs.read());
}

void loop(){
  /*
  while(!gprs.available()) {
      gprs.write("AT+CGATT?");
      delay(2000);
  }
  Serial.println(gprs.read());*/
  delay(100000);
}




