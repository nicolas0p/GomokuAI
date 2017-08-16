#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include <utility>

#include "Board.h"

class InputComponent {
	public:
		InputComponent();
		virtual ~InputComponent();
		virtual std::pair<int, int> get_move(const Board& board) = 0;
};

#endif /* INPUTCOMPONENT_H */
