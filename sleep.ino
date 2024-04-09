void sleep() {
  DEBUGLN(F(L_SLEEP));

  memory.updateNow();
  power_turn_off();
  
  wifi_station_disconnect();
  wifi_set_opmode_current(NULL_MODE);
  wifi_fpm_set_wakeup_cb(on_wakeup);
  wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
  wifi_fpm_open();
  gpio_pin_wakeup_enable(BTN_PIN, GPIO_PIN_INTR_LOLEVEL);
  wifi_fpm_do_sleep(0xFFFFFFF);
  delay(20);
}

void on_wakeup() {
  DEBUGLN(F(L_WAKEUP));
  ESP.restart();
}

void led_set_state(bool st) {
  ledPowerFlag = st;
  digitalWrite(MOSFET_PIN, st ? MOSFET_OPENED_STATE : MOSFET_CLOSED_STATE);
}

void power_turn_off() {
  animationFlag = false;
  
  data.power = 0;
  int i = 0;
  while (i < 255) {
     if (ledUpdateTimer.period()) {
      FastLED.setBrightness(getRealBrightness());
      FastLED.show();
      i++;
    }
  }
  
  FastLED.clear();
  FastLED.setBrightness(0);
  FastLED.show();
  led_set_state(false);
  digitalWrite(VIBRO_POWER_PIN, 0);
}

void power_turn_on() {
  led_set_state(true);
  digitalWrite(VIBRO_POWER_PIN, 1);
  animationFlag = true;
}
