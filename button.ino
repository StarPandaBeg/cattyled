void btnTick() {
  if (!btn.poll(digitalRead(BTN_PIN))) return;

  if (btn.hasClicks(1)) {
    data.power = !data.power;
  }

  if (btn.hasClicks(2) && data.mode == 0) {
    data.colorH += 20;
  }
}
