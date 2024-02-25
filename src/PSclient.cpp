#include <PubSubClient.h>
#include <defines_.h>
#include <WiFi.h>
#include <gpioFunc.h>
#include <string.h>

using std::string;

extern uint8_t relayState;
extern bool automaticSensor;

void callback(const char topic[], byte *payload, unsigned int length){
  string receiveBuffer;
  for (int i = 0; i < length; i++){
    receiveBuffer.append(1, (char)payload[i]);
  }
  if (receiveBuffer.compare("1") == 0){    
    relayState = 1;
  }
  else if (receiveBuffer.compare("0") == 0){    
    relayState = 0;  
  }
  else if (receiveBuffer.compare("manual") == 0){    
    automaticSensor = false;
  }
  else if (receiveBuffer.compare("auto") == 0){    
    automaticSensor = true;
  }
}

bool setupPSclient(PubSubClient *client, WiFiClient *wiFiClient, const char *mqttServer, const int port){
    client->setClient(*wiFiClient);
    client->setServer(mqttServer, port);

    if (client->connect("ESP32")){
        Serial.println("Succesfully connected");
        client->publish(MQTT_TOPIC, "polaczono_z_mqtt");
        Serial.println("Polaczono z mqtt");
        client->setCallback(callback);
        return true;
    }
    else{
        Serial.println("Nie polaczono z brokerem");
        return false;
    }

}

bool subscribeTopic(PubSubClient *client, const char *topic){
  if(client->connected()){
    if (client->subscribe(topic)){
        return true;
    }    
  }
  return false;
}


