/* Содержит алгоритмы анимации светодиодной ленты */

void animationTick() {
  if (!(animationTimer.period() && animation_flag)) return;
  
  switch (data.mode) {
    case 0:
      colorSmoothFill(data.colorH, data.colorS, data.colorV);
      break;
  }

  DEBUGPLOTONE(getRealBrightness());
  FastLED.setBrightness(getRealBrightness());
  FastLED.show();
}
