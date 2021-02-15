//
// Created by orb1t on 22.11.20.
//

#ifndef INC_201122_212358_ESP32DEV_SNAKE_H
#define INC_201122_212358_ESP32DEV_SNAKE_H


#include "../globals.h"
#include "./Game.h"
#include "./GamesHelper.h"
#include "../matrixHelper.h"

#define DEBUG 0

#define START_LENGTH 4    // начальная длина змейки
#define MAX_LENGTH 80     // максимальная длина змейки

void buttonsTick();
void newGame();
void snakeDemo();



class Snake : public Game {
private:
    int8_t vectorX, vectorY;
    int8_t headX, headY, buttX, buttY;
    int8_t appleX, appleY;
    boolean apple_flag, missDelete = false;



    int8_t buttVector[MAX_LENGTH];
    int snakeLength;
    boolean butt_flag, pizdetc;




public:
    Snake(): Game() {
	  name = (char *)"Snake";
	  interval =200;
	  gameDemo = false;
	  Snake::timerMux = portMUX_INITIALIZER_UNLOCKED;
	  Snake::gameTimerisReady = false;
    }

  static void snakeTimerHandler( TimerHandle_t xTimer ){
#if DEBUG == 1
	Serial.printf("\ngameTimerHandler\n");
#endif
//	if ( NULL != _game_pattern ) {
	portENTER_CRITICAL(&Snake::timerMux);
	Snake::gameTimerisReady = true;
	portEXIT_CRITICAL(&Snake::timerMux);
//	}
	ESP_LOGI(TAG,"tring tring!!!");
//    snakeRoutine();
//    matrix->show();
  }

    void newGame() {
        matrix->clear();
        // свежее зерно для генератора случайных чисел
        randomSeed(millis());

        // длина из настроек, начинаем в середине экрана, бла-бла-бла
        snakeLength = START_LENGTH;
        headX = MX_WIDTH / 2;
        headY = MX_HEIGHT / 2;
        buttY = headY;

        vectorX = 1;  // начальный вектор движения задаётся вот здесь
        vectorY = 0;
        _GAME_BUTTONS_ = 4;

        // первоначальная отрисовка змейки и забивка массива векторов для хвоста
        for (byte i = 0; i < snakeLength; i++) {
//    matrix->drawPixel(headX - i, headY, (uint16_t)GLOBAL_COLOR_1);
		  	matrixleds[XY_EF(headX - i, headY)] = GLOBAL_COLOR_1;
            buttVector[i] = 0;
        }
        matrix->show();
        buttX = headX - snakeLength;   // координата хвоста как голова - длина
        missDelete = false;
        apple_flag = false;
    }


    void start() {
//        matrix->clear();
//        loadingFlag = false;
        newGame();
//        gamemodeFlag = true;
//        __CURRENT_MODE = __MODE_GAME;

        portENTER_CRITICAL(&Snake::timerMux);
	  	Snake::gameTimerisReady = false;
        portEXIT_CRITICAL(&Snake::timerMux);

	  if ( Snake::tmr != NULL ) {
		xTimerStop(Snake::tmr, pdMS_TO_TICKS(interval));
	  }
	  vTaskDelay(interval*2);
	  startGameTimer(Snake::snakeTimerHandler, interval, 1, &Snake::tmr);
//	  startGameTimer
#if DEBUG == 1
        Serial.printf("\nstarted\n");
#endif
    }


    void buttonsTick() {
        if (checkButtons()) {
            if (_GAME_BUTTONS_ == 3) {   // кнопка нажата
                vectorX = -1;
                vectorY = 0;
                _GAME_BUTTONS_ = 4;
            }
            if (_GAME_BUTTONS_ == 1) {   // кнопка нажата
                vectorX = 1;
                vectorY = 0;
                _GAME_BUTTONS_ = 4;
            }
            if (_GAME_BUTTONS_ == 0) {   // кнопка нажата
                vectorY = 1;
                vectorX = 0;
                _GAME_BUTTONS_ = 4;
            }
            if (_GAME_BUTTONS_ == 2) {   // кнопка нажата
                vectorY = -1;
                vectorX = 0;
                _GAME_BUTTONS_ = 4;
            }
        }
    }

//    boolean checkButtons() {
//#if DEBUG == 1
//        Serial.printf("\ncheckButtons()\n");
//#endif
//        if (_GAME_BUTTONS_ != 4) return true;
//        return false;
//    }

    void snakeDemo() {
#if DEBUG == 1
        Serial.printf("\nsnakeDemo()\n");
#endif
        // смещение головы змеи
        int8_t nextX = headX + vectorX;
        int8_t nextY = headY + vectorY;

        // ищем яблоко
        if (headX == appleX) {                // яблоко на одной линии по вертикали
            if (headY < appleY) _GAME_BUTTONS_ = 0;
            if (headY > appleY) _GAME_BUTTONS_ = 2;
        }
        if (headY == appleY) {                // яблоко на одной линии по горизонтали
            if (headX < appleX) _GAME_BUTTONS_ = 1;
            if (headX > appleX) _GAME_BUTTONS_ = 3;
        }

        if (getPixColorXYCRGB(nextX, nextY) == (CRGB)GLOBAL_COLOR_1) {   // проверка на столкновение с собой
            // поворачиваем налево
            if (vectorX > 0) _GAME_BUTTONS_ = 0;
            if (vectorX < 0) _GAME_BUTTONS_ = 2;
            if (vectorY > 0) _GAME_BUTTONS_ = 3;
            if (vectorY < 0) _GAME_BUTTONS_ = 1;
            return;
        }

        if (nextX < 0 || nextX > MX_WIDTH - 1 || nextY < 0        // проверка на столкновение со стеной
            || nextY > MX_HEIGHT - 1) {

            // поворачиваем направо в обычном случае или налево в углу
            if (vectorX > 0) _GAME_BUTTONS_ = 2;
            if (vectorX > 0 && headY == 0) _GAME_BUTTONS_ = 0;

            if (vectorX < 0 && headY == MX_HEIGHT - 1) _GAME_BUTTONS_ = 2;
            if (vectorX < 0) _GAME_BUTTONS_ = 0;

            if (vectorY > 0) _GAME_BUTTONS_ = 1;
            if (vectorY > 0 && headX == MX_WIDTH - 1) _GAME_BUTTONS_ = 3;

            if (vectorY < 0 && headX == 0) _GAME_BUTTONS_ = 1;
            if (vectorY < 0) _GAME_BUTTONS_ = 3;
        }
    }

    
    unsigned int drawFrame() {
#if DEBUG == 1
        Serial.printf("\ndrawFrame()\n");
#endif
        buttonsTick();
//	  	drawPixelMask();

        if (Snake::gameTimerisReady) {
#if DEBUG == 1
            Serial.printf("\ngameTimerisReady\n");
#endif
            // БЛОК ГЕНЕРАЦИИ ЯБЛОКА
            while (!apple_flag) {                         // пока яблоко не создано
                appleX = random(0, MX_WIDTH);                  // взять случайные координаты
                appleY = random(0, MX_HEIGHT);

                // проверить, не совпадает ли координата с телом змеи
//        uint16_t xy = XY_EF(appleX, appleY);
//        effects.leds[xy] = GLOBAL_COLOR_2;
                CRGB _colApple = getPixColorXYCRGB(appleX, appleY);
                if (_colApple == (CRGB)CRGB::Black) {
//        if (_col == CRGB::Black) {
                    apple_flag = true;                        // если не совпадает, считаем что яблоко создано
//        matrix->drawPixel(appleX, appleY, (uint16_t)GLOBAL_COLOR_2 );        // и рисуем
//				  	matrixleds[XY_EF(appleX, appleY)] = GLOBAL_COLOR_2;
				  leds[ XY_EF(appleX, appleY)] = GLOBAL_COLOR_2;
                    matrix->show();
                }
            }

            // запоминаем, куда повернули голову
            // 0 - право, 1 - лево, 2 - вверх, 3 - вниз
            if (vectorX > 0) buttVector[snakeLength] = 0;
            else if (vectorX < 0) buttVector[snakeLength] = 1;
            if (vectorY > 0) buttVector[snakeLength] = 2;
            else if (vectorY < 0) buttVector[snakeLength] = 3;

            // смещение головы змеи
            headX += vectorX;
            headY += vectorY;

            if (headX < 0 || headX > MX_WIDTH - 1 || headY < 0 || headY > MX_HEIGHT - 1) { // если вышла за границы поля
                pizdetc = true;
            }

            if (!pizdetc) {
                // проверка на pizdetc
                if (getPixColorXYCRGB(headX, headY) != (CRGB)CRGB::Black && getPixColorXYCRGB(headX, headY) != (CRGB)GLOBAL_COLOR_2) {   // если змея врезалась во что то, но не в яблоко
                    pizdetc = true;                           // флаг на отработку
                }

                // БЛОК ОТРАБОТКИ ПОЕДАНИЯ ЯБЛОКА
                if (!pizdetc && ( getPixColorXYCRGB(headX, headY) == (CRGB)GLOBAL_COLOR_2) ) { // если попали головой в яблоко
                    apple_flag = false;                       // флаг что яблока больше нет
                    snakeLength++;                            // увеличить длину змеи
                    buttVector[snakeLength] = 4;              // запоминаем, что надо будет не стирать хвост
                }

                // вычисляем координату хвоста (чтобы стереть) по массиву вектора
                switch (buttVector[0]) {
                    case 0: buttX += 1;
                        break;
                    case 1: buttX -= 1;
                        break;
                    case 2: buttY += 1;
                        break;
                    case 3: buttY -= 1;
                        break;
                    case 4: missDelete = true;  // 4 значит не стирать!
                        break;
                }

                // смещаем весь массив векторов хвоста ВЛЕВО
                for (byte i = 0; i < snakeLength; i++) {
                    buttVector[i] = buttVector[i + 1];
                }

                // если змея не в процессе роста, закрасить бывший хвост чёрным
                if (!missDelete) {
//        matrix->drawPixel(buttX, buttY, (uint16_t)0x000000);
				  leds[ XY_EF(buttX, buttY)] = CRGB::Black;
				  matrix->show();
                }
                else missDelete = false;

                // рисуем голову змеи в новом положении
//      matrix->drawPixel(headX, headY, (uint16_t)GLOBAL_COLOR_1);
			  	leds[ XY_EF(headX, headY)] = GLOBAL_COLOR_1;
                matrix->show();
            }
            if (gameDemo) {
                snakeDemo();
            }
            portENTER_CRITICAL(&Snake::timerMux);
		  	Snake::gameTimerisReady = false;
            portEXIT_CRITICAL(&Snake::timerMux);

        }

        // если он настал
        if (pizdetc) {
            pizdetc = false;

            // ну в общем плавно моргнуть, типо змейке "больно"
            for (byte bright = 0; bright < 15; bright++) {
                matrix->setBrightness(bright);
                for (int i = 0; i < NUM_LEDS; i++) {
//        leds[i] = CRGB::Red;
//				  matrixleds[i] = CRGB::Red;
				  leds[i] = CRGB::Red;
                }
                matrix->show();
			  vTaskDelay( 10 / portTICK_PERIOD_MS ); //delay(10);
            }
		  	vTaskDelay( 100 / portTICK_PERIOD_MS ); //delay(100);
            matrix->clear();
		  	matrix->show();
		  	matrix->setBrightness(BRIGHTNESS);
            displayScore(snakeLength - START_LENGTH);
		  vTaskDelay( 1000 / portTICK_PERIOD_MS ); //delay(1000);
            matrix->clear();
		  	matrix->show();
            newGame();                          // миша, всё ху.я, давай по новой
        }
        return 15;
    }
};


#endif //INC_201122_212358_ESP32DEV_SNAKE_H
