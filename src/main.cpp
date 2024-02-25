#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <defines_.h>
#include <setupWiFi.h>
#include <PSclient.h>
#include <gpioFunc.h>
#include <ld2410.h>
#include <string.h>

WiFiClient espClient;
PubSubClient mqttClient(espClient);
ld2410 motionSensor;
HardwareSerial radarMonitor(1);

//debugging
unsigned long startTime = 0;
unsigned long endTime = 0;
unsigned long mqttIntervalStart = 0;
unsigned long mqttIntervalEnd = 0;

//timing variables
unsigned long powerIntervalStart = 0;
unsigned long powerIntervalEnd = 0;
bool wokedUp = false;

//time that LD2410 is being off
unsigned long beingOff = 2000;
//time that LD2410 is powered
unsigned long beingPowered = 5000;
//manual or automate
extern bool automaticSensor = true;
extern uint8_t relayState;

bool analogDetected = false;
int presenceDetedtedCounter = 0;
int notPresenceDetedtedCounter = 0;
unsigned long mqttInterval = 1000;
bool lightState = false;

void setup() {
  //serials init
  Serial.begin(9600);
  radarMonitor.begin(256000, SERIAL_8N1, GPIO_NUM_5, GPIO_NUM_18);

  //wireless communication init
  setupWiFi(WIFI_SSID, WIFI_PWD);
  setupPSclient(&mqttClient, &espClient, MQTT_SERVER, MQTT_PORT);
  subscribeTopic(&mqttClient, MQTT_TOPIC);

  //GPIO init
  setupPins();
  relayState = 0;  

  //motion sensor init
  motionSensor.begin(radarMonitor);    
}

void loop() {  
  mqttClient.loop();
  motionSensor.read();
  startTime = millis();
  mqttIntervalStart = millis();  
  powerIntervalStart = millis();

  if (automaticSensor){
    if (wokedUp){
      if(motionSensor.isConnected()){        
        //calculating wheter turn on the light or not
        //done by deciding if most of powering time, sensor 
        //detedcted presence or not
        if (digitalRead(LD_PIN) == HIGH){
          analogDetected = true;
        }
        else if (digitalRead(LD_PIN) == LOW){
          analogDetected = false;
        }         

        if (100 < startTime - endTime){
          if (analogDetected){
            presenceDetedtedCounter++;
          }
          else if (!analogDetected){
            notPresenceDetedtedCounter++;
          }
          endTime = millis();
        }

        //for reducing turning on delay
        if (motionSensor.movingTargetDetected()){
          digitalWrite(RELAY_PIN, HIGH);
          lightState = true;
        }
        
        //power management
        if (beingPowered < powerIntervalStart - powerIntervalEnd){     
          if (presenceDetedtedCounter > notPresenceDetedtedCounter){
            digitalWrite(RELAY_PIN, HIGH);
            lightState = true;
          }
          else if (presenceDetedtedCounter < notPresenceDetedtedCounter){
            digitalWrite(RELAY_PIN, LOW);     
            lightState = false;                   
          }
          presenceDetedtedCounter = 0;
          notPresenceDetedtedCounter = 0;
          digitalWrite(LD_POWER_PIN, HIGH);
          wokedUp = false;      
          powerIntervalEnd = millis();            
        }
      }
    }
    else if (!wokedUp){  
      //power management
      if (!motionSensor.isConnected()){    
        if (beingOff < powerIntervalStart - powerIntervalEnd){           
          digitalWrite(LD_POWER_PIN, LOW);
          wokedUp = true;
          powerIntervalEnd = millis();      
        }
      }
    }
  }
  else if (!automaticSensor){
    //turning off sensor
    digitalWrite(LD_POWER_PIN, HIGH);
    wokedUp = false;
    
    if (1 == relayState){
      digitalWrite(RELAY_PIN, HIGH);
      lightState = true;
    }
    else if (0 == relayState){
      digitalWrite(RELAY_PIN, LOW);
      lightState = false;
    }    
  }

  if (mqttInterval < mqttIntervalStart - mqttIntervalEnd){
    if (lightState){
      mqttClient.publish(MQTT_TOPIC, "1");
    }
    if (!lightState){
      mqttClient.publish(MQTT_TOPIC, "0");
    }
    mqttIntervalEnd = millis();
  }
}