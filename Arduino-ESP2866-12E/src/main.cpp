#include <Arduino.h>
#include <Hash.h>
#include <FS.h>

#include <wifi.h>
#include <conf.h>
#include <mqtt.h>
#include <jsonInfo.h>
#include <webapi.h>

  WifiO wifi;
  Mqtt mqtt;
  Configuration conf;
  Webapi webapi;

  void configureESP();
  void setWebApi();
  void setApWifi();
  void setStaWifi();
  void checkReboot();
  void setMqtt();

 /** flag to reboot the ESP */
  bool shouldReboot = false;

  // put your setup code here, to run once:
  void setup() {
	  Serial.begin(9600);
    SPIFFS.begin();
    Serial.println (".");
    Serial.println ("..");
    Serial.println ( "----- Setup Init -----" );
    configureESP();
    Serial.println ( "----- Setup Done -----" );
  }

  // put your main code here, to run repeatedly:
  void loop() {
    checkReboot();
    wifi.checkConnect();
  }

  void configureESP(){
    wifi.scanNetworks();
    bool cargaConfig = conf.loadConfig();
    if(cargaConfig)
    {
      Serial.println ( "config.json exist, configuring AP & STA" );
      setApWifi();
      setStaWifi();
      if(conf.mqtt_enable){
        setMqtt();
      }
    }
    else{
      Serial.println ( "config.json do no exist, using default values" );
      wifi.setAPCredentials("ESP_ap","12345678");
      wifi.setApNetwork(IPAddress(192,168,4,1),IPAddress(255,255,255,0));
    }
    setWebApi();
    wifi.connectWifiAp();
  }

  void checkReboot(){
   if(shouldReboot){
     Serial.println("Rebooting...");
      delay(100);
      ESP.restart();
    }
  }

  void setApWifi(){
    Serial.println ( "Configuring AP" );
    IPAddress _ip,_nm;
    _ip.fromString(conf.ap_ip);
    _nm.fromString(conf.ap_netmask);
    wifi.setAPCredentials(conf.ap_ssid,
                          conf.ap_password);
    wifi.setApNetwork(_ip,_nm);
  }

  void setStaWifi(){
    Serial.println ( "Configuring STA" );
    wifi.setCredentials(conf.sta_ssid,
                        conf.sta_password);
    if(!conf.sta_dhcp)
    {
      IPAddress _ip,_nm,_gw,_d0,_d1;
      _ip.fromString(conf.sta_ip);
      _nm.fromString(conf.sta_netmask);
      _gw.fromString(conf.sta_gateway);
      _d0.fromString(conf.sta_dns0);
      _d1.fromString(conf.sta_dns1);
      wifi.setStaNetwork(_ip,_nm,_gw,_d0,_d1);
    }
    wifi.shouldConnect();
  }

  void setMqtt(){
    Serial.println ( "Configuring mqtt" );
    mqtt.connect(conf.mqtt_server,conf.mqtt_port,conf.mqtt_user,conf.mqtt_password);
  }

  void setWebApi(){
    Serial.println ( "Configuring webapi" );
    webapi.begin();
  }
