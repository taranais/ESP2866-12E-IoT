// #ifndef mqtt
// #define mqtt
//
// #include <Arduino.h>
// #include <AsyncMqttClient.h>
// #include <ArduinoJson.h>
//
//   void configureMqtt();
//   void onMqttConnect(bool sessionPresent);
//   void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
//   void onMqttSubscribe(uint16_t packetId, uint8_t qos);
//   void onMqttUnsubscribe(uint16_t packetId);
//   void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
//   void onMqttPublish(uint16_t packetId);
//
//   AsyncMqttClient mqttClient;
//
//    IPAddress mqttBroker(192, 168, 1, 37);
//    int mqttPort= 1883;
//    char *mqttUser = "test";
//    char *mqttPass = "test";
//
// void configureMqtt() {
//     mqttClient.onConnect(onMqttConnect);
//     mqttClient.onDisconnect(onMqttDisconnect);
//     mqttClient.onSubscribe(onMqttSubscribe);
//     mqttClient.onUnsubscribe(onMqttUnsubscribe);
//     mqttClient.onMessage(onMqttMessage);
//     mqttClient.onPublish(onMqttPublish);
//     mqttClient.setServer(mqttBroker, mqttPort);
//     mqttClient.setKeepAlive(5).setCleanSession(false).setWill("topic/online", 2, true, "no").setCredentials(mqttUser, mqttPass).setClientId("Fuck");
//     Serial.println("Connecting to MQTT...");
//     mqttClient.connect();
//   }
//
// void onMqttConnect(bool sessionPresent) {
//   Serial.println("** Connected to the broker **");
//   Serial.print("Session present: ");
//   Serial.println(sessionPresent);
//
//   uint16_t packetIdSub = mqttClient.subscribe("test/lol", 2);
//   Serial.print("Subscribing at QoS 2, packetId: ");
//
//   // This sends off your payload.
//   char* json = "{\"Prueba\":\"fuck\"}";
//   DynamicJsonBuffer jsonBuffer;
//   JsonObject& payload = jsonBuffer.parseObject(json);
//   char buffer[256];
//   payload.printTo(buffer, sizeof(buffer));
//   mqttClient.publish("test/lol", 0, true, buffer);
//   Serial.println("Publishing at QoS 0");
//
// }
//
// void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
//   Serial.println("** Disconnected from the broker **");
//   Serial.println("Reconnecting to MQTT...");
//   mqttClient.connect();
// }
//
// void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
//   Serial.println("** Subscribe acknowledged **");
// }
//
// void onMqttUnsubscribe(uint16_t packetId) {
//   Serial.println("** Unsubscribe acknowledged **");
// }
//
// void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
//   Serial.println("** Publish received **");
//   Serial.print("  topic: ");
//   Serial.println(topic);
//   Serial.print("  qos: ");
//   Serial.println(properties.qos);
//   DynamicJsonBuffer jsonBuffer;
//   JsonObject &object = jsonBuffer.parseObject((char *) payload);
//   if (!object.success())
//   {
//       // Parsing fail
//   }
//   else{
//       Serial.print("  dump: ");
//       object.printTo(Serial);
//   }
// }
//
// void onMqttPublish(uint16_t packetId) {
//   Serial.println("** Publish acknowledged **");
// }
//
//
// #endif
