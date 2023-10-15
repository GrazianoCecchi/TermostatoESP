//#include <Arduino.h>
#ifndef WIFI_H
#define WIFI_H


#include <ESP8266WiFi.h>
#include <secrets.h>

/*-------------------------------------------------------------------------------------------------------------------------------------*/
//TODOP------------------------------------------------ Configurazione ----------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------*/
IPAddress           local_IP(192,168,1,134);    //!IP della scheda MODIFICARE
IPAddress           gateway(192, 168, 1, 1);    
IPAddress           subnet(255, 255, 255, 0);
IPAddress           primaryDNS(8, 8, 8, 8);   //opzionale
IPAddress           secondaryDNS(8, 8, 4, 4); //opzionale






/*-------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------- Variabiloi pubbliche --------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------*/
String              IDUnico;




// //--------------------------------------
// // function setup_wifi called once
// //--------------------------------------
// void setup_wifi() {

//   delay(10);
//   Serial.println();
//   Serial.print("Connessione a ");
//   delay(10000);
//   Serial.println(ssid);
//   Serial.println("Configuro la WiFi");
//   if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
//     Serial.println("STA Fallita la configurazione");
//   }
//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   ;
// }

void loopWiFi(){
  static unsigned long TempoPrecedente;
  unsigned long TempoAttuale = millis();
    if(TempoAttuale -TempoPrecedente > 10000){
    TempoPrecedente = TempoAttuale ;
    Serial.println("Questo è un ESP8266");
    Serial.print("Indentificativo unico: ");
    Serial.println(IDUnico); 
    Serial.print("Indirizzo IP: ");
    Serial.println(WiFi.localIP());
  }
}




/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------ Funzioni pubbliche -----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
void setup_wifi() 
{
    int counter = 0;
    byte mac[6];
    delay(10);
    // We start by connecting to a WiFi network
    Serial.print("Connessione a: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    WiFi.macAddress(mac);
    IDUnico =  String(mac[0],HEX) +String(mac[1],HEX) +String(mac[2],HEX) +String(mac[3],HEX) + String(mac[4],HEX) + String(mac[5],HEX);

    Serial.print("Indentificativo unico: ");
    Serial.println(IDUnico);    
   
    while(WiFi.status() != WL_CONNECTED && counter++ < 8) 
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("");

    if(WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WiFi connessa");
        Serial.print("Indirizzo IP: ");
        Serial.println(WiFi.localIP());
    } else
    {
        Serial.println("WiFi NON CONNESSA !!!");
    }
}






















#endif //WIFI_H
