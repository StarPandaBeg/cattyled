/* Содержит алгоритмы анимации светодиодной ленты */

void animationTick() {
  if (!animation_flag) return;

  if(fireTimer.period()) {
    for (int i = 0; i < LED_AMOUNT; i++) {
      zoneRndValues[i] = random(0, 10);
    }
  }

  if (glowTimer.period()) {
    l_index = ((l_index + 1) >= LED_AMOUNT) ? 0 : (l_index + 1);
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
    case 2:
      animationGlow();
      break;
    case 3:
      animationHeart();
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

void animationGlow() {
  int indexA = l_index;
  int indexB = antipodal_index(l_index);

  leds[indexA] = CHSV(0, 255, 255);
  leds[indexB] = CHSV(160, 255, 255);
  FastLED.show();
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

void animationHeart() {
  bool heartMain = heartTimer1.period();
  bool heartSecondary = heartTimer2.period();
  bool heartDelay = heartTimer3.period();
  
  if (heartMain || heartSecondary) {
    colorSmoothFill(HEART_BEAT_HUE, HEART_BEAT_SAT, HEART_BEAT_VAL);

    if (heartSecondary) {
      heartTimer2.state = 0;
    } else {
      heartTimer2.restart();
    }
  } else if (heartDelay) {
    colorSmoothFill(HEART_HUE, HEART_SAT, HEART_VAL);
  }
}

void animationFire() {
  int thisPos = 0, lastPos = 0;
  for (int i = 0; i < LED_AMOUNT; i++) {
    zoneValues[i] = (float)zoneValues[i] * (1 - FIRE_SMOOTH_COEF) + (float)zoneRndValues[i] * 10 * FIRE_SMOOTH_COEF;
    leds[i] = getFireColor(zoneValues[i]);
  }
}
