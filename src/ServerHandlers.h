#ifndef _SERVERHANDLERS_H_
#define _SERVERHANDLERS_H_



// Import required libraries
#include "ESPAsyncWebServer.h"
#include <ArduinoJson.h>

#include "globals.h"
#include "system/SPIFFS_functions.h"
#include "effects/EffectsHelper.h"
#include "globalModes.h"


//unsigned char resp[NUM_LEDS];
DeserializationError err;
static void handle_update_progress_cb(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
void setupServer(AsyncWebServer* server);
void serverTaskCode( void * pvParameters );

AsyncWebSocketClient * globalWsClient;

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
 
  if(type == WS_EVT_CONNECT){
    Serial.println("Websocket client connection received");
//    client->text("Hello from ESP32 Server");
    globalWsClient = client;
    globalWsClient->text("Hello from ESP32 Server");
 
  } else if(type == WS_EVT_DISCONNECT){
    Serial.println("Client disconnected");
	globalWsClient= NULL;
  }
}



//const uint8_t __files_max = 50;
//String __files[__files_max];
//uint8_t __files_idx = 0;
//uint8_t __files_cnt = 0;
//
//void clearFilesList(){
//    for ( int i = 0; i  < __files_max; i++){
//        __files[i] = "";
//    }
//    __files_idx = 0;
//    __files_cnt = 0;
//}
//
//void collectFiles(File file){
//    if ( __files_max > __files_idx ) {
//        __files[__files_idx++] = file.name();
//        __files_cnt++;
//    }
//}
//
//void findImages(String fileType) {
//    clearFilesList();
//    if ( fileType ==".gif" ){
//        findFilesInDir(SPIFFS, __GIFS_FOLDER.c_str(), fileType.c_str(), 3, collectFiles);
//    } else
//    if ( fileType == ".jpg" ){
//        findFilesInDir(SPIFFS, __JPGS_FOLDER.c_str(), fileType.c_str(), 3, collectFiles);
//    }
//}

//#define _RESP_BUFF_SIZE 1024
//char responseBuf[_RESP_BUFF_SIZE];

//bool __SERVER_CONFIGURED = false;

//TaskHandle_t serverTaskHandle;
//QueueHandle_t  serverTaskFlagsQueue=xQueueCreate(20,sizeof(unsigned long));

#define SERVER_FLAG_NONE 0
#define SERVER_FLAG_RESTART 1
ulong _SERVER_FLAG = SERVER_FLAG_NONE;

//String pathOnNotFound;
//File spiFileOnNotFound;
//String dataType;// = String("text/plain");
//int dataLeftSpiFileOnNotFound;


struct notFoundRequestItem{
  notFoundRequestItem(AsyncWebServerRequest *request) : request(request) {}

  String* pathOnNotFound;
  File spiFileOnNotFound;
  String* dataType;// = String("text/plain");
  int dataLeftSpiFileOnNotFound;
  AsyncWebServerRequest *request;
};
//xNotFoundRequestItem;

std::vector<notFoundRequestItem*> notFoundRequests;


#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
void feedTheDog(){
  // feed dog 0
  TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE; // write enable
  TIMERG0.wdt_feed=1;                       // feed dog
  TIMERG0.wdt_wprotect=0;                   // write protect
  // feed dog 1
  TIMERG1.wdt_wprotect=TIMG_WDT_WKEY_VALUE; // write enable
  TIMERG1.wdt_feed=1;                       // feed dog
  TIMERG1.wdt_wprotect=0;                   // write protect
}

QueueHandle_t  queueHandle=NULL;



void setupServer(AsyncWebServer* server){
  if ( __SERVER_STARTED ) {
	Serial.println("Server already started! Restarting!");
	server->reset();
//    server->end();
	__SERVER_STARTED = false;
  }

  if ( !__SERVER_STARTED ) {
	// I guess this string can work the same way as setupDefaultEndpoints(&server)
	server->serveStatic("/", SPIFFS, "/").setCacheControl("max-age=31536000");
	server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html").setFilter(ON_STA_FILTER);
//	server->serveStatic("/_assets/", SPIFFS, "/_assets/").setCacheControl("max-age=31536000");
//	server->serveStatic("/js/", SPIFFS, "/js/").setCacheControl("max-age=31536000");
//	server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.htm");
	server->on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
	  request->send(SPIFFS, "/index.html", "text/html");
	});


	server->on("/brightness", HTTP_POST, [](AsyncWebServerRequest *request) {
	}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
	  Serial.printf("/brightness\n");
//        JsonObject jsonObj = json.as<JsonObject>();
	  size_t capacity = JSON_OBJECT_SIZE(1) * 2;
	  DynamicJsonDocument root(capacity);
	  err = deserializeJson(root, (const char *) data);
	  if (err == DeserializationError::Ok) {
		// Get a reference to the root object
		JsonObject obj = root.as<JsonObject>();

//        BRIGHTNESS = 0;
		if (obj.containsKey("brightness")) {
		  __MX_BRIGHTNESS = obj["brightness"].as<unsigned char>();
		}
		matrix->setBrightness(__MX_BRIGHTNESS);
	  }
	}).setFilter(ON_STA_FILTER);


	server->on("/speed", HTTP_POST, [](AsyncWebServerRequest *request) {
	  //nothing and dont remove it
	}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
	  Serial.printf("/speed\n");
	  DynamicJsonDocument root(128);
	  err = deserializeJson(root, (const char *) data);
	  if (err == DeserializationError::Ok) {
		// Get a reference to the root object
		JsonObject obj = root.as<JsonObject>();
		__SPEED__ = obj["speed"];
		Serial.printf("Speed: %u\n\n", __SPEED__);
		request->send(200, "text/plain", "ack");
	  } else
		request->send(404, "text/plain", ":(");
	}).setFilter(ON_STA_FILTER);

	server->on("/clock", HTTP_POST, [](AsyncWebServerRequest *request) {
	}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
	  Serial.printf("/clock:\n");

	  DynamicJsonDocument root(512);
	  err = deserializeJson(root, (const char *) data);
	  if (err == DeserializationError::Ok) {
		// Get a reference to the root object
		JsonObject obj = root.as<JsonObject>();
		uint16_t phTime = 0;
		if (obj.containsKey("phoneTime")) {
		  phTime = obj["phoneTime"];
		}
		uint8_t src = 0;
		if (obj.containsKey("src")) {
		  src = obj["src"].as<unsigned char>();
		  if (src == 1) {
			int epoch_time = phTime;
			timeval epoch = {epoch_time, 0};
			const timeval *tv = &epoch;
			timezone utc = {0, 0};
			const timezone *tz = &utc;
			settimeofday(tv, tz);
		  }
		}
		unsigned char r = obj["color"]["r"];
		unsigned char g = obj["color"]["g"];
		unsigned char b = obj["color"]["b"];
		__RUNNING_STRING_COLOR = CRGB(r, g, b);
		unsigned char br = obj["backgroundColor"]["r"];
		unsigned char bg = obj["backgroundColor"]["g"];
		unsigned char bb = obj["backgroundColor"]["b"];
		__RUNNING_STRING_BACKGROUND_COLOR = CRGB(br, bg, bb);

		Serial.printf("str = %s, spd = %u, r: %u g: %u b: %u, br: %u bg: %u bb: %u\n\n", _RUN_STRING_TEXT_.c_str(),
					  (int) __SPEED__, r, g, b, br, bg, bb);
		__NEXT_MODE = __MODE_CLOCK;
		request->send(200, "text/plain", "ack");
	  } else
		request->send(404, "text/plain", ":(");
	}).setFilter(ON_STA_FILTER);

	server->on("/show", HTTP_GET, [](AsyncWebServerRequest *request) {
	  Serial.printf("/show\n");
	  //Check if parameter exists (Compatibility)
	  if (request->hasParam("img")) {
		__imgName = request->getParam("img")->value();
		String path = "/";
		if (__imgName.endsWith(".gif")) {
		  __NEXT_MODE = __MODE_GIF;
		} else if (__imgName.endsWith(".jpg")) {
		  __NEXT_MODE = __MODE_JPG;
		}
	  }
	  __MODE_STOPED_FLAG = false;
	  request->send(200, "text/plain", "ack");
	}).setFilter(ON_STA_FILTER);

	server->on("/play-gifs", HTTP_GET, [](AsyncWebServerRequest *request) {
	  Serial.printf("/play-gifs\n");
	  //Check if parameter exists (Compatibility)
	  if (request->hasArg("speed"))
		__SPEED__ = request->arg("speed").toInt();
	  __NEXT_MODE = __MODE_GIF_PLAY;
	  request->send(200, "text/plain", "ack");
	}).setFilter(ON_STA_FILTER);

	server->on("/play-jpgs", HTTP_GET, [](AsyncWebServerRequest *request) {
	  Serial.printf("/play-jpgs\n");
	  //Check if parameter exists (Compatibility)
	  if (request->hasArg("speed"))
		__SPEED__ = request->arg("speed").toInt();
	  __NEXT_MODE = __MODE_JPG_PLAY;
	  request->send(200, "text/plain", "ack");
	}).setFilter(ON_STA_FILTER);


	server->on("/picture-effects", HTTP_POST, [](AsyncWebServerRequest *request) {
	  //nothing and dont remove it
	}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
	  Serial.printf("/picture-effects(POST)\n");
	  DynamicJsonDocument root(128);
	  err = deserializeJson(root, (const char *) data);
	  if (err == DeserializationError::Ok) {
		// Get a reference to the root object
		JsonObject obj = root.as<JsonObject>();
		_EFFECT_ID = obj["effect"];
		if (obj.containsKey("speed"))
			__SPEED__ = obj["speed"];

		Serial.printf("Effect ID: %u, speed: %u\n\n", _EFFECT_ID, __SPEED__);
		__NEXT_MODE = __MODE_EFFECTS;
		effectsDrawer.needStart = true;
//        item = 

		request->send(200, "text/plain", "ack");
	  } else
		request->send(404, "text/plain", ":(");
	}).setFilter(ON_STA_FILTER);

	server->on("/rotate", HTTP_POST, [](AsyncWebServerRequest *request) {
	  _MX_ROTATION = (++_MX_ROTATION > 3 ? 0 : _MX_ROTATION);
	  Serial.printf("/rotate: %d\n", _MX_ROTATION);
	  matrix->setRotation(_MX_ROTATION);
	  request->send(200, "text/plain", "ack");
	}).setFilter(ON_STA_FILTER);

	// handler for the /update form POST (once file upload finishes)
	server->on("/send-file", HTTP_POST, [](AsyncWebServerRequest *request) {
	  request->send(200);
	}, handle_update_progress_cb).setFilter(ON_STA_FILTER);

	server->onNotFound([](AsyncWebServerRequest *request) {
	  request->send(404);
	});

// Управление играми: нажата кнопка: 0 - верх, 1 - право, 2 - низ, 3 - лево, 4 - не нажата
	server->on("/up", HTTP_POST, [](AsyncWebServerRequest *request) {
	  Serial.printf("/up\n");
	  _GAME_BUTTONS_ = 0;
//	  if ( __CURRENT_MODE == __MODE_GAME )
//		((Game*)__CURRENT_MODE_FUNC)->gameDemo = false;
	  request->send(200, "text/plain", "ack");
	}).setFilter(ON_STA_FILTER);

	server->on("/right", HTTP_POST, [](AsyncWebServerRequest *request) {
	  Serial.printf("/right\n");
	  _GAME_BUTTONS_ = 1;
//	  if ( __CURRENT_MODE == __MODE_GAME )
//	  	((Game*)__CURRENT_MODE_FUNC)->gameDemo = false;
	  request->send(200, "text/plain", "ack");
	}).setFilter(ON_STA_FILTER);

	server->on("/down", HTTP_POST, [](AsyncWebServerRequest *request) {
	  Serial.printf("/down\n");
	  _GAME_BUTTONS_ = 2;
//	  if ( __CURRENT_MODE == __MODE_GAME )
//		((Game*)__CURRENT_MODE_FUNC)->gameDemo = false;
	  request->send(200, "text/plain", "ack");
	}).setFilter(ON_STA_FILTER);

	server->on("/left", HTTP_POST, [](AsyncWebServerRequest *request) {
	  Serial.printf("/left\n");
	  _GAME_BUTTONS_ = 3;
//	  if ( __CURRENT_MODE == __MODE_GAME )
//		((Game*)__CURRENT_MODE_FUNC)->gameDemo = false;
	  request->send(200, "text/plain", "ack");
	}).setFilter(ON_STA_FILTER);

	server->on("/games", HTTP_POST, [](AsyncWebServerRequest *request) {
	  //nothing and dont remove it
	}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
	  Serial.printf("/games\n");
	  DynamicJsonDocument root(128);
	  err = deserializeJson(root, (const char *) data);
	  if (err == DeserializationError::Ok) {
		// Get a reference to the root object
		JsonObject obj = root.as<JsonObject>();
		_GAME_NAME_ = obj["game"].as<String>();
		Serial.printf("%s\n", _GAME_NAME_.c_str());
		if ( __CURRENT_MODE == __MODE_GAME ){
		  ((Game*)__CURRENT_MODE_FUNC)->gameDemo = false;
//		  ((Game*)__CURRENT_MODE_FUNC)->interval =__SPEED__;
		  ((Game*)__CURRENT_MODE_FUNC)->start();
		}
		__NEXT_MODE = __MODE_GAME;
//      __CURRENT_MODE_FUNC = &__FNC_MODE_GAME;
//      enableGameMode();
//      __MODE_STOPED_FLAG = false;
		request->send(200, "text/plain", "ack");
	  } else
		request->send(404, "text/plain", ":(");
	}).setFilter(ON_STA_FILTER);

	server->on("/fill-matrix", HTTP_POST, [](AsyncWebServerRequest *request) {
	  //nothing and dont remove it
	}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
	  Serial.printf("/fill-matrix:\n");
	  DynamicJsonDocument root(128);
	  err = deserializeJson(root, (const char *) data);
	  if (err == DeserializationError::Ok) {
		// Get a reference to the root object
		JsonObject obj = root.as<JsonObject>();
		unsigned char r = obj["r"];
		unsigned char g = obj["g"];
		unsigned char b = obj["b"];
		Serial.printf("r: %u g: %u b: %u\n\n", r, g, b);

		__NEXT_MODE = __MODE_DRAW;
		matrix->clear();
		matrix->fillScreen(matrix->Color(r, g, b));
		matrix->show();
		if ( NULL != globalWsClient ) {
		  globalWsClient->binary((const char*)matrixleds, BITMAP_SIZE);
		}
		request->send(200, "text/plain", "ack");
	  } else
		request->send(404, "text/plain", ":(");
	}).setFilter(ON_STA_FILTER);

	server->on("/matrix-state", HTTP_GET, [](AsyncWebServerRequest *request) 	{
//    AsyncWebServerResponse *response = request->beginChunkedResponse("application/octet-stream", [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
	  AsyncWebServerResponse *response = request->beginResponse("application/octet-stream", BITMAP_SIZE,
																[](uint8_t *buffer, size_t maxLen,
																   size_t index) -> size_t {
#if DEBUG == 1
																  Serial.printf("\n/matrix-state[%u]:%3d, %3d\n", NUM_LEDS, index, maxLen);
#endif
//      if( index){ //already sent
//          return 0;
//      }
//      int res = (maxLen > BITMAP_SIZE) ? BITMAP_SIZE : maxLen;
																  memmove(buffer, matrixleds, BITMAP_SIZE);
																  return BITMAP_SIZE;
																});
	  response->addHeader("Cache-Control", "no-cache");
	  response->addHeader("Access-Control-Allow-Origin", "*");
	  request->send(response);
	}).setFilter(ON_STA_FILTER);

	server->on("/animations", HTTP_GET, [](AsyncWebServerRequest *request) {
//	  findImages(".gif");
//	  size_t capacity = JSON_ARRAY_SIZE(jpgDrawer.getConfigSize()) * 2 + JSON_OBJECT_SIZE(1);
	  Serial.printf("\n/animations\n");
	  AsyncResponseStream *response = request->beginResponseStream("application/json");
//	  DynamicJsonDocument jsonBuffer(capacity);

//	  JsonArray arr = jsonBuffer.createNestedArray("gifsList");
//	  for (int i = 0; i < __files_cnt; i++) {
//		Serial.println(__files[i]);
//		arr.add(__files[i]);
//	  }
//
	  response->setCode(200);
//	  const char* res = gifDrawer.getConfig();
//	  const char* res =
//!!!TODO:
	    // gifDrawer.getConfig(response);
		response->write("{");
		response->write(gifDrawer.getConfigJson()->str().c_str());
		response->write("}");
//	  serializeJson(, *response);
	  request->send(response);
	}).setFilter(ON_STA_FILTER);

	server->on("/images", HTTP_GET, [](AsyncWebServerRequest *request) {
//!!!!	  findImages(".jpg");
//!!!!	  size_t capacity = JSON_ARRAY_SIZE(__files_cnt) * 2 + JSON_OBJECT_SIZE(1);
	  Serial.printf("\n/images\n");
	  AsyncResponseStream *response = request->beginResponseStream("application/json");
//	  DynamicJsonDocument jsonBuffer(capacity);
//
//
//	  JsonArray arr = jsonBuffer.createNestedArray("imageList");
//	  for (int i = 0; i < __files_cnt; i++) {
//		Serial.println(__files[i]);
//		arr.add(__files[i]);
//	  }
//!!!TODO:
	//   jpgDrawer.getConfig(response);
	// *response << jpgDrawer.getConfigJson()->rdbuf();
	response->write("{");
	response->write(jpgDrawer.getConfigJson()->str().c_str());
	response->write("}");
//	  serializeJson(jpgDrawer.getConfig(), *response);

	  response->setCode(200);
	  request->send(response);

//	  request->send(200, "application/json", response);
	}).setFilter(ON_STA_FILTER);

	server->on("/matrix-parameters", HTTP_GET, [](AsyncWebServerRequest *request) {
	  dumpHeap("0");
	//   const size_t capacity = JSON_ARRAY_SIZE(gamesRunner.getConfigSize() + effectsDrawer.getConfigSize()) * 2 + JSON_OBJECT_SIZE(1) * 5;
	//   DynamicJsonDocument data(capacity);
//	  AsyncResponseStream *response = request->beginResponseStream("application/json");
	//   Serial.printf("\n/matrix-parameters[%d]\n", capacity);
	  

	  std::stringstream* res = new std::stringstream();
	//   Stream res;
	//   res.

	  *res << "{\"width\":" << MX_WIDTH \
	  	<< ",\"height\":" << MX_HEIGHT \
		<< ",\"speed\":" << __SPEED__ \
		<< ",\"speedMax\":" << __SPEED_MAX__ \
		<< ",\"brightness\":" << 8 \
		<< "," << effectsDrawer.getConfigJson()->rdbuf() \
		<< "," << gamesRunner.getConfigJson()->rdbuf() \
	    << "}";

	//   data["width"] = MX_WIDTH;
	//   data["height"] = MX_HEIGHT;

	//   data["speed"] = __SPEED__;
	//   data["speedMax"] = __SPEED_MAX__;
	//   data["brightness"] = FastLED.getBrightness();
	//   serializeJson(data, Serial);

//	  JsonArray arr = data.createNestedArray("effects");
//	  arr.set(((DynamicJsonDocument)effectsDrawer.getConfig()).as<JsonArray>());
//data.add(((DynamicJsonDocument)effectsDrawer.getConfig()).as<JsonArray>());
//	  JsonObject tmp3
//	  char* tmp3 = effectsDrawer.getConfig(NULL);//.as<JsonObject>();
//	  dumpHeap("2");
//	  JsonObject::iterator it3 = tmp3.begin();
//	  char* name3 = new char ( strlen( it3->key().c_str() ));
//	  strcpy( name3, it3->key().c_str() );
//	  data[name3] = tmp3[name3];//serialized(buf);
//	  data["effects"] = String ( tmp3 );//serialized(buf);
//	  deserializeJson(data["effects"], tmp3);
//	  JsonArray _ar = data.createNestedArray("effects");
//	  DeserializationError err = deserializeJson(_ar, tmp3);


//	  _ar =
//	  dumpHeap("3");
//	  free ( tmp3 );
//	  JsonArray garr = data.createNestedArray("games");
//	  garr.set(((DynamicJsonDocument)gamesRunner.getConfig()).as<JsonArray>());
//	  data.add(((DynamicJsonDocument)gamesRunner.getConfig()).as<JsonArray>());
//	  JsonObject tmp2

//
//	  char* tmp2 = gamesRunner.getConfig(NULL);//.as<JsonObject>();
//	  dumpHeap("4");
////	  JsonObject::iterator it2 = tmp2.begin();
////	  char* name2 = new char ( strlen( it2->key().c_str() ));
////	  strcpy( name2, it2->key().c_str() );
////	  data[name2] = tmp2[name2];//serialized(buf);
//	  data["games"] = String ( tmp2 );//serialized(buf);
//	  dumpHeap("5");
////	  free( tmp2 );
//	  dumpHeap("6");

//	  AsyncResponseStream *response = request->beginResponseStream("application/json");
//	  serializeJson(data, *response);
//	  response->setCode(200);
//	  request->send(response);
//	  data.clear();

	//   String response;
	//   serializeJson(data, response);
	// request->send(res, "application/json", res->str().length() );
	dumpHeap("1");
	std::string sss = res->str();
	dumpHeap("2");
	// sss << "zxc";
	  request->send(200, "application/json", sss.c_str());
	}).setFilter(ON_STA_FILTER);

	server->on("/running-text", HTTP_POST, [](AsyncWebServerRequest *request) {
	}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
	  Serial.printf("/running-text:\n");

	  DynamicJsonDocument root(512);
	  err = deserializeJson(root, (const char *) data);
	  if (err == DeserializationError::Ok) {
		// Get a reference to the root object
		JsonObject obj = root.as<JsonObject>();
		if (obj.containsKey("string")) {
		  _RUN_STRING_TEXT_ = obj["string"].as<String>();
		}
		if (obj.containsKey("speed")) {
		  __SPEED__ = obj["speed"].as<unsigned char>();
		}
		unsigned char r = obj["color"]["r"];
		unsigned char g = obj["color"]["g"];
		unsigned char b = obj["color"]["b"];
		__RUNNING_STRING_COLOR = CRGB(r, g, b);
		unsigned char br = obj["backgroundColor"]["r"];
		unsigned char bg = obj["backgroundColor"]["g"];
		unsigned char bb = obj["backgroundColor"]["b"];
		__RUNNING_STRING_BACKGROUND_COLOR = CRGB(br, bg, bb);

		Serial.printf("str = %s, spd = %u, r: %u g: %u b: %u, br: %u bg: %u bb: %u\n\n", _RUN_STRING_TEXT_.c_str(),
					  (int) __SPEED__, r, g, b, br, bg, bb);
		__NEXT_MODE = __MODE_TEXT;
		request->send(200, "text/plain", "ack");
	  } else
		request->send(404, "text/plain", ":(");
	}).setFilter(ON_STA_FILTER);

	server->on("/draw", HTTP_POST, [](AsyncWebServerRequest *request) {
	  //nothing and dont remove it
	}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
	  Serial.printf("/draw: len=%u, idx=%u, ttl=%u\n", len, index, total);

	  uint16_t _didx = 0;
	  for (uint16_t _idx = 0; _idx < NUM_LEDS; _idx++) {
		drawTaskBitmapBuffer[_idx] = matrix->Color(data[_didx++], data[_didx++], data[_didx++]);
	  }
	  drawTaskBitmapInfo = (bitmapInfo) {(const short unsigned int *) &drawTaskBitmapBuffer, 16, 16, 0};


	  __NEXT_MODE = __MODE_CANVAS;
	  request->send(200, "text/plain", "ack");
	}).setFilter(ON_STA_FILTER);

	server->on("/pixel", HTTP_POST, [](AsyncWebServerRequest *request) {
	  //nothing and dont remove it
	}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
	  Serial.printf("/pixel:\n");
	  DynamicJsonDocument root(128);
	  err = deserializeJson(root, (const char *) data);
	  if (err == DeserializationError::Ok) {
		// Get a reference to the root object
		JsonObject obj = root.as<JsonObject>();
		unsigned char x = obj["x"];
		unsigned char y = obj["y"];
		unsigned char r = obj["r"];
		unsigned char g = obj["g"];
		unsigned char b = obj["b"];
		Serial.printf("(%u:%u) r: %u g: %u b: %u\n\n", x, y, r, g, b);
//		if (__CURRENT_MODE == __MODE_CANVAS) {
//		  if (!MODE_DRAW_FLAG)
//			MODE_DRAW_FLAG = true;
		  __NEXT_MODE = __MODE_DRAW;
		  matrix->drawPixel(x, y, CRGB(r, g, b));
		  matrix->show();
		  if ( NULL != globalWsClient ) {
			globalWsClient->binary((const char*)matrixleds, BITMAP_SIZE);
		  }
//		}
		request->send(200, "text/plain", "ack");
	  } else
		request->send(404, "text/plain", ":(");
	}).setFilter(ON_STA_FILTER);


	ws.onEvent(&onWsEvent);
//server->on();
	server->addHandler(&ws).setFilter(ON_STA_FILTER);
	// Start server
	server->begin();
	__SERVER_STARTED = true;
	Serial.println("Server Started!");
  }

}



bool __UPLOADING_MEDIA_FLAG = false;
bool __UPLOADING_MEDIA_SKIP = false;
uint8_t __MEDIA_MODE = __MODE_NULL;
static void handle_update_progress_cb(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  Serial.printf("Update: %s[%d]: dta: %2X, len=%d, final=%d...\n", filename.c_str(), index, *data, len, final );  
  
  if ( !__UPLOADING_MEDIA_SKIP && !index){
    String path = "/";
    if ( filename.endsWith(".gif") ){
      __UPLOADING_MEDIA_FLAG = true;
      __MEDIA_MODE = __MODE_GIF;
      path = __GIFS_FOLDER + "/";
    } else
    if ( filename.endsWith(".jpg") ){
      __UPLOADING_MEDIA_FLAG = true;
      __MEDIA_MODE = __MODE_JPG;
      path = __JPGS_FOLDER + "/";
    } else
      __UPLOADING_MEDIA_FLAG = false;
    if ( !SPIFFS.exists(String(path + filename).c_str()) ) {
        Serial.printf("!index path = %s\n", path.c_str());
        request->_tempFile = SPIFFS.open(path + filename, FILE_WRITE);
        if (!request->_tempFile) {
            Serial.println("There was an error opening the file for writing");
            return;
        }
    } else {
        __UPLOADING_MEDIA_SKIP = true;
    }
  }

  

  if ( !__UPLOADING_MEDIA_SKIP && request->_tempFile && len ){
    Serial.printf("request->_tempFile && len\n"); 
    request->_tempFile.write(data,len);
    Serial.println("..done");
  }

  if(final){
    if ( !__UPLOADING_MEDIA_SKIP ) {
        request->_tempFile.close();
        Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index+len);
    } else
        Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index+len);
//    listFiles();
    if ( __UPLOADING_MEDIA_FLAG && __UPLOADING_MEDIA_SKIP ) {
      __UPLOADING_MEDIA_FLAG = false;
        __UPLOADING_MEDIA_SKIP = false;
        __imgName = filename;
      __NEXT_MODE = __MEDIA_MODE;
    }
  }
}

#endif //_SERVERHELPER_H_
