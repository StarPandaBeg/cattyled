/* Содержит функции, связанные с WiFi подключением */

bool initWifi() {
   if (strlen(data.wifiSSID) == 0) {
     return false;
   }

   WiFi.mode(WIFI_STA);
   WiFi.begin(data.wifiSSID, data.wifiPassword);

   DEBUG(F(L_WIFI_CONNECTING));
   DEBUGLN((char*)data.wifiSSID);

   Timer wifiConnectionTimer(WIFI_CONNECTION_TIMER);
   wifiConnectionTimer.restart();
   
   while (WiFi.status() != WL_CONNECTED) {
     yield();
     animationLoadingTick(CRGB::Blue);
     
     if (wifiConnectionTimer.period() || btnWifiTick()) {
       WiFi.disconnect();
       while (!animationLoadingEndTick()) { yield(); }
       return false;
     }
   }
   WiFi.setAutoReconnect(true);
   WiFi.persistent(false);
   WiFi.setSleepMode(WIFI_NONE_SLEEP);
   
   while (!animationLoadingEndTick()) { yield(); btnWifiTick(); }
   DEBUGLN(F(L_WIFI_CONNECTED));
   
   return true;
}

void startAP() {
  IPAddress ip = IPAddress(data.ip[0], data.ip[1], data.ip[2], data.ip[3]);
  isAP = true;
  DEBUG(F(L_WIFI_AP_CREATING));

  WiFi.mode(WIFI_AP);
  WiFi.softAP(F(WIFI_AP_NAME), F(WIFI_AP_PASSWORD));

  DEBUGLN(F(WIFI_AP_NAME));
  DEBUG(F(L_WIFI_AP_PASSWORD));
  DEBUG(F(L_QUOTE_L));
  DEBUG(WIFI_AP_PASSWORD); 
  DEBUGLN(F(L_QUOTE_R)); 
  DEBUG(F(L_IP_LOCAL_ADDRESS)); DEBUGLN(WiFi.softAPIP());

  dnsServer.start(53, "*", WiFi.softAPIP());

  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
  serverWriteData(WiFi.softAPIP(), true);
  server.begin();

  while (1) { 
    yield();
    dnsServer.processNextRequest();
    animationLoadingTick(CRGB::Gold);
    btnAPTick();

    socketTick();
    memoryTick();
  }

  WiFi.softAPdisconnect(true);
  while (!animationLoadingEndTick()) { yield(); }
}
