void btn_tick() {
  if (!btn.poll(digitalRead(BTN_PIN))) return;
}
