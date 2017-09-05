#ifndef PLAYER_H
#define PLAYER_H

#include <utility>
#include <memory>

#include "Board.h"
#include "InputComponent.h"

using std::unique_ptr;

class Player {
	public:
		Player(Board * board, unique_ptr<InputComponent> && input_component, Board::Moves player);
		virtual ~Player();
		virtual std::pair<int, int> make_move();
		void add_points(int p);
		int get_points();
		Board::Moves get_player();

	protected:
		int _point;
		Board * _board;
		unique_ptr<InputComponent> _input_component;
		Board::Moves _player;
};

#endif //PLAYER_H
