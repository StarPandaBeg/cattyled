/* Содержит функции для работы с EEPROM памятью */
void memorySyncIP() {
  IPAddress ip = IPAddress(data.ip[0], data.ip[1], data.ip[2], data.ip[3]);
  if (ip != WiFi.localIP()) {
    ip = WiFi.localIP();
    for (int i = 0; i < 4; i++) data.ip[i] = ip[i];
    memory.update();
  }
}

void initMemory() {
  EEPROM.begin(sizeof(data) + 1);
  memory.begin(0, EEPROM_KEY);

  DEBUG(F(L_EEPROM_INIT_COMPLETED));
  DEBUG(F(L_QUOTE_L));
  DEBUG(EEPROM_KEY); 
  DEBUGLN(F(L_QUOTE_R));
}

void memoryTick() {
  memory.tick();
}

void memoryReset() {
  memory.reset();
}
