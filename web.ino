void initServer() {
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
}

void serverWriteData(IPAddress ip) {
  File file = LittleFS.open("/config.js", "w");
  
  String s = String("window.catl_addr='");
  s += ip.toString();
  s += "';window.catl_header='";
  s += PROTOCOL_HEADER;
  s += "';";
  
  file.print(s);
  delay(1);
  file.close();
}
