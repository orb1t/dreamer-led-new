//
// Created by orb1t on 13.12.20.
//

#ifndef INC_201122_212358_ESP32DEV_WIFIPORTAL_H
#define INC_201122_212358_ESP32DEV_WIFIPORTAL_H

#include <ESPmDNS.h>
#include "../../.pio/libdeps/esp32dev/ArduinoJson/ArduinoJson.h"
#include "../../.pio/libdeps/esp32dev/ArduinoJson/src/ArduinoJson/Deserialization/DeserializationError.hpp"
#include <Update.h>

extern "C" {
//#include <user_interface.h>
#include <esp_wifi.h>
}


static EventGroupHandle_t wifi_event_group;
const int CONNECTED_EVENT = BIT0;
const int DISCONNECTED_EVENT = BIT1;



// Wi-Fi Configurations
//#define WIFI_SSID "AndroidAP" //"GL-MT300N-V2-413" // Wi-Fi SSID
//#define AP_SSID "AndroidAP" //"GL-MT300N-V2-413" // Wi-Fi SSID
//#define WIFI_PASS "01234567"//"goodlife"         // Wi-Fi Password
#define WIFI_CONNECT_INTERVAL 5000   // Connection retry interval in milliseconds
#define WIFI_WATCHDOG_INTERVAL 5000  // Wi-Fi watchdog interval in milliseconds

#define SECRETS_PATH "/secrets.json"

#define WAIT_FOR_WIFI_TIME_OUT 6000UL

#define WIFI_CONNECT_RETRY_COUNT 10
#define WIFI_CONNECT_RETRY_DELAY_SEC 10
#define AP_TO_WIFI_CONNECT_RETRY_DELAY_SEC 60
#define WIFI_WATCHDOG_CONNECTION_RECHECK_DELAY_SEC 30
uint8_t __MAX_RETRY_COUNT = WIFI_CONNECT_RETRY_COUNT;
uint8_t __RETRY_COUNT = 0;

#define WIFI_CONNECT_MODE_NONE 0
#define WIFI_CONNECT_MODE_PORTAL 1
#define WIFI_CONNECT_MODE_STATION 2
uint8_t __WIFI_CONNECT_MODE = WIFI_CONNECT_MODE_NONE;
uint8_t __WIFI_CONNECT_MODE_LAST = WIFI_CONNECT_MODE_NONE;


// Callback methods prototypes
void wifi_connect_cb();
void wifi_watchdog_cb();
void printStationLists(void);
void startWifiAp();
char* stringToCharPtr(String* str);
void printFile(const char *filename);
bool startWifiSta(char* _ssid, char* _pass);
void handleGetSavSecreteJson(AsyncWebServerRequest *request);
void printStaClients();
bool loadConfiguration(const char *filename, std::vector<Config*> &config);
void runWifiPortal(AsyncWebServer* server);
bool isSsidInRange(String _ssid);
void startMDNS();
void handleFileList(AsyncWebServerRequest *request);
void handleFileDelete(AsyncWebServerRequest * request);
void _handleUpload(AsyncWebServerRequest *request, String filename, String redirect, size_t index, uint8_t *data, size_t len, bool final);
void getWifiScanJson(AsyncWebServerRequest * request);

// Tasks
//Task wifi_connect_task(WIFI_CONNECT_INTERVAL, TASK_FOREVER, &wifi_connect_cb);
//Task wifi_watchdog_task(WIFI_WATCHDOG_INTERVAL, TASK_FOREVER, &wifi_watchdog_cb);
//
//// Task runner
//Scheduler runner;


unsigned long restartSystem;
File fsUploadFile;

bool __STA_DISCONNECTED = true;

// Wi-Fi events
void _wifi_event(WiFiEvent_t event){
  switch (event) {
	case SYSTEM_EVENT_WIFI_READY:
	  Serial.print(PSTR("[Wi-Fi] Event: Wi-Fi interface ready\n"));
	  break;
	case SYSTEM_EVENT_SCAN_DONE:
	  Serial.print(PSTR("[Wi-Fi] Event: Completed scan for access points\n"));
	  break;
	case SYSTEM_EVENT_STA_START:
	  Serial.print(PSTR("[Wi-Fi] Event: Wi-Fi client started\n"));
	  xEventGroupSetBits(wifi_event_group, DISCONNECTED_EVENT);
	  break;
	case SYSTEM_EVENT_STA_STOP:
	  Serial.print(PSTR("[Wi-Fi] Event: Wi-Fi clients stopped\n"));
	  break;
	case SYSTEM_EVENT_STA_CONNECTED:
	  Serial.printf(PSTR("[Wi-Fi] Event: Connected to access point: %s \n"), WiFi.localIP().toString().c_str());
	  __STA_DISCONNECTED = false;
	  //    wifi_connect_task.disable();
	  xEventGroupClearBits(wifi_event_group, DISCONNECTED_EVENT);
	  break;
	case SYSTEM_EVENT_STA_DISCONNECTED:
	  if ( ! __STA_DISCONNECTED ) {
		Serial.print(PSTR("[Wi-Fi] Event: Not connected to Wi-Fi network\n"));
		//    wifi_connect_task.enable();
		xEventGroupSetBits(wifi_event_group, DISCONNECTED_EVENT);
		__STA_DISCONNECTED = true;
	  }
	  break;
	case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
	   Serial.print(PSTR("[Wi-Fi] Event: Authentication mode of access point has changed\n"));
	  break;
	case SYSTEM_EVENT_STA_GOT_IP:
	  Serial.printf(PSTR("[Wi-Fi] Event: Obtained IP address: %s\n"), WiFi.localIP().toString().c_str());
	  //    wifi_watchdog_task.enable();
	  xEventGroupSetBits(wifi_event_group, CONNECTED_EVENT);
	  break;
	case SYSTEM_EVENT_STA_LOST_IP:
	  Serial.print(PSTR("[Wi-Fi] Event: Lost IP address and IP address is reset to 0\n"));
//	  wifi_connect_task.enable();
	  break;
	case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
	   Serial.print(PSTR("[Wi-Fi] Event: Wi-Fi Protected Setup (WPS): succeeded in enrollee mode\n"));
	  break;
	case SYSTEM_EVENT_STA_WPS_ER_FAILED:
	   Serial.print(PSTR("[Wi-Fi] Event: Wi-Fi Protected Setup (WPS): failed in enrollee mode\n"));
//	  wifi_connect_task.enable();
	  break;
	case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
	   Serial.print(PSTR("[Wi-Fi] Event: Wi-Fi Protected Setup (WPS): timeout in enrollee mode\n"));
//	  wifi_connect_task.enable();
	  break;
	case SYSTEM_EVENT_STA_WPS_ER_PIN:
	   Serial.print(PSTR("[Wi-Fi] Event: Wi-Fi Protected Setup (WPS): pin code in enrollee mode\n"));
	  break;
	case SYSTEM_EVENT_AP_START:
	  Serial.print(PSTR("[Wi-Fi] Event: Wi-Fi access point started\n"));
//!!!!	  wifi_connect_task.enableDelayed(10000);
	  break;
	case SYSTEM_EVENT_AP_STOP:
	  Serial.print(PSTR("[Wi-Fi] Event: Wi-Fi access point  stopped\n"));
//	  wifi_connect_task.disable();
//!!!!	  wifi_connect_task.enableDelayed(10000);
	  xEventGroupSetBits(wifi_event_group, DISCONNECTED_EVENT);
	  break;
	case SYSTEM_EVENT_AP_STACONNECTED:
	  Serial.print(PSTR("[Wi-Fi] Event: Client connected\n"));
	  break;
	case SYSTEM_EVENT_AP_STADISCONNECTED:
	  Serial.print(PSTR("[Wi-Fi] Event: Client disconnected\n"));
	  break;
	case SYSTEM_EVENT_AP_STAIPASSIGNED:
	  Serial.print(PSTR("[Wi-Fi] Event: Assigned IP address to client\n"));
	  printStaClients();
	  break;
	case SYSTEM_EVENT_AP_PROBEREQRECVED:
	   Serial.print(PSTR("[Wi-Fi] Event: Received probe request\n"));
	  break;
	case SYSTEM_EVENT_GOT_IP6:
	   Serial.print(PSTR("[Wi-Fi] Event: IPv6 is preferred\n"));
	  break;
	case SYSTEM_EVENT_ETH_START:
	   Serial.print(PSTR("[Wi-Fi] Event: SYSTEM_EVENT_ETH_START\n"));
	  break;
	case SYSTEM_EVENT_ETH_STOP:
	   Serial.print(PSTR("[Wi-Fi] Event: SYSTEM_EVENT_ETH_STOP\n"));
	  break;
	case SYSTEM_EVENT_ETH_CONNECTED:
	   Serial.print(PSTR("[Wi-Fi] Event: SYSTEM_EVENT_ETH_CONNECTED\n"));
	  break;
	case SYSTEM_EVENT_ETH_DISCONNECTED:
	   Serial.print(PSTR("[Wi-Fi] Event: SYSTEM_EVENT_ETH_DISCONNECTED\n"));
	  break;
	case SYSTEM_EVENT_ETH_GOT_IP:
	   Serial.print(PSTR("[Wi-Fi] Event: SYSTEM_EVENT_ETH_GOT_IP\n"));
	  break;
	case SYSTEM_EVENT_MAX:
	  break;
  }
}



// Wi-Fi connect task
wl_status_t wifi_connect_lastStatus = WL_IDLE_STATUS;
/* Soft AP network parameters */
//IPAddress apIP(192, 168, 4, 1);
//IPAddress apIP(192, 168, 4, 1);
IPAddress apIP(8, 8, 8, 8);
IPAddress netMsk(255, 255, 255, 0);

// Wi-Fi watchdog task
wl_status_t wifi_watchdog_lastStatus = WL_IDLE_STATUS;
bool __PORTAL_STARTED = false;



class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request) {
	//request->addInterestingHeader("ANY");
	return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
	String _url = "http://" + String(host) + ".local/index.html";
	Serial.printf("CaptiveRequestHandler.handleRequest: redirecting to %s\n", _url.c_str());
	request->redirect(_url);
  }
};


void watchdog_task(void *pvParam){
  printf("watchdog_task task started \n");
  while(1){
	vTaskDelay(10000 / portTICK_PERIOD_MS );
	xEventGroupWaitBits(wifi_event_group,CONNECTED_EVENT,true,true,portMAX_DELAY);

//        tcpip_adapter_ip_info_t ip_info;
//      ESP_ERROR_CHECK(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info));
//      printf("IP :  %s\n", ip4addr_ntoa(&ip_info.ip));
	if (WiFi.status() != WL_CONNECTED) {
	  Serial.println(PSTR("[Watchdog] Wi-Fi Disconnected"));
	  // Disconnect Wi-Fi
	  //    WiFi.disconnect();
	  // Disable watchdog task
	  //    wifi_watchdog_task.disable();
	  // Enable Wi-Fi connect task to reconnect to Wi-Fi AP
	  //    wifi_connect_task.enableDelayed(2500);
	  xEventGroupSetBits(wifi_event_group, DISCONNECTED_EVENT);
	  return;
	}
	Serial.println(PSTR("[Watchdog] Wi-Fi is connected!"));
  }
}

void wifi_connect_task(void *pvParam){
  printf("wifi_connect_task task started \n");
  while(1){
	vTaskDelay(10000 / portTICK_PERIOD_MS );
	xEventGroupWaitBits(wifi_event_group,DISCONNECTED_EVENT,true,true,portMAX_DELAY);
	printf("----WiFi disconnected from AP----\n");
	if ( ! startWifiSta(config[0]->ssid, config[0]->pass) ) {
	  if ( ! __PORTAL_STARTED )
		startWifiAp();
	  //    wifi_connect_task.enableDelayed(10000);
	  vTaskDelay(10000 / portTICK_PERIOD_MS );
	  xEventGroupSetBits(wifi_event_group, DISCONNECTED_EVENT);
	} else {
	  vTaskDelay(10000 / portTICK_PERIOD_MS );
	  xEventGroupSetBits(wifi_event_group, CONNECTED_EVENT);
	  //    wifi_watchdog_task.enableDelayed(10000);
	}
  }
}




// Wi-Fi connect task
//void wifi_connect_cb()
//{
//  // Disable this task to avoid further iterations
//  wifi_connect_task.disable();
//  if ( ! startWifiSta(config[0]->ssid, config[0]->pass) ) {
//	if ( ! __PORTAL_STARTED )
//	  startWifiAp();
//	wifi_connect_task.enableDelayed(10000);
//  } else {
//	wifi_watchdog_task.enableDelayed(10000);
//  }
//}

// Wi-Fi watchdog task
//void wifi_watchdog_cb(){
//  wl_status_t status = WiFi.status();
////  Serial.println(PSTR("[Watchdog] Checking Wi-Fi ..."));
//  if (status != WL_CONNECTED)
//  {
//	Serial.println(PSTR("[Watchdog] Wi-Fi Disconnected"));
//	// Disconnect Wi-Fi
////    WiFi.disconnect();
//	// Disable watchdog task
//	wifi_watchdog_task.disable();
//	// Enable Wi-Fi connect task to reconnect to Wi-Fi AP
//	__PORTAL_STARTED = false;
//	wifi_connect_task.enableDelayed(10000);
//	return;
//  } else {
//	if (wifi_watchdog_lastStatus != WL_CONNECTED) {
//	  Serial.println(PSTR("[Watchdog] Wi-Fi is connected!"));
//	}
//  }
//  wifi_watchdog_lastStatus = status;
////  Serial.println(PSTR("[Watchdog] Wi-Fi is connected!"));
//}



bool startWifiSta(char* _ssid, char* _pass){
  bool res = false;
  if ( isSsidInRange ( _ssid ) ) {
	Serial.printf(PSTR("\n[Wi-Fi] Status: Connecting to SSID: %s...\n"), _ssid);

	// Disconnect from Wi-Fi network
	WiFi.disconnect();

	// Connect to Wi-Fi network
	WiFi.setHostname(host);
	WiFi.begin(_ssid, _pass);

	// Wait for connection result and capture the result code
	uint8_t result = WiFi.waitForConnectResult();

	Serial.printf("[Wi-Fi] Connection Result: %d \n", result);

	if (result == WL_NO_SSID_AVAIL || result == WL_CONNECT_FAILED)
	{
	  // Fail to connect or SSID no available
	  Serial.println(PSTR("[Wi-Fi] Status: Could not connect to Wi-Fi AP"));
	  // Wait and reenable this task to keep trying to connect
//      wifi_connect_task.enableDelayed(1000);
	}
	else if (result == WL_IDLE_STATUS)
	{
	  // Wi-Fi Idle. This means that it's connected to the AP but the DHCP has not assigned an IP yet
	  Serial.println(PSTR("[Wi-Fi] Status: Idle | No IP assigned by DHCP Server"));
//      WiFi.disconnect(); // Optional to disconnect and start again instead of wait for DHCP to assign IP.
	}
	else if (result == WL_CONNECTED)
	{
	  // Wi-Fi Connected
	  Serial.printf(PSTR("[Wi-Fi] Status: Connected | IP: %s\n"), WiFi.localIP().toString().c_str());
	  __WIFI_CONNECT_MODE_LAST = __WIFI_CONNECT_MODE;
	  __WIFI_CONNECT_MODE = WIFI_CONNECT_MODE_STATION;
	  MDNS.end();
	  serverPortal.reset();
	  serverPortal.end();
	  dnsServer.stop();

	  //init and get the time
	  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
	  getTime();
	  printTime(&timeinfo);
	  // Wi-Fi Connected

	  __RETRY_COUNT = 0;
	  setupServer(&server);
	  startMDNS();
	  led.blink1();
	  res = true;
	  __PORTAL_STARTED = false;
	}
  }
  return res;
}


void startWifiAp() {// Disconnect from Wi-Fi network
  MDNS.end();
  serverPortal.reset();
//  serverPortal.end();
  server.reset();
  server.end();
  WiFi.disconnect();
  vTaskDelay(500);
  WiFi.mode(WIFI_AP);                    // Changing ESP32 wifi mode to AccessPoint
//  WiFi.onEvent(_wifi_event);
  WiFi.softAPsetHostname(host);
  Serial.printf("\n[Wi-Fi] Status: Configuring soft-AP: %s...%s\n", ssid_ap, WiFi.softAPConfig(apIP, apIP, netMsk) ? "Ready" : "Failed!");
  // You can remove the Appassword parameter if you want the hotspot to be open.
  if ( password_ap != "" )
	WiFi.softAP(ssid_ap, password_ap);
  else
	WiFi.softAP(ssid_ap);
  IPAddress myIP = WiFi.softAPIP();
  Serial.printf(PSTR("[Wi-Fi] Status: Connected | IP: %s\n"), myIP.toString().c_str());
  __PORTAL_STARTED = true;


  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", WiFi.softAPIP());
//more handlers...
  runWifiPortal(&serverPortal) ;
  startMDNS();
  __WIFI_CONNECT_MODE_LAST = __WIFI_CONNECT_MODE;
  __WIFI_CONNECT_MODE = WIFI_CONNECT_MODE_PORTAL;
//!!!!  wifi_connect_task.enableDelayed(AP_TO_WIFI_CONNECT_RETRY_DELAY_SEC*1000);
//  led.blink4();
  led.fastBlinking();
}




void runWifiPortal(AsyncWebServer* server) {
  if ( __PORTAL_STARTED ) {
	Serial.println("Server already started! Restarting!");
	server->reset();
//    server->end();
	__PORTAL_STARTED = false;
  }
//  server->reset();


	if ( !__PORTAL_STARTED ) {
	  IPAddress myIP;
	  myIP = WiFi.softAPIP();

	  Serial.print(F("AP IP address: "));
	  Serial.println(myIP);

	  server->serveStatic("/", SPIFFS, "/ap/").setDefaultFile("index.html").setFilter(ON_AP_FILTER);

	  server->on("/secrets.json", HTTP_GET, [](AsyncWebServerRequest *request) {
		Serial.println("/secrets.json");
//		getWifiScanJson(request);
		request->send(SPIFFS, "/secrets.json", "application/json");

	  }).setFilter(ON_AP_FILTER);

	  server->on("/saveSecret/", HTTP_ANY, [](AsyncWebServerRequest *request) {
		Serial.println("/saveSecret");
		handleGetSavSecreteJson(request);
	  }).setFilter(ON_AP_FILTER);

	  server->on("/list", HTTP_ANY, [](AsyncWebServerRequest *request) {
		Serial.println("/list");
		handleFileList(request);
	  }).setFilter(ON_AP_FILTER);

	  // spiff delete
	  server->on("/edit", HTTP_DELETE, [](AsyncWebServerRequest *request) {
		Serial.println("/edit[DELETE]");
		handleFileDelete(request);
	  }).setFilter(ON_AP_FILTER);

	  // spiff upload
	  server->on("/edit", HTTP_POST, [](AsyncWebServerRequest *request) {},
				 [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
					size_t len, bool final) {
				   Serial.println("/edit[POST]");
				   _handleUpload(request, filename, "/wifi_spiffs_admin.html", index, data, len, final);
				 }).setFilter(ON_AP_FILTER);

	  server->on("/wifiScan.json", HTTP_GET, [](AsyncWebServerRequest *request) {
		Serial.println("/wifiScan.json");
		getWifiScanJson(request);

	  }).setFilter(ON_AP_FILTER);

	  // Simple Firmware Update Form
	  server->on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
		Serial.println("/update[GET]");
		request->send(SPIFFS, "/wifi_upload.html");
	  }).setFilter(ON_AP_FILTER);

	  server->on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
		Serial.println("/update[POST]");
		uint8_t isSuccess = !Update.hasError();
		if (isSuccess)
		  restartSystem = millis();
		AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", isSuccess ? "OK" : "FAIL");
		response->addHeader("Connection", "close");
		request->send(response);


	  }, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
		if (!index) {
		  Serial.printf("Update Start: %s\n", filename.c_str());

#if defined(ESP8266)
		  Update.runAsync(true);
		  if (!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)) {
			Update.printError(Serial);
		  }
#else
		  if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
			Update.printError(Serial);
		  }
#endif
		}
		if (!Update.hasError()) {
		  if (Update.write(data, len) != len) {
			Update.printError(Serial);
		  }
		}
		if (final) {
		  if (Update.end(true)) {
			Serial.printf("Update Success: %uB\n", index + len);
		  } else {
			Update.printError(Serial);
		  }
		}
	  }).setFilter(ON_AP_FILTER);

	  server->on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {
		Serial.println("/restart");
		request->send(200, "text/html", "OK");
		restartSystem = millis();
	  }).setFilter(ON_AP_FILTER);

	  server->onNotFound([](AsyncWebServerRequest *request) {
		Serial.println("/NotFound: redirecting to /");
//		Serial.println("handle not found");
//	request->send(404);
		request->redirect("/");
//		request->send(404, "text/plain", ":(");
	  });

	  server->addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP

	  server->begin();
	  __PORTAL_STARTED = true;
	  Serial.println("Portal Started!");
	}
}

void startMDNS() {
  if (!MDNS.begin(host)) { //http://esp32.local
	Serial.println("Error setting up MDNS responder!");
  } else {
	Serial.printf("\nmDNS responder started: http://%s.local\n", host);
	// Add service to MDNS-SD
	MDNS.addService("http", "tcp", 80);
  }
}



//std::vector<wifiSsidInfo*>* scanWiFi(  ){
////  bool res = false;
//
//  std::vector<wifiSsidInfo *> *res = NULL;//new std::vector<wifiSsidInfo*>();
////  _arr->clear();
////  uint8_t res = false;
//
//  int n = WiFi.scanComplete();
//  if (n == -2) {
//	WiFi.scanNetworks(true);
//  } else if (n) {
//	res = new std::vector<wifiSsidInfo*>(n);
//	for (int i = 0; i < n; ++i) {
////	  Serial.printf("\nSSID: %s( %d db )", WiFi.SSID(i).c_str(), WiFi.RSSI(i));
//	  res->push_back(new wifiSsidInfo(WiFi.SSID(i).c_str(), WiFi.RSSI(i) ) );
////	  if ( _ssid == WiFi.SSID(i) ){
////		res = true;
////		break;
////	  }
//	}
//	WiFi.scanDelete();
//	if (WiFi.scanComplete() == -2) {
//	  WiFi.scanNetworks(true);
//	}
//  }
////  Serial.printf("\nSearching for SSID %s among %d networks.. result: %s",_ssid.c_str(), n, (res?"true":"false"));
//  return res;
//}



// Loads the configuration from a file
bool loadConfiguration(const char *filename, std::vector<Config*> &config) {
  printFile(SECRETS_PATH);
  // Open file for reading
  File file = SPIFFS.open(filename);

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
//  StaticJsonDocument<512> doc;
  size_t capacity = 1024;//JSON_ARRAY_SIZE(config.size()) * 2 + JSON_OBJECT_SIZE(1);
  DynamicJsonDocument doc(capacity);

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error)
	Serial.println(F("Failed to read file, using default configuration"));

  config.clear();
  JsonArray arr = doc.as<JsonArray>();
  // using C++11 syntax (preferred):
  int idx = 0;
  for (JsonVariant v : arr) {
//	Serial.println(value.as<Config*>().);
	String _s = String(v["ssid"].as<String>());
	char* _ssid = new char [_s.length()];
	strcpy ( _ssid, _s.c_str() );
	String _p = String(v["pass"].as<String>());
	char* _pass = new char [_p.length()];
	strcpy ( _pass, _p.c_str() );
	Serial.printf("\n%d: %s : %s\n", idx, _ssid, _pass);
	Config* c = new Config(_ssid, _pass );
	config.push_back(c);
  }

/*
  for (int i = 0; i< arr.size(); i++){
//    String _s = doc["ssid"].as<String>();
//	char* __ssid = new char [_s.length()];
//	strcpy(__ssid, _s.c_str() );
//
//	_s = doc["pass"].as<String>();
//	char* __pass = new char [_s.length()];
//	strcpy(__pass, _s.c_str() );

//	Config* c = new Config(__ssid, __pass);

String _s = String(doc["ssid"].as<String>());
	char* _ssid = new char [_s.length()];
	strcpy ( _ssid, _s.c_str() );
	  //stringToCharPtr(new String( doc["ssid"].as<String>() ) );
	String _p = String(doc["pass"].as<String>());
	char* _pass = new char [_p.length()];
	strcpy ( _pass, _p.c_str() );//stringToCharPtr(new String ( doc["pass"].as<String>()));
	Serial.printf("\n%d: %s : %s\n", i, _ssid, _pass);
	Config* c = new Config(_ssid, _pass );

	config.push_back(c);
  }

*/

//  // Copy values from the JsonDocument to the Config
//  config.ssid = doc["ssid"];
//  char* c = new char [WiFi.SSID(i).length()];
//  strcpy(c, WiFi.SSID(i).c_str() );
//  strlcpy(config.hostname,                  // <- destination
//		  doc["hostname"] | "example.com",  // <- source
//		  sizeof(config.hostname));         // <- destination's capacity

  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();
  if ( config.size() > 0 )
	return true;
  else
	return false;
}

// Saves the configuration to a file
void saveConfiguration(const char *filename, std::vector<Config*> &config) {
  // Delete existing file, otherwise the configuration is appended to the file
  SPIFFS.remove(filename);

  // Open file for writing
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file) {
	Serial.println(F("Failed to create file"));
	return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
//  StaticJsonDocument<256> doc;
  size_t capacity = 1024;//JSON_ARRAY_SIZE(config.size()) * 2 + JSON_OBJECT_SIZE(1);
  DynamicJsonDocument doc(capacity);
//  JsonObject obj = root.as<JsonObject>();

//  JsonArray arr = root.createNestedArray("secrets"); //["secrets"].as<JsonArray>();
//  JsonArray arr = obj.createNestedArray("secrets");
  JsonArray arr = doc.to<JsonArray>();
  for (int i = 0; i < config.size(); i++) {
	String _s = String(config[i]->ssid);
	char* _ssid = new char [_s.length()];
	strcpy ( _ssid, _s.c_str() );
	String _p = String(config[i]->pass);
	char* _pass = new char [_p.length()];
	strcpy ( _pass, _p.c_str() );
	Serial.printf("\n%d: %s : %s\n", i, _ssid, _pass);
//	if ( _s.length() > 0 ) {
//	  c = new Config(_ssid, _pass);
//	  config.push_back(c);
//	}
//	JsonObject obj2 = arr.createNestedObject();
//	obj2["ssid"] = String(_s);
	arr[i]["ssid"] = _ssid;//String(_s);
//	obj2["pass"] = String(_p);
	arr[i]["pass"] = _pass;//String(_p);
	Serial.printf("\n%s:%s\n", arr[i]["ssid"].as<char*>(), arr[i]["pass"].as<char*>() );
//	arr.add(obj2);
//	arr.[i] = obj2;

//	JsonObject obj2 = arr.createNestedObject();
//	Serial.printf("\n%s:%s\n", config[i]->ssid, config[i]->pass );
//	obj2["ssid"] = String(config[i]->ssid);
//	obj2["pass"] = String(config[i]->pass);
//	Serial.printf("\n%s:%s\n", obj2["ssid"], obj2["pass"] );
//	arr.add(obj2);
  }
  Serial.printf("\nsaveConfiguration: size = %d, arr_size = %d\n", config.size(), arr.size());
//  for (int i = 0; i< config.size(); i++) {
//	arr
//  }
//  // Set the values in the document
//  doc["hostname"] = config.hostname;
//  doc["port"] = config.port;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
	Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();
}

// Prints the content of a file to the Serial
void printFile(const char *filename) {
  // Open file for reading
  File file = SPIFFS.open(filename);
  if (!file) {
	Serial.println(F("Failed to read file"));
	return;
  }

  // Extract each characters by one by one
  while (file.available()) {
	Serial.print((char)file.read());
  }
  Serial.println();

  // Close the file
  file.close();
}


/*
   getWifiScanJson()
*/
void getWifiScanJson(AsyncWebServerRequest * request){
  String json = "{\"scan_result\":[";
  int n = WiFi.scanComplete();
  if (n == -2) {
	WiFi.scanNetworks(true);
  } else if (n) {
	for (int i = 0; i < n; ++i) {
	  if (i) json += ",";
	  json += "{";
	  json += "\"RSSI\":" + String(WiFi.RSSI(i));
	  json += ",\"SSID\":\"" + WiFi.SSID(i) + "\"";
	  json += "}";
	}
	WiFi.scanDelete();
	if (WiFi.scanComplete() == -2) {
	  WiFi.scanNetworks(true);
	}
  }
  json += "]}";
  request->send(200, "application/json", json);
  json = String();
}


void printItem(uint8_t i, WiFiScanInfo* item){
  Serial.printf ( "#%d: SSID: %s%s( %ddb )\n", i, item->ssid, (String(*item->ssid) == String(ssid_sta) ? "*" : ""), item->rssi );
}

void printVector(std::vector<WiFiScanInfo*> &vect){
  if ( vect.size() <= 0 ) {
	Serial.printf("no items in vector\n");
  } else {
	Serial.printf("%d items in vector\n", vect.size());
	for (int i=0; i<vect.size(); i++) {
	  printItem(i, vect[i]);
//      Serial.println(vect[i]);
	}
  }
}



bool isSsidInRange111(String _ssid) {
  bool res = false;
  int n = WiFi.scanComplete();
  if (n == -2) {
	WiFi.scanNetworks(true);
  } else if (n) {
	for (int i = 0; i < n; ++i) {
	  Serial.printf("\nSSID: %s( %d db )", WiFi.SSID(i).c_str(), WiFi.RSSI(i));
	  if ( _ssid == WiFi.SSID(i) ){
		res = true;
		break;
	  }
	}
	WiFi.scanDelete();
	if (WiFi.scanComplete() == -2) {
	  WiFi.scanNetworks(true);
	}
  }
  Serial.printf("\nSearching for SSID %s among %d networks.. result: %s",_ssid.c_str(), n, (res?"true":"false"));
  return res;
}

bool isSsidInRange(String _ssid) {
  bool res = false;
  int n = WiFi.scanNetworks();
  Serial.printf("\nSearching for SSID %s among %d networks..",_ssid.c_str(), n);
  if ( n > 0 ) {
	Serial.print(n);
	Serial.println(" networks found");
	for (int i = 0; i < n; ++i) {
	  Serial.printf("\nSSID: %s( %d db )", WiFi.SSID(i).c_str(), WiFi.RSSI(i));
	  if ( _ssid == WiFi.SSID(i) ){
		res = true;
		break;
	  }
	}
  }
  Serial.printf("\n%s\n",( res == true ? "Found!" : "Not Found!"));
//  led.off();
  return res;
}

char* stringToCharPtr(String* str){
  char* res =  new char [str->length()];
  strcpy(res, reinterpret_cast<const char *>(*str->c_str()));
  return res;
}

/*
   handleGetSavSecreteJson()
*/
void handleGetSavSecreteJson(AsyncWebServerRequest *request) {
  config.clear();
  AsyncWebParameter* p;

  int idx = 0;
  Config *c = NULL;

  String _s = String(request->getParam("ssid1", true)->value());
  char* _ssid = new char [_s.length()];
  strcpy ( _ssid, _s.c_str() );
  String _p = String(request->getParam("pass1", true)->value());
  char* _pass = new char [_p.length()];
  strcpy ( _pass, _p.c_str() );
  Serial.printf("\n%d: %s : %s\n", idx++, _ssid, _pass);
  if ( _s.length() > 0 ) {
	c = new Config(_ssid, _pass);
	config.push_back(c);
  }

  _s = String(request->getParam("ssid2", true)->value());
  _ssid = new char [_s.length()];
  strcpy ( _ssid, _s.c_str() );
  _p = String(request->getParam("pass2", true)->value());
  _pass = new char [_p.length()];
  strcpy ( _pass, _p.c_str() );
  Serial.printf("\n%d: %s : %s\n", idx++, _ssid, _pass);
  if ( _s.length() > 0 ) {
	c = new Config(_ssid, _pass);
	config.push_back(c);
  }

  _s = String(request->getParam("ssid3", true)->value());
  _ssid = new char [_s.length()];
  strcpy ( _ssid, _s.c_str() );
  _p = String(request->getParam("pass3", true)->value());
  _pass = new char [_p.length()];
  strcpy ( _pass, _p.c_str() );
  Serial.printf("\n%d: %s : %s\n", idx++, _ssid, _pass);
  if ( _s.length() > 0 ) {
	c = new Config(_ssid, _pass);
	config.push_back(c);
  }

  saveConfiguration(SECRETS_PATH, config);
  printFile(SECRETS_PATH);

  request->send(200, "text/html", "<h1>Restarting .....</h1>");
  restartSystem = millis();
}


/*
   setUpAPService()
*/

void handleFileList(AsyncWebServerRequest *request) {
  Serial.println("handle fle list");
  if (!request -> hasParam("dir")) {
	request->send(500, "text/plain", "BAD ARGS");
	return;
  }


  AsyncWebParameter* p = request->getParam("dir");
  String path = p->value().c_str();
  Serial.println("handleFileList: " + path);
  String output = "[";
#if defined(ESP8266)

  Dir dir = SPIFFS.openDir(path);
  while (dir.next()) {
    File entry = dir.openFile("r");
    if (output != "[") {
      output += ',';
    }
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += String(entry.name()).substring(1);
    output += "\"}";
    entry.close();
  }

#else

  File root = SPIFFS.open("/", "r");
  if (root.isDirectory()) {
	Serial.println("here ??");
	File file = root.openNextFile();
	while (file) {
	  if (output != "[") {
		output += ',';
	  }
	  output += "{\"type\":\"";
	  output += (file.isDirectory()) ? "dir" : "file";
	  output += "\",\"name\":\"";
	  output += String(file.name()).substring(1);
	  output += "\"}";
	  file = root.openNextFile();
	}
  }
#endif

  path = String();
  output += "]";
  Serial.println(output);
  request->send(200, "application/json", output);
}


void handleFileDelete(AsyncWebServerRequest * request) {
  Serial.println("in file delete");
  if (request->params() == 0) {
	return request->send(500, "text/plain", "BAD ARGS");
  }
  AsyncWebParameter* p = request->getParam(0);
  String path = p->value();
  Serial.println("handleFileDelete: " + path);
  if (path == "/") {
	return request->send(500, "text/plain", "BAD PATH");
  }

  if (!SPIFFS.exists(path)) {
	return request->send(404, "text/plain", "FileNotFound");
  }

  SPIFFS.remove(path);
  request->send(200, "text/plain", "");
  path = String();
}


//==============================================================
//   handleUpload
//==============================================================
void _handleUpload(AsyncWebServerRequest *request, String filename, String redirect, size_t index, uint8_t *data, size_t len, bool final) {
  if (!index) {
	if (!filename.startsWith("/")) filename = "/" + filename;
	Serial.println((String)"UploadStart: " + filename);
	fsUploadFile = SPIFFS.open(filename, "w");            // Open the file for writing in SPIFFS (create if it doesn't exist)
  }
  for (size_t i = 0; i < len; i++) {
	fsUploadFile.write(data[i]);
	//Serial.write(data[i]);
  }
  if (final) {
	Serial.println((String)"UploadEnd: " + filename);
	fsUploadFile.close();
	request->redirect(redirect);
  }
}


void printStaClients(){
  wifi_sta_list_t wifi_sta_list;
  tcpip_adapter_sta_list_t adapter_sta_list;

  memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
  memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));

  esp_wifi_ap_get_sta_list(&wifi_sta_list);
  tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);

  for (int i = 0; i < adapter_sta_list.num; i++) {

	tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];

	Serial.print("station nr ");
	Serial.println(i);

	Serial.print("MAC: ");

	for(int i = 0; i< 6; i++){

	  Serial.printf("%02X", station.mac[i]);
	  if(i<5)Serial.print(":");
	}

	Serial.print("\nIP: ");
	Serial.println(ip4addr_ntoa(&(station.ip)));
  }

  Serial.println("-----------");
}



#endif //INC_201122_212358_ESP32DEV_WIFIPORTAL_H
