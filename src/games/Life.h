//
// Created by orb1t on 22.11.20.
//

#ifndef LIFE_H
#define LIFE_H

#define DEBUG 0


#include "../helpers.h"
#include "Game.h"
#include "matrixHelper.h"

#define __W 3
#define __H 3

class Life : public Game {
private:
    int8_t _w, _h;
    uint8_t map[__W][__H];

public:
    Life(): Game() {
        name = (char *)"Life";
        interval =200;
        _w=__W;
        _h=__H;

    }

    void newGame() {
	  matrix->clear();
	  matrixleds[XY_EF( 0, 1 )] = (CRGB)GLOBAL_COLOR_1;
	  matrixleds[XY_EF( 1, 1 )] = (CRGB)GLOBAL_COLOR_1;
	  matrixleds[XY_EF( 2, 1 )] = (CRGB)GLOBAL_COLOR_1;
	  matrix->show();
    }


    void start() {
        matrix->clear();
//        loadingFlag = false;
        newGame();
//        gamemodeFlag = true;
//        __CURRENT_MODE = __MODE_GAME;

        portENTER_CRITICAL(&timerMux);
        gameTimerisReady = false;
        portEXIT_CRITICAL(&timerMux);
#if DEBUG == 1
        Serial.printf("\nstarted\n");
#endif
    }

    uint8_t getNeighbours(uint8_t px, uint8_t py){
        uint8_t res = 0;
        for ( int8_t y = py-1; y <= py+1; y++ ) {
            for (int8_t x = px-1; x <= px+1; x++) {
                if ( ( x >= 0 ) && ( y >= 0 )  ) { // GLOBAL_COLOR_1
//                    if ( ( x != px  ) && ( y != py ) ) {
                        CRGB _col = getPixColorXYCRGB(x, y);
#if DEBUG == 1
                        Serial.printf("\neffects.leds[%d, %d]: ", x, y);
                        dumpCRGB(_col);
                        Serial.println();
#endif
//                    if ( effects.leds[XY_EF(x, y)] != (CRGB)CRGB::Black ){
                        if (_col != (CRGB) CRGB::Black) {
                            res++;
                        }
//                    }
                }
            }
        }
        if ( getPixColorXYCRGB(px, py) == (CRGB)GLOBAL_COLOR_1)
            res--;
#if DEBUG == 1
        Serial.printf("getNeighbours(%d,%d): %d\n\n", px, py, res);
#endif
        return res;
    }

    unsigned int drawFrame() {
#if DEBUG == 1
        Serial.printf("\ndrawFrame()\n");
#endif

        if (gameTimerisReady) {
#if DEBUG == 1
            Serial.printf("\ngameTimerisReady\n");
#endif
            for ( uint8_t y = 0; y < _h; y++ )
                for (uint8_t x = 0; x < _w; x++)
                    map[x][y] = 0;


            for ( uint8_t y = 0; y < _h; y++ ) {
                for (uint8_t x = 0; x < _w; x++) {
                    CRGB currentPixel = getPixColorXYCRGB(x,y);
                    uint8_t neighbours = getNeighbours(x, y);

                    if ( ( currentPixel == (CRGB)CRGB::Black ) && ( ( neighbours > 2 ) || ( neighbours < 3 ) )){
                        map[x][y] = 1;
                    }
                    if ( ( currentPixel == (CRGB)GLOBAL_COLOR_1 ) && (  neighbours == 3 ) ) {
                        map[x][y] = 1;
                    }
//                    if ( ( neighbours < 2) || ( neighbours > 3) ){
//                        map[x][y] = 0;
//                    }
                }
            }

            for ( uint8_t y = 0; y < _h; y++ )
                for (uint8_t x = 0; x < _w; x++)
                    if ( map[x][y] > 0 )
					  matrixleds[XY_EF(x, y)] = (CRGB)GLOBAL_COLOR_1;
                    else
					  matrixleds[XY_EF(x, y)] = (CRGB)CRGB::Black;

//            effects.leds[XY_EF(headX, headY)] = GLOBAL_COLOR_1;
            matrix->show();

            portENTER_CRITICAL(&timerMux);
            gameTimerisReady = false;
            portEXIT_CRITICAL(&timerMux);

        }

        return 42;
    }


};


#endif //INC_201122_212358_ESP32DEV_SNAKE_H
