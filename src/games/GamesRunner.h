#ifndef _GAMESRUNNER_H_
#define _GAMESRUNNER_H_

#define DEBUG 0

#include "Game.h"
#include "Snake.h"
#include "Tetris.h"

#include <map>
#include  <utility>

//Life life;
Tetris tetris;
Snake snake;




class GamesRunner : public AuroraDrawable {
private:
  // std::map<String, Game*> _items = {std::pair<String, Game*>{String(snake.name), &snake}, std::pair<String, Game*>{String(tetris.name), &tetris}};
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

std::stringstream* getConfigJson() override {
	std::stringstream* res = new std::stringstream();
	// Serial.printf("\nMedia Ext: %s, path: %s, conunt %d:\n", info->ext, info->path, info->arr.size());
	*res << "\"games\":[";

for (auto itm : items) {
        *res << "\"" << itm->name  << ( items[items.size()-1] == itm ? "\"" : "\"," );
    }

  // for ( const auto &item : _items ) {
  //   *res << "\"" << item.first << ( std::prev(_items.end())->first == item.first ? "\"" : "\"," );
  // }

	// for ( int i = 0; i < _items.size(); i++ ){
	// //   Serial.printf("\nItem[%d]: %s\n", i, info->arr[i]->c_str());
	// 	*res << images.path << "/" << images.arr[i]->c_str() << images.ext << ( i == images.arr.size()-1 ? "\"," : "\"" );
	// }
	*res << "]";
	return res;
  }

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
