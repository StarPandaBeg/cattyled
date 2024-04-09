/* Содержит объявление глобальных переменных */
const int TOP_INDEX = int(LED_AMOUNT / 2);
const uint8_t HEADER_LENGTH = strlen(PROTOCOL_HEADER);
char* fsVersion;

struct LampData data;
CRGB leds[LED_AMOUNT];
VButton btn, vibroBtn;
GFilterRA brightnessFilter;
GFilterRA batteryFilter;
WiFiClient espClient;
PubSubClient mqtt(espClient);
EEManager memory(data);
DNSServer dnsServer;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
HTTPClient http;

byte zoneValues[LED_AMOUNT];
byte zoneRndValues[LED_AMOUNT];

CRGB lightsColors[] = { CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow, CRGB::Magenta, CRGB::Cyan };

int lIndex = 0;
bool isAP = false;
bool onlineFlag = false;
bool vibroFlag = true;
bool ledPowerFlag = false;
bool needUpdate = false;
bool animationFlag = true;

bool needUpdateCheck = false;
AsyncWebSocketClient* lastClient;
short updateType = -1;

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
Timer lightsTimer1(800);
Timer lightsTimer2(1600);
Timer mqttConnectTimer(15000, false);
Timer remoteOnlineTimer(15000, false);
Timer mqttPingTimer(5000);
