#define FIRMWARE_VERSION "1.2.0"
#define CATTY_VERSION "CattyLED v" FIRMWARE_VERSION " by StarPanda"

#include "Locale.h"
#include "Libraries.h"
#include "Configuration.h"
#include "Global.h"

void setup() {
  startup();
}

void loop() {
  if (needUpdate) {
    otaStartUpdate();
    needUpdate = false;
    ws.enable(true);
    return;
  }

  #if USE_BATTERY
    batteryTick();
  #endif
  
  btnTick();
  mqttTick();
  mdnsTick();
  socketTick();
  animationTick();
  memoryTick();
}
