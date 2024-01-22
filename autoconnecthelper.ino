#ifdef DEBUG
  #define AC_DEBUG  //Enable Autoconnect Debug Messages
#endif
#include <AutoConnect.h>
#include <AutoConnectCredential.h>
#include <Preferences.h>
#include <ESPmDNS.h>


static const char PAGE_ADMIN[] PROGMEM = R"(
{
  "uri": "/adminlinks",
  "title": "Admin",
  "menu": true,
  "auth": "digest",
  "element": [
    {
      "name": "text",
      "type": "ACText",
      "value": "Admin Links",
      "style": "font-family:Arial;font-size:18px;font-weight:400;color:#191970"
    },
    {
      "name" : "DeleteAllSsids",
      "value" : "Delete All SSIDS",
      "type" : "ACSubmit",
      "uri" : "/deleteSsids"
    },
    {
      "name" : "EditFiles",
      "value" : "Edit Files",
      "type" : "ACSubmit",
      "uri" : "/edit"
    },
    {
      "name" : "Settings",
      "value" : "Settings",
      "type" : "ACSubmit",
      "uri" : "/settings"
    }
  ]
}
)";


  
const char* testHostname = "www.google.com";  
IPAddress testHostIP;
//String AsciiPassword;
WebServer Server;
AutoConnect portal(Server);
AutoConnectConfig Config;
//AutoConnectCredential Credential;
//AutoConnectUpdate autoConnectUpdate;
//AutoConnectUpdate update("10.100.6.36", 1380, "/updates/ESP32LR88", 30000, HIGH);
//WiFiServer server(80);
//WiFiServer tcpServer(0); carl

bool useStatic = false;
IPAddress staticIp(192, 168, 7, 2);
IPAddress staticGateway(192, 168, 7, 1);
IPAddress staticSubnet(255, 255, 255, 0);
IPAddress staticPrimaryDns(8, 8, 8, 8); 
IPAddress staticSecondaryDns(8, 8, 4, 4);

IPAddress softApIp(10,17,28,1);  //Soft AP IP Address  https://github.com/Hieromon/AutoConnect/issues/85#issuecomment-522297724
IPAddress softApNetmask(255,255,255,0); //Soft Ap Netmask
IPAddress softApGateway(10,17,28,1);  //Soft AP Gatewayy 0,0,0,0 for no Gateway

bool connectedToAP = false;
uint8_t wifiState;
void setupAutoConnect(void){
  //Credential = AutoConnectCredential();
  if(useStatic){
    Config.staip = staticIp;   //Static IP when Connected to AP
    Config.staGateway = staticGateway; //Static Gateway
    Config.staNetmask = staticSubnet; //Static Netmask
    Config.dns1 = staticPrimaryDns; // Static DNS1
    Config.dns2 = staticSecondaryDns; // Static DNS2
  }
  Config.beginTimeout = 15000; // Timeout sets to 15[s]  https://hieromon.github.io/AutoConnect/apiconfig.html#begintimeout
  Config.boundaryOffset = 256;
  Config.immediateStart = true;
  Config.autoRise = false; // https://hieromon.github.io/AutoConnect/apiconfig.html#autorise
  Config.retainPortal = true;    //https://hieromon.github.io/AutoConnect/apiconfig.html#retainportal
  Config.autoReconnect = true;   
  Config.reconnectInterval = 2;   //https://hieromon.github.io/AutoConnect/apiconfig.html#reconnectinterval  Click Reset to Reconnect
  Config.ota=AC_OTA_BUILTIN;
  Config.autoReset = false;
  String chipid = String((uint32_t)ESP.getEfuseMac(), HEX);
  hostName = hostNamePrefix + "_" + chipid.substring(chipid.length()-4);
  Config.apid = hostName; //Soft AP SSID
  Config.psk = ""; //Soft AP Password Blank for No Password
  Config.apip = softApIp;  //Soft AP IP Address  https://github.com/Hieromon/AutoConnect/issues/85#issuecomment-522297724
  Config.netmask = softApNetmask; //Soft Ap NetMask
  Config.gateway = softApGateway;  //Soft AP Gatewayy 0,0,0,0 for no Gateway
  Config.ticker = true;
  Config.tickerPort = BUILTIN_LED;
  Config.tickerOn = HIGH;
  //Config.autoSave = AC_SAVECREDENTIAL_NEVER;
  //Config.authScope = AC_AUTHSCOPE_PARTIAL;
  Config.auth = AC_AUTH_DIGEST;
  Config.authScope = AC_AUTHSCOPE_PORTAL;
  Config.username = "admin";
  Config.password = "admin";
  Config.menuItems = (AC_MENUITEM_DELETESSID | AC_MENUITEM_DEVINFO | AC_MENUITEM_CONFIGNEW | AC_MENUITEM_OPENSSIDS | AC_MENUITEM_DISCONNECT | AC_MENUITEM_RESET | AC_MENUITEM_HOME);
  portal.config(Config);
  portal.load(FPSTR(PAGE_ADMIN));
  //update.setLedPin(Led, LOW);
  //update.attach(portal);
  setupHttpServer();
  portal.onDetect(atDetect);
  portal.onConnect(onConnect); 
  Serial.println("Calling portal.begin()");
  canvas.println("portal.begin()");
  connectToAp();
  Config.autoRise = true;  //this is the trick to getting the loop to run and softap to start if not connected 
  portal.config(Config);
  MDNS.begin(mDnsHostName);
  MDNS.setInstanceName(hostName);
  MDNS.addService("_http", "_tcp", 80);
}

void connectToAp(void){
  connectedToAP = portal.begin();
  Serial.println("portal.begin() returned " + String(connectedToAP));
  wifiState = portal.portalStatus();
  if (WiFi.status() == WL_CONNECTED) {
    if (wifiState & AutoConnect::AC_AUTORECONNECT){
      Serial.println("Auto reconnection applied");
    }
    
    Serial.println("WiFi connection Established. " +  WiFi.SSID());
    strWifiStatus = "Wifi Connected. " +  WiFi.SSID();
    
  }
  else {
    if (wifiState & AutoConnect::AC_TIMEOUT){
      Serial.println("Connection timeout");
    }
  }

  if (wifiState & AutoConnect::AC_CAPTIVEPORTAL){
    Serial.println("Captive portal activated. " + WiFi.softAPSSID());
    strWifiStatus = "SoftAP " + WiFi.softAPSSID() ;
  }else{
    Serial.println("Captive portal is not available");
    strWifiStatus = "No SoftAP";
  }
}
bool isFirstLoop = false;
void loopAutoConnect(void){
  portal.handleClient(); 
  uint8_t transition = portal.portalStatus();
  if (transition != wifiState) {
    if (transition & AutoConnect::AC_CAPTIVEPORTAL){
      Serial.println("Captive portal activated. " + WiFi.softAPSSID());
      strWifiStatus = "SoftAP " + WiFi.softAPSSID() ;
    }
    if (transition & AutoConnect::AC_AUTORECONNECT){
      Serial.println("Auto reconnection applied");
    }
    if ((transition & AutoConnect::AC_ESTABLISHED)){
      Serial.println("WiFi connection Established. " +  WiFi.SSID());
      strWifiStatus = "Wifi Connected. " +  WiFi.SSID();
    }
    wifiState = transition;
  }
  if(isFirstLoop){
    isFirstLoop = true;
    //connectToAp();
    
  }
}

void deleteAllCredentials(void) {
  AutoConnectCredential credential;
  station_config_t config;
  uint8_t ent = credential.entries();

  Serial.println("AutoConnectCredential deleting");
  if (ent)
    Serial.printf("Available %d entries.\n", ent);
  else {
    Serial.println("No credentials saved.");
    return;
  }

  while (ent--) {
    credential.load((int8_t)0, &config);
    if (credential.del((const char*)&config.ssid[0]))
      Serial.printf("%s deleted.\n", (const char*)config.ssid);
    else
      Serial.printf("%s failed to delete.\n", (const char*)config.ssid);
  }
}

bool atDetect(IPAddress ip) {
  Serial.println("Captive Portal started, IP:" + ip.toString());

  return true;
}

void onConnect(IPAddress& ipaddr) {
  Serial.print("WiFi connected with ");
  Serial.print(WiFi.SSID());
  Serial.print(", IP:");
  Serial.println(ipaddr.toString());
  // if (WiFi.getMode() & WIFI_AP) {
  //   WiFi.softAPdisconnect(true);
  //   WiFi.enableAP(false);
  //   Serial.printf("SoftAP:%s shut down\n", WiFi.softAPSSID().c_str());
  // }
  if (WiFi.hostByName(testHostname, testHostIP)) {
      Serial.println("Resolved " +  String(testHostname));
  } else {
      Serial.println("Unable to resolve " +  String(testHostname));
  }
}

void loadnvmSettings(void){
    unsigned int x; 
    nvm.begin("digitalexample", false);    // Note: Namespace name is limited to 15 chars
  //   local_IP = nvm.getUInt("IPAddress", 0);
  //   gateway = nvm.getUInt("GateWay", 0);
  //   subnet = nvm.getUInt("SubNet", 0);
  //   primaryDNS = nvm.getUInt("primaryDNS", 0);
  //   secondaryDNS = nvm.getUInt("secondaryDNS", 0);
  //   nvm.getString("ssid", ssid, sizeof(ssid)-1);
  //   nvm.getString("WifiPassword", WifiPassword, sizeof(WifiPassword)-1);
  //   //strcpy(password2, "********");
  //   //nvm.getString("AsciiPassword", AsciiPassword, BUFSIZE-1); 
  //  //AsciiPort = nvm.getUInt("AsciiPort", 80);
}