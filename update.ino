#define UPDATE_FIRMWARE 0
#define UPDATE_FS 1

#define UPDATE_ERR_BATTERY 0

void initOTA() {
  ESPhttpUpdate.onStart(updateStarted);
  ESPhttpUpdate.onEnd(updateFinished);
  ESPhttpUpdate.onProgress(updateProgress);
  ESPhttpUpdate.onError(updateError);
  ESPhttpUpdate.rebootOnUpdate(false);
}

void initLocalOTA() {
  ArduinoOTA.onStart(updateStarted);
  ArduinoOTA.onEnd(updateFinished);
  ArduinoOTA.onProgress(updateProgress);
  ArduinoOTA.onError(updateError);
}

void otaStartAP() {
  DEBUGLN(F(L_OTA_MODE));
  DEBUG(F(L_WIFI_OTA_CREATING));
  DEBUGLN(F(WIFI_OTA_AP_NAME));
  DEBUG(F(L_WIFI_OTA_PASSWORD));
  DEBUG(F(L_QUOTE_L));
  DEBUG(F(WIFI_OTA_AP_PASSWORD)); 
  DEBUGLN(F(L_QUOTE_R));
  otaMode = true;

  WiFi.mode(WIFI_AP);
  WiFi.softAP(F(WIFI_OTA_AP_NAME), F(WIFI_OTA_AP_PASSWORD));

  DEBUG(F(L_IP_LOCAL_ADDRESS));
  DEBUGLN(WiFi.softAPIP());

  initLocalOTA();
  ArduinoOTA.begin();
  while (1) {
    yield();
    if (btnOTATick()) break;
    animationLoadingTick(CRGB::Crimson);
    ArduinoOTA.handle();
  }
  WiFi.softAPdisconnect(true);
  while (!animationLoadingEndTick()) { yield(); }
}

void otaStartUpdate() {
  if (isBatteryLow()) {
    socketSendTo(packetUpdateError(UPDATE_ERR_BATTERY), lastClient);
    return;
  }
  bool isFsUpdated = fsUpdate();
  bool isFirmwareUpdated = firmwareUpdate();

  delay(100);
  socketSendTo(packetUpdateCompleted(), lastClient);
  delay(10);
  if (isFirmwareUpdated || isFsUpdated) ESP.restart();
}

bool firmwareUpdate() {
  if (!isFirmwareUpdateRequired()) return false;
  updateType = UPDATE_FIRMWARE;
  t_httpUpdate_return ret = ESPhttpUpdate.update(espClient, UPDATE_URL);
  return (ret == HTTP_UPDATE_OK);
}

bool fsUpdate() {
  if (!isFSUpdateRequired()) return false;
  updateType = UPDATE_FS;
  t_httpUpdate_return ret = ESPhttpUpdate.updateFS(espClient, UPDATE_FS_URL);
  return (ret == HTTP_UPDATE_OK);
}

void updateStarted() {
  DEBUGLN(F(L_OTA_UPDATE_STARTED));
  animationFlag = false;
  otaInProgress = true;
}

void updateFinished() {
  while (animationLoadingEndTick()) { yield(); }
  DEBUGLN(F(L_OTA_UPDATE_FINISHED));
  animationFlag = true;
  otaInProgress = false;
}

void updateProgress(int cur, int total) {
  static int lastPercent = 0;
  int percent = round(((double)cur) / total * 100);
  if (lastPercent != percent) {
    #if (DEBUG_MODE)
      Serial.printf(L_OTA_UPDATE_PROGRESS, percent, cur, total);
      DEBUGLN();
    #endif
    delayMicroseconds(100);
    socketSendTo(packetUpdateStatus(updateType, percent), lastClient);
    lastPercent = percent;
  }
  
  animationLoading(CRGB::Red);
}

void updateError(int err) {
  DEBUGF(L_OTA_UPDATE_ERROR, err); DEBUGLN();
  otaInProgress = false;
}

void loadFSVersion() {
  if (!LittleFS.exists("/ver.txt")) {
    fsVersion = "0.0.0";
    return;
  }
  File f = LittleFS.open("/ver.txt", "r");

  static char ver[32];
  int i;
  for(i = 0; i < 32 && f.available(); i++) {
    ver[i] = f.read();
  }
  ver[i] = '\0';
  f.close();

  fsVersion = ver;
}

char* getRemoteVersion(char* url) {
  if (WiFi.status() != WL_CONNECTED) return "0.0.0";
  mqtt.disconnect();
  ws.enable(false);
  delay(1);
  
  char ver[32];
  memset(ver, '\0', 32);

  http.begin(String(url));
  int code = http.GET();
  
  if (code != 200) return "0.0.0";
  char* payload = (char*)(http.getString().c_str());
  strcpy(ver, payload);
  
  http.end();
  ws.enable(true);
  return ver;
}

int versionCompare(char* v1, char* v2) 
{
    int vnum1 = 0, vnum2 = 0;
    for (int i = 0, j = 0; (i < strlen(v1) 
                            || j < strlen(v2));) {
        while (i < strlen(v1) && v1[i] != '.') { 
            vnum1 = vnum1 * 10 + (v1[i] - '0'); 
            i++;
        }
        while (j < strlen(v2) && v2[j] != '.') { 
            vnum2 = vnum2 * 10 + (v2[j] - '0'); 
            j++; 
        } 
  
        if (vnum1 > vnum2) 
            return 1; 
        if (vnum2 > vnum1) 
            return -1;
        vnum1 = vnum2 = 0; 
        i++; 
        j++; 
    }
    return 0; 
}

bool isFSUpdateRequired() {
  return versionCompare(getRemoteVersion(UPDATE_FS_VERSION_URL), fsVersion) == 1;
}

bool isFirmwareUpdateRequired() {
  return versionCompare(getRemoteVersion(UPDATE_VERSION_URL), FIRMWARE_VERSION) == 1;
}
