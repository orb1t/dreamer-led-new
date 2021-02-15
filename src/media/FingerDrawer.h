
#ifndef FingerDraw_H
#define FingerDraw_H

#define DISPLAY_TIME_SECONDS 10
#define NUMBER_FULL_CYCLES   1
#define START_WITH_RANDOM_JPG   1


#include "Drawable.h"
#include "../matrixHelper.h"
#include "../system/GfxHelper.h"
#include "MediaHelper.h"

#define DEBUG 0


class FingerDrawer : public AuroraDrawable {
public:
  FingerDrawer() {
	name = (char *) "FingerDrawer";
	_flags.redraw = false;
	_flags.clear = false;
	_flags.wait = false;
  };

  void setup() {
  }

  void start() {
  }

  unsigned int drawFrame() {
	yield();
	return 42;
  }

  virtual ~FingerDrawer() {
  }

};




#endif // TextDraw_H
