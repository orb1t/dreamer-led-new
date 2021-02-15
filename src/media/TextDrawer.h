
#ifndef TextDraw_H
#define TextDraw_H

#define DISPLAY_TIME_SECONDS 10
#define NUMBER_FULL_CYCLES   1
#define START_WITH_RANDOM_JPG   1


#include "Drawable.h"
#include "../matrixHelper.h"
#include "../system/GfxHelper.h"
#include "MediaHelper.h"

#define DEBUG 0


class TextDrawer : public AuroraDrawable {
private:
  unsigned long displayStartTime_millis;
  unsigned long cycleStartTime_millis;
  int nextJpg = 1;     // we haven't loaded a GIF yet on first pass through, make sure we do that
  unsigned long now;
  int _index = 0;
  uint16_t __color = matrix->Color24to16(__RUNNING_STRING_COLOR);
  uint16_t __bgcolor = matrix->Color24to16(__RUNNING_STRING_BACKGROUND_COLOR);
  int16_t _to = 0;
  int16_t _frm = 0;
  int16_t  _crsr = 0;
  uint8_t _len = 0;
  uint8_t  size = 0;
  int16_t x = 0;

  uint8_t _lCnt = 0;
  uint8_t _lCntIdx = 0;
  int _incr = 0;
public:
  TextDrawer() {
	name = (char *) "TextDrawer";
	_flags.redraw = false;
	_flags.clear = false;
	_flags.wait = false;
  };

  void setup() {
	size = max(int(MX_WIDTH/8), 1);
	matrix->clear();
	matrix->setTextWrap(false);  // we don't wrap text so it scrolls nicely
	matrix->setTextSize(size);

//	__color = matrix-> Color(tcolor.r, tcolor.g, tcolor.b);
//	__bgcolor = matrix->Color(bgcolor.r, bgcolor.g, bgcolor.b);
	matrix->setTextColor(__color, __bgcolor);
  }

  void start() {

#if DEBUG == 1
	Serial.printf("CGRB color: %2X:%2X:%2X\n", color.r, color.g, color.b);
Serial.printf("rgb565 color: %4X\n", c16b);
Serial.printf("CGRB BG color: %2X:%2X:%2X\n", bgcolor.r, bgcolor.g, bgcolor.b);
Serial.printf("rgb565 BG color: %4X\n\n", c16bbg);
#endif
	_frm = 8*size;
	_to = -6*8*size-MX_WIDTH*2;
	_crsr = MX_WIDTH/2-size*4;
	_len = size*5*_RUN_STRING_TEXT_.length();
	_to = ( _len + _len * 0.1 ) * -1;
	_lCnt = max(_frm, _to ) - min ( _frm, _to);
	_incr = -1;
	x=_frm;
//#if DEBUG == 1
	Serial.printf("txt sz: %d,from %d, to %d, cursor %d, _incr: %d ==> _lCnt: %d\n\n", size, _frm, _to, _crsr, _incr, _lCnt);
//#endif
  }

  unsigned int drawFrame() {
	display_scrollText(_RUN_STRING_TEXT_, __RUNNING_STRING_COLOR, __RUNNING_STRING_BACKGROUND_COLOR);
	return 42;
  }

  unsigned int drawFrame11() {
	if ( _lCntIdx++ >= _lCnt )
	  start();
//	now = millis();
//#if 1
//	// default behavior is to play the gif for DISPLAY_TIME_SECONDS or for NUMBER_FULL_CYCLES, whichever comes first
//	if((now - displayStartTime_millis) > (DISPLAY_TIME_SECONDS * 1000) ) {
//	  nextJpg = 1;
//	  if (++_index >= images.count)
//		_index = 0;
//	}
//#else
//	// alt behavior is to play the gif until both DISPLAY_TIME_SECONDS and NUMBER_FULL_CYCLES have passed
//    if((now - displayStartTime_millis) > (DISPLAY_TIME_SECONDS * 1000) && decoder.getCycleNumber() > NUMBER_FULL_CYCLES)
//        nextJpg = 1;
//#endif
//
//	if ( nextJpg == 1 ) {
//
//
//
//	  for (int16_t x=_frm; x>=_to; x--) {
		if ( !__MODE_STOPING_FLAG ){
//		  break;
//		} else {
		  yield();
		  matrix->clear();
		  x += _incr;
		  Serial.printf("\nsetCursor(%d,%d)\n", x, _crsr);
		  matrix->setCursor(x,_crsr);
		  _lCntIdx++;
		  matrix->print(_RUN_STRING_TEXT_.c_str());
		  matrix->show();
		  // note that on a big array the refresh rate from show() will be slow enough that
		  // the delay become irrelevant. This is already true on a 32x32 array.
		  vTaskDelay( __SPEED__/size / portTICK_PERIOD_MS ); //delay(__SPEED__/size);
		}

//	  }
//      matrix->show();
//	  displayStartTime_millis = now;
//      delay(__SPEED__);
//		nextJpg = 0;

	  }

  virtual ~TextDrawer() {

  }



//}

};




#endif // TextDraw_H
