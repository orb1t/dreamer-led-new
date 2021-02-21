#ifndef _EFFECTSHELPER_H_
#define _EFFECTSHELPER_H_

#define DEBUG 0

#include <jsonHelper.h>
#include "../globals.h"


#include "./PatternRadar.h"
PatternRadar radar;
#include "./PatternCube.h"
PatternCube cube;
#include "./PatternAttract.h"
PatternAttract attract;
#include "./PatternBounce.h"
PatternBounce bounce;
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
#include "./PatternSpiral.h"
PatternSpiral spiral;
#include "./PatternSpiro.h"
PatternSpiro spiro;
#include "./PatternSwirl.h"
PatternSwirl swirl;
#include "./PatternWave.h"
PatternWave wave;
#include "PatternMunch.h"
PatternMunch munch;
#include "PatternPlasma.h"
PatternPlasma plasma;



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
#if DEBUG == 1
  void dumpEffects(){
	Serial.println("Got following patterns:");
	for ( int item = 0; item <=  items.size() - 1; item++ ){
	  pattern = items[item];
	  Serial.print(item);
	  Serial.print(": ");
	  Serial.println(pattern->name);
	}
  }
#endif

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
	items.push_back((AuroraDrawable *)&munch);
	items.push_back((AuroraDrawable *)&plasma);

	effects.Setup();
#if DEBUG == 1
	dumpEffects();
#endif

	_flags.redraw = false;
	_flags.clear = false;
	_flags.wait = false;
  }

  std::stringstream* getConfigJson() override {
	std::stringstream* res = new std::stringstream();
	// Serial.printf("\nMedia Ext: %s, path: %s, conunt %d:\n", info->ext, info->path, info->arr.size());
	*res << "\"effects\":[";
	for ( int i = 0; i < items.size(); i++ ){
	//   Serial.printf("\nItem[%d]: %s\n", i, info->arr[i]->c_str());
		*res << "\"" << items[i]->name << ( i == items.size()-1 ? "\"" : "\"," );
	}
	*res << "]";
	
	return res;
  }

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
