
#include <set>
#include <limits>

#include "Minimax.h"

Minimax::Minimax(const std::function<int (const Board &, const Board::Moves &)>& heuristic, const std::function<std::set<std::pair<int, int>> (const Board&)>& move_generator, unsigned int difficulty, const Board::Moves& player) :
	InputComponent(player),
	_heuristic(heuristic),
	_generate_moves(move_generator),
	_difficulty(difficulty)
{}

static const std::function<bool (const int&, const int&)> gt = std::greater<int>();
static const std::function<bool (const int&, const int&)> ls = std::less<int>();

std::function<bool (const int&, const int&)> get_other_compare(const std::function<bool (const int&, const int&)>& function)
{
	if(function.target<bool (const int&, const int&)>() == gt.target<bool (const int&, const int&)>()) {
		return ls;
	}
	return gt;
}

/*
 * */
std::pair<int, int> Minimax::get_move(Board board)
{
	return step(&board, _difficulty, std::greater<int>(), _player, std::numeric_limits<int>::max()).first;
}

/**
 * Step of the minimax algorithm. It selects the maximum or minimum node of its children and chooses it.
 * @param board the board from which it is desired to generate moves of this subtree.
 * @param depth the depth that the algorithm should go.
 * @param compare function to compare different moves in order to choose the best. Be it greater or lesser.
 * @param pruning_factor contains alpha or beta for pruning. Max steps receive beta and min steps receive alpha from their parent.
 * @returns a tuple containing the chosen move of this subtree and the value this move produces in the board, calculated by the heuristic.
 * */
std::pair<std::pair<int, int>, int> Minimax::step(Board * board, unsigned int depth, const std::function<bool (const int&, const int&)>& compare, const Board::Moves& player, int pruning_factor)
{
	//set of moves
	auto moves = _generate_moves(*board);
	std::pair<std::pair<int, int>, int> choice = {{0,0}, pruning_factor};
	const Board::Moves other_player = get_other_player(player);
	const std::function<bool (const int&, const int&)> other_compare = get_other_compare(compare);
	//bottom of tree
	if (depth <= 1) {
		choice.first = *moves.begin();
		board->insert_move(choice.first, player);
		choice.second = _heuristic(*board, _player);
		board->remove_move(choice.first);
		moves.erase(moves.begin());
		for(auto it : moves) {
			board->insert_move(it, player);
			int current = _heuristic(*board, _player);
			if(compare(current, choice.second)) {
				choice.first = it;
				choice.second = current;
				if(compare(current,pruning_factor)) {
					board->remove_move(it);
					return choice; //pruned!
				}
			}
			board->remove_move(it);
		}
		return choice;
	}
	board->insert_move(*moves.begin(), player);
	choice = step(board, depth - 1, other_compare, other_player, choice.second);
	board->remove_move(*moves.begin());
	moves.erase(moves.begin());
	for(auto it : moves) {
		board->insert_move(it, player);
		int current = step(board, depth - 1, other_compare, other_player, choice.second).second;
		if(compare(current, choice.second)) {
			choice.first = it;
			choice.second = current;
			if(compare(current,pruning_factor)) {
				board->remove_move(it);
				return choice; //pruned!
			}
		}
		board->remove_move(it);
	}
	return choice;
}
