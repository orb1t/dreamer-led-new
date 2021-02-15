#ifndef _DEFS_H_
#define _DEFS_H_

//#include <Adafruit_GFX.h>
#include <FastLED_NeoMatrix.h>
#define FASTLED_ALL_PINS_HARDWARE_SPI
#include <FastLED.h>

#include <SPIFFS.h>
#include <vector>
#include <map>


struct WiFiScanInfo {
  WiFiScanInfo(const char *_ssid, int32_t _rssi) : ssid(const_cast<char *>(_ssid)), rssi(_rssi) {}
  char*  ssid;    //  Far addresses are untyped 32 bit integers.
  int32_t rssi;
};


struct Config {
  Config(char* _ssid, char* _pass) : ssid(_ssid), pass(_pass) {}
  char* ssid;
  char* pass;
};

std::vector<Config*> config;                         // <- global configuration object
//std::map<String*, String*> config;                         // <- global configuration object

struct drawableFlags{
  bool redraw = true;
  bool clear = true;
  bool wait = true;
};

struct mediaTypeInfo {
  char*  ext;    //  Far addresses are untyped 32 bit integers.
  char* path;
//  uint8_t count;
  std::vector<String*> arr;  // declare the array
};


#define mmin(a,b) ((a<b)?(a):(b))
#define mmax(a,b) ((a>b)?(a):(b))


typedef struct {
  uint16_t r  : 5;   // 1..31 [5 bits]
  uint16_t g  : 6;   // 0..63 [6 bits]
  uint16_t b  : 5;   // 0..31 [5 bits]  
} RGB565;

// min/max are complicated. Arduino and ESP32 layers try to be helpful by using
// templates that take specific kinds of arguments, but those do not always work
// with mixed types:
// error: no matching function for call to 'max(byte&, int16_t&)'
// These defines get around this problem.
#define mmin(a,b) ((a<b)?(a):(b))
#define mmax(a,b) ((a>b)?(a):(b))


typedef void (*doOnFileFoundCallback)(File file);
typedef void (*taskFunctionPtr)(void * pvParameters);


#define __FNC_PTR_FLG_NONE 0
#define __FNC_PTR_FLG_CLEAR 1
#define __FNC_PTR_FLG_REDRAW 2
#define __FNC_PTR_FLG_CLEAR_REDRAW __FNC_PTR_FLG_CLEAR & __FNC_PTR_FLG_REDRAW
//typedef void (*funcPtr)();
//struct funcPtrInfo
//{
//  String name;
//  byte flags;
//  funcPtr func;    //  Far addresses are untyped 32 bit integers.
//};



// Max is 255, 32 is a conservative value to not overload
// a USB power supply (500mA) for 12x12 pixels.
#define BRIGHTNESS 32
#define MX_COLORS 3
#define MX_WIDTH 16
#define MATRIX_WIDTH MX_WIDTH
#define MX_HEIGHT 16
#define MATRIX_HEIGHT MX_HEIGHT

#define  MATRIX_CENTER_X (MX_WIDTH / 2)
#define  MATRIX_CENTER_Y (MX_HEIGHT / 2)

#define  MATRIX_CENTRE_X (MATRIX_CENTER_X - 1)
#define  MATRIX_CENTRE_Y (MATRIX_CENTER_Y - 1)


#define NUM_LEDS (MX_WIDTH*MX_HEIGHT)
#define NUMMATRIX NUM_LEDS
#define BITMAP_SIZE ( NUM_LEDS * MX_COLORS )



struct bitmapInfo
{
  const unsigned short* bitmap;    //  Far addresses are untyped 32 bit integers.
  uint8_t width;  
  uint8_t height;
  byte flags;
};



// Allow temporaly dithering, does not work with ESP32 right now
#ifndef ESP32
#define delay FastLED.delay
#endif



// https://learn.adafruit.com/adafruit-neopixel-uberguide/neomatrix-library
// MATRIX DECLARATION:
// Parameter 1 = width of EACH NEOPIXEL MATRIX (not total display)
// Parameter 2 = height of each matrix
// Parameter 3 = number of matrices arranged horizontally
// Parameter 4 = number of matrices arranged vertically
// Parameter 5 = pin number (most are valid)
// Parameter 6 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the FIRST MATRIX; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs WITHIN EACH MATRIX are
//     arranged in horizontal rows or in vertical columns, respectively;
//     pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns WITHIN
//     EACH MATRIX proceed in the same order, or alternate lines reverse
//     direction; pick one.
//   NEO_TILE_TOP, NEO_TILE_BOTTOM, NEO_TILE_LEFT, NEO_TILE_RIGHT:
//     Position of the FIRST MATRIX (tile) in the OVERALL DISPLAY; pick
//     two, e.g. NEO_TILE_TOP + NEO_TILE_LEFT for the top-left corner.
//   NEO_TILE_ROWS, NEO_TILE_COLUMNS: the matrices in the OVERALL DISPLAY
//     are arranged in horizontal rows or in vertical columns, respectively;
//     pick one or the other.
//   NEO_TILE_PROGRESSIVE, NEO_TILE_ZIGZAG: the ROWS/COLUMS OF MATRICES
//     (tiles) in the OVERALL DISPLAY proceed in the same order for every
//     line, or alternate lines reverse direction; pick one.  When using
//     zig-zag order, the orientation of the matrices in alternate rows
//     will be rotated 180 degrees (this is normal -- simplifies wiring).
//   See example below for these values in action.



#define THIS_X x
#define THIS_Y y


// This could also be defined as matrix->Color(255,0,0) but those defines
// are meant to work for adafruit_gfx backends that are lacking Color()
#define LED_BLACK    0

#define LED_RED_VERYLOW   (3 <<  11)
#define LED_RED_LOW     (7 <<  11)
#define LED_RED_MEDIUM    (15 << 11)
#define LED_RED_HIGH    (31 << 11)

#define LED_GREEN_VERYLOW (1 <<  5)   
#define LED_GREEN_LOW     (15 << 5)  
#define LED_GREEN_MEDIUM  (31 << 5)  
#define LED_GREEN_HIGH    (63 << 5)  

#define LED_BLUE_VERYLOW  3
#define LED_BLUE_LOW    7
#define LED_BLUE_MEDIUM   15
#define LED_BLUE_HIGH     31

#define LED_ORANGE_VERYLOW  (LED_RED_VERYLOW + LED_GREEN_VERYLOW)
#define LED_ORANGE_LOW    (LED_RED_LOW     + LED_GREEN_LOW)
#define LED_ORANGE_MEDIUM (LED_RED_MEDIUM  + LED_GREEN_MEDIUM)
#define LED_ORANGE_HIGH   (LED_RED_HIGH    + LED_GREEN_HIGH)

#define LED_PURPLE_VERYLOW  (LED_RED_VERYLOW + LED_BLUE_VERYLOW)
#define LED_PURPLE_LOW    (LED_RED_LOW     + LED_BLUE_LOW)
#define LED_PURPLE_MEDIUM (LED_RED_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_PURPLE_HIGH   (LED_RED_HIGH    + LED_BLUE_HIGH)

#define LED_CYAN_VERYLOW  (LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_CYAN_LOW    (LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_CYAN_MEDIUM   (LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_CYAN_HIGH   (LED_GREEN_HIGH    + LED_BLUE_HIGH)

#define LED_WHITE_VERYLOW (LED_RED_VERYLOW + LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_WHITE_LOW   (LED_RED_LOW     + LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_WHITE_MEDIUM  (LED_RED_MEDIUM  + LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_WHITE_HIGH    (LED_RED_HIGH    + LED_GREEN_HIGH    + LED_BLUE_HIGH)



#endif
