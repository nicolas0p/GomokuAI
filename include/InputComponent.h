#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include <utility>

#include "Board.h"

class InputComponent {
	public:
		InputComponent(const Board::Moves& player) : _player{player} {}
		virtual ~InputComponent() {}
		virtual std::pair<int, int> get_move(Board board) = 0;

	protected:
		Board::Moves _player;

};

#endif /* INPUTCOMPONENT_H */
