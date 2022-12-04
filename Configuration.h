/* Содержит большинство настроек прошивки */

// Режим отладки
// 0 - отладка отключена
// 1 - вывод лога в COM-порт
// 2 - вывода данных для плоттера
#define DEBUG_MODE 2

// Настройки светодиодов
#define LED_PIN 1
#define LED_AMOUNT 16
#define LED_TYPE WS2812B
#define LED_ORDER GRB

// Настройки кнопки
#define BTN_PIN D2
#define BTN_COLOR_STEP 20

// Настройки подключения
#define WIFI_CONNECTION_TIMER 15000
#define WIFI_AP_NAME "CattyLED"
#define WIFI_AP_PASSWORD "cattyled"

// Настройки анимаций
#define HEART_HUE 0
#define HEART_SAT 255
#define HEART_VAL 80
#define HEART_BEAT_HUE 0
#define HEART_BEAT_SAT 255
#define HEART_BEAT_VAL 255

#define FIRE_HUE_START 0
#define FIRE_HUE_GAP 16
#define FIRE_SMOOTH_COEF 0.1
#define FIRE_MIN_BRIGHT 80
#define FIRE_MAX_BRIGHT 255
#define FIRE_MIN_SAT 245
#define FIRE_MAX_SAT 255

struct LampData {
  bool power = false;
  byte colorH = 0;
  byte colorS = 255;
  byte colorV = 255;
  byte mode = 0;

  byte brightness = 150;

  char wifi_ssid[24];
  char wifi_password[24];
  uint8_t ip[4] = {0, 0, 0, 0};
};

// ==========
#if DEBUG_MODE == 1
#define DEBUG(x) Serial.print(x)
#define DEBUGF(x,y) Serial.printf(x,y)
#define DEBUGLN(x) Serial.println(x)
#define DEBUGPLOT(x)
#define DEBUGPLOTSTART(x)
#define DEBUGPLOTEND(x)
#define DEBUGPLOTONE(x)
#elif DEBUG_MODE == 2
#define DEBUG(x)
#define DEBUGF(x,y)
#define DEBUGLN(x)
#define DEBUGPLOT(x) Serial.print(F(" "));Serial.print(x)
#define DEBUGPLOTSTART(x) Serial.print(x)
#define DEBUGPLOTEND(x) Serial.print(F(" "));Serial.println(x)
#define DEBUGPLOTONE(x) Serial.println(x);
#else
#define DEBUG(x)
#define DEBUGF(x,y)
#define DEBUGLN(x)
#define DEBUGPLOT(x)
#define DEBUGPLOTSTART(x)
#define DEBUGPLOTEND(x)
#define DEBUGPLOTONE(x)
#endif

#define MODE_AMOUNT 6
