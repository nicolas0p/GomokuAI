
#include "Minimax.h"

Minimax::Minimax(const std::function<int (const Board &)>& heuristic) :
	_heuristic(heuristic)
{}
Minimax::~Minimax() {}
std::pair<int, int> Minimax::get_move(const Board& board) {}
