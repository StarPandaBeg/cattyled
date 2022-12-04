/* Общие функции протокола связи */ 

char* packetPing() {
  return PROTOCOL_HEADER "0";
}

char* packetSync() {
  static String s;

  s = PROTOCOL_HEADER "1" PROTOCOL_SEPARATOR;
  s += data.power;
  s += PROTOCOL_SEPARATOR;
  s += data.colorH;
  s += PROTOCOL_SEPARATOR;
  s += data.colorS;
  s += PROTOCOL_SEPARATOR;
  s += data.colorV;
  s += PROTOCOL_SEPARATOR;
  s += data.mode;
  
  return (char*)s.c_str();
}

char* packetSyncRequest() {
  return PROTOCOL_HEADER "2";
}

char* packetPower() {
  static String s;
  s = PROTOCOL_HEADER "3" PROTOCOL_SEPARATOR;
  s += data.power;
  return (char*)s.c_str();
}

char* packetColor() {
  static String s;
  s = PROTOCOL_HEADER "4" PROTOCOL_SEPARATOR;
  s += data.colorH;
  s += PROTOCOL_SEPARATOR;
  s += data.colorS;
  s += PROTOCOL_SEPARATOR;
  s += data.colorV;
  return (char*)s.c_str();
}

char* packetMode() {
  static String s;
  s = PROTOCOL_HEADER "5" PROTOCOL_SEPARATOR;
  s += data.mode;
  return (char*)s.c_str();
}

char* packetWink() {
  return PROTOCOL_HEADER "6";
}

char* packetBrightness() {
  static String s;
  s = PROTOCOL_HEADER "7" PROTOCOL_SEPARATOR;
  s += data.brightness;
  return (char*)s.c_str();
}

char* packetStatus() {
  static String s;
  s = PROTOCOL_HEADER "8" PROTOCOL_SEPARATOR;
  s += data.brightness;
  s += PROTOCOL_SEPARATOR;
  s += mqtt.connected();
  s += PROTOCOL_SEPARATOR;
  s += (!remoteOnlineTimer.elapsed()) && mqtt.connected();
  return (char*)s.c_str();
}
