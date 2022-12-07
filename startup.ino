/* Содержит код функций, выполняемых при загрузке платы */

void startup() {
  delay(20);
  
  if (DEBUG_MODE) Serial.begin(115200);
  DEBUGLN(F(CATTY_VERSION)); DEBUGLN();

  initMemory();
  initLED();
  initFilters();
  
  pinMode(BTN_PIN, INPUT_PULLUP);
  btn.setStepTimeout(64);

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  if (!initWifi()) {
    DEBUGLN(F(L_WIFI_CONNECTION_ERROR));
    startAP();
    ESP.restart();
  }
  
  randomSeed(micros());
  memorySyncIP();
  mqttInit();
  initSocket();
  server.begin();

  IPAddress ip = IPAddress(data.ip[0], data.ip[1], data.ip[2], data.ip[3]);
  DEBUG(F(L_IP_ADDRESS)); DEBUGLN(ip.toString());
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
}
