bool initWifi() {
   if (strlen(data.wifi_ssid) == 0) {
     return false;
   }

   WiFi.mode(WIFI_STA);
   WiFi.begin(data.wifi_ssid, data.wifi_password);

   Timer wifiConnectionTimer(WIFI_CONNECTION_TIMER);
   wifiConnectionTimer.restart();
   
   while (WiFi.status() != WL_CONNECTED) {
     yield();
     animationLoadingTick(CRGB::Blue);

     if (wifiConnectionTimer.period()) {
       WiFi.disconnect();
       while (!animationLoadingEndTick()) {}
       return false;
     }
   }
   WiFi.setAutoReconnect(true);
   WiFi.persistent(false);
   WiFi.setSleepMode(WIFI_NONE_SLEEP);
   
   while (!animationLoadingEndTick()) {}
   return true;
}

void startAP() {
  IPAddress ip = IPAddress(data.ip[0], data.ip[1], data.ip[2], data.ip[3]);
  isAP = true;

  WiFi.mode(WIFI_AP);
  WiFi.softAP(F(WIFI_AP_NAME), F(WIFI_AP_PASSWORD));

  while (1) { 
    yield();
    animationLoadingTick(CRGB::Gold);
    
    if (btnAPTick()) {
      break;
    }
  }

  WiFi.softAPdisconnect(true);
  while (!animationLoadingEndTick()) { yield(); }
  ESP.restart();
}
