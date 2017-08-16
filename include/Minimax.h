#ifndef MINIMAX_H
#define MINIMAX_H

#include "InputComponent.h"

class Minimax : public InputComponent {
	public:
		Minimax();
		~Minimax();
		std::pair<int, int> get_move(const Board& board);

};
#endif /* MINIMAX_H */
