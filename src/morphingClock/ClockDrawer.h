
#ifndef ClockDraw_H


#include "Drawable.h"
#include "../matrixHelper.h"

#include "../globals.h"
#include "../helpers.h"

//=== SEGMENTS ===
#include "Digit.h"

Digit digit1(matrix, 0, 12, 8, matrix->Color(0, 255, 0)); //SS_h
Digit digit0(matrix, 0, 12, 1, matrix->Color(0, 255, 0)); //SS_l

Digit digit3(matrix, 0, 0, 0, matrix->Color(255, 0, 0)); //MM_h
Digit digit2(matrix, 0, 7, 0, matrix->Color(255, 0, 0)); //MM_l


Digit digit5(matrix, 0, 0, 9, matrix->Color(0, 0, 255)); //HH_h
Digit digit4(matrix, 0, 7, 9, matrix->Color(0, 0, 255)); //HH_l

#define DEBUG 0

class ClockDrawer : public AuroraDrawable {
  private:
    int hh = 0;
    int mm = 0;
    int ss = 0;
    int prevss = ss;
    int prevhh = hh;
    int prevmm = mm;


    void updateTime(){
        hh = timeinfo.tm_hour;
        mm = timeinfo.tm_min;
        ss = timeinfo.tm_sec;
    }

  public:
    uint16_t theta = 0;
    ClockDrawer() {
      name = (char *)"ClockDraw";
	  _flags.redraw = false;
	  _flags.clear = false;
	  _flags.wait = false;
	  needStart = true;
    }

    void start() {
        getTime();
        updateTime();
        matrix->clear();
        digit1.Draw(ss / 10);
        digit0.Draw(ss % 10);

        digit3.Draw(mm / 10);
        digit2.Draw(mm % 10);

        digit5.Draw(hh / 10);
        digit4.Draw(hh % 10);

        matrix->show();
    };

    unsigned int drawFrame() {
        if ( needStart ) {
            start();
            needStart = false;
        }
	  getTime();
	  updateTime();
        if (ss!=prevss) {
#if DEBUG == 1
            printTime(&timeinfo);
#endif
//            if (ss != prevss) {
                int s0 = ss % 10;
                int s1 = ss / 10;
                if (s0 != digit0.Value()) digit0.Morph(s0);
                if (s1 != digit1.Value()) digit1.Morph(s1);
                //ntpClient.PrintTime();
//            }

            if (mm != prevmm) {
                int m0 = mm % 10;
                int m1 = mm / 10;
                if (m0 != digit2.Value()) digit2.Morph(m0);
                if (m1 != digit3.Value()) digit3.Morph(m1);
                prevmm = mm;
            }

            if (hh != prevhh) {
                int h0 = hh % 10;
                int h1 = hh / 10;
                if (h0 != digit4.Value()) digit4.Morph(h0);
                if (h1 != digit5.Value()) digit5.Morph(h1);
                prevhh = hh;
            }
        }
	  prevss = ss;
        matrix->show();
	  vTaskDelay( 33 / portTICK_PERIOD_MS ); //delay(33);
        yield();
      return 42;
    }

  virtual ~ClockDrawer() {

  }

};

#endif
