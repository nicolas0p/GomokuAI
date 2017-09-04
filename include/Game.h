#ifndef GAME_H
#define GAME_H

#include <memory>

#include "Player.h"
#include "Board.h"
#include "UserInterface.h"
#include "Heuristic.h"

using std::unique_ptr;

class Game {
	public:
		Game(Board board, unique_ptr<Player> first_player, unique_ptr<Player> second_player, unique_ptr<UserInterface> user_interface);
		virtual ~Game();
		void start_game();
		void help();
		int points_calculator(Board & _board);

	private:
		Board _board;
		int _n_move;
		unique_ptr<Player> _first_player;
		unique_ptr<Player> _second_player;
		unique_ptr<UserInterface> _user_interface;
		int _line,_col;

};
#endif /* GAME_H */
