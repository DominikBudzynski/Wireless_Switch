#include <defines_.h>
#include <WiFi.h>

void setupWiFi(const char *ssid, const char *pwd){
    Serial.println("\nConnecting to: ");
    Serial.println(ssid);
    WiFi.begin(ssid, pwd);

    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.println(".");
    } 
}