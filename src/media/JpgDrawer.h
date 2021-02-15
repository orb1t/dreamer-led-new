
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
#include "../../.pio/libdeps/esp32dev/TJpg_Decoder/src/TJpg_Decoder.h"
//#include "jpgHelper.h"
#include <TJpg_Decoder.h>

#define DEBUG 0

uint16_t* _buff_ptr;
uint16_t __w = 0;
uint16_t __h = 0;
uint16_t __scale = 1;
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap){
//Serial.printf("x: %d, y: %d; w: %d, h: %d, bitmap: %d, bitmap_sz: %d\n", x, y, w, h, bitmap, sizeof(bitmap));
// Stop further decoding as image is running off bottom of screen
  if ( y >= MX_HEIGHT )
	return 0;
  for (int16_t j = 0; j < h; j++) {
	for (int16_t i = 0; i < w; i++) {
#if DEBUG == 1
	  Serial.printf("i: %d, j: %d; x+i: %d, y+j: %d, minn: %d\n", i, j, x+i, y+j, -(2*i));
#endif
	  uint16_t _t = bitmap[XY(i, j, w)];
	  _buff_ptr[XY(x + i, y + j, ( __w >= MX_WIDTH / 2 ? MX_WIDTH : __w ) )] = _t;
	}
  }
  // Return 1 to decode next block
  return 1;
}

class JpgDrawer : public AuroraDrawable {
private:

  unsigned long displayStartTime_millis;
  unsigned long cycleStartTime_millis;
  int nextJpg = 1;     // we haven't loaded a GIF yet on first pass through, make sure we do that
  unsigned long now;
  int num_files;
//  File file;
  int _index = 0;
  void renderJpegToBuffer(const char *filename, int xpos, int ypos, uint16_t* _buff) {
	// Get the width and height in pixels of the jpeg if you wish
	TJpgDec.getFsJpgSize(&__w, &__h, filename); // Note name preceded with "/"
//  tft.setRotation(w > h ? 1 : 0);
#if DEBUG == 1
	Serial.print("Width = "); Serial.print(__w); Serial.print(", height = "); Serial.println(__h);
#endif
	for (__scale = 1; __scale <= 8; __scale <<= 1) {
	  if (__w <= MX_WIDTH * __scale && __h <= MX_HEIGHT * __scale) break;
	}
	TJpgDec.setJpgScale(__scale);
	_buff_ptr = _buff;
	// Draw the image, top left at 0,0
	TJpgDec.drawFsJpg(0, 0, filename);
  }



public:
  mediaTypeInfo images;
//  char *filename;
  JpgDrawer() {
	name = (char *) "JpgDrawer";

	_flags.redraw = false;
	_flags.clear = false;
	_flags.wait = false;

	// The jpeg image can be scaled by a factor of 1, 2, 4, or 8
	TJpgDec.setJpgScale(1);
	// The byte order can be swapped (set true for TFT_eSPI)
	TJpgDec.setSwapBytes(false);
	// The decoder must be given the exact name of the rendering function above
	TJpgDec.setCallback(tft_output);

#if (START_WITH_RANDOM_JPG == 1)
	// Seed the random number generator
	randomSeed(analogRead(14));
#endif
  };
  void setup() {
// Determine how many animated GIF files exist
	num_files = enumerateMediaFiles(__JPGS_FOLDER.c_str(), ".jpg", &images, true);
	Serial.printf("\nFOLDER: %s, contains %d files\n", __JPGS_FOLDER.c_str(), num_files);
	if(num_files < 0) { Serial.println("No jpgs in directory"); }
	if(!num_files) { Serial.println("Empty jpgs directory"); }
//	dumpMediaTypeInfo(&images);
	nextJpg = 1;
	_index = 0;
//	file = SPIFFS.open(images.arr[_index]);
  }
//   DynamicJsonDocument getConfig() override {
// 	  return vectorToJsonArray(static_cast<mediaTypeInfo* const>(&images), (const char*)&"imageList");
//   }
//   uint8_t getConfigSize() override {
// 	return images.arr.size();
//   };
char* getConfig(AsyncResponseStream* strm) override {
	return vectorToJsonArray(static_cast<mediaTypeInfo* const>(&images), (const char*)&"imageList", strm);
  }
  uint8_t getConfigSize() override {
	return images.arr.size();
  };


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
	now = millis();
#if 1
	// default behavior is to play the gif for DISPLAY_TIME_SECONDS or for NUMBER_FULL_CYCLES, whichever comes first
	if(  (now - displayStartTime_millis) > (DISPLAY_TIME_SECONDS * 1000) ) {
	  if ( __CURRENT_MODE == __MODE_JPG ) {
		int8_t _idx = getMediaIdxInTypeInfo(&images, &__imgName);
		if ( _idx != _index ) {
		  if ((_idx < 0) || (_idx > images.arr.size())) {
			Serial.printf("\nSo such media: %s ( %d )\n", __imgName.c_str(), _idx);
		  } else {
			_index = _idx;
			nextJpg = 1;
		  }
		}
	  }

	  if ( __CURRENT_MODE == __MODE_JPG_PLAY ) {
		if ( ++_index >= images.arr.size()) {
		  _index = 0;
		}
		nextJpg = 1;
	  }
	}

#else
	// alt behavior is to play the gif until both DISPLAY_TIME_SECONDS and NUMBER_FULL_CYCLES have passed
    if((now - displayStartTime_millis) > (DISPLAY_TIME_SECONDS * 1000) && decoder.getCycleNumber() > NUMBER_FULL_CYCLES)
        nextJpg = 1;
#endif

	if ( nextJpg == 1 ) {
//	  cycleStartTime_millis = now;
	  String fp = String(images.path) + "/" + String ( *images.arr[_index]) + String(images.ext); //openMediaFilenameByIndex(__GIFS_FOLDER.c_str(), ".gif", _index);
	  Serial.printf("\nRendering JPG: %s\n", fp.c_str());
	  //      matrix->clear();
	  renderJpegToBuffer(fp.c_str(), 0 , 0, (uint16_t*)&drawTaskBitmapBuffer);     // 240 x 320 image
      renderBufferToMatrix( 0, 0, MX_WIDTH, MX_HEIGHT, (uint16_t*)&drawTaskBitmapBuffer, 0);
//	  matrix->drawRGBBitmap(0, 0,  (uint16_t*)&drawTaskBitmapBuffer, MX_WIDTH, MX_HEIGHT);
//      matrix->show();
	  displayStartTime_millis = now;
//      delay(__SPEED__);
		nextJpg = 0;
	  }
	}

  virtual ~JpgDrawer() {

  }
};


#endif
