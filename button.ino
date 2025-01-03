/* Функции для кнопки */

void btnTick() {
  buttonTick();
  vibroButtonTick();
}

void buttonTick() {
  if (!btn.poll(!digitalRead(BTN_PIN))) return;

  if (btn.hasClicks(1)) {
    sleep();
    return;
  }
  if (btn.hasClicks(2)) {
    vibroFlag = !vibroFlag;
  }
  static int8_t dir = 8;
  if (btn.step()) {
    data.brightness = constrain(data.brightness + dir, 20, 255);
    if (data.brightness == 255) {
      animationFlag = false;
      animationBlink();
    }
  }
  if (btn.releaseStep()) {
    dir = -dir;
    animationFlag = true;
    socketSend(packetStatus());
    memory.update();
  }
}

void vibroButtonTick() {
  if (!vibroBtn.poll(vibroSensorRead(VIBRO_PIN))) return;
  if (!vibroFlag) return;
  
  if (vibroBtn.hasClicks(1)) {
    data.power = !data.power;
    mqttSend(packetPower());
    socketSend(packetPower());
    memory.update();
  }
  if (!data.power) return;
  if (vibroBtn.hasClicks(4) && data.mode == 0) {
    data.colorH = (data.colorH + BTN_COLOR_STEP > 255) ? 0 : data.colorH + BTN_COLOR_STEP;
    mqttSend(packetColor());
    socketSend(packetColor());
    memory.update();
  }
  if (vibroBtn.hasClicks(3)) {
    data.mode = (data.mode + 1 >= MODE_AMOUNT) ? 0 : data.mode + 1;
    mqttSend(packetMode());
    socketSend(packetMode());
    memory.update();
  }
  if (vibroBtn.hasClicks(2)) {
    mqttSend(packetWink());
    socketSend(packetWink());
    animationWink();
  }
}

void btnAPTick() {
  btn.poll(!digitalRead(BTN_PIN));
  if (btn.hasClicks(1)) {
    sleep();
  }
  if (btn.hasClicks(10)) { memory.reset(); ESP.restart(); }
}

bool btnOTATick() {
  btn.poll(!digitalRead(BTN_PIN));
  if (btn.hasClicks(1) && !otaInProgress) {
    return true;
  }
  return false;
}

bool btnWifiTick() {
  if (!btn.poll(!digitalRead(BTN_PIN))) return false;
  if (btn.hasClicks(1)) { sleep(); return true; }
  if (btn.hasClicks(3)) { return true; }
  if (btn.hasClicks(5)) { 
    otaMode = true;
    return true; 
  }
  return false;
}

bool vibroSensorRead(const uint8_t pin) {
  static bool flag = false;
  static long tmr = 0;
  
  byte val = 1 - digitalRead(pin);
  if (val == 1 && !flag && millis() - tmr > 150) {
    flag = true;
    tmr = millis();
  } else if (val == 0 && flag && millis() - tmr > 150) {
    flag = false;
    tmr = millis();
  }
  return flag;
}
