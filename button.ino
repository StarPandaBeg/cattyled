/* Функции для кнопки */

void btnTick() {
  if (!btn.poll(digitalRead(BTN_PIN))) return;

  if (btn.hasClicks(1)) {
    data.power = !data.power;
    mqttSend(packetPower());
    socketSend(packetPower());
    memory.update();
  }
  if (!data.power) return;

  if (btn.hasClicks(2) && data.mode == 0) {
    data.colorH = (data.colorH + BTN_COLOR_STEP > 255) ? 0 : data.colorH + BTN_COLOR_STEP;
    mqttSend(packetColor());
    socketSend(packetColor());
    memory.update();
  }
  if (btn.hasClicks(3)) {
    data.mode = (data.mode + 1 >= MODE_AMOUNT) ? 0 : data.mode + 1;
    mqttSend(packetMode());
    socketSend(packetMode());
    memory.update();
  }
  if (btn.hasClicks(4)) {
    mqttSend(packetWink());
    socketSend(packetWink());
    animationWink();
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

bool btnAPTick() {
  if (!btn.poll(digitalRead(BTN_PIN))) return false;
  return btn.hasClicks(3);
}
