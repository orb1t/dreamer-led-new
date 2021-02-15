/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef PatternRadarDraw_H
#define PatternRadarDraw_H

//#include "../matrix.h"
//#ifndef _SEPARATE_
//#include "../matrix.h"
//#include "h2.h"
//#else
//#include "h2.h"
//#endif

#include "Drawable.h"
#include "matrixHelper.h"

#define DEBUG 1

class PatternRadarDraw : public AuroraDrawable {
  private:    
    byte hueoffset = 0;
    byte frame = 0;
    bool flag = false;

  public:
    uint16_t theta = 0;
    PatternRadarDraw() {
      name = (char *)"RadarDraw";
    }

unsigned int drawFrame() {
      if (MATRIX_WIDTH < 25) {
          effects.DimAll(252);
      } else {
          effects.DimAll(245);
      }

      for (int offset = 0; offset < MATRIX_CENTER_X; offset++) {
        //byte hue = 255 - (offset * 16 + hueoffset);
  byte hue = 255 - (offset * (256 / MATRIX_CENTER_X) + hueoffset);
        CRGB color = effects.ColorFromCurrentPalette(hue);
        uint8_t x = mapcos8(theta, offset, (MATRIX_WIDTH - 1) - offset);
        uint8_t y = mapsin8(theta, offset, (MATRIX_HEIGHT - 1) - offset);
        uint16_t xy = XY_EF(x, y);
        effects.leds[xy] = color;

        EVERY_N_MILLIS(5) {
          theta += 2;
          hueoffset += 1;
        }
      }

      return 0;
    }

};

#endif
