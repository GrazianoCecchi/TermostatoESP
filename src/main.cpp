#include <Arduino.h>
#include <WiFi.h>
#include <mqtt.h>
#include <OTA.h>







//Invia un messaggio mqtt ogni 10000 mils 10 secondi
unsigned long previousMillis = 0;
const long interval = 10000;

void sendMQTTmsg(){
  
  unsigned long currentMillis = millis(); // ottieni il tempo attuale
  if (currentMillis - previousMillis >= interval) { // controlla se sono passati 10 secondi
    // salva l'ultimo momento in cui un'azione è stata eseguita
    previousMillis = currentMillis;
  int dimensionebuffer = 50;
  char msg[dimensionebuffer];
  // temperatura correte
  snprintf(msg, dimensionebuffer, "{\"state\":%f}", 25.0); 
  mqttClient.publish("termostato_casa/climate/termostato/current_temperature", msg);
  //Set point
  snprintf(msg, dimensionebuffer, "{\"state\":%f}", 26.0); 
  mqttClient.publish("termostato_casa/climate/termostato/target_temperature", msg);
  // mode
  snprintf(msg, dimensionebuffer, "{\"state\":%s}", "off"); 
  mqttClient.publish("termostato_casa/climate/termostato/mode", msg);
  // action
  snprintf(msg, dimensionebuffer, "{\"state\":%s}", "off"); 
  mqttClient.publish("termostato_casa/climate/termostato/action", msg);

  }
}




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



void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  setup_wifi();
  setup_mqtt();
  OTAsetup();

  
}

unsigned long TempoPrecedente = 0;


void loop() {
  loop_mqtt();
  //sendMQTTmsg();
  loopOTA();
  loopWiFi();
  loopLed();
}







