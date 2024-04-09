float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void initBattery() {
  int percent = getBatteryPercentage();
  for (int i = 0; i < 100; i++) {
    batteryFilter.filtered(percent);
  }
}

void batteryTick() {
}

float getBatteryVoltage() {
  float sensorValue = analogRead(BATTERY_PIN);
  return ((sensorValue * 3.3) / 1024) * 2 + BATTERY_COEF;
}

int getBatteryPercentage() {
  float voltage = getBatteryVoltage();
  float val = mapfloat(voltage, 2.8, 4.2, 0, 100);
  val = (val > 100) ? 100 : val;
  val = (val < 0) ? 0 : val;
  return val;
}

int getBatteryPercentageFiltered() {
  return batteryFilter.filtered(getBatteryPercentage());
}

bool getChargingState() {
  return digitalRead(CHARGING_PIN);
}

bool isBatteryLow() {
  return getBatteryPercentage() <= BATTERY_LOW_LEVEL;
}

bool isBatteryCritical() {
  return getBatteryPercentage() <= BATTERY_CRITICAL_LEVEL;
}
