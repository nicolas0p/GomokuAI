#ifndef MINIMAX_H
#define MINIMAX_H

#include <functional>
#include <memory>
#include <set>

#include "InputComponent.h"
#include "Board.h"

using std::unique_ptr;

class Minimax : public InputComponent {
	public:
		Minimax(const std::function<int (const Board &, const Board::Moves &)>& heuristic, const std::function<std::set<std::pair<int, int>> (const Board&)>& move_generator, unsigned int difficulty, Board::Moves player);
		std::pair<int, int> get_move(Board board, Board::Moves player);

		//difficulty is set by the depth of the minimax algorithm

	private:
		//returns a tuple<move, value of move>
		//value of move comes from the minimax execution or the heuristic if at leaf nodes
		std::tuple<std::pair<int, int>, int> mmax(Board * board, unsigned int depth);
		std::tuple<std::pair<int, int>, int> mmin(Board * board, unsigned int depth);

		std::function<int (const Board &, const Board::Moves &)> _heuristic;
		std::function<std::set<std::pair<int, int>> (const Board& board)> _generate_moves;
		unsigned int _difficulty;
		Board::Moves _player;

};
#endif /* MINIMAX_H */
