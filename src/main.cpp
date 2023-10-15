#include <Arduino.h>
#include <WiFi.h>
#include <mqtt.h>
#include <OTA.h>







// //Invia un messaggio mqtt ogni 10000 mils 10 secondi
// unsigned long previousMillis = 0;
// const long interval = 10000;

// void sendMQTTmsg(){
  
//   unsigned long currentMillis = millis(); // ottieni il tempo attuale
//   if (currentMillis - previousMillis >= interval) { // controlla se sono passati 10 secondi
//     // salva l'ultimo momento in cui un'azione è stata eseguita
//     previousMillis = currentMillis;
//   int dimensionebuffer = 50;
//   char msg[dimensionebuffer];
//   // temperatura correte
//   snprintf(msg, dimensionebuffer, "{\"state\":%f}", 25.0); 
//   mqttClient.publish("termostato_casa/climate/termostato/current_temperature", msg);
//   //Set point
//   snprintf(msg, dimensionebuffer, "{\"state\":%f}", 26.0); 
//   mqttClient.publish("termostato_casa/climate/termostato/target_temperature", msg);
//   // mode
//   snprintf(msg, dimensionebuffer, "{\"state\":%s}", "off"); 
//   mqttClient.publish("termostato_casa/climate/termostato/mode", msg);
//   // action
//   snprintf(msg, dimensionebuffer, "{\"state\":%s}", "off"); 
//   mqttClient.publish("termostato_casa/climate/termostato/action", msg);

//   }
// }




//faccio lampeggiare il led per manifestare che il software è vivo
unsigned long Intervallo = 1500;
void loopLed(){
  static unsigned long TempoPrecedente;
  unsigned long TempoAttuale = millis();
  if(TempoAttuale -TempoPrecedente > Intervallo){
    TempoPrecedente = TempoAttuale ;
    //inverto il tempo di accensione per tenerlo spento solo mezzo secondo
    switch (Intervallo)
    {
      case 1500:
        Intervallo = 500;
        digitalWrite(LED_BUILTIN, false);
      break;

      case 500:
        Intervallo = 1500;
        digitalWrite(LED_BUILTIN, true);
      break;
    }
}


}





// void MqttHomeAssistantDiscovery()
// {
//     String discoveryTopic;
//     String payload;
//     String strPayload;
//     if(g_mqttPubSub.connected())
//     {
//         Serial.println("SEND HOME ASSISTANT DISCOVERY!!!");
//         StaticJsonDocument<600> payload;
//         JsonObject device;
//         JsonArray identifiers;

//         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//         // Temperature
//         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//         discoveryTopic = "homeassistant/sensor/esp32iotsensor/" + g_deviceName + "_temp" + "/config";
        
//         payload["name"] = g_deviceName + ".temp";
//         payload["uniq_id"] = "g_UniqueId" + "_temp";
//         payload["stat_t"] = g_mqttStatusTopic;
//         payload["dev_cla"] = "temperature";
//         payload["val_tpl"] = "{{ value_json.temp | is_defined }}";
//         payload["unit_of_meas"] = "°C";
//         device = payload.createNestedObject("device");
//         device["name"] = g_deviceName;
//         device["model"] = g_deviceModel;
//         device["sw_version"] = g_swVersion;
//         device["manufacturer"] = g_manufacturer;
//         identifiers = device.createNestedArray("identifiers");
//         identifiers.add(g_UniqueId);

//         serializeJsonPretty(payload, Serial);
//         Serial.println(" ");
//         serializeJson(payload, strPayload);

//         g_mqttPubSub.publish(discoveryTopic.c_str(), strPayload.c_str());

//         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//         // Humidity
//         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//         payload.clear();
//         device.clear();
//         identifiers.clear();
//         strPayload.clear();

//         discoveryTopic = "homeassistant/sensor/esp32iotsensor/" + g_deviceName + "_hum" + "/config";
        
//         payload["name"] = g_deviceName + ".hum";
//         payload["uniq_id"] = g_UniqueId + "_hum";
//         payload["stat_t"] = g_mqttStatusTopic;
//         payload["dev_cla"] = "humidity";
//         payload["val_tpl"] = "{{ value_json.hum | is_defined }}";
//         payload["unit_of_meas"] = "%";
//         device = payload.createNestedObject("device");
//         device["name"] = g_deviceName;
//         device["model"] = g_deviceModel;
//         device["sw_version"] = g_swVersion;
//         device["manufacturer"] = g_manufacturer;
//         identifiers = device.createNestedArray("identifiers");
//         identifiers.add(g_UniqueId);

//         serializeJsonPretty(payload, Serial);
//         Serial.println(" ");
//         serializeJson(payload, strPayload);

//         g_mqttPubSub.publish(discoveryTopic.c_str(), strPayload.c_str());

//         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//         // Binary Door
//         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//         payload.clear();
//         device.clear();
//         identifiers.clear();
//         strPayload.clear();

//         discoveryTopic = "homeassistant/binary_sensor/esp32iotsensor/" + g_deviceName + "_door" + "/config";
        
//         payload["name"] = g_deviceName + ".door";
//         payload["uniq_id"] = g_UniqueId + "_door";
//         payload["stat_t"] = g_mqttStatusTopic;
//         payload["dev_cla"] = "door";
//         payload["val_tpl"] = "{{ value_json.inputstatus | is_defined }}";
//         device = payload.createNestedObject("device");
//         device["name"] = g_deviceName;
//         device["model"] = g_deviceModel;
//         device["sw_version"] = g_swVersion;
//         device["manufacturer"] = g_manufacturer;
//         identifiers = device.createNestedArray("identifiers");
//         identifiers.add(g_UniqueId);

//         serializeJsonPretty(payload, Serial);
//         Serial.println(" ");
//         serializeJson(payload, strPayload);

//         g_mqttPubSub.publish(discoveryTopic.c_str(), strPayload.c_str());

//     }
// }









void setup() {
  Serial.begin(115200);
  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);

    Serial.println("");
    Serial.println("");
    Serial.println("----------------------------------------------");
    Serial.print("MODELO: ");
    Serial.println(ModelloDispositivo);
    Serial.print("DISPOSITIVO: ");
    Serial.println(NomeDispositvo);
    Serial.print("SW Rev: ");
    Serial.println(VersioneFirmware);
    Serial.println("----------------------------------------------");

  setup_wifi();
  setup_mqtt();
  OTAsetup();

  
}

unsigned long TempoPrecedente = 0;
bool g_InitSystem = true;

void loop() {
  loop_mqtt();
  //sendMQTTmsg();
  loopOTA();
  loopWiFi();
  loopLed();


}







