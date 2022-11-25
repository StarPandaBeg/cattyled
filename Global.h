/* Содержит объявление глобальных переменных */

struct LampData data;
CRGB leds[LED_AMOUNT];
VButton btn;
GFilterRA brightnessFilter;

Timer animationTimer(20);

bool animation_flag = true;
