void mdnsInit() {
  mdnsActive = MDNS.begin(MDNS_DOMAIN);
  
  if (mdnsActive) {
    DEBUG(F(L_MDNS_OK));
    DEBUGLN(F(MDNS_DOMAIN));
    MDNS.addService("http", "tcp", 80);
  } else {
    DEBUGLN(F(L_MDNS_FAIL));
  }
}

void mdnsTick() {
  MDNS.update();
}
