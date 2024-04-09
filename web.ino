void initServer() {
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html");
  });
}

void serverWriteData(IPAddress ip, bool is_ap) {
  File file = LittleFS.open("/config.js", "w");
  
  String s = String("window.catl_addr='");
  s += ip.toString();
  s += "';window.catl_header='";
  s += PROTOCOL_HEADER;
  s += "';window.catl_ap=";
  s += is_ap ? "true;" : "false;";
  
  file.print(s);
  delay(1);
  file.close();
}

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html");
  }
};
