#include <PubSubClient.h>
#include <secrets.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>



#ifndef MQTT_H
#define MQTT_H

// file che contiene il certificato TLS
// #include "certificate.h" //da abilitare e creare il file per abilitare la comunicazione TLS

#define CERT mqtt_broker_cert
#define MSG_BUFFER_SIZE (50)

//--------------------------------------
// Configurazione (modificare prima della compilazione)
//--------------------------------------
//#define MQTT_TLS // se non commentato abilita la comunicazione TLS
//#define MQTT_TLS_VERIFY // se non commentato abilita la verifica TLS della connessione con il brocker

const uint16_t mqtt_server_port = 1883; // o 8883 sono moltoi usate 
const char* mqttTopicIn = "Termostato-in";
const char* mqttTopicOut = "Termostato-out";



//--------------------------------------
// variabili globali
//--------------------------------------
#ifdef MQTT_TLS
  WiFiClientSecure wifiClient;
#else
  WiFiClient wifiClient;
#endif
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
PubSubClient mqttClient(wifiClient);



//--------------------------------------
// funzione per connettere o riconnettere il brocker
// mqtt
//--------------------------------------
void connect() {
  while (!mqttClient.connected()) {
    Serial.print("Attendo  la connessione MQTT ...");
    String mqttClientId = "";
    if (mqttClient.connect(mqttClientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("    connesso");
      mqttClient.subscribe(mqttTopicIn);
    } else {
      Serial.print("Fallito, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" ritento fra 5 secondi");
      delay(5000);
    }
  }
}








//--------------------------------------
// funzione callback che viene lanciata ogni volta che riceve un messaggio
// inserisci qua il tuo codice
//--------------------------------------
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Messagio arrivato su topic: '");
  Serial.print(topic);
  Serial.print("' con payload: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  String myCurrentTime = timeClient.getFormattedTime();
  mqttClient.publish(mqttTopicOut,("ESP8266: Risponde: " + myCurrentTime).c_str());
}




//--------------------------------------
// sub che deve essere eseguita ad durante il set up
// inserisci qui il tuo codice
//--------------------------------------
void setup_mqtt(){
    mqttClient.setServer(mqtt_server, mqtt_server_port);
    mqttClient.setCallback(callback);

      timeClient.begin();

#ifdef MQTT_TLS
  #ifdef MQTT_TLS_VERIFY
    X509List *cert = new X509List(CERT);
    wifiClient.setTrustAnchors(cert);
  #else
    wifiClient.setInsecure();
  #endif
#endif
}

//--------------------------------------
// sub che deve essere eseguita ad ogni loop
// inserisci qui il tuo codice
//--------------------------------------
void loop_mqtt(){
    if (!mqttClient.connected()) {
        connect();
    }
 
    mqttClient.loop();
    timeClient.update();
}


#endif //MQTT_H