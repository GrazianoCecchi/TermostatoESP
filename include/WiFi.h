//#include <Arduino.h>



#ifndef WIFI_H
#define WIFI_H


#include <ESP8266WiFi.h>
#include <secrets.h>



// Set your Static IP address
IPAddress local_IP(192, 168, 1, 34);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional



//--------------------------------------
// function setup_wifi called once
//--------------------------------------
void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connessione a ");
  delay(10000);
  Serial.println(ssid);
  Serial.println("Configuro la WiFi");
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Fallita la configurazione");
  }
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  ;
}

void loopWiFi(){
  static unsigned long TempoPrecedente;
  unsigned long TempoAttuale = millis();
    if(TempoAttuale -TempoPrecedente > 5000){
    TempoPrecedente = TempoAttuale ;
    Serial.print("Questo è un ESP8266, ");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}


#endif //WIFI_H
