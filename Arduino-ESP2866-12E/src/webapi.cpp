// #ifndef webapi
// #define webapi
//
// #include <Arduino.h>
// #include <ESPAsyncWebServer.h>
// #include <ArduinoJson.h>
// #include <AsyncJson.h>
// #include <FS.h>
//
// #include "wifi.cpp"
// #include "tools.cpp"
//
//   void setupWebApi();
//   void PrintRequestArgsAndParams(AsyncWebServerRequest *request, JsonObject &json);
//   void onRequestNotFound(AsyncWebServerRequest *request);
//   void EmptyHandler(AsyncWebServerRequest *request);
//   void onBodyWifiPost(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
//   void LoginHandler(AsyncWebServerRequest *request);
//   void ReconectHandler(AsyncWebServerRequest *request);
//   void WifiStatusHandler(AsyncWebServerRequest *request);
//   void StatusHandler(AsyncWebServerRequest *request);
//   void WifiScanHandler(AsyncWebServerRequest *request);
//
//   const char* http_username = "admin";
//   const char* http_password = "admin";
//
//   AsyncWebServer server(80);
//
//   void setupWebApi(){
//     Serial.println("Configuring WebApi...");
//
//     server.on("/login", HTTP_GET, LoginHandler);
//
//     server.on("/api/status", HTTP_GET, StatusHandler);
//
//     server.on("/api/wifi", HTTP_POST, EmptyHandler).onBody(onBodyWifiPost);
//
//     server.on("/api/wifi/scan", HTTP_GET, WifiScanHandler);
//
//     server.on("/api/wifi/status", HTTP_GET, WifiStatusHandler);
//
//     server.on("/api/wifi/reconnect", HTTP_POST, ReconectHandler);
//
//     // Serve files in directory "/www/" when request url starts with "/"
//     // Request to the root or none existing files will try to server the defualt
//     server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
//
//     server.onNotFound(onRequestNotFound);
//     server.begin();
//   }
//
//   void PrintRequestArgsAndParams(AsyncWebServerRequest *request, JsonObject &json){
//     //List all args (Compatibility)
//     JsonArray&  paramsArray  = json.createNestedArray("params");
//     int params = request->params();
//     for(int i=0;i<params;i++){
//       JsonObject& nestedObject = paramsArray.createNestedObject();
//       AsyncWebParameter* p = request->getParam(i);
//       if(p->isFile()){ //p->isPost() is also true
//         nestedObject["file"]    = p->name().c_str();
//         nestedObject["name"]    = p->value().c_str();
//         nestedObject["size"]    = p->size();
//       } else if(p->isPost()){
//         nestedObject["post"]    = p->name().c_str();
//         nestedObject["name"]    = p->value().c_str();
//       } else {
//         nestedObject["get"]    = p->name().c_str();
//         nestedObject["name"]    = p->value().c_str();
//       }
//     }
//
//     //List all parameters (Compatibility)
//     JsonArray&  argsdArray  = json.createNestedArray("args");
//     int args = request->args();
//     for(int i=0;i<args;i++){
//       JsonObject& nestedObject = paramsArray.createNestedObject();
//       nestedObject["arg"]    = request->argName(i).c_str();
//       nestedObject["name"]    = request->arg(i).c_str();
//     }
//   }
//
//   void onRequestNotFound(AsyncWebServerRequest *request){
//     //Handle Unknown Request
//     request->send(404);
//   }
//
//   void onBodyWifiPost(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
//     if(!index){
//       Serial.println (".");
//       Serial.printf("BodyStart: %u B\n", total);
//     }
//     for(size_t i=0; i<len; i++){
//       Serial.write(data[i]);
//     }
//     if(index + len == total){
//       AsyncJsonResponse * response = new AsyncJsonResponse();
//       JsonObject& root = response->getRoot();
//       Serial.println (".");
//       Serial.printf("BodyEnd: %u B\n", total);
//
//       DynamicJsonBuffer jsonBuffer;
//       JsonObject &object = jsonBuffer.parseObject((char *) data);
//       String ssid     = object["ssid"];
//       String password = object["password"];
//       if(   (ssid == NULL  )&& (ssid.length() >= 8) && (ssid.length() <=32)
//          || (password == NULL && (password.length() >= 8) && (password.length() <=32) )){
//         root["result"]  = "fail";
//         Serial.println ("fail");
//       }
//       else{
//           root["result"]  = "ok";
//           saveWifi(ssid,password);
//       }
//         request->send(response);
//     }
//   }
//
//   void EmptyHandler(AsyncWebServerRequest *request) { request->send(200); }
//
//   void LoginHandler(AsyncWebServerRequest *request) {
//     if(!request->authenticate(http_username, http_password))
//         return request->requestAuthentication();
//
//     AsyncJsonResponse * response = new AsyncJsonResponse();
//     response->addHeader("Access-Control-Allow-Origin","*");
//     JsonObject& root = response->getRoot();
//     root["login"] = "OK";
//     request->send(response);
//   }
//
//   void ReconectHandler(AsyncWebServerRequest *request) {
//     AsyncJsonResponse * response = new AsyncJsonResponse();
//     JsonObject& root = response->getRoot();
//     reconnectWifiClient();
//     response->setLength();
//     response->addHeader("Access-Control-Allow-Origin","*");
//     request->send(response);
//   }
//
//   void WifiStatusHandler(AsyncWebServerRequest *request) {
//     AsyncJsonResponse * response = new AsyncJsonResponse();
//     JsonObject& root = response->getRoot();
//     InfoWifiAP(root);
//     InfoWifiSTA(root);
//     response->setLength();
//     response->addHeader("Access-Control-Allow-Origin","*");
//     request->send(response);
//   }
//
//   void StatusHandler(AsyncWebServerRequest *request) {
//     AsyncJsonResponse * response = new AsyncJsonResponse();
//     JsonObject& root = response->getRoot();
//     getInfo(root);
//     response->setLength();
//     response->addHeader("Access-Control-Allow-Origin","*");
//     request->send(response);
//   }
//
//   void WifiScanHandler(AsyncWebServerRequest *request) {
//     AsyncJsonResponse * response = new AsyncJsonResponse();
//     JsonObject& root = response->getRoot();
//     ScanWifiJson(root);
//     response->setLength();
//     response->addHeader("Access-Control-Allow-Origin","*");
//     request->send(response);
//   }
//
//
// #endif
