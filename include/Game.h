#ifndef GAME_H
#define GAME_H

#include <memory>

#include "Player.h"
#include "Board.h"

using std::unique_ptr;

class Game {
	public:
		Game(Board board, unique_ptr<Player> && first_player, unique_ptr<Player> && second_player);
		virtual ~Game();
		void start_game();

	private:
		Board _board;
		unique_ptr<Player> _first_player;
		unique_ptr<Player> _second_player;

};
#endif /* GAME_H */
