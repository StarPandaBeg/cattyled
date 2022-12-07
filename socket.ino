void initSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void socketTick() {
  ws.cleanupClients();
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
  }
}
