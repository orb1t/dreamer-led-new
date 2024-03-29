/*
*
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

#ifndef PatternIncrementalDrift_H
#define PatternIncrementalDrift_H

#include "defs.h"

class PatternIncrementalDrift : public AuroraDrawable {
  public:
    PatternIncrementalDrift() {
      name = (char *)"Incremental Drift";
    }

    unsigned int drawFrame() {
      uint8_t dim = beatsin8(2, 230, 250);
      effects.DimAll(dim);

      for (int i = 2; i <= MATRIX_WIDTH / 2; i++)
      {
        CRGB color = effects.ColorFromCurrentPalette((i - 2) * (240 / (MATRIX_WIDTH / 2)));

	// This looked good, even on a 64x96 screens
	//uint8_t x = beatcos8((17 - i) * 2, MATRIX_CENTER_X - i, MATRIX_CENTER_X + i);
        //uint8_t y = beatsin8((17 - i) * 2, MATRIX_CENTER_Y - i, MATRIX_CENTER_Y + i);

	// This didn't look so good if the screen wasn't square
        //uint8_t x = beatcos8((MATRIX_CENTER_X + 1 - i) * 2, MATRIX_CENTER_X - i, MATRIX_CENTER_X + i);
        //uint8_t y = beatsin8((MATRIX_CENTER_Y + 1 - i) * 2, MATRIX_CENTER_Y - i, MATRIX_CENTER_Y + i);

	// This looks not bad
        //uint8_t x = beatcos8((mmin(MATRIX_CENTER_X, MATRIX_CENTER_Y) + 1 - i) * 2, MATRIX_CENTER_X - i, MATRIX_CENTER_X + i);
        //uint8_t y = beatsin8((mmin(MATRIX_CENTER_X, MATRIX_CENTER_Y) + 1 - i) * 2, MATRIX_CENTER_Y - i, MATRIX_CENTER_Y + i);

	// This looks better on a non square thing, feels more dense
        uint8_t x = beatcos8((max(MATRIX_CENTER_X, MATRIX_CENTER_Y) + 1 - i) * 2, MATRIX_CENTER_X - i, MATRIX_CENTER_X + i);
        uint8_t y = beatsin8((max(MATRIX_CENTER_X, MATRIX_CENTER_Y) + 1 - i) * 2, MATRIX_CENTER_Y - i, MATRIX_CENTER_Y + i);

        //backgroundLayer.drawPixel(x, y, color);
        matrix->drawPixel(x, y, color);
      }

      return 0;
    }
};

#endif
