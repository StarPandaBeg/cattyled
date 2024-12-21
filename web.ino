void initServer() {
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Cache-Control", "public, max-age=604800");
  DefaultHeaders::Instance().addHeader("ETag", fsVersion);
  
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
  s += "window.catl_id_local='" LOCAL_ID "';";
  s += "window.catl_id_remote='" REMOTE_ID "';";
  s += "window.catl_update_server='" UPDATE_SERVER "';";
  s += "window.catl_battery=";
  s += USE_BATTERY ? "true;" : "false;";
  
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
