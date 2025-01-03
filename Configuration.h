/* Содержит большинство настроек прошивки */

// Режим отладки
// 0 - отладка отключена
// 1 - вывод лога в COM-порт
// 2 - вывода данных для плоттера
#define DEBUG_MODE 0

#define USE_BATTERY 0

// Инвертирует ID ламп
// Установите в 1, чтобы получить прошивку для лампы-пары
#define INVERT_PAIR 0

// Настройки светодиодов
#define LED_PIN 1
#define LED_AMOUNT 16
#define LED_TYPE WS2812B
#define LED_ORDER GRB

// Настройки кнопки
#define BTN_PIN D2
#define BTN_COLOR_STEP 20

// Настройки вибродатчика
#define VIBRO_PIN D3
#define VIBRO_POWER_PIN D5

// Мосфет
#define MOSFET_PIN D6
#define MOSFET_OPENED_STATE LOW
#define MOSFET_CLOSED_STATE HIGH

// Индикатор заряда
#define CHARGING_PIN D7
#define BATTERY_PIN A0
#define BATTERY_COEF 0.36
#define BATTERY_LOW_LEVEL 25
#define BATTERY_CRITICAL_LEVEL 10

// Настройки подключения
#define WIFI_CONNECTION_TIMER 15000
#define WIFI_AP_NAME "CattyLED"
#define WIFI_AP_PASSWORD "cattyled"
#define WIFI_OTA_AP_NAME "CattyLED_OTA"
#define WIFI_OTA_AP_PASSWORD "cattyled"

// Настройки MQTT соединения
#define MQTT_HOST "broker.emqx.io"
#define MQTT_PORT 1883
#define MQTT_PREFIX "/HXKxvE3FvgcWUQ4u/CattyLED_/"
#define MQTT_USER ""
#define MQTT_PASS ""

// Настройки пары ламп
#define _LOCAL_ID "LampKo"
#define _REMOTE_ID "LampDi"

#if INVERT_PAIR == 1
#define LOCAL_ID _REMOTE_ID
#define REMOTE_ID _LOCAL_ID
#else
#define LOCAL_ID _LOCAL_ID
#define REMOTE_ID _REMOTE_ID
#endif

// Настройки протокола соединения. Не рекомендуется изменять без крайней необходимости.
#define PROTOCOL_HEADER "CATL:"
#define PROTOCOL_SEPARATOR ","
#define PROTOCOL_APP_HEADER "CATLAPP:"

#define MDNS_DOMAIN "cattyled"

// Настройки обновления
#define UPDATE_SERVER "https://cattyled-server.onrender.com/"
#define UPDATE_URL UPDATE_SERVER LOCAL_ID "/firmware.bin"
#define UPDATE_VERSION_URL UPDATE_SERVER LOCAL_ID "/firmware_ver.txt"
#define UPDATE_FS_URL UPDATE_SERVER LOCAL_ID "/fs.bin"
#define UPDATE_FS_VERSION_URL UPDATE_SERVER LOCAL_ID "/fs_ver.txt"

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

#define LOADING_ANIMATION_BRIGHTNESS_MIN 5
#define LOADING_ANIMATION_BRIGHTNESS_MAX 140

// Ключ EEPROM. При изменении все содержимое EEPROM перезаписывается.
#define EEPROM_KEY 'b'

struct LampData {
  bool power = false;
  byte colorH = 0;
  byte colorS = 255;
  byte colorV = 255;
  byte mode = 0;

  byte brightness = 150;

  char wifiSSID[24] = "";
  char wifiPassword[24] = "";
  uint8_t ip[4] = {0, 0, 0, 0};

  char localName[10] = LOCAL_ID;
  char remoteName[10] = REMOTE_ID;

  bool mqttActive = true;
  char mqttHost[32] = MQTT_HOST;
  uint16_t mqttPort = MQTT_PORT;
  char mqttPrefix[32] = MQTT_PREFIX;
  char mqttUser[32] = MQTT_USER;
  char mqttPassword[32] = MQTT_PASS;
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
