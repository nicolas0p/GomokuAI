
#include "Minimax.h"

Minimax::Minimax(const std::function<int (const Board &)>& heuristic) :
	_heuristic(heuristic)
{}

Minimax::~Minimax() {}

std::pair<int, int> Minimax::get_move(Board board)
{
	return std::get<0>(mmax(&board, difficulty));
}

std::tuple<std::pair<int, int>, int> Minimax::mmax(Board * board, unsigned int depth)
{
}

std::tuple<std::pair<int, int>, int> Minimax::mmin(Board * board, unsigned int depth)
{
}
