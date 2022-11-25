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

struct LampData {
  bool power = false;
  byte colorH = 0;
  byte colorS = 255;
  byte colorV = 255;
  byte mode = 0;

  byte brightness = 255;
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
