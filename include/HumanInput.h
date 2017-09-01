#ifndef HUMANINPUT_H
#define HUMANINPUT_H

#include <utility>

#include "InputComponent.h"
#include "Board.h"

class HumanInput : public InputComponent {
	public:
		HumanInput(const Board::Moves& player);
		~HumanInput();
		std::pair<int, int> get_move(Board board);

	private:
		int _line,_col;

};
#endif /* HUMANINPUT_H */
