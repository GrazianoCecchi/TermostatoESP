#include <PubSubClient.h>
#include <secrets.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>




#ifndef MQTT_H
#define MQTT_H



/*-------------------------------------------------------------------------------------------------------------------------------------*/
//TODOP------------------------------------------------ Configurazione ----------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------*/
const char*         ModelloDispositivo    = "NodeMCU_V3";                            // Modello scheda
const char*         VersioneFirmware      = "1.0";                                    // Versione Firmware
const char*         Produttore            = "Graziano Cecchi";                        // Produttore
String              NomeDispositvo        = "Termostato-casa";                        //! Nome del dispositivo
String              mqttStatusTopic       = "nodemcuv3/" + NomeDispositvo;       // MQTT Topic
const char*         ServerMQTT            = "192.168.1.8";                           // Indirizzo del server MQTT
int                 PortaServerMQTT       = 1883;    


/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------ Variabili pubbliche --------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
WiFiClient          g_WiFiClient;
PubSubClient        g_mqttPubSub(g_WiFiClient);
bool                MQTT_Inizializzato = false;
// unsigned long       g_Time = 0;
// int                 g_count = 0;
// int                 g_input_Door; 
// int                 g_old_input_Door = 0;
// String              g_strDoorStatus;
 int                   g_mqttCounterConn = 0;
// float               g_Humidity = 0.0;
// float               g_Temperature = 0.0;
// bool                MQTT_Inizializzato = true;
String                 IdentificativoUnico;
// bool                g_SendMqttData = false;
// DHT                 g_dht(DHTPIN, DHTTYPE);

void setup_mqtt(){
     g_mqttPubSub.setServer(ServerMQTT, PortaServerMQTT);
    g_mqttPubSub.setCallback(MqttReceiverCallback);
}

void loop_mqtt(){


    if(WiFi.status() == WL_CONNECTED)
    {
        if(!g_mqttPubSub.connected())
            MqttReconnect();
        else
            g_mqttPubSub.loop();
    }

    if(!MQTT_Inizializzato)
    {
        delay(100);
        MQTT_Inizializzato = true;
        Serial.println("Inizializzo connessione MQTT ...");
        MqttHomeAssistantDiscovery();     // Invio i dati per il discoveri
    }


  static unsigned long TempoPrecedente;
  unsigned long TempoAttuale = millis();
  if(TempoAttuale -TempoPrecedente > 5000){
    TempoPrecedente = TempoAttuale ;      
        {
            StaticJsonDocument<200> payload;  
            payload["temp"] = 27.3;

            String strPayload;
            serializeJson(payload, strPayload);

            if(g_mqttPubSub.connected())
            {
                g_mqttPubSub.publish(mqttStatusTopic.c_str(), strPayload.c_str()); 
                Serial.println("MQTT: Send Data!!!");
                Serial.println(" ");
                Serial.println(" ");

            }
        }

}

}

void MqttHomeAssistantDiscovery()
{
    String discoveryTopic;
    String payload;
    String strPayload;
    if(g_mqttPubSub.connected())
    {
        Serial.println("INVIO HOME ASSISTANT DISCOVERY!!!");
        StaticJsonDocument<600> payload;
        JsonObject device;
        JsonArray identifiers;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Temperature
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        discoveryTopic = "homeassistant/sensor/nodemcuv3/" + NomeDispositvo + "_temp" + "/config";
        
        payload["name"] = NomeDispositvo + ".temp";
        payload["uniq_id"] = IdentificativoUnico + "_temp";
        payload["stat_t"] = mqttStatusTopic;
        payload["dev_cla"] = "temperature";
        payload["val_tpl"] = "{{ value_json.temp | is_defined }}";
        payload["unit_of_meas"] = "°C";
        device = payload.createNestedObject("device");
        device["name"] = NomeDispositvo;
        device["model"] = ModelloDispositivo;
        device["sw_version"] = VersioneFirmware;
        device["manufacturer"] = Produttore;
        identifiers = device.createNestedArray("identifiers");
        identifiers.add(IdentificativoUnico);

        serializeJsonPretty(payload, Serial);
        Serial.println(" ");
        serializeJson(payload, strPayload);

        g_mqttPubSub.publish(discoveryTopic.c_str(), strPayload.c_str());

       

    }
}

void MqttReconnect() 
{
    // Loop until we're reconnected
    while (!g_mqttPubSub.connected()  && (g_mqttCounterConn++ < 4))
    {
        Serial.print("Tentativo di connessione MQTT...");
        // Attempt to connect
        if (g_mqttPubSub.connect(NomeDispositvo.c_str(), mqttUser, mqttPassword)) 
        {
            Serial.println("connesso");
            // Subscribe
            g_mqttPubSub.subscribe("homeassistant/status");
            delay(100);
        } else 
        {
            Serial.print("falliuto, rc=");
            Serial.print(g_mqttPubSub.state());
            Serial.println(" ritento fra un secondo");
            delay(1000);
        }
    }  
    g_mqttCounterConn = 0;
}

void MqttReceiverCallback(char* topic, byte* inFrame, unsigned int length) 
{
    Serial.print("Arrivato messaggio con topic: ");
    Serial.print(topic);
    Serial.print(". Messaggio: ");
    byte state = 0;
    String messageTemp;
    
    for (int i = 0; i < length; i++) 
    {
        Serial.print((char)inFrame[i]);
        messageTemp += (char)inFrame[i];
    }
    Serial.println();
  
    if(String(topic) == String("homeassistant/status")) 
    {
        if(messageTemp == "online")
            MqttHomeAssistantDiscovery();
    }
}


#endif //MQTT_H