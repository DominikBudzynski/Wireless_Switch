#ifndef _PS_CLIENT_
#define _PS_CLIENT_

#include <PubSubClient.h>
#include <WiFi.h>

bool setupPSclient(PubSubClient *client, WiFiClient *wiFiClient, const char *mqttServer, const int port);
void callback(const char topic[], byte *payload, unsigned int length);
bool subscribeTopic(PubSubClient *client, const char *topic);

#endif