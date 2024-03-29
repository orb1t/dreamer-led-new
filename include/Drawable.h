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

#ifndef AuroraDrawable_H
#define AuroraDrawable_H


#include "../.pio/libdeps/esp32dev/Framebuffer GFX/Framebuffer_GFX.h"
// #include <ArduinoJson.h>


class AuroraDrawable {
protected:
  Framebuffer_GFX *_framebuffer;
  drawableFlags _flags;
public:
  virtual std::stringstream* getConfigJson(){};

  bool needStart = true;
  uint8_t speedDivider = 10;
public:
  char* name;
  virtual drawableFlags getFlags(){
	return _flags;
  };
  virtual void setFlags(drawableFlags flags){
	_flags = flags;
  };


  virtual bool isRunnable() {
	  return false;
  }

  virtual bool isPlaylist() {
	  return false;
  }

  // a single frame should be drawn as fast as possible, without any delay or blocking
  // return how many millisecond delay is requested before the next call to drawFrame()
  virtual unsigned int drawFrame() {
  // not visible in this file (defined in neomatrix_config.h)
	  //backgroundLayer.fillScreen({ 0, 0, 0 });
	  return 0;
  };
//  virtual void play(const String* name) {};
  virtual void setup(Framebuffer_GFX *framebuffer = NULL) {
    _framebuffer = framebuffer;
  };
  virtual void start() {};
  virtual void stop() {}

  virtual ~AuroraDrawable() {};
};

class Drawable : public AuroraDrawable {
};


#endif
