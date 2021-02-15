// FastLED_NeoMatrix example for single NeoPixel Shield.
// By Marc MERLIN <marc_soft@merlins.org>
// Contains code (c) Adafruit, license BSD

// LittleFS has higher priority than SPIFFS
#define USE_LITTLEFS    false
#define USE_SPIFFS      true

#define ESP32
#define CONFIG_ASYNC_TCP_RUNNING_CORE 1
#define CONFIG_ASYNC_TCP_USE_WDT 0
#define ESP_DRD_USE_SPIFFS
//#define ARDUHAL_LOG_LEVEL ARDUHAL_LOG_LEVEL_VERBOSE

#include <Arduino.h>

#include "soc/rtc_wdt.h"
#include <esp_task_wdt.h>
//#include "heap/include/esp_heap_caps.h"
#include "esp_heap_caps.h"
#include "esp_wifi.h"



//#include "../.pio/libdeps/esp32dev/FastLED/src/platforms.h"
//#include <TaskScheduler.h>
//#include <ESP32Ping.h>



#include "defs.h"
#include "helpers.h"

#include "ServerHandlers.h"
#include <system/GfxHelper.h>
#include "system/WiFiPortal.h"
#include "system/OTAHelper.h"
#include "system/SPIFFS_functions.h"
#include "globalModes.h"
#include "../.pio/libdeps/esp32dev/ArduinoJson/src/ArduinoJson/Misc/SerializedValue.hpp"





unsigned long check_wifi = 30000;

//bool wifi_connected = false;


/*
int load_png_fs(const char *url, double scale = 1.0){
  pngle_t *pngle = pngle_new();
  pngle_set_draw_callback(pngle, pngle_on_draw);
  g_scale = scale; // XXX:

//  size_t* z = NULL;
  char buf[1024];// = new char(1024);
  size_t remain = 0;
  int len;

  File file = SPIFFS.open(url, "r");
  if (!file) {
	Serial.println("Error opening file for reading");
  } else {
	int s = file.size();
//	char *buf = new char(s);
	int bytesReaded = file.read(reinterpret_cast<uint8_t *>(buf), s);//print("TEST SPIFFS");

	len = fread(buf + remain, 1, sizeof(buf) - remain, reinterpret_cast<FILE *>(&file));
	if (len <= 0) {
	  printf("EOF\n");
//	  break;
	}

	int fed = pngle_feed(pngle, buf, remain + len);
	if (fed < 0) {
//	  if (argc > 1) fprintf(stderr, "%s: ", argv[1]);
	  fprintf(stderr, "ERROR; %s\n", pngle_error(pngle));
	  return -1;
	}

	remain = remain + len - fed;
	if (remain > 0) memmove(buf, buf + fed, remain);

	if (bytesReaded > s) {
	  Serial.println("File was Readed");
	  Serial.println(bytesReaded);
	  return s;
	} else {
	  Serial.println("File read failed");
	}
	file.close();
  }



//  int fed = pngle_feed(pngle, buf, *z);
//  if (fed < 0) {
//	cls();
//	Serial.printf("ERROR: %s\n", pngle_error(pngle));
////	break;
//  } else
//	Serial.printf("Should be ok): %s\n", fed);

  pngle_destroy(pngle);
}

void load_png(const char *url, double scale = 1.0)
{
  HTTPClient http;

  http.begin(url);

  int httpCode = http.GET();
  if (httpCode != HTTP_CODE_OK) {
	Serial.printf("HTTP ERROR: %d\n", httpCode);
	http.end();
	return ;
  }

  WiFiClient *stream = http.getStreamPtr();

  pngle_t *pngle = pngle_new();
  pngle_set_draw_callback(pngle, pngle_on_draw);
  g_scale = scale; // XXX:

  uint8_t buf[2048];
  int remain = 0;
  while (http.connected()) {
	size_t size = stream->available();
	Serial.printf("sz: %d, remain: %d\n", size, remain);
	if (!size) { delay(1); continue; }

	if (size > sizeof(buf) - remain) {
	  size = sizeof(buf) - remain;
	}

	int len = stream->readBytes(buf + remain, size);
	if (len > 0) {
	  int fed = pngle_feed(pngle, buf, remain + len);
	  if (fed < 0) {
		cls();
		Serial.printf("ERROR: %s\n", pngle_error(pngle));
		break;
	  }
	  remain = remain + len - fed;
	  if (remain > 0) memmove(buf, buf + fed, remain);
	} else {
	  delay(1);
	}
  }

  pngle_destroy(pngle);

  http.end();
}
*/




// ===================================================

//void dumpHeap( const char* msg ){
//  uint freeRAM = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
////		ESP_LOGI(TAG, "free RAM is %d.", freeRAM);
//  Serial.printf("Free RAM is %d ( %s )\n", freeRAM, ( NULL != msg ? msg : "" ));
//}


void setup() {
  // Time for serial port to work?
  delay(1000);
  pinMode(TRIGGER_PIN, INPUT);
//  randomSeed(analogRead(0));
  led.begin();
  led.on();
  Serial.begin(115200);
  dumpSystemInfo();

  initGfx();
  drawPixelMask();
  delay(3000);

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    led.blink3();
//    while (1) {
//      yield(); // Stay here twiddling thumbs waiting
//      led.loop();
//    }
  } else {
#if DEBUG == 1
	listFiles(0);
#endif
	updateFW();
	printFile(SECRETS_PATH);
	loadConfiguration(SECRETS_PATH, config);
	gifDrawer.setup();//start();
	pngDrawer.setup();//start();
  }

//  gifDrawer.start();
  pngDrawer.start();
  effectsDrawer.setup();
  for(;;){
//	pngDrawer.load_file(SPIFFS, "/imgs/0.png");
//	matrix->show();
//	displayStartTime_millis = now;
//	delay(__SPEED__);
//	gifDrawer.drawFrame();
//	pngDrawer.drawFrame();
	effectsDrawer.drawFrame();
	delay(33);
  }

/*
  String fname = "/trnsfrmr.png";
  for(;;){
	load_file(SPIFFS, "/1.png");
	delay(5000);
	load_file(SPIFFS, "/2.png");
	delay(5000);
	load_file(SPIFFS, "/3.png");
	delay(5000);
	load_file(SPIFFS, "/4.png");
	delay(5000);
	load_file(SPIFFS, "/5.png");
	delay(5000);
  }
//  load_png_fs(fname.c_str(), 1);
	File fp = SPIFFS.open(fname, "r"); //fopen(argv[0], "rb");
	if (fp == NULL) {
	  Serial.printf("%s: %s\n", fname.c_str(), strerror(errno));

	} else {
//	  char buf[1024];
	  size_t remain = 0;
	  int len;

	  pngle_t *pngle = pngle_new();

//	  pngle_set_init_callback(pngle, init_screen);
//	  pngle_set_draw_callback(pngle, draw_pixel);
//	  pngle_set_done_callback(pngle, flush_screen);
//
//	  pngle_set_display_gamma(pngle, display_gamma);
//	  pngle_t *pngle = pngle_new();
	  pngle_set_draw_callback(pngle, pngle_on_draw);
	  g_scale = 1; // XXX:

//	  file = fopen(fname, "r");
//	  if(file != NULL) {
//		fseek(file, 0, SEEK_END);
	  int file_size = fp.size();//ftell(file);
//		fseek(file, 0, SEEK_SET);
//		sprintf(cont_len, "%d", file_size);
	  Serial.printf("File size: %d Bytes\n", file_size);

//		printf("content-length %s\n", cont_len);

	  uint8_t *file_buf = (uint8_t *) malloc(file_size);
	  if (file_buf == NULL) {
		Serial.printf("Failed to allocate memory\n");

	  }
	  int bytes_read = fp.read(file_buf, file_size);//, 1, file);
	  int fed = pngle_feed(pngle, file_buf, remain + len);
	  if (fed < 0) {
//		if (argc > 1) fprintf(stderr, "%s: ", argv[1]);
		Serial.printf("ERROR; %s\n", pngle_error(pngle));
//		return -1;
	  }
//		std::string l_output(file_buf,file_size);
//		l_output = mspUtils::trim(l_output);
	  Serial.printf("output:\n");
	  Serial.printf("%s\n", file_buf);
	  Serial.printf("#####\n");

	  fp.close();
	}
	  //  matrix->show();
	  for(;;);
	  */
//	  } else {
//		printf("Failed to open file for sending\n");
//	  }

//	  while (!feof(fp)) {
//	  do{
//		if (remain >= sizeof(buf)) {
////		  if (argc > 1) fprintf(stderr, "%s: ", argv[1]);
//		  Serial.printf("Buffer exceeded\n");
////		  return -1;
//		}
//
//		len = fread(buf + remain, 1, sizeof(buf) - remain, fp);
//		if (len <= 0) {
//		  //printf("EOF\n");
//		  break;
//		}
//
//		int fed = pngle_feed(pngle, buf, remain + len);
//		if (fed < 0) {
//		  if (argc > 1) fprintf(stderr, "%s: ", argv[1]);
//		  fprintf(stderr, "ERROR; %s\n", pngle_error(pngle));
//		  return -1;
//		}
//
//		remain = remain + len - fed;
//		if (remain > 0) memmove(buf, buf + fed, remain);
//	  }
//
//	  if (fp != stdin) fclose(fp);
//	}



//  WiFi.begin("Keenetic-1933", "sMTVCwBP");
//  while (WiFi.status() != WL_CONNECTED) {
//	delay(100);
//  }
//  Serial.printf("WiFi connected.\n");
//  load_png("http://dev4.evolv.redwerk.com/png/s09n3p02.png");
//  load_png("https://raw.githubusercontent.com/kikuchan/pngle/master/tests/pngsuite/PngSuite.png");
//  load_png_fs("/trnsfrmr.png");
//  matrix->show();
//  for(;;);



//  setupMedia();
//  setupGames();
  gamesRunner.setup();
//  setupEffects(matrixleds);
  effectsDrawer.setup();
//  jpgDrawer.filename = "/imgs/14_.jpg";
//  setupModes();
  Serial.println("\r\nInitialisation done.");
  led.off();

  /*
  rtc_wdt_set_length_of_reset_signal(RTC_WDT_SYS_RESET_SIG, RTC_WDT_LENGTH_3_2us);
  rtc_wdt_set_stage(RTC_WDT_STAGE0, RTC_WDT_STAGE_ACTION_RESET_SYSTEM);
  rtc_wdt_set_time(RTC_WDT_STAGE0, 250);
*/


  /*
  dumpHeap("0");
  size_t capacity = 5048;//JSON_ARRAY_SIZE(gamesRunner.getConfigSize() + effectsDrawer.getConfigSize()) * 2 + JSON_OBJECT_SIZE(1) * 5;
  DynamicJsonDocument data(capacity);

  data["width"] = MX_WIDTH;
  data["height"] = MX_HEIGHT;

  data["speed"] = __SPEED__;
  data["speedMax"] = __SPEED_MAX__;
  data["brightness"] = FastLED.getBrightness();
  dumpHeap("1");
  JsonObject tmp1 = gifDrawer.getConfig().as<JsonObject>();
  JsonObject::iterator it1 = tmp1.begin();
  char* name1 = new char ( strlen( it1->key().c_str() ));
  strcpy( name1, it1->key().c_str() );
  data[name1] = tmp1[name1];//serialized(buf);
  dumpHeap("2");

  JsonObject tmp2 = jpgDrawer.getConfig().as<JsonObject>();
//  serializeJsonPretty(tmp2, Serial);
JsonObject::iterator it2 = tmp2.begin();
  char* name2 = new char ( strlen( it2->key().c_str() ));
 strcpy( name2, it2->key().c_str() );
  dumpHeap("2");
//  char buf[2048];
//  serializeJson(tmp2[name], buf);
//  Serial.printf("\n!! BUF ( %s ) !!:\n%s\n", name,  buf);
data[name2] = tmp2[name2];//serialized(buf);
//data.add(serialized(buf));
//o = jpgDrawer.getConfig().as<JsonObject>();
  dumpHeap("7");

  dumpHeap("1");
  JsonObject tmp3 = effectsDrawer.getConfig().as<JsonObject>();
  JsonObject::iterator it3 = tmp3.begin();
  char* name3 = new char ( strlen( it3->key().c_str() ));
  strcpy( name3, it3->key().c_str() );
  data[name3] = tmp3[name3];//serialized(buf);
  dumpHeap("2");

  serializeJsonPretty(data, Serial);
  Serial.println(data.memoryUsage());
  for(;;);
*/

  show_free_mem();

  // Set Wi-Fi mode
  esp_wifi_set_ps (WIFI_PS_NONE);
  WiFi.persistent(false);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  // Wi-Fi events listener
  WiFi.onEvent(_wifi_event);

  wifi_event_group = xEventGroupCreate();


//  __MODES_FNCS_.push_back((funcPtrInfo){ "BOOT", __FNC_PTR_FLG_NONE,    &__FNC_MODE_BOOT } );
//  __MODES_FNCS_.push_back((funcPtrInfo){ "CANVAS", __FNC_PTR_FLG_REDRAW,  &__FNC_MODE_CANVAS });
  __MODES_FNCS_.insert({__MODE_TEXT, (AuroraDrawable*)&textDrawer});
//  __MODES_FNCS_.push_back((funcPtrInfo){ "GAME", __FNC_PTR_FLG_REDRAW,    &__FNC_MODE_GAME });
  __MODES_FNCS_.insert({__MODE_GAME, (AuroraDrawable*)&gamesRunner});
  __MODES_FNCS_.insert({__MODE_GIF, (AuroraDrawable*)&gifDrawer});
//  __MODES_FNCS_.push_back((funcPtrInfo){ "GIF_PLAY", __FNC_PTR_FLG_NONE, &__FNC_MODE_GIF_PLAY });
  __MODES_FNCS_.insert({__MODE_GIF_PLAY, (AuroraDrawable*)&gifDrawer});
//  __MODES_FNCS_.push_back((funcPtrInfo){ "JPG", __FNC_PTR_FLG_REDRAW,     &__FNC_MODE_JPG });
//  __MODES_FNCS_.insert({__MODE_JPG, (AuroraDrawable*)&jpgDrawer});
  __MODES_FNCS_.insert({__MODE_PNG, (AuroraDrawable*)&pngDrawer});
//  __MODES_FNCS_.push_back((funcPtrInfo){ "JPG_PLAY", __FNC_PTR_FLG_REDRAW,&__FNC_MODE_JPG_PLAY });
//  __MODES_FNCS_.insert({__MODE_JPG_PLAY, (AuroraDrawable*)&jpgDrawer});
  __MODES_FNCS_.insert({__MODE_PNG_PLAY, (AuroraDrawable*)&pngDrawer});
//  __MODES_FNCS_.push_back((funcPtrInfo){ "EFFECTS", __FNC_PTR_FLG_REDRAW, &__FNC_MODE_EFFECTS });
  __MODES_FNCS_.insert({__MODE_EFFECTS, (AuroraDrawable*)&effectsDrawer});
//  __MODES_FNCS_.push_back((funcPtrInfo){ "CLOCK", __FNC_PTR_FLG_NONE, &__FNC_MODE_CLOCK });
  __MODES_FNCS_.insert({__MODE_CLOCK, (AuroraDrawable*)&clockDrawer});
//  __MODES_FNCS_.push_back((funcPtrInfo){ "DRAW", __FNC_PTR_FLG_NONE, &__FNC_MODE_DRAW });
  __MODES_FNCS_.insert({__MODE_DRAW, (AuroraDrawable*)&fingerDrawer});

  //    __LAST_MODE = __MODE_NULL;
  __CURRENT_MODE = __MODE_NULL;//__MODE_GIF_PLAY;//
  __NEXT_MODE = __MODE_GIF;//__MODE_EFFECTS;//__MODE_GAME;//__MODE_NULL;//__MODE_EFFECTS;
  __CURRENT_MODE_FUNC = NULL;//__MODES_FNCS_[__CURRENT_MODE];
  __MODE_STOPING_FLAG = false;
  __MODE_STOPED_FLAG = false;




//#define __MODE_NULL     -1
//#define __MODE_BOOT     0
//#define __MODE_CANVAS   1
//#define __MODE_TEXT     2
//#define __MODE_GAME     3
//#define __MODE_GIF      4
//#define __MODE_GIF_PLAY 5
//#define __MODE_JPG      6
//#define __MODE_JPG_PLAY 7
//#define __MODE_EFFECTS  8
//#define __MODE_CLOCK    9
//#define __MODE_DRAW     10
////#define __MODES_COUNT 	11

  // Prepare task runner
//  runner.init();
//  runner.addTask(wifi_connect_task);
//  runner.addTask(wifi_watchdog_task);
//  // Wait and enable wifi connect task
//  wifi_connect_task.enableDelayed(5000);

  WiFi.setSleep(false);

  xTaskCreate(&watchdog_task,"watchdog_task",2048,NULL,5,NULL);
  xTaskCreate(&wifi_connect_task,"wifi_connect_task",2048,NULL,5,NULL);


/*
#define TWDT_TIMEOUT_S 20
//Initialize or reinitialize TWDT
  esp_task_wdt_init(TWDT_TIMEOUT_S, false);
//Subscribe Idle Tasks to TWDT if they were not subscribed at startup
#ifndef CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU0
  esp_task_wdt_add(xTaskGetIdleTaskHandleForCPU(0));
#endif
#ifndef CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU1
  esp_task_wdt_add(xTaskGetIdleTaskHandleForCPU(1));
#endif
*/

}

void loop(){
  led.loop();
//	if ( ping() ){
//	  //init and get the time
//	  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
//	  getTime();
//	  printTime(&timeinfo);
//	}
  yield();
//  runner.execute();

  switch (__WIFI_CONNECT_MODE) {
	case WIFI_CONNECT_MODE_PORTAL:{
	  dnsServer.processNextRequest();
	  if (restartSystem) {
		if (restartSystem + 1000 < millis()) {
		  ESP.restart();
		} //end if
	  } //end if
	  break;
	};
	case WIFI_CONNECT_MODE_STATION:{
	  break;
	};
	case WIFI_CONNECT_MODE_NONE:{
	  break;
	}
  }

  EVERY_N_MILLISECONDS(255) {
	  if (__NEXT_MODE != __MODE_NULL) {
//		  heap_caps_print_heap_info(heap_caps_get_info());
		uint freeRAM = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
//		ESP_LOGI(TAG, "free RAM is %d.", freeRAM);
		Serial.printf("__CURRENT_MODE: %d, __NEXT_MODE: %d, free RAM is %d\n", __CURRENT_MODE,  __NEXT_MODE, freeRAM);
		  if (!__MODE_STOPING_FLAG) {
			  __MODE_STOPING_FLAG = true;
		  }
	  }
	  if (__MODE_STOPING_FLAG && __MODE_STOPED_FLAG) {
//		  __LAST_MODE = __CURRENT_MODE;
		  __CURRENT_MODE = __NEXT_MODE;
		  __NEXT_MODE = __MODE_NULL;
		  __CURRENT_MODE_FUNC = __MODES_FNCS_[__CURRENT_MODE];
//		__CURRENT_MODE_FUNC->setup();
		__CURRENT_MODE_FUNC->start();
		__CURRENT_MODE_FUNC->needStart = true;
		  __MODE_STOPING_FLAG = false;
		  __MODE_STOPED_FLAG = false;
	  }
	yield();
  }

//  EVERY_N_MILLISECONDS(128) {
	  if ( __MODE_STOPING_FLAG && !__MODE_STOPED_FLAG ) {
		  __MODE_STOPED_FLAG = true;
	  }
	  if ( !__MODE_STOPED_FLAG && __CURRENT_MODE_FUNC != NULL ) {
//		if ( __CURRENT_MODE_FUNC->getFlags().clear ) {
//		  matrix->clear();
//		}

		__CURRENT_MODE_FUNC->drawFrame();

//		if ( __CURRENT_MODE_FUNC->getFlags().redraw ) {
//		  matrix->show();
//		}
//		if ( __CURRENT_MODE_FUNC->getFlags().wait) {
//		  vTaskDelay(__SPEED__ / portTICK_PERIOD_MS); //delay(__SPEED__);
//		}
	  }
//	  yield();
//  }
}



/*
void loop111(){
  yield();
  runner.execute();

  switch (__WIFI_CONNECT_MODE) {
	case WIFI_CONNECT_MODE_PORTAL:{
	  dnsServer.processNextRequest();
	  if (restartSystem) {
		if (restartSystem + 1000 < millis()) {
		  ESP.restart();
		} //end if
	  } //end if
	  break;
	};
	case WIFI_CONNECT_MODE_STATION:{
	  break;
	};
	case WIFI_CONNECT_MODE_NONE:{
	  break;
	}
  }
  radarDraw.drawFrame();
  matrix->show();
  vTaskDelay(333/ portTICK_PERIOD_MS );
}
*/

//bool ping(){
//  bool success = Ping.ping(ntpServer, 3);
//  if(!success){
//	Serial.println("Ping failed");
////	return false;
//  } else
//  	Serial.println("Ping succesful.");
//  return success;
//}


// vim:sts=4:sw=4
