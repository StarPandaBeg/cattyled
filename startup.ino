/* Содержит код функций, выполняемых при загрузке платы */

void startup() {
  delay(20);
  
  if (DEBUG_MODE) Serial.begin(115200);
  DEBUGLN(F(CATTY_VERSION)); DEBUGLN();
  
  if (LittleFS.begin()) DEBUGLN(F(L_FS_INIT_COMPLETED));
  else {
    DEBUGLN(F(L_FS_INIT_ERROR));
    while (1) {}
  }
  loadFSVersion();
  DEBUGF(L_FS_VERSION, fsVersion); DEBUGLN();
  
  http.setReuse(false);
  espClient.setInsecure();
  
  #if USE_BATTERY
    initBattery();
  #endif
  
  initMemory();
  initLED();
  initFilters();
  initOTA();

  // if (isBatteryCritical()) { sleep(); while (1) {}}
  
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(VIBRO_PIN, INPUT_PULLUP);
  pinMode(CHARGING_PIN, INPUT_PULLUP);
  pinMode(MOSFET_PIN, OUTPUT);
  pinMode(VIBRO_POWER_PIN, OUTPUT);
  
  btn.setStepTimeout(64);
  power_turn_on();

  initSocket();
  randomSeed(micros());
  initServer();
  
  if (!initWifi()) {
    if (otaMode) {
      otaStartAP();
      DEBUGLN(F(L_RESTART));
      ESP.restart();
      return;
    }
    DEBUGLN(F(L_WIFI_CONNECTION_ERROR));
    startAP();
    DEBUGLN(F(L_RESTART));
    ESP.restart();
  }
  
  memorySyncIP(WiFi.localIP());
  IPAddress ip = IPAddress(data.ip[0], data.ip[1], data.ip[2], data.ip[3]);
  DEBUG(F(L_IP_ADDRESS)); DEBUGLN(ip.toString());
  
  mqttInit();
  mdnsInit();
  serverWriteData(ip, false);
  server.begin();
}

void initLED() {
  FastLED.addLeds<LED_TYPE, LED_PIN, LED_ORDER>(leds, LED_AMOUNT).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(getRealBrightness());
  FastLED.clear();
  FastLED.show();
}

void initFilters() {
  brightnessFilter.setCoef(0.05);
  brightnessFilter.setStep(5);

  batteryFilter.setCoef(0.05);
  batteryFilter.setStep(5);
}
