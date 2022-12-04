/* Содержит код функций, выполняемых при загрузке платы */

void startup() {
  delay(20);

  if (DEBUG_MODE) Serial.begin(115200);
  DEBUGLN(F(CATTY_VERSION)); DEBUGLN();

  initLED();
  initFilters();
  
  pinMode(BTN_PIN, INPUT_PULLUP);
  btn.setStepTimeout(64);

  if (!initWifi()) {
    startAP();
  }
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
