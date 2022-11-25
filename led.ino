/* Содержит различные вспомогательные функции для работы с светодиодной лентой */

byte getRealBrightness() {
  byte brightness = data.brightness;
  if (! data.power) brightness = 0;
  
  return brightnessFilter.filteredTime(brightness);
}

void colorSmoothFill(byte h, byte s, byte v) {
  CRGB color = CHSV(h, s, v);
  CRGB lColor = leds[0];
    
  fill_solid(leds, LED_AMOUNT, blend(lColor, color, 15));
}
