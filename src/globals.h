#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "defs.h"
#include "../.pio/libdeps/esp32dev/BlinkControl/src/BlinkControl.h"
//#include "../.pio/libdeps/esp32dev/FastLED NeoMatrix/FastLED_NeoMatrix.h"
#include "../.pio/libdeps/esp32dev/ESP Async WebServer/src/ESPAsyncWebServer.h"
#include "../.pio/libdeps/esp32dev/FastLED NeoMatrix/FastLED_NeoMatrix.h"
//#include "../.pio/libdeps/esp32dev/FastLED NeoMatrix/FastLED_NeoMatrix.h"
//#include "ServerHandlers.h"

byte _GAME_BUTTONS_ = 4;




// Global Time related data
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7200;
const int   daylightOffset_sec = 3600;

tm timeinfo;

// End of Global Time related data



// LED pins
#define LED_BUILTIN 2
#include <BlinkControl.h>
BlinkControl led(LED_BUILTIN);



//uint16_t _buff[BITMAP_SIZE*4];
// for jpg helper
//uint16_t __w = 0, __h = 0, __scale;
String __imgName;

const char* host = "dreamer-led";
const char* ssid_ap = "dreamer-led";
const char* password_ap = "dreamer-led1101";
//#define DEV
//#define GUEST
//#define IPHONE
#define ANDROID
#if defined(DEV)
const char* ssid_sta = "Keenetic-1933";//"AndroidAP";//
const char* password_sta = "sMTVCwBP";//"01234567";//
#elif defined(IPHONE)
const char* ssid_sta = "iPhone vova";//"AndroidAP";
const char* password_sta = "1234567890";//"01234567";
#elif defined(ANDROID)
const char* ssid_sta = "AndroidAP";
const char* password_sta = "01234567";
#elif defined(GUEST)
const char* ssid_sta = "Guest";
const char* password_sta = "";
#endif
bool __SERVER_STARTED = false;



//int id = 1;
//uint8_t interval = 192;




String __GIFS_FOLDER = "/gifs";
String __PNGS_FOLDER = "/imgs";

volatile uint16_t __SPEED__ = 128; // скорость анимаций ( точнее - delay() после отрисовки очередного кадра, или скорость игр в демо режиме (мс)
volatile uint16_t __SPEED_MAX__ = 2048;



byte frameNum;                     // Номер фрейма проигрывания анимации
//byte buttons = 4;                  // Управление играми: нажата кнопка: 0 - верх, 1 - право, 2 - низ, 3 - лево, 4 - не нажата

// о поддерживаемых цветах читай тут https://alexgyver.ru/gyvermatrixos-guide/
#define GLOBAL_COLOR_1 CRGB::Green    // основной цвет №1 для игр
#define GLOBAL_COLOR_2 CRGB::Orange   // основной цвет №2 для игр


#define GAME_NAME_SIZE 10
String _GAME_NAME_;//[GAME_NAME_SIZE];

uint8_t _EFFECT_ID = 0;



bitmapInfo drawTaskBitmapInfo;
//uint8_t drawTaskBitmapBuffer[BITMAP_SIZE*4];
uint16_t drawTaskBitmapBuffer[NUM_LEDS];




#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <DNSServer.h>
//#include "system/GfxHelper.h"

// Create AsyncWebServer object on port 80
AsyncWebSocketClient * globalClient=NULL;
AsyncWebServer server(80);
AsyncWebServer serverPortal(80);
DNSServer dnsServer;
AsyncWebSocket ws("/echo");
#define TRIGGER_PIN 0


#define __MATRIX_PIN  18
volatile uint8_t __MX_BRIGHTNESS = BRIGHTNESS;

//CRGB leds[NUM_LEDS];
CRGBArray<NUM_LEDS> leds;
CRGB *matrixleds = leds.leds;
// Define matrix width and height.
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(matrixleds, MX_WIDTH, MX_HEIGHT,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG + 
    NEO_TILE_TOP + NEO_TILE_RIGHT +  NEO_TILE_PROGRESSIVE);

uint8_t _MX_ROTATION = 0;
char __BOOT_STR__[100];

uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint16_t speed = 255;

float matrix_gamma = 1; // higher number is darker, needed for Neomatrix more than SmartMatrix


//#include "effects/Drawable.h"
//#include "effects/Effects.h"
//Effects effects;


    
// тип текущего эффекта: 0 бегущая строка, 1 часы, 2 игры, 3 нойс маднесс и далее, 21 гифка или картинка
#define __MODE_NULL     -1
#define __MODE_BOOT     0
#define __MODE_CANVAS   1
#define __MODE_TEXT     2
#define __MODE_GAME     3
#define __MODE_GIF      4
#define __MODE_GIF_PLAY 5
#define __MODE_PNG      6
#define __MODE_PNG_PLAY 7
#define __MODE_EFFECTS  8
#define __MODE_CLOCK    9
#define __MODE_DRAW     10
//#define __MODES_COUNT 	11

#include "Drawable.h"

volatile int16_t __CURRENT_MODE = __MODE_NULL;//__MODE_TEXT;
volatile int16_t __NEXT_MODE = __MODE_NULL;//__MODE_TEXT;
//volatile int16_t __LAST_MODE = __MODE_NULL;//__MODE_TEXT;
AuroraDrawable* __CURRENT_MODE_FUNC;//=NULL;
//volatile AuroraDrawable* __LAST_MODE_FUNC;
volatile bool __MODE_STOPED_FLAG = false;
volatile bool __MODE_STOPING_FLAG = false;
volatile bool MODE_DRAW_FLAG = false;
String __MEDIA_FILE_NAME = "";

//volatile 
//String _GAME_NAME_;


//volatile 
String _RUN_STRING_TEXT_ = "hello world";

CRGB __RUNNING_STRING_COLOR = CRGB::Yellow;
CRGB __RUNNING_STRING_BACKGROUND_COLOR = CRGB::Blue;


void getTime(){
  if (!getLocalTime(&timeinfo)) {
	Serial.println("Failed to obtain time");
  }
//    hh = timeinfo.tm_hour;
//    mm = timeinfo.tm_min;
//    ss = timeinfo.tm_sec;
}

void printTime(tm* _timeinfo)
{
  Serial.println(_timeinfo, "%A, %B %d %Y %H:%M:%S");
}



#endif
