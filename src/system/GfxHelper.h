#ifndef _GFXHELPER_H_
#define _GFXHELPER_H_

//#include "helpers.h"
#include "globals.h"
//#include "../../.pio/libdeps/esp32dev/FastLED/src/chipsets.h"

/*
void display_scrollText_multyline(String txt, const CRGB tcolor, const CRGB bgcolor) {

//  String tmp[2];
  int count = split(txt, ';');
#if DEBUG == 1
  for (int j = 0; j < count; ++j)
  {
    if (strings_for_split[j].length() > 0)
      Serial.println(strings_for_split[j]);
  }
#endif

  uint8_t size = max(int(MX_WIDTH/8), 1);
  int16_t _len = size * 5 * max ( strings_for_split[0].length(), strings_for_split[1].length());
  int16_t _to = ( _len * 1.2 ) * -1;
  
  matrix->clear();
  matrix->setTextWrap(false);  // we don't wrap text so it scrolls nicely
  matrix->setTextSize(1);
  matrix->setRotation(0);
  for (int8_t x=7; x>=_to; x--) {
    if ( __MODE_STOPED_FLAG || __MODE_STOPING_FLAG ){
      break;
    }
    else{
      yield();
      matrix->clear();
      matrix->setCursor(x,0);
      matrix->setTextColor(LED_GREEN_HIGH);
      matrix->print(strings_for_split[0]);
      if (MX_HEIGHT>11) {
        matrix->setCursor(-20-x,MX_HEIGHT-7);
        matrix->setTextColor(LED_ORANGE_HIGH);
        matrix->print(strings_for_split[1]);
      }
      matrix->show();
	  vTaskDelay( __SPEED__/size / portTICK_PERIOD_MS ); //delay(__SPEED__/size);
    }
  }
}
*/
#define DEBUG 0

void display_scrollText(String txt, const CRGB tcolor, const CRGB bgcolor) {
  uint8_t size = max(int(MX_WIDTH/8), 1);
  matrix->clear();
  matrix->setTextWrap(false);  // we don't wrap text so it scrolls nicely
  matrix->setTextSize(size);

uint16_t c16b = matrix->Color(tcolor.r, tcolor.g, tcolor.b);
uint16_t c16bbg = matrix->Color(bgcolor.r, bgcolor.g, bgcolor.b);
#if DEBUG == 1
Serial.printf("CGRB color: %2X:%2X:%2X\n", tcolor.r, tcolor.g, tcolor.b);
Serial.printf("rgb565 color: %4X\n", c16b);
Serial.printf("CGRB BG color: %2X:%2X:%2X\n", bgcolor.r, bgcolor.g, bgcolor.b);
Serial.printf("rgb565 BG color: %4X\n\n", c16bbg);
#endif
matrix->setTextColor(c16b, c16bbg);
int16_t _frm = 8*size;
int16_t _to = -6*8*size-MX_WIDTH*2;
int16_t _crsr = MX_WIDTH/2-size*4;
int16_t _len = size*5*txt.length();
_to = ( _len + _len * 0.1 ) * -1;
#if DEBUG == 1
Serial.printf("txt sz: %d,from %d, to %d, cursor %d\n\n", size, _frm, _to, _crsr);
#endif

  for (int16_t x=_frm; x>=_to; x--) {
    if ( __MODE_STOPED_FLAG ){
      break;
    } else {
      yield();
      matrix->clear();
#if DEBUG == 1
	  Serial.printf("\nsetCursor(%d,%d)\n", x, _crsr);
#endif
      matrix->setCursor(x,_crsr);
      matrix->print((const char*)txt.c_str());
      matrix->show();
      // note that on a big array the refresh rate from show() will be slow enough that
      // the delay become irrelevant. This is already true on a 32x32 array.
	  vTaskDelay( __SPEED__/size / portTICK_PERIOD_MS ); //delay(__SPEED__/size);
    }
  }
}


// Scroll within big bitmap so that all if it becomes visible or bounce a small one.
// If the bitmap is bigger in one dimension and smaller in the other one, it will
// be both panned and bounced in the appropriate dimensions.
void display_panOrBounceBitmap(const bitmapInfo* bitmapNfo) {
  uint8_t bitmapSize = bitmapNfo->width;
  // keep integer math, deal with values 16 times too big
  // start by showing upper left of big bitmap or centering if the display is big
  int16_t xf = max(0, (MX_WIDTH-bitmapSize)/2) << 4;
  int16_t yf = max(0, (MX_HEIGHT-bitmapSize)/2) << 4;
  // scroll speed in 1/16th
  int16_t xfc = 6;
  int16_t yfc = 3;
  // scroll down and right by moving upper left corner off screen 
  // more up and left (which means negative numbers)
  int16_t xfdir = -1;
  int16_t yfdir = -1;

#if DEBUG == 1
  Serial.printf("\ndisplay_panOrBounceBitmap\n: bitmapSize: %d, xf: %d, yf: %d; xfc: %d, yfc: %d; xfdir: %d, yfdir: %d\n", bitmapSize, xf, yf, xfc, yfc, xfdir, yfdir);
#endif

  for (uint16_t i=1; i<200; i++) {
    if ( ! __MODE_STOPED_FLAG ) {
      bool updDir = false;
    
      // Get actual x/y by dividing by 16.
      int16_t x = xf >> 4;
      int16_t y = yf >> 4;
    
      matrix->clear();
      matrix->drawRGBBitmap(x, y, bitmapNfo->bitmap, bitmapNfo->width, bitmapNfo->height);  
      matrix->show();
       
      // Only pan if the display size is smaller than the pixmap
      // but not if the difference is too small or it'll look bad.
      if (bitmapSize-MX_WIDTH>2) {
          xf += xfc*xfdir;
          if (xf >= 0)                      { xfdir = -1; updDir = true ; };
          // we don't go negative past right corner, go back positive
          if (xf <= ((MX_WIDTH-bitmapSize) << 4)) { xfdir = 1;  updDir = true ; };
      }
      if (bitmapSize-MX_HEIGHT>2) {
          yf += yfc*yfdir;
          // we shouldn't display past left corner, reverse direction.
          if (yf >= 0)                      { yfdir = -1; updDir = true ; };
          if (yf <= ((MX_HEIGHT-bitmapSize) << 4)) { yfdir = 1;  updDir = true ; };
      }
      // only bounce a pixmap if it's smaller than the display size
      if (MX_WIDTH>bitmapSize) {
          xf += xfc*xfdir;
          // Deal with bouncing off the 'walls'
          if (xf >= (MX_WIDTH-bitmapSize) << 4) { xfdir = -1; updDir = true ; };
          if (xf <= 0)           { xfdir =  1; updDir = true ; };
      }
      if (MX_HEIGHT>bitmapSize) {
          yf += yfc*yfdir;
          if (yf >= (MX_HEIGHT-bitmapSize) << 4) { yfdir = -1; updDir = true ; };
          if (yf <= 0)           { yfdir =  1; updDir = true ; };
      }
      
      if (updDir) {
          // Add -1, 0 or 1 but bind result to 1 to 1.
          // Let's take 3 is a minimum speed, otherwise it's too slow.
          xfc = constrain(xfc + random(-1, 2), 3, 16);
          yfc = constrain(xfc + random(-1, 2), 3, 16);
      }
	  vTaskDelay( __SPEED__/ portTICK_PERIOD_MS ); //delay(__SPEED__);
    }
  }
}



// Convert a BGR 4/4/4 bitmap to RGB 5/6/5 used by Adafruit_GFX
void fixdrawRGBBitmap(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h) {
  uint16_t RGB_bmp_fixed[w * h];
  for (uint16_t pixel=0; pixel<w*h; pixel++) {
    uint8_t r,g,b;
    uint16_t color = pgm_read_word(bitmap + pixel);
  
    //Serial.print(color, HEX);
    b = (color & 0xF00) >> 8;
    g = (color & 0x0F0) >> 4;
    r = color & 0x00F;
  #if DEBUG == 1  
    Serial.print(" ");
    Serial.print(b);
    Serial.print("/");
    Serial.print(g);
    Serial.print("/");
    Serial.print(r);
    Serial.print(" -> ");
  #endif  
    // expand from 4/4/4 bits per color to 5/6/5
    b = map(b, 0, 15, 0, 31);
    g = map(g, 0, 15, 0, 63);
    r = map(r, 0, 15, 0, 31);
  #if DEBUG == 1  
    Serial.print(r);
    Serial.print("/");
    Serial.print(g);
    Serial.print("/");
    Serial.print(b);
  #endif  
    RGB_bmp_fixed[pixel] = (r << 11) + (g << 5) + b;
#if DEBUG == 1    
    //Serial.print(" -> ");
    //Serial.println(RGB_bmp_fixed[pixel], HEX);
#endif    
  }
  matrix->drawRGBBitmap(x, y, RGB_bmp_fixed, w, h);
}

void drawPixelMask(){
//  matrix->clear();
//  matrix->drawPixel(0, 0, (uint16_t)CRGB::Red);
  leds[ XY_EF(0, 0)] = CRGB::Red;
//  matrix->drawPixel(MX_WIDTH-1, MX_HEIGHT-1, (uint16_t)CRGB::Yellow);
leds[ XY_EF(MX_WIDTH-1, MX_HEIGHT-1)] = CRGB::Yellow;
//  matrix->drawPixel(MX_WIDTH-1, 0, (uint16_t)CRGB::Green);
    leds[ XY_EF(MX_WIDTH-1, 0)] = CRGB::Green;
//  matrix->drawPixel(0, MX_HEIGHT-1, (uint16_t)CRGB::Blue);
    leds[ XY_EF(0, MX_HEIGHT-1)] = CRGB::Blue;
  matrix->show();
}

void initGfx(){
  
//  FastLED.addLeds<WS2812, 4, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.addLeds<WS2812, __MATRIX_PIN, GRB>(matrixleds, NUM_LEDS).setCorrection(TypicalSMD5050);//TypicalSMD5050);
//  FastLED.addLeds<NEOPIXEL,4>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
//  FastLED.setBrightness( BRIGHTNESS );
  Serial.print("Matrix Size: ");
  Serial.print(MX_WIDTH);
  Serial.print(" ");
  Serial.print(MX_HEIGHT);
  Serial.print(" ");
  Serial.println(NUM_LEDS);

  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setBrightness(__MX_BRIGHTNESS);
//  Serial.println("If the code crashes here, decrease the brightness or turn off the all white display below");
  matrix->clear();
  matrix->fillScreen(CRGB::Black);
//  drawPixelMask();
  matrix->show();
}


#endif
