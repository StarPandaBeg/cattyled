bool initWifi() {
   if (strlen(data.wifi_ssid) == 0) {
     return false;
   }

   WiFi.mode(WIFI_STA);
   WiFi.begin(data.wifi_ssid, data.wifi_password);

   Timer wifi_connection_timer(WIFI_CONNECTION_TIMER);
   while (WiFi.status() != WL_CONNECTED) {
     yield();

     if (wifi_connection_timer.period()) {
       WiFi.disconnect();
       return false;
     }
   }
   WiFi.setAutoReconnect(true);
   WiFi.persistent(false);
   WiFi.setSleepMode(WIFI_NONE_SLEEP);

   return true;
}

void startAP() {
  IPAddress ip = IPAddress(data.ip[0], data.ip[1], data.ip[2], data.ip[3]);
  is_ap = true;

  WiFi.mode(WIFI_AP);
  WiFi.softAP(F(WIFI_AP_NAME), F(WIFI_AP_PASSWORD));

  while (1) { 
    yield();

    if (btnAPTick()) {
      break;
    }
  }

  WiFi.softAPdisconnect(true);
  ESP.restart();
}
