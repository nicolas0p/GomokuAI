#ifndef MINIMAX_H
#define MINIMAX_H

#include <functional>
#include <memory>

#include "InputComponent.h"

using std::unique_ptr;

class Minimax : public InputComponent {
	public:
		Minimax(const std::function<int (const Board &)>& heuristic);
		~Minimax();
		std::pair<int, int> get_move(const Board& board);

	private:
		std::function<int (const Board &)> _heuristic;
};
#endif /* MINIMAX_H */
