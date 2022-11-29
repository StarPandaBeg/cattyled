/* Содержит различные вспомогательные функции для работы с светодиодной лентой */

byte getRealBrightness() {
  byte brightness = data.brightness;
  if (! data.power) brightness = 0;
  
  return brightnessFilter.filteredTime(brightness);
}


CRGB getFireColor(int val) {
  return CHSV(
           FIRE_HUE_START + map(val, 20, 60, 0, FIRE_HUE_GAP),                    // H
           constrain(map(val, 20, 60, FIRE_MAX_SAT, FIRE_MIN_SAT), 0, 255),       // S
           constrain(map(val, 20, 60, FIRE_MIN_BRIGHT, FIRE_MAX_BRIGHT), 0, 255)  // V
         );
}

void colorSmoothFill(byte h, byte s, byte v) {
  CRGB color = CHSV(h, s, v);
  CRGB lColor = leds[0];

  for (int i = 0; i < LED_AMOUNT; i++) {
    leds[i] = blend(leds[i], color, 15);
  }
  FastLED.show();
}

int antipodal_index(int i) {
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX) {
    iN = ( i + TOP_INDEX ) % LED_AMOUNT;
  }
  return iN;
}
