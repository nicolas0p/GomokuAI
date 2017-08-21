#ifndef HUMANINPUT_H
#define HUMANINPUT_H

#include <utility>

#include "InputComponent.h"
#include "Board.h"

class HumanInput : public InputComponent {
	public:
		HumanInput();
		~HumanInput();
		std::pair<int, int> get_move(Board board, Board::Moves player);

	private:
		int _line,_col;

};
#endif /* HUMANINPUT_H */
