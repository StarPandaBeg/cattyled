void initSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void socketTick() {
  ws.cleanupClients();

  if (needUpdateCheck) {
    needUpdateCheck = false;
    socketSendTo(packetUpdates(), lastClient);
  }
}

void socketSend(char* str) {
  DEBUG(F(L_SOCKET_OUT)); DEBUGLN(str);
  ws.textAll(str);
}

void socketSendTo(char* str, AsyncWebSocketClient *client) {
  DEBUG(F(L_SOCKET_OUT)); DEBUGLN(str);
  client->printf(str);
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
      case WS_EVT_CONNECT:
        DEBUG(F(L_SOCKET_CLIENT_CONNECTED));
        DEBUGLN(client->remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        DEBUGLN(F(L_SOCKET_CLIENT_DISCONNECTED));
        break;
      case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len, client);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
  }
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strncmp((char*)data, PROTOCOL_HEADER, HEADER_LENGTH)) return;
    parseWebSocketMessage((char*)data, client);
  }
}

void parseWebSocketMessage(char* str, AsyncWebSocketClient *client) {
  str += HEADER_LENGTH;
  DEBUG(F(L_SOCKET_IN)); DEBUGLN(str);

  int val = getFromIndex(str, 0);
  if (val >= 0) parseDefaultCommands(str, client);
  else parseConfigCommands(str, client);
}

void parseDefaultCommands(char* str, AsyncWebSocketClient *client) {
  switch (getFromIndex(str, 0)) {
    case 0:
      // Ping (TODO)
      break;
    case 1:
      // Sync
      data.power = getFromIndex(str, 1);
      data.colorH = getFromIndex(str, 2);
      data.colorS = getFromIndex(str, 3);
      data.colorV = getFromIndex(str, 4);
      data.mode = getFromIndex(str, 5);
      memory.update();
      mqttSend(packetSync());
      break;
    case 2:
      // Sync request
      socketSendTo(packetSync(), client);
      break;
    case 3:
      // Power
      data.power = getFromIndex(str, 1);
      memory.update();
      mqttSend(packetPower());
      break;
    case 4:
      // Color
      data.colorH = getFromIndex(str, 1);
      data.colorS = getFromIndex(str, 2);
      data.colorV = getFromIndex(str, 3);
      memory.update();
      mqttSend(packetColor());
      break;
    case 5:
      // Mode
      data.mode = getFromIndex(str, 1);
      memory.update();
      mqttSend(packetMode());
      break;
    case 6:
      // Wink
      mqttSend(packetWink());
      break;
    case 7:
      // Brightness
      data.brightness = getFromIndex(str, 1);
      memory.update();
      break;
    case 9:
      // Status
      socketSendTo(packetStatus(), client);
      break;
    case 11:
      // Battery status
      socketSendTo(packetBattery(), client);
      break;
  }
}

void parseConfigCommands(char* str, AsyncWebSocketClient *client) {
  switch (getFromIndex(str, 0)) {
    case -1:
      socketSendTo(packetWifi(), client);
      break;
     
    case -3:
      // WiFi settings
      char* ssid;
      char* password;

      strtok(str, PROTOCOL_SEPARATOR);
      ssid = strtok(NULL, PROTOCOL_SEPARATOR);
      password = strtok(NULL, PROTOCOL_SEPARATOR);
      
      strcpy(data.wifiSSID, ssid);
      strcpy(data.wifiPassword, password);
      memory.updateNow();
      break;

    case -5:
      socketSendTo(packetMqtt(), client);
      break;

    case -7:
      socketSendTo(packetVersion(), client);
      break;

    case -9:
      needUpdateCheck = true;
      lastClient = client;
      break;

    case -11:
      needUpdate = true;
      ws.enable(false);
      lastClient = client;
      break;

    case -13:
      char* tmp;

      strtok(str, PROTOCOL_SEPARATOR);
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
      break;

    case -15:
      ESP.restart();
      break;

    case -17:
      socketSendTo(packetIp(), client);
      break;
  }
}
