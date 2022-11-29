/* Функции для кнопки */

void btnTick() {
  if (!btn.poll(digitalRead(BTN_PIN))) return;

  if (btn.hasClicks(1)) {
    data.power = !data.power;
  }
  if (!data.power) return;

  if (btn.hasClicks(2) && data.mode == 0) {
    data.colorH = (data.colorH + BTN_COLOR_STEP > 255) ? 0 : data.colorH + BTN_COLOR_STEP;
  }
  if (btn.hasClicks(3)) {
    data.mode = (data.mode + 1 >= MODE_AMOUNT) ? 0 : data.mode + 1;
  }

  static int8_t dir = 8;
  if (btn.step()) {
    data.brightness = constrain(data.brightness + dir, 20, 255);
    if (data.brightness == 255) {
      animation_flag = false;
      animationBlink();
    }
  }
  if (btn.releaseStep()) {
    dir = -dir;
    animation_flag = true;
  }
}
