
#include "MoveGenerator.h"

std::set<std::pair<int, int>> simple_move_generator(const Board& board)
{
	return board.available_positions();
}
