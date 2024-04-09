/* Содержит алгоритмы анимации светодиодной ленты */

void animationTick() {
  if (!animationFlag) return;
  byte br = getRealBrightness();

  if(fireTimer.period()) {
    for (int i = 0; i < LED_AMOUNT; i++) {
      zoneRndValues[i] = random(0, 10);
    }
  }

  if (glowTimer.period()) {
    lIndex = ((lIndex + 1) >= LED_AMOUNT) ? 0 : (lIndex + 1);
  }
  
  if (ledUpdateTimer.period()) {
    FastLED.setBrightness(br);
    FastLED.show();
  }
  if (!(animationTimer.period())) return;
  if (br <= 1) {
    if (ledPowerFlag) led_set_state(false);
    return;
  } else {
    if (!ledPowerFlag) led_set_state(true);
  }

  switch (data.mode) {
    case 0:
      animationSingle();
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
    case 5:
      animationLights();
      break;
  }

  DEBUGPLOTONE(getRealBrightness());
}

void animationLoadingTick(CRGB color) {
  if (animationTimer.period()) {
    animationLoading(color);
  }
}

bool animationLoadingEndTick() {
  loadingDirection = (loadingDirection > 0) ? loadingDirection : -loadingDirection;

  if (animationTimer.period()) {  
    return animationLoadingEnd();
  }
  return false;
}

void animationSingle() {
  colorSmoothFill(data.colorH, data.colorS, data.colorV);
  FastLED.show();
}

void animationRainbow() {
  static uint8_t hue = 0;
  hue++;
  colorSmoothFill(hue, 255, 255);
  FastLED.show();
}

void animationGlow() {
  int indexA = lIndex;
  int indexB = antipodalIndex(lIndex);

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
    FastLED.show();

    if (heartSecondary) {
      heartTimer2.state = 0;
    } else {
      heartTimer2.restart();
    }
  } else if (heartDelay) {
    colorSmoothFill(HEART_HUE, HEART_SAT, HEART_VAL);
    FastLED.show();
  }
}

void animationFire() {
  int thisPos = 0, lastPos = 0;
  for (int i = 0; i < LED_AMOUNT; i++) {
    zoneValues[i] = (float)zoneValues[i] * (1 - FIRE_SMOOTH_COEF) + (float)zoneRndValues[i] * 10 * FIRE_SMOOTH_COEF;
    leds[i] = getFireColor(zoneValues[i]);
  }
  FastLED.show();
}

void animationLights() {
  static byte colorIndex = 0;
  static bool dir = false;

  if (lightsTimer1.period()) dir = !dir;
  if (lightsTimer2.period()) colorIndex = (colorIndex + 1 > 5) ? 0 : colorIndex + 1;

  colorSmoothFill(dir ? lightsColors[colorIndex] : CRGB::Black);
  FastLED.show();
}

void animationLoading(CRGB color) { 
  loadingValue += loadingDirection;
  if (loadingValue >= LOADING_ANIMATION_BRIGHTNESS_MAX || loadingValue <= LOADING_ANIMATION_BRIGHTNESS_MIN) loadingDirection = -loadingDirection;
  fill_solid(leds, LED_AMOUNT, color);
  FastLED.setBrightness(loadingValue);
  FastLED.show();
}

bool animationLoadingEnd() {
  loadingValue -= loadingDirection;
  if (loadingValue <= 0) {
    loadingValue = LOADING_ANIMATION_BRIGHTNESS_MIN;
    FastLED.clear(); 
    FastLED.show();
    return true;
  }
  FastLED.setBrightness(loadingValue);
  FastLED.show();  
  return false;
}

void animationWink() {
  if (! data.power) return;
  brightLoop(data.brightness, 0, 20);
  brightLoop(0, 255, 20);
  brightLoop(255, 0, 20);
  brightLoop(0, 255, 20);
  brightLoop(255, 0, 20);
  brightLoop(0, data.brightness, 20);
}
