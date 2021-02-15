#ifndef _GAMESRUNNER_H_
#define _GAMESRUNNER_H_

#define DEBUG 0

#include "Game.h"
#include "Snake.h"
#include "Tetris.h"

//Life life;
Tetris tetris;
Snake snake;




class GamesRunner : public AuroraDrawable {
private:
  std::vector<Game*> items;
  std::map<String, Game*> _items;
  Game* game;

public:
  GamesRunner(){
	name = (char *) "GamesDrawer";
  }

  void setup(){
	items.push_back(&snake);
	_items.insert({snake.name, &snake});
	items.push_back(static_cast<Game *const>(&tetris));
	_items.insert({tetris.name, &tetris});
	_GAME_NAME_ = snake.name;
	game = _items[_GAME_NAME_];
//	_game_pattern = game;

	_flags.redraw = false;
	_flags.clear = false;
	_flags.wait = false;
  };

  char* getConfig(AsyncResponseStream* strm) override {
	return vectorToJsonArray(items, NULL/*(const char*)&"games"*/, strm);
  }
  uint8_t getConfigSize() override {
	return items.size();
  };


  void start(){
//	startGameTimer(gameTimerHandler, game->interval, 1, reinterpret_cast<TimerHandle_t *>(gameTimerHandler));
	game = _items[_GAME_NAME_];
	game->setup();
	game->start();
  };
  unsigned int drawFrame(){
	game->drawFrame();
//	matrix->show();
  };
  void stop(){

  }

  virtual ~GamesRunner() {};

};


#endif
