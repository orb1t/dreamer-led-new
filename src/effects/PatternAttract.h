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

//#ifndef PatternAttract_H
#ifndef PatternAttract_H
#define PatternAttract_H


#include "./defs.h"
#include "Attractor.h"


class PatternAttract : public AuroraDrawable {
private:
    const int count = mmin(MX_HEIGHT/3, AVAILABLE_BOID_COUNT);
    Attractor attractor;

public:
    PatternAttract() {
        name = (char *)"Attract";
    }

    void start() {
        int direction = random(0, 2);
        if (direction == 0)
            direction = -1;

        for (int i = 0; i < count; i++) {
	    Boid boid = Boid(MATRIX_CENTER_X - 1, (MX_HEIGHT - 1) - i);
            boid.mass = 1; // random(0.1, 2);
	    // boid.velocity.x = ((float) random(40, 50)) / 100.0;
            boid.velocity.x = ((float) random(20, 80)) / 100.0;
            boid.velocity.x *= direction;
            boid.velocity.y = 0;
            //boid.colorIndex = i * 32;
	    boid.colorIndex = i * (240 / count);
            boids[i] = boid;
            //dim = random(170, 250);
        }
    }

    unsigned int drawFrame() {
        // dim all pixels on the display
        uint8_t dim = beatsin8(2, 170, 250);
        effects.DimAll(dim);

        for (int i = 0; i < count; i++) {
            Boid boid = boids[i];

            PVector force = attractor.attract(boid);
            boid.applyForce(force);

            boid.update();
	    // backgroundLayer.drawPixel(boid.location.x, boid.location.y, effects.ColorFromCurrentPalette(boid.colorIndex));
	    // this writes an out of bounds pixel that looks bad
            //effects.leds[XY_EF(boid.location.x, boid.location.y)] = effects.ColorFromCurrentPalette(boid.colorIndex);
	    // drawPixel takes care of it
	    
	    // drawPixel now accepts native CRGB
	    CRGB color = effects.ColorFromCurrentPalette(boid.colorIndex);
            matrix->drawPixel(boid.location.x, boid.location.y, color);

	    // drawPixel also takes 32bit 0x00RRGGBB (in addition to 16bit RGB565)
	    //uint32_t color32 = color.r*65536+color.g*256+color.b;
            //matrix->drawPixel(boid.location.x, boid.location.y, color32);

            boids[i] = boid;
        }

        return 15;
    }
};

#endif
