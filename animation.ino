/* Содержит алгоритмы анимации светодиодной ленты */

void animationTick() {
  if (!animation_flag) return;
  
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
