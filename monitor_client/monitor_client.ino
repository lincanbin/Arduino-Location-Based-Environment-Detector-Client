#include "GPRS_Shield_Arduino.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include "Suli.h"
#include "DHT11.h"
#define DHT11PIN 2
dht11 DHT11;
GPRS gprs(7, 8, BAUDRATE,"cmnet");
char target_url[61];
char buffer[128];

void setup(){

  Serial.begin(9600);
  Serial.print("Temperature (oC): ");
  Serial.println((float)DHT11.temperature, 2);
  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);
  //snprintf(http_cmd,sizeof(http_cmd),"GET monitor.ourjnu.com/submit.php?device_index=13726247339&longitude=113.540718&latitude=22.256467&temperature=%03d&humidity=%03d&particulate_matter=25 HTTP/1.0\r\n\r\n",DHT11.temperature,DHT11.humidity);
  //sprintf(http_cmd,"GET /submit.php?device_index=13726247339&longitude=113.540718&latitude=22.256467&temperature=%03d&humidity=%03d&particulate_matter=25 HTTP/1.0\r\n\r\n",DHT11.temperature,DHT11.humidity);
  //Serial.println(http_cmd);
  //Use rewrited url to prevent memory overflow
  snprintf(target_url, sizeof(target_url), "GET /13726247339-113.540718-22.256467-%03d-%03d-25 HTTP/1.0\r\n\r\n",DHT11.temperature,DHT11.humidity);
  Serial.println(target_url);


 gprs.init();
  // attempt DHCP
  while(false == gprs.join()) {
      Serial.println("gprs join network error");
      delay(2000);
  }

  // successful DHCP
  Serial.print("IP Address is ");
  Serial.println(gprs.getIPAddress());

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
  gprs.close();
  gprs.disconnect();

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




