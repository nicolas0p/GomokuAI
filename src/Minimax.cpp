
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

Board::Moves get_other_player(const Board::Moves& player)
{
	if(player == Board::FIRSTPLAYER) {
		return Board::SECONDPLAYER;
	}
	return Board::FIRSTPLAYER;
}

/*
 * */
std::pair<int, int> Minimax::get_move(Board board)
{
	return std::get<0>(step(&board, _difficulty, std::greater<int>(), _player));
}

/* Step of the minimax algorithm. It selects the maximum or minimum node of its children and chooses it
 * @param board the board from which it is desired to generate moves of this subtree
 * @param depth the depth that the algorithm should go
 * @param compare function to compare different moves in order to choose the best. Be it greater or lesser.
 * @returns a tuple containing the chosen move of this subtree, the value this move produces in the board, calculated by the heuristic, and its pruning alpha or beta
 * */
std::tuple<std::pair<int, int>, int, int> Minimax::step(Board * board, unsigned int depth, const std::function<bool (const int&, const int&)>& compare, const Board::Moves& player)
{
	//set of moves
	auto moves = _generate_moves(*board);
	std::tuple<std::pair<int, int>, int, int> choice;
	const Board::Moves other_player = get_other_player(player);
	const std::function<bool (const int&, const int&)> other_compare = get_other_compare(compare);
	int alpha = std::numeric_limits<int>::min();
	//bottom of tree
	if (depth <= 1) {
		std::get<0>(choice) = *moves.begin();
		board->insert_move(std::get<0>(choice), player);
		std::get<1>(choice) = _heuristic(*board, _player);
		board->remove_move(std::get<0>(choice));
		//moves.erase(moves.begin());
		for(auto it : moves) {
			board->insert_move(it, player);
			int current = _heuristic(*board, _player);
			if(compare(current, std::get<1>(choice))) {
				std::get<0>(choice) = it;
				std::get<1>(choice) = current;
			}
			board->remove_move(it);
		}
		return choice;
	}
	board->insert_move(*moves.begin(), player);
	choice = step(board, depth - 1, other_compare, other_player);
	board->remove_move(*moves.begin());
	//moves.erase(moves.begin());
	for(auto it : moves) {
		board->insert_move(it, player);
		int current = std::get<1>(step(board, depth - 1, other_compare, other_player));
		if(compare(current, std::get<1>(choice))) {
			std::get<0>(choice) = it;
			std::get<1>(choice) = current;
		}
		board->remove_move(it);
	}
	return choice;
}
