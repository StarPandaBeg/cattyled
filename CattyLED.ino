#define CATTY_VERSION "CattyLED v0.0.1 (27.10.2022) by StarPanda"

#include "Locale.h"
#include "Libraries.h"
#include "Configuration.h"
#include "Global.h"

void setup() {
  startup();
}

void loop() {
  btnTick();

  animationTick();
}
