#ifndef _EFFECTSHELPER_H_
#define _EFFECTSHELPER_H_

#define DEBUG 0

#include <jsonHelper.h>
#include "../globals.h"

//#include "PatternPongClock.h"
#include "PatternNoiseSmearing.h"
//PatternNoiseSmearing noiseSmearing;
//#include "PatternAnalogClock.h"
#include "PatternMunch.h"
#include "PatternElectricMandala.h"
//#include "PatternSpin.h"
#include "PatternSimplexNoise.h"
#include "PatternInfinity.h"
#include "PatternPlasma.h"
//#include "PatternSnake.h"
//#include "PatternInvaders.h"
//#include "PatternLife.h"
//#include "PatternMaze.h"
//#include "PatternPulse.h"
//#include "PatternRainbowSmoke.h"
//#include "PatternSpark.h"

PatternRainbowFlag rainbowFlag;
PatternPaletteSmear paletteSmear;
PatternMultipleStream multipleStream;
PatternMultipleStream2 multipleStream2;
PatternMultipleStream3 multipleStream3;
PatternMultipleStream4 multipleStream4;
PatternMultipleStream5 multipleStream5;
PatternMultipleStream8 multipleStream8;
//PatternAnalogClock analogClock;
PatternMunch munch;
PatternElectricMandala electricMandala;
//PatternSpin spin;
PatternSimplexNoise simplexNoise;
PatternInfinity pinfinity;
PatternPlasma plasma;
//PatternInvadersSmall invadersSmall;
//PatternInvadersMedium invadersMedium;
//PatternInvadersLarge invadersLarge;
//PatternSnake psnake;
//PatternLife plife;
//PatternMaze maze;
//PatternPulse pulse;
//PatternRainbowSmoke rainbowSmoke;
//PatternSpark spark;


#include "./PatternAttract.h"
PatternAttract attract;
#include "./PatternBounce.h"
PatternBounce bounce;
#include "./PatternCube.h"
PatternCube cube;
#include "./PatternFlock.h"
PatternFlock Aflock;
#include "./PatternFlowField.h"
PatternFlowField flowfield;
#include "./PatternIncrementalDrift.h"
PatternIncrementalDrift incrementaldrift;
#include "./PatternIncrementalDrift2.h"
PatternIncrementalDrift2 incrementaldrift2;
#include "./PatternPendulumWave.h"
PatternPendulumWave pendulumwave;
#include "./PatternRadar.h"
PatternRadar radar;
#include "./PatternSpiral.h"
PatternSpiral spiral;
#include "./PatternSpiro.h"
PatternSpiro spiro;
#include "./PatternSwirl.h"
PatternSwirl swirl;
#include "./PatternWave.h"
PatternWave wave;

#include "./PatternFire.h"
PatternFire fire;

//#include "PatternRadarDraw.h"
//AuroraDrawable* radarDraw();
//#include "RunningTextDraw.h"
//AuroraDrawable* textDraw();


//#define DEBUG 1


class EffectsDrawer : public AuroraDrawable {
private:
//  getConfig();
  AuroraDrawable *pattern;
//  bool new_pattern = true;
  std::vector<AuroraDrawable*> items;
public:
//  char* name;
//  drawableFlags flags;

public:
  void dumpEffects(){
	Serial.println("Got following patterns:");
	for ( int item = 0; item <=  items.size() - 1; item++ ){
	  pattern = items[item];
	  Serial.print(item);
	  Serial.print(": ");
	  Serial.println(pattern->name);
	}
  }
  uint16_t theta = 0;
  EffectsDrawer() {
	name = (char *) "EffectsDrawer";
  }
  void setup() {
	effects.leds = leds;
	items.push_back((AuroraDrawable *)&cube);
	items.push_back((AuroraDrawable *)&radar);
	items.push_back((AuroraDrawable *)&attract);
	items.push_back((AuroraDrawable *)&bounce);
	items.push_back((AuroraDrawable *)&Aflock);
	items.push_back((AuroraDrawable *)&flowfield);
	items.push_back((AuroraDrawable *)&incrementaldrift);
	items.push_back((AuroraDrawable *)&incrementaldrift2);
	items.push_back((AuroraDrawable *)&pendulumwave);
	items.push_back((AuroraDrawable *)&spiral);
	items.push_back((AuroraDrawable *)&spiro);
	items.push_back((AuroraDrawable *)&swirl);
	items.push_back((AuroraDrawable *)&wave);
	items.push_back((AuroraDrawable *)&fire);

	items.push_back((AuroraDrawable *)&rainbowFlag);
	items.push_back((AuroraDrawable *)&paletteSmear);
	items.push_back((AuroraDrawable *)&multipleStream);
	items.push_back((AuroraDrawable *)&multipleStream2);
	items.push_back((AuroraDrawable *)&multipleStream3);
	items.push_back((AuroraDrawable *)&multipleStream4);
	items.push_back((AuroraDrawable *)&multipleStream5);
	items.push_back((AuroraDrawable *)&multipleStream8);
//	items.push_back((AuroraDrawable *)& analogClock);
	items.push_back((AuroraDrawable *)&munch);
	items.push_back((AuroraDrawable *)&electricMandala);
//	items.push_back((AuroraDrawable *)&spin);
	items.push_back((AuroraDrawable *)&simplexNoise);
	items.push_back((AuroraDrawable *)&pinfinity);
	items.push_back((AuroraDrawable *)&plasma);
//	items.push_back((AuroraDrawable *)&invadersSmall);
//	items.push_back((AuroraDrawable *)& invadersMedium);
//	items.push_back((AuroraDrawable *)&invadersLarge);
//	items.push_back((AuroraDrawable *)&psnake);
//	items.push_back((AuroraDrawable *)&plife);
//	items.push_back((AuroraDrawable *)&maze);
//	items.push_back((AuroraDrawable *)&pulse);
//	items.push_back((AuroraDrawable *)&rainbowSmoke);
//	items.push_back((AuroraDrawable *)& spark);

	effects.Setup();
#if DEBUG == 1
	dumpEffects();
#endif

	_flags.redraw = false;
	_flags.clear = false;
	_flags.wait = false;
  }
  char* getConfig( AsyncResponseStream* strm ) override {
	return vectorToJsonArray(items, NULL/*(const char*)&"effects"*/, strm);
  }
  uint8_t getConfigSize() override {
	return items.size();
  };

  void start() {
	if (needStart) {
	  if (_EFFECT_ID >= items.size())
		_EFFECT_ID = random(0, items.size()-1);

	  pattern = items[_EFFECT_ID];
	  _flags = pattern->getFlags();
	  pattern->start();
	  Serial.print("Switching to pattern #");
	  Serial.print(_EFFECT_ID);
	  Serial.print(": ");
	  Serial.println(pattern->name);
	  matrix->clear();
	}
  };

  unsigned int drawFrame() {
	if ( needStart ) {
	  start();
	  needStart = false;
	}

	if ( ! __MODE_STOPING_FLAG ) {
	  uint16_t sleepFor = pattern->drawFrame();
	  matrix->show();
	  vTaskDelay(__SPEED__ / speedDivider / portTICK_PERIOD_MS );
	}
	return 42;
  }

  virtual ~EffectsDrawer() {

  }


};






#endif
