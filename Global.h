/* Содержит объявление глобальных переменных */

struct LampData data;
CRGB leds[LED_AMOUNT];
VButton btn;
GFilterRA brightnessFilter;

byte zoneValues[LED_AMOUNT];
byte zoneRndValues[LED_AMOUNT];

Timer animationTimer(30);
Timer ledUpdateTimer(1);
Timer blinkTimer(150);
Timer fireTimer(100);

bool animation_flag = true;
