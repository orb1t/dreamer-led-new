//
// Created by orb1t on 18.12.20.
//

#ifndef INC_201122_212358_ESP32DEV_GLOBALMODES_H
#define INC_201122_212358_ESP32DEV_GLOBALMODES_H


#include "media/GifDrawer.h"
GifDrawer gifDrawer;
//#include "media/JpgDrawer.h"
//JpgDrawer jpgDrawer;
#include "media/PngDrawer.h"
PngDrawer pngDrawer;
#include "media/TextDrawer.h"
TextDrawer textDrawer;
#include "effects/EffectsHelper.h"
EffectsDrawer effectsDrawer;
#include "morphingClock/ClockDrawer.h"
ClockDrawer clockDrawer;
#include "media/FingerDrawer.h"
FingerDrawer fingerDrawer;

#include "games/GamesRunner.h"
GamesRunner gamesRunner;

std::map<std::int8_t , AuroraDrawable*> __MODES_FNCS_;

#endif //INC_201122_212358_ESP32DEV_GLOBALMODES_H
