/* Содержит код функций, выполняемых при загрузке платы */

void startup() {
  delay(20);

  if (DEBUG_MODE) Serial.begin(115200);
  DEBUGLN(F(CATTY_VERSION)); DEBUGLN();

  initLED();
  pinMode(BTN_PIN, INPUT_PULLUP);
}

void initLED() {
  FastLED.addLeds<LED_TYPE, LED_PIN, LED_ORDER>(leds, LED_AMOUNT).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(data.brightness);
  FastLED.clear();
  FastLED.show();
}
