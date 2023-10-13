#include <PubSubClient.h>
#include <secrets.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>




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





String stateTopic = "home/plants/" + String(1) + "/state";
void sendMQTTTemperatureDiscoveryMsg() {
  String discoveryTopic = "homeassistant/sensor/plant_sensor_" + String(1) + "/temperature/config";

  DynamicJsonDocument doc(1024);
  char buffer[256];

  doc["name"] = "Plant " + String(1) + " Temperature";
  doc["stat_t"]   = stateTopic;
  doc["unit_of_meas"] = "°C";
  doc["dev_cla"] = "temperature";
  doc["frc_upd"] = true;
  doc["val_tpl"] = "{{ value_json.temperature|default(0) }}";

  size_t n = serializeJson(doc, buffer);

  mqttClient.publish(discoveryTopic.c_str(), buffer, n);
  mqttClient.publish(discoveryTopic.c_str(), buffer, n);
}

void sendMQTTTemperatureMSG(){

 DynamicJsonDocument doc(1024);
    char buffer[256];

    // doc["humidity"] = humidity;
    doc["temperature"]   = 23.5;
    // doc["moisture"] = moisturePercentage;

    size_t n = serializeJson(doc, buffer);

    bool published = mqttClient.publish(stateTopic.c_str(), buffer, n);


}



//invio il messaggio ogni tot di tempo
void SendMQTTMSG(){
  static unsigned long TempoPrecedente;
  unsigned long TempoAttuale = millis();
  if(TempoAttuale -TempoPrecedente > 3000){
    TempoPrecedente = TempoAttuale ;
    sendMQTTTemperatureMSG();

}


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
sendMQTTTemperatureDiscoveryMsg();


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
  SendMQTTMSG();




}





// // My numeric sensor ID, you can change this to whatever suits your needs
// int sensorNumber = 1

// // This is the topic this program will send the state of this device to.
// String stateTopic = "home/plants/" + String(sensorNumber) + "/state";

// void sendMQTTTemperatureDiscoveryMsg() {
//   // This is the discovery topic for this specific sensor
//   String discoveryTopic = "homeassistant/sensor/termostato_casa_" + String(sensorNumber) + "/temperature/config";

//   DynamicJsonDocument doc(1024);
//   char buffer[256];

//   doc["name"] = "Plant " + String(sensorNumber) + " Temperature";
//   doc["stat_t"]   = stateTopic;
//   doc["unit_of_meas"] = "°C";
//   doc["dev_cla"] = "temperature";
//   doc["frc_upd"] = true;
//   // I'm sending a JSON object as the state of this MQTT device
//   // so we'll need to unpack this JSON object to get a single value
//   // for this specific sensor.
//   doc["val_tpl"] = "{{ value_json.temperature|default(0) }}";

//   size_t n = serializeJson(doc, buffer);

//   client.publish(discoveryTopic.c_str(), buffer, n);
// }


#endif //MQTT_H