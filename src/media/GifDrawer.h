
#ifndef GifDraw_H
#define GifDraw_H


#include "Drawable.h"
#include "../matrixHelper.h"
#include "SPIFFS.h"

#define DEBUG 0

//#include "../../.pio/libdeps/esp32dev/GifDecoder/src/GifDecoder.h"
#include <GifDecoder.h>
#include "MediaHelper.h"

// If the matrix is a different size than the GIFs, allow panning through the GIF
// while displaying it, or bouncing it around if it's smaller than the display
int OFFSETX = 0;
int OFFSETY = 0;
int FACTX = 4;
int FACTY = 4;
//#define SCALING true

#define DISPLAY_TIME_SECONDS 10
#define NUMBER_FULL_CYCLES   1
#define START_WITH_RANDOM_GIF   1
//#define __GIFS_FOLDER "/gifs"



unsigned long displayStartTime_millis;
unsigned long cycleStartTime_millis;
int nextGIF = 1;     // we haven't loaded a GIF yet on first pass through, make sure we do that
unsigned long now;

GifDecoder<MX_WIDTH, MX_HEIGHT, 12> decoder;
//GifDecoder<1, 1, 1> decoder;
int num_files;
File file;

void screenClearCallback(void) {
  matrix->clear();
}
void updateScreenCallback(void) {
  matrix->show();
}

void drawPixelCallback111(int16_t x, int16_t y, uint8_t red, uint8_t green, uint8_t blue) {
  red   = matrix->gamma[red];
  green = matrix->gamma[green];
  blue  = matrix->gamma[blue];
//#if DEBUG == 1
//  if (y == DEBUGLINE) {
//      Serial.print(x);
//      Serial.print(",");
//      Serial.print(y);
//      Serial.print(">");
//      Serial.print(red);
//      Serial.print(",");
//      Serial.print(green);
//      Serial.print(",");
//      Serial.print(blue);
//      Serial.println("");
//  }
//  if (y > DEBUGLINE) return;
//#endif
//#ifdef NEOMATRIX
  CRGB color = CRGB(red, green, blue);

  uint16_t basex = x*FACTX/10+OFFSETX;
  uint16_t basey = y*FACTY/10+OFFSETY;

  matrix->drawPixel(basex, basey, color);

  if (FACTX > 10 && FACTY > 10) {
      matrix->drawPixel(basex+1, basey,   color);
      matrix->drawPixel(basex,   basey+1, color);
      matrix->drawPixel(basex+1, basey+1, color);
  } else if (FACTY > 10) {
      matrix->drawPixel(basex,   basey+1, color);
  } else if (FACTX > 10) {
      matrix->drawPixel(basex+1, basey,   color);
  }
//#else
//  backgroundLayer.drawPixel(x, y, {red, green, blue});
//#endif
}

void drawPixelCallback(int16_t x, int16_t y, uint8_t red, uint8_t green, uint8_t blue) {
  CRGB color = CRGB(red, green, blue);
#if defined(SCALING)
  uint16_t basex = x*FACTX/10+OFFSETX;
  uint16_t basey = y*FACTY/10+OFFSETY;

  matrix->drawPixel(basex, basey, color);

  if (FACTX > 10 && FACTY > 10) {
	matrix->drawPixel(basex+1, basey,   color);
	matrix->drawPixel(basex,   basey+1, color);
	matrix->drawPixel(basex+1, basey+1, color);
  } else if (FACTY > 10) {
	matrix->drawPixel(basex,   basey+1, color);
  } else if (FACTX > 10) {
	matrix->drawPixel(basex+1, basey,   color);
  }
#else
  matrix->drawPixel(x, y, color);
#endif
}
bool fileSeekCallback(unsigned long position) {
  return file.seek(position);
}
unsigned long filePositionCallback(void) {
  return file.position();
}
int fileReadCallback(void) {
  return file.read();
}
int fileReadBlockCallback(void *buffer, int numberOfBytes) {
  return file.read((uint8_t *) buffer, numberOfBytes);
}
int fileSizeCallback(void) {
  return file.size();
}

class GifDrawer : public AuroraDrawable {
private:
  String _currentMedia;
//  uint16_t x;
//  uint16_t y;
//  uint16_t _w = 0;
//  uint16_t _h = 0;
public:
  mediaTypeInfo animations;

  int _index = -1;
  GifDrawer() {
	name = (char *)"GifDrawer";
  }

  void setup() {
	decoder.setScreenClearCallback(screenClearCallback);
	decoder.setUpdateScreenCallback(updateScreenCallback);
	decoder.setDrawPixelCallback(drawPixelCallback);

	decoder.setFileSeekCallback(fileSeekCallback);
	decoder.setFilePositionCallback(filePositionCallback);
	decoder.setFileReadCallback(fileReadCallback);
	decoder.setFileReadBlockCallback(fileReadBlockCallback);

	// NOTE: new callback function required after we moved to using the external AnimatedGIF library to decode GIFs
	decoder.setFileSizeCallback(fileSizeCallback);

#if (START_WITH_RANDOM_GIF == 1)
	// Seed the random number generator
	randomSeed(analogRead(14));
#endif
	// Determine how many animated GIF files exist
	num_files = enumerateMediaFiles(__GIFS_FOLDER.c_str(), ".gif", &animations, true);
	if(num_files < 0) { Serial.println("No gifs directory"); }
	if(!num_files) { Serial.println("Empty gifs directory"); }
//	dumpMediaTypeInfo(&animations);
	_flags.redraw = false;
	_flags.clear = false;
	_flags.wait = false;
  }

  std::stringstream* getConfigJson() override {
	std::stringstream* res = new std::stringstream();
	// Serial.printf("\nMedia Ext: %s, path: %s, conunt %d:\n", info->ext, info->path, info->arr.size());
	*res << "\"gifsList\":[";
	for ( int i = 0; i < animations.arr.size(); i++ ){
	//   Serial.printf("\nItem[%d]: %s\n", i, info->arr[i]->c_str());
		*res << "\"" << animations.path << "/" << animations.arr[i]->c_str() << animations.ext << ( i == animations.arr.size()-1 ? "\"" : "\"," );
	}
	*res << "]";
	return res;
  }

  void start() {

  }

  // Scroll within big bitmap so that all if it becomes visible or bounce a small one.
// If the bitmap is bigger in one dimension and smaller in the other one, it will
// be both panned and bounced in the appropriate dimensions.
// speed can be 1, 2, or 4
/*
  void panOrBounce (uint16_t *x, uint16_t *y, uint16_t sizeX, uint16_t sizeY, uint8_t speed = 1, bool reset = false ) {
	// keep integer math, deal with values 16 times too big
	// start by showing upper left of big bitmap or centering if the display is big
	static int16_t xf;
	static int16_t yf;
	// scroll speed in 1/64th
	static int16_t xfc;
	static int16_t yfc;
	// scroll down and right by moving upper left corner off screen
	// more up and left (which means negative numbers)
	static int16_t xfdir;
	static int16_t yfdir;

	if (reset) {
	  xf = max(0, (MX_WIDTH-sizeX)/2) << 6;
	  yf = max(0, (MX_HEIGHT-sizeY)/2) << 6;
	  xfc = 16*speed;
	  yfc = 16*speed;
	  xfdir = -1;
	  yfdir = -1;
	}

	bool changeDir = false;

	// Get actual x/y by dividing by 16.
	*x = xf >> 6;
	*y = yf >> 6;

	// Only pan if the display size is smaller than the pixmap
	// but not if the difference is too small or it'll look bad.
	if (sizeX-MX_WIDTH>2) {
	  xf += xfc*xfdir;
	  if (xf >= 0)                 { xfdir = -1; changeDir = true ; };
	  // we don't go negative past right corner, go back positive
	  if (xf <= ((MX_WIDTH-sizeX) << 6)) { xfdir = 1;  changeDir = true ; };
	}
	if (sizeY-MX_HEIGHT>2) {
	  yf += yfc*yfdir;
	  // we shouldn't display past left corner, reverse direction.
	  if (yf >= 0)                 { yfdir = -1; changeDir = true ; };
	  if (yf <= ((MX_HEIGHT-sizeY) << 6)) { yfdir = 1;  changeDir = true ; };
	}
	// only bounce a pixmap if it's smaller than the display size
	if (MX_WIDTH>sizeX) {
	  xf += xfc*xfdir;
	  // Deal with bouncing off the 'walls'
	  if (xf >= (MX_WIDTH-sizeX) << 6) { xfdir = -1; changeDir = true ; };
	  if (xf <= 0)           { xfdir =  1; changeDir = true ; };
	}
	if (MX_HEIGHT>sizeY) {
	  yf += yfc*yfdir;
	  if (yf >= (MX_HEIGHT-sizeY) << 6) { yfdir = -1; changeDir = true ; };
	  if (yf <= 0)           { yfdir =  1; changeDir = true ; };
	}

	if (changeDir) {
	  // Add -1, 0 or 1 but bind result to 1 to 1.
	  // Let's take 3 is a minimum speed, otherwise it's too slow.
	  xfc = constrain(xfc + random(-1*speed, 2*speed), 3*speed, 16*speed);
	  yfc = constrain(yfc + random(-1*speed, 2*speed), 3*speed, 16*speed);
	}
  }
  */

  unsigned int drawFrame() {
	now = millis();
#if 1
	// default behavior is to play the gif for DISPLAY_TIME_SECONDS or for NUMBER_FULL_CYCLES, whichever comes first
	if((now - displayStartTime_millis) > (DISPLAY_TIME_SECONDS * 1000) /*|| decoder.getCycleNumber() > NUMBER_FULL_CYCLES*/) {
	  if ( __CURRENT_MODE == __MODE_GIF ) {
		if( ( __imgName.length() > 0 ) && ( ! _currentMedia.endsWith( __imgName ) ) ){
		  int16_t _idx = getMediaIdxInTypeInfo(&animations, &__imgName);
		  if ( _idx != _index ) {
			if ((_idx < 0) || (_idx > animations.arr.size())) {
			  Serial.printf("\nSo such media: %s ( %d )\n", __imgName.c_str(), _idx);
			} else {
			  _index = _idx;
			  nextGIF = 1;
			}
		  }
//		  nextPng = 1;
		} else {
		  if ( 0 > _index ) {
			_index = 0;
			nextGIF = 1;
		  } else
			nextGIF = 0;
		}

	  }
//		int16_t _idx = getMediaIdxInTypeInfo(&animations, &__imgName);
//		if ( _idx != _index ) {
//		  if ((_idx < 0) || (_idx > animations.arr.size())) {
//			Serial.printf("\nSo such media: %s ( %d )\n", __imgName.c_str(), _idx);
//		  } else {
//			_index = _idx;
//			nextGIF = 1;
//		  }
//		}
//	  }


//	  nextGIF = 1;
	}
#else
	// alt behavior is to play the gif until both DISPLAY_TIME_SECONDS and NUMBER_FULL_CYCLES have passed
    if((now - displayStartTime_millis) > (DISPLAY_TIME_SECONDS * 1000) && decoder.getCycleNumber() > NUMBER_FULL_CYCLES)
        nextGIF = 1;
#endif

	if(nextGIF)	{
	  cycleStartTime_millis = now;
	  nextGIF = 0;
	  _currentMedia = String(animations.path) + "/" + String( *animations.arr[_index] ) + animations.ext;
	  Serial.printf("\nRendering GIF: %s\n", _currentMedia.c_str());
		file = SPIFFS.open(_currentMedia); //openMediaFilenameByIndex(__GIFS_FOLDER.c_str(), ".gif", _index);
	  if ( file ) {

		if(decoder.startDecoding() < 0) {
		  nextGIF = 1;
		} else {
//		  decoder.getSize( &_w, &_h );
		  // Calculate time in the future to terminate animation
		  displayStartTime_millis = now;
//		  panOrBounce(&x, &y, _w, _h, 8, true);
		}

	  }
	  // get the index for the next pass through
	  if (++_index >= animations.arr.size()) {
		_index = 0;
	  }
	}

//	panOrBounce(&x, &y, _w, _h, 8);
//	OFFSETX = x;
//	OFFSETY = y;

	int returnCode = decoder.decodeFrame();
	if(returnCode < 0) {
	  // There's an error with this GIF, go to the next one
	  nextGIF = 1;
	}

	// we completed one pass of the GIF, print some stats
	if(returnCode == 123){//ERROR_DONE_PARSING) {
//#if DEBUG == 1
//	  // Print the stats for this GIF
//	  char buf[80];
//	  int32_t frames       = decoder.getFrameCount();
//	  int32_t cycle_design = decoder.getCycleTime();  // Intended duration
//	  int32_t cycle_actual = now - cycleStartTime_millis;       // Actual duration
//	  int32_t percent = 100 * cycle_design / cycle_actual;
//	  sprintf(buf, "[%ld frames = %ldms] actual: %ldms speed: %ld%%",
//			  frames, cycle_design, cycle_actual, percent);
//	  Serial.println(buf);
//#endif
	  cycleStartTime_millis = now;
	}
  }

  virtual ~GifDrawer() {};
};

#endif
