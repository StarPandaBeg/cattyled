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
    
  fill_solid(leds, LED_AMOUNT, blend(lColor, color, 15));
}
