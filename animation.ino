/* Содержит алгоритмы анимации светодиодной ленты */

void animationTick() {
  if (!animation_flag) return;

  if(fireTimer.period()) {
    for (int i = 0; i < LED_AMOUNT; i++) {
      zoneRndValues[i] = random(0, 10);
    }
  }
  
  if (ledUpdateTimer.period()) {
    FastLED.setBrightness(getRealBrightness());
    FastLED.show();
  }
  if (!(animationTimer.period())) return;

  switch (data.mode) {
    case 0:
      colorSmoothFill(data.colorH, data.colorS, data.colorV);
      break;
    case 1:
      animationRainbow();
      break;
    case 4:
      animationFire();
      break;
  }

  DEBUGPLOTONE(getRealBrightness());
}

void animationRainbow() {
  static uint8_t hue = 0;
  hue++;
  colorSmoothFill(hue, 255, 255);
}

void animationBlink() {
  static bool dir = false;
  if (blinkTimer.period()) {
    if (dir) FastLED.setBrightness(255);
    else FastLED.setBrightness(60);
    FastLED.show();
    dir = !dir;
  }
}

void animationFire() {
  int thisPos = 0, lastPos = 0;
  for (int i = 0; i < LED_AMOUNT; i++) {
    zoneValues[i] = (float)zoneValues[i] * (1 - FIRE_SMOOTH_COEF) + (float)zoneRndValues[i] * 10 * FIRE_SMOOTH_COEF;
    leds[i] = getFireColor(zoneValues[i]);
  }
}
