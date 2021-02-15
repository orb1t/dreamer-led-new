
#ifndef JpgDraw_H
#define JpgDraw_H

#define DISPLAY_TIME_SECONDS 10
#define NUMBER_FULL_CYCLES   1
#define START_WITH_RANDOM_JPG   1


#include "Drawable.h"
#include "../matrixHelper.h"
#include "SPIFFS.h"
//#include "../../.pio/libdeps/esp32dev/GifDecoder/src/GifDecoder.h"
//#include <GifDecoder.h>
//#include "../../.pio/libdeps/esp32dev/TJpg_Decoder/src/TJpg_Decoder.h"
#include "MediaHelper.h"
//#include "../../.pio/libdeps/esp32dev/TJpg_Decoder/src/TJpg_Decoder.h"
//#include "jpgHelper.h"
//#include <TJpg_Decoder.h>

#include "../.pio/libdeps/esp32dev/pngle/pngle.h"
//#include <HTTPClient.h>
#include <math.h>
#include "../system/GfxHelper.h"
//#include <pngle.h>
//#include <mbcontroller.h>


#define DEBUG 0

//uint16_t* _buff_ptr;
//uint16_t __w = 0;
//uint16_t __h = 0;
//double __scale = 1.0;

void cls()
{
//  M5.Lcd.fillScreen(TFT_BLACK);
//
//  M5.Lcd.setCursor(0, 0);
//  M5.Lcd.setTextColor(WHITE);
//  M5.Lcd.setTextSize(1);
  matrix->clear();
//matrix->show();
}

// ===================================================
// pngle example for m5stack
// ===================================================

double g_scale = 1.0;
void pngle_on_draw(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4])
{
  uint16_t color = (rgba[0] << 8 & 0xf800) | (rgba[1] << 3 & 0x07e0) | (rgba[2] >> 3 & 0x001f);

//  Serial.printf("pngle_on_draw: x=%d, y = %d, w=%d, h=%d ==> %XX:%XX:%XX::\n", x, y, w, h, rgba[0], rgba[1], rgba[2], rgba[3]);

  if (rgba[3]) {
	x = ceil(x * g_scale);
	y = ceil(y * g_scale);
	w = ceil(w * g_scale);
	h = ceil(h * g_scale);
//	M5.Lcd.fillRect(x, y, w, h, color);
	matrix->fillRect(x, y, w, h, matrix->Color(rgba[0], rgba[1], rgba[2]));
	drawPixelMask();
	matrix->show();
  }
}


class PngDrawer : public AuroraDrawable {
private:
  String _currentMedia;
  unsigned long displayStartTime_millis;
  unsigned long cycleStartTime_millis;
  int nextPng = 1;     // we haven't loaded a GIF yet on first pass through, make sure we do that
  unsigned long now;
  int num_files;
//  File file;
  int _index = 0;
  uint8_t* buf = NULL;

public:
  mediaTypeInfo images;
//  char *filename;
  PngDrawer() {
	name = (char *) "PngDrawer";

	_flags.redraw = false;
	_flags.clear = false;
	_flags.wait = false;

	buf = new uint8_t(1024);
  };

  char* getConfig(AsyncResponseStream* strm) override {
	return vectorToJsonArray(static_cast<mediaTypeInfo* const>(&images), (const char*)&"imageList", strm);
  }
  uint8_t getConfigSize() override {
	return images.arr.size();
  };

  void setup() {
// Determine how many animated GIF files exist
	num_files = enumerateMediaFiles(__PNGS_FOLDER.c_str(), ".png", &images, true);
	Serial.printf("\nFOLDER: %s, contains %d files\n", __PNGS_FOLDER.c_str(), num_files);
	if(num_files < 0) { Serial.println("No PNG's in directory"); }
	if(!num_files) { Serial.println("Empty PNG's directory"); }
//	dumpMediaTypeInfo(&images);
//	nextPng = 1;
//	_index = -1;
//	file = SPIFFS.open(images.arr[_index]);
  }

  void start() {
//	if (needStart) {
//
//	}
  }

//  void play(const String* name) {
//	_index = getMediaIdxInTypeInfo(&images, name);
//  };

  unsigned int drawFrame() {
//	if ( needStart ) {
//	  start();
//	  needStart = false;
//	}

/*
	now = millis();
#if 1
	// default behavior is to play the gif for DISPLAY_TIME_SECONDS or for NUMBER_FULL_CYCLES, whichever comes first
	if(  (now - displayStartTime_millis) > (DISPLAY_TIME_SECONDS * 1000) ) {
	  if (__CURRENT_MODE == __MODE_PNG ) {
		if ( ( __imgName.length() > 0 ) && ( ! _currentMedia.endsWith( __imgName ) ) ){
		  int16_t _idx = getMediaIdxInTypeInfo(&images, &__imgName);
		  if ( _idx != _index ) {
			if ((_idx < 0) || (_idx > images.arr.size())) {
			  Serial.printf("\nSo such media: %s ( %d )\n", __imgName.c_str(), _idx);
			} else {
			  _index = _idx;
			  nextPng = 1;
			}
		  }
//		  nextPng = 1;
		} else {
		  if ( 0 > _index ){
			_index = 0;
			nextPng = 1;
		  } else
			nextPng = 0;
		}


	  }

//	  }

	  if (__CURRENT_MODE == __MODE_PNG_PLAY ) {
		if ( ++_index >= images.arr.size()) {
		  _index = 0;
		}
		nextPng = 1;
	  }
	}

#else
	// alt behavior is to play the gif until both DISPLAY_TIME_SECONDS and NUMBER_FULL_CYCLES have passed
    if((now - displayStartTime_millis) > (DISPLAY_TIME_SECONDS * 1000) && decoder.getCycleNumber() > NUMBER_FULL_CYCLES)
        nextJpg = 1;
#endif
*/
//	if (nextPng == 1 ) {

	  _currentMedia = String(images.path) + "/" + String(images.arr[_index]->c_str()) + String(images.ext); //openMediaFilenameByIndex(__GIFS_FOLDER.c_str(), ".gif", _index);
	  Serial.printf("\nRendering PNG: %s\n", _currentMedia.c_str());
	  load_file(SPIFFS, _currentMedia.c_str());
	  matrix->show();
	  displayStartTime_millis = now;
      delay(__SPEED__);
		nextPng = 0;
//	  }
	}

  virtual ~PngDrawer() {
//	free(buf);
  }

// Render from SPIFFS or SD
void load_file(fs::FS &fs, const char *path)
{
  fs::File file = fs.open(path);
  if (!file) {
	Serial.println("Failed to open file for reading");
	return ;
  }

  pngle_t *pngle = pngle_new();
  pngle_set_draw_callback(pngle, pngle_on_draw);

  // Feed data to pngle


  int remain = 0;
  int len;
//  #if !defined(USE_ADAFRUIT_GFX) && !defined(USE_LINE_BUFFER)
//   tft.startWrite(); // Crashes Adafruit_GFX
//  #endif
  matrix->clear();

  while ((len = file.read(buf + remain, sizeof(buf) - remain)) > 0) {
	int fed = pngle_feed(pngle, buf, remain + len);
	if (fed < 0) {
	  Serial.printf("ERROR: %s\n", pngle_error(pngle));
	  break;
	}

	remain = remain + len - fed;
	if (remain > 0) memmove(buf, buf + fed, remain);
  }

//!!!!
//  if (pc) {
//    matrix->drawRGBBitmap(png_dx + sx, png_dy + sy, lbuf, pc, 1);
//    matrix->show();
//    pc = 0;
//  }

//#ifdef USE_LINE_BUFFER
//   // Draw any remaining pixels - had no warning that image has ended...
//  if (pc) {
//  #ifdef USE_ADAFRUIT_GFX
//    tft.drawRGBBitmap(png_dx + sx, png_dy + sy, lbuf, pc, 1);
//  #else
//    tft.pushImage(png_dx + sx, png_dy + sy, pc, 1, lbuf);
//  #endif
//    pc = 0;
//  }
//#endif
//  #if !defined(USE_ADAFRUIT_GFX) && !defined(USE_LINE_BUFFER)
//    tft.endWrite();
//  #endif
//  pngle_destroy(pngle);
  file.close();
}
};


#endif
