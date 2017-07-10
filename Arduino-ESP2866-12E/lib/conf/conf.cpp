#include "conf.h"


Configuration::Configuration(){}

bool Configuration::saveConfig(){
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json["hostname"] =                  hostname;

  json["sta"]["ssid"] =               sta_ssid ;
  json["sta"]["password"] =           sta_password;
  json["sta"]["dchp"] =               sta_dhcp;

  json["sta"]["network"]["ip"] =      sta_ip;
  json["sta"]["network"]["netmask"] = sta_netmask;
  json["sta"]["network"]["gateway"] = sta_gateway;
  json["sta"]["network"]["dns0"] =    sta_dns0;
  json["sta"]["network"]["dns1"] =    sta_dns1;

  json["ap"]["ssid"] =                ap_ssid;
  json["ap"]["password"] =            ap_password;
  json["ap"]["network"]["ip"] =       ap_ip;
  json["ap"]["network"]["netmask"] =  ap_netmask;

  json["mqtt-enable"] =               mqtt_enable;

  json["mqtt"]["server"] =            mqtt_server;
  json["mqtt"]["port"] =              mqtt_port ;
  json["mqtt"]["user"] =              mqtt_user;
  json["mqtt"]["password"] =          mqtt_password;

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  json.printTo(configFile);

  Serial.println ( "Saved config.json" );
  return true;
}

bool Configuration::loadConfig(){
    File configFile = SPIFFS.open("/config.json", "r");
    if (!configFile) {
      Serial.println("Failed to open config file");
      return false;
    }

    size_t size = configFile.size();
    if (size > 1024) {
      Serial.println("Config file size is too large");
      return false;
    }

    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);

    // // We don't use String here because ArduinoJson library requires the input
    // // buffer to be mutable. If you don't use ArduinoJson, you may as well
    // // use configFile.readString instead.
     configFile.readBytes(buf.get(), size);

     DynamicJsonBuffer jsonBuffer;
     JsonObject& json = jsonBuffer.parseObject(buf.get());

     if (!json.success()) {
      Serial.println("Failed to parse config file");
      return false;
    }



   strcpy(hostname,     json["hostname"]);

   strcpy(sta_ssid,     json["sta"]["ssid"]);
   strcpy(sta_password, json["sta"]["password"]);
   sta_dhcp =           json["sta"]["dchp"];

   strcpy(sta_ip,       json["sta"]["network"]["ip"]);
   strcpy(sta_netmask,  json["sta"]["network"]["netmask"]);
   strcpy(sta_gateway,  json["sta"]["network"]["gateway"]);
   strcpy(sta_dns0,     json["sta"]["network"]["dns0"]);
   strcpy(sta_dns1,     json["sta"]["network"]["dns1"]);

   strcpy(ap_ssid,      json["ap"]["ssid"]);
   strcpy(ap_password,  json["ap"]["password"]);

   strcpy(ap_ip,        json["ap"]["network"]["ip"]);
   strcpy(ap_netmask,   json["ap"]["network"]["netmask"]);

   mqtt_enable =        json["mqtt-enable"] ;
   strcpy(mqtt_server,  json["mqtt"]["server"]);
   mqtt_port =          json["mqtt"]["port"];
   strcpy(mqtt_user,    json["mqtt"]["user"]);
   strcpy(mqtt_password,json["mqtt"]["password"]);

  Serial.println ( "Loaded config.json" );
  return true;
}
