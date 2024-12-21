/* Функции, связанные с MQTT соединением */
void mqttInit() {
  mqtt.setServer(data.mqttHost, data.mqttPort);
  mqtt.setCallback(mqttCallback);
}

void mqttSetState(bool mqttState) {
  data.mqttActive = mqttState;

//  if (!data.mqttActive) {
//    if (mqtt.connected()) mqtt.disconnect();
//    onlineFlag = false;
//  }
}

void mqttCallback(char* topic, byte* payload, uint16_t len) {
  payload[len] = '\0';
  char* str = (char*)payload;

  int cmpProto = strncmp(str, PROTOCOL_HEADER, HEADER_LENGTH);
  int cmpApp = strncmp(str, PROTOCOL_APP_HEADER, HEADER_APP_LENGTH);

  if (cmpProto && cmpApp) return;
  if (!data.mqttActive && cmpProto == 0) return;
  DEBUG(F(L_MQTT_IN));DEBUGLN(str);
  str += cmpApp ? HEADER_LENGTH : HEADER_APP_LENGTH;

  switch (getFromIndex(str, 0)) {
    case 0:
      // Ping
      remoteOnlineTimer.restart();
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
    case 7:
      // Brightness (APP ONLY)
      if (cmpApp == 0) {
         data.brightness = getFromIndex(str, 1);
         memory.update();
      }
      break;
    case 8:
      // Status GET (APP ONLY)
      if (cmpApp == 0) {
         mqttSend(packetStatus());
      }
      break;
    case -1:
      // WiFi Settings GET (APP ONLY)
      if (cmpApp == 0) {
         mqttSend(packetWifi());
      }
      break;
    case -3:
      // WiFi settings (APP ONLY)
      if (cmpApp == 0) {
        char* ssid;
        char* password;
  
        strtok(str, PROTOCOL_SEPARATOR);
        ssid = strtok(NULL, PROTOCOL_SEPARATOR);
        password = strtok(NULL, PROTOCOL_SEPARATOR);
        
        strcpy(data.wifiSSID, ssid);
        strcpy(data.wifiPassword, password);
        memory.updateNow();
      }
      break;
    case -5:
      // Mqtt Settings GET (APP ONLY)
      if (cmpApp == 0) {
         mqttSend(packetMqtt());
      }
      break;
    case -7:
      // Version GET (APP ONLY)
      if (cmpApp == 0) {
         mqttSend(packetVersion());
      }
      break;
    case -13:
      // MQTT Settings (APP ONLY)
      if (cmpApp == 0) {
        char* tmp;
  
        strtok(str, PROTOCOL_SEPARATOR);
        tmp = strtok(NULL, PROTOCOL_SEPARATOR);
        mqttSetState(tmp[0] == '1');
        
        tmp = strtok(NULL, PROTOCOL_SEPARATOR);
        strcpy(data.mqttHost, tmp);
        tmp = strtok(NULL, PROTOCOL_SEPARATOR);
        data.mqttPort = atoi(tmp);
        tmp = strtok(NULL, PROTOCOL_SEPARATOR);
        strcpy(data.mqttPrefix, tmp);
        tmp = strtok(NULL, PROTOCOL_SEPARATOR);
        if (tmp[0] == '1') {
          tmp = strtok(NULL, PROTOCOL_SEPARATOR);
          strcpy(data.mqttUser, tmp);
          tmp = strtok(NULL, PROTOCOL_SEPARATOR);
          strcpy(data.mqttPassword, tmp);
        } else {
          strcpy(data.mqttUser, "");
          strcpy(data.mqttPassword, "");
        }
        memory.updateNow();
      }
      break;
    case -15:
      // Restart (APP ONLY)
      ESP.restart();
      break;
    case -17:
      // IP GET (APP ONLY)
      if (cmpApp == 0) {
         mqttSend(packetIp());
      }
      break;
    case -19:
      // MQTT Set Sync Enabled (APP ONLY)
      if (cmpApp == 0) {
        mqttSetState(getFromIndex(str, 1));
        memory.updateNow();
      }
      break;
    case -21:
      // MQTT Ids (APP ONLY)
      if (cmpApp == 0) {
        mqttSend(packetIds());
      }
      break;
    case -23:
      // Get Update Server (APP ONLY)
      if (cmpApp == 0) {
        mqttSend(packetUpdateServer());
      }
      break;
  }
}

void mqttConnect() {
  // if (!data.mqttActive) return;
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
  // if (!data.mqttActive) return;
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
