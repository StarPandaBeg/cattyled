/* Содержит объявление глобальных переменных */

struct LampData data;
CRGB leds[LED_AMOUNT];
VButton btn;
GFilterRA brightnessFilter;

Timer animationTimer(30);
Timer ledUpdateTimer(1);
Timer blinkTimer(150);

bool animation_flag = true;
