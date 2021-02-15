//
// Created by orb1t on 22.12.20.
//

#ifndef _GAMESHELPER_H_
#define _GAMESHELPER_H_

Game* _game_pattern = NULL;

#define DEBUG 1

void gameTimerHandler( TimerHandle_t xTimer ){
#if DEBUG == 1
  Serial.printf("\ngameTimerHandler\n");
#endif
  if ( NULL != _game_pattern ) {
	portENTER_CRITICAL(&_game_pattern->timerMux);
	_game_pattern->gameTimerisReady = true;
	portEXIT_CRITICAL(&_game_pattern->timerMux);
  }
  ESP_LOGI(TAG,"tring tring!!!");
//    snakeRoutine();
//    matrix->show();
}


void startGameTimer(TimerCallbackFunction_t func, uint8_t interval, uint8_t id, TimerHandle_t *_tmr) {
  _tmr = static_cast<TimerHandle_t *>(xTimerCreate("MyTimer", pdMS_TO_TICKS(interval), pdTRUE, (void *) id, func));
  if( xTimerStart(_tmr, 10 ) != pdPASS ) {
	printf("\nTimer start error\n");
  } else
	printf("\nTimer successfully started\n");
}

#endif //INC_201122_212358_ESP32DEV_GAMESHELPER_H
