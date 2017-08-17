#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include <utility>

#include "Board.h"

class InputComponent {
	public:
		virtual ~InputComponent() {}
		virtual std::pair<int, int> get_move(Board board) = 0;
};

#endif /* INPUTCOMPONENT_H */
