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

char* packetBattery() {
  static String s;
  s = PROTOCOL_HEADER "10" PROTOCOL_SEPARATOR;
  s += getBatteryPercentage();
  s += PROTOCOL_SEPARATOR;
  // s += getChargingState() ? 1 : 0;
  s += 0;
  return (char*)s.c_str();
}

// ===

char* packetWifi() {
  static String s;
  s = PROTOCOL_HEADER "-2" PROTOCOL_SEPARATOR;
  s += data.wifiSSID;
  s += PROTOCOL_SEPARATOR;
  s += data.wifiPassword;
  return (char*)s.c_str();
}

char* packetMqtt() {
  static String s;
  bool val = strlen(data.mqttUser) == 0;
  s = PROTOCOL_HEADER "-6" PROTOCOL_SEPARATOR;
  s += data.mqttHost;
  s += PROTOCOL_SEPARATOR;
  s += data.mqttPort;
  s += PROTOCOL_SEPARATOR;
  s += data.mqttPrefix;
  s += PROTOCOL_SEPARATOR;
  s += val ? 0 : 1;
  if (val) return (char*)s.c_str();
  
  s += PROTOCOL_SEPARATOR;
  s += data.mqttUser;
  s += PROTOCOL_SEPARATOR;
  s += data.mqttPassword;
  return (char*)s.c_str();
}

char* packetVersion() {
  static String s;
  s = PROTOCOL_HEADER "-8" PROTOCOL_SEPARATOR FIRMWARE_VERSION PROTOCOL_SEPARATOR;
  s += fsVersion;
  return (char*)s.c_str();
}

char* packetUpdates() {
  static String s;
  s = PROTOCOL_HEADER "-10" PROTOCOL_SEPARATOR;
  s += (isFSUpdateRequired() || isFirmwareUpdateRequired()) ? 1 : 0;
  return (char*)s.c_str();
}

char* packetUpdateStatus(int type, int percent) {
  static String s;
  s = PROTOCOL_HEADER "-12" PROTOCOL_SEPARATOR;
  s += type;
  s += PROTOCOL_SEPARATOR;
  s += percent;
  return (char*)s.c_str();
}

char* packetUpdateCompleted() {
  return PROTOCOL_HEADER "-14";
}

char* packetUpdateError(int code) {
  static String s;
  s = PROTOCOL_HEADER "-16" PROTOCOL_SEPARATOR;
  s += code;
  return (char*)s.c_str();
}

char* packetIp() {
  static String s;
  s = PROTOCOL_HEADER "-18" PROTOCOL_SEPARATOR;
  s += data.ip[0];
  s += '.';
  s += data.ip[1];
  s += '.';
  s += data.ip[2];
  s += '.';
  s += data.ip[3];
  return (char*)s.c_str();
}
