/* Функции, связанные с MQTT соединением */
void mqttInit() {
  mqtt.setServer(data.mqttHost, data.mqttPort);
  mqtt.setCallback(mqttCallback);
}

void mqttCallback(char* topic, byte* payload, uint16_t len) {
  payload[len] = '\0';
  char* str = (char*)payload;
}

void mqttConnect() {
  if (!mqttConnectTimer.elapsed()) return;
  
  String id("CattyLED-");
  id += String(random(0xffffff), HEX);

  bool connection = false;
  if (strlen(data.mqttUser) == 0) {
    connection = mqtt.connect(id.c_str());
  } else {    
    connection = mqtt.connect(id.c_str(), data.mqttUser, data.mqttPassword);
  }

  if (!connection) { 
    onlineFlag = false; 
    mqttConnectTimer.restart(); 
    return; 
  }

  mqtt.subscribe(getLocalTopic());
}

void mqttTick() {
  if (WiFi.status() != WL_CONNECTED) return;
  if (!mqtt.connected()) { mqttConnect(); return; }

  if (!onlineFlag) {
    onlineFlag = true;
   //  mqttSend(packetSyncRequest());
  }
  mqtt.loop();
  
  if (mqttPingTimer.period()) {
    // mqttSend(packetPing());
  }
}

char* getLocalTopic() {
  static char topic[48];
  strcpy(topic, data.mqttPrefix);
  strcat(topic, data.localName);
  return topic;
}

char* getRemoteTopic() {
  static char topic[48];
  strcpy(topic, data.mqttPrefix);
  strcat(topic, data.remoteName);
  return topic;
}
