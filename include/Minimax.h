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
		std::pair<int, int> get_move(Board board);

	enum Diff {
		EASY = 1,
		MEDIUM = 2,
		HARD = 4
	};

	private:
		//returns a tuple<move, value of move>
		//value of move comes from the minimax execution or the heuristic if at leaf nodes
		std::tuple<std::pair<int, int>, int> mmax(Board * board, unsigned int depth);
		std::tuple<std::pair<int, int>, int> mmin(Board * board, unsigned int depth);

		std::function<int (const Board &)> _heuristic;
		Diff difficulty;
};
#endif /* MINIMAX_H */
