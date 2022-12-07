/* Содержит объявление глобальных переменных */
const int TOP_INDEX = int(LED_AMOUNT / 2);
const uint8_t HEADER_LENGTH = strlen(PROTOCOL_HEADER);

struct LampData data;
CRGB leds[LED_AMOUNT];
VButton btn;
GFilterRA brightnessFilter;
WiFiClient espClient;
PubSubClient mqtt(espClient);
EEManager memory(data);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

byte zoneValues[LED_AMOUNT];
byte zoneRndValues[LED_AMOUNT];

int lIndex = 0;
bool isAP = false;
bool onlineFlag = false;

int8_t loadingDirection = 1;
uint8_t loadingValue = LOADING_ANIMATION_BRIGHTNESS_MIN;

Timer animationTimer(20);
Timer ledUpdateTimer(1);
Timer blinkTimer(150);
Timer fireTimer(100);
Timer glowTimer(80);
Timer heartTimer1(2100);
Timer heartTimer2(500, false);
Timer heartTimer3(5);
Timer mqttConnectTimer(15000, false);
Timer remoteOnlineTimer(15000, false);
Timer mqttPingTimer(5000);

bool animationFlag = true;
