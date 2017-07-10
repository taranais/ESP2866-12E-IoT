/*

*/
#ifndef Confo_h
#define Confo_h

#include "Arduino.h"
#include "FS.h"
#include <ArduinoJson.h>
#include <WiFiClient.h>

class Configuration
{
  public:
    char hostname[16];
    // STA Configuration
    char sta_ssid[32];
    char sta_password[32];
    bool sta_dhcp = false;
    //  STA network
    char sta_ip[16];
    char sta_netmask[16];
    char sta_gateway[16];
    char sta_dns0[16];
    char sta_dns1[16];
    // AP Configuration
    char ap_ssid[32];
    char ap_password[32];
    // AP network
    char ap_ip[16];
    char ap_netmask[16];
    // MQTT Configuration
    char mqtt_server[16];
    bool mqtt_enable = false;
    int mqtt_port;
    char mqtt_user[32];
    char mqtt_password[32];
    
    Configuration();
    bool saveConfig();
    bool loadConfig();
};


#endif
