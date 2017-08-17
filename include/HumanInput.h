#ifndef HUMANINPUT_H
#define HUMANINPUT_H

#include <utility>

#include "InputComponent.h"
#include "Board.h"

class HumanInput : public InputComponent {
	public:
		HumanInput();
		~HumanInput();
		std::pair<int, int> get_move(Board board);

};
#endif /* HUMANINPUT_H */
