/* Функции, связанные с MQTT соединением */
void mqttInit() {
  mqtt.setServer(data.mqttHost, data.mqttPort);
  mqtt.setCallback(mqttCallback);
}

void mqttCallback(char* topic, byte* payload, uint16_t len) {
  payload[len] = '\0';
  char* str = (char*)payload;

  if (strncmp(str, PROTOCOL_HEADER, HEADER_LENGTH)) return;
  DEBUG(F(L_MQTT_IN));DEBUGLN(str);
  str += HEADER_LENGTH;

  switch (getFromIndex(str, 0)) {
    case 0:
      // Ping
      break;
    case 1:
      // Sync
      data.power = getFromIndex(str, 1);
      data.colorH = getFromIndex(str, 2);
      data.colorS = getFromIndex(str, 3);
      data.colorV = getFromIndex(str, 4);
      data.mode = getFromIndex(str, 5);
      memory.update();
      socketSend(packetSync());
      break;
    case 2:
      // Sync request
      mqttSend(packetSync());
      break;
    case 3:
      // Power
      data.power = getFromIndex(str, 1);
      memory.update();
      socketSend(packetPower());
      break;
    case 4:
      // Color
      data.colorH = getFromIndex(str, 1);
      data.colorS = getFromIndex(str, 2);
      data.colorV = getFromIndex(str, 3);
      memory.update();
      socketSend(packetColor());
      break;
    case 5:
      // Mode
      data.mode = getFromIndex(str, 1);
      memory.update();
      socketSend(packetMode());
      break;
    case 6:
      // Wink
      animationWink();
      break;
  }
  remoteOnlineTimer.restart();
}

void mqttConnect() {
  if (!mqttConnectTimer.elapsed()) return;
  
  String id("CattyLED-");
  id += String(random(0xffffff), HEX);

  DEBUG(F(L_MQTT_CONNECTING));
  DEBUG(data.mqttHost);DEBUG(F(":"));DEBUGLN(data.mqttPort);

  bool connection = false;
  if (strlen(data.mqttUser) == 0) {
    connection = mqtt.connect(id.c_str());
  } else {    
    connection = mqtt.connect(id.c_str(), data.mqttUser, data.mqttPassword);
    DEBUG(F(L_MQTT_USER));
    DEBUGLN(data.mqttUser);
  }

  if (!connection) { 
    DEBUGLN(F(L_MQTT_CONNECTION_ERROR));
    onlineFlag = false; 
    mqttConnectTimer.restart(); 
    return; 
  }

  DEBUG(F(L_MQTT_SUBCRIBING_TO_TOPIC));
  DEBUGLN(getLocalTopic());
  mqtt.subscribe(getLocalTopic());
}

void mqttTick() {
  if (WiFi.status() != WL_CONNECTED) return;
  if (!mqtt.connected()) { mqttConnect(); return; }

  if (!onlineFlag) {
    onlineFlag = true;
    mqttSend(packetSyncRequest());
  }
  mqtt.loop();
  
  if (mqttPingTimer.period()) {
    mqttSend(packetPing());
  }
}

bool mqttSend(char* str) {
  if (!mqtt.connected()) return false;
  DEBUG(F(L_MQTT_OUT)); DEBUGLN(str);
  mqtt.publish(getRemoteTopic(), str);
  return true;
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
