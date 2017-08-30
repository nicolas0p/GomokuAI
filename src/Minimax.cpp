
#include <set>

#include "Minimax.h"

Minimax::Minimax(const std::function<int (const Board &, const Board::Moves &)>& heuristic, const std::function<std::set<std::pair<int, int>> (const Board&)>& move_generator, unsigned int difficulty, const Board::Moves& player) :
	InputComponent(player),
	_heuristic(heuristic),
	_generate_moves(move_generator),
	_difficulty(difficulty)
{}

std::pair<int, int> Minimax::get_move(Board board)
{
	return std::get<0>(mmax(&board, _difficulty));
}

std::tuple<std::pair<int, int>, int> Minimax::mmax(Board * board, unsigned int depth)
{
	//set of moves
	auto moves = _generate_moves(*board);
	std::tuple<std::pair<int, int>, int> choice;
	//bottom of tree
	if (depth <= 1) {
		std::get<0>(choice) = *moves.begin();
		board->insert_move(std::get<0>(choice), _player);
		std::get<1>(choice) = _heuristic(*board, _player);
		board->remove_move(std::get<0>(choice));
		moves.erase(moves.begin());
		for(auto it : moves) {
			board->insert_move(it,_player);
			int current = _heuristic(*board, _player);
			if(current > std::get<1>(choice)) {
				std::get<0>(choice) = it;
				std::get<1>(choice) = current;
			}
			board->remove_move(it);
		}
		return choice;
	}

	board->insert_move(*moves.begin(),_player);
	choice = mmin(board, depth - 1);
	board->remove_move(*moves.begin());
	moves.erase(moves.begin());
	for(auto it : moves) {
		board->insert_move(it,_player);
		int current = std::get<1>(mmin(board, depth - 1));
		if(current > std::get<1>(choice)) {
			std::get<0>(choice) = it;
			std::get<1>(choice) = current;
		}
		board->remove_move(it);
	}
	return choice;
}

std::tuple<std::pair<int, int>, int> Minimax::mmin(Board * board, unsigned int depth)
{
	//set of moves
	std::set<std::pair<int, int>> moves = _generate_moves(*board);
	std::tuple<std::pair<int, int>, int> choice;
	//bottom of tree
	if (depth <= 1) {
		std::get<0>(choice) = *moves.begin();
		board->insert_move(std::get<0>(choice), _player);
		std::get<1>(choice) = _heuristic(*board, _player);
		board->remove_move(std::get<0>(choice));
		moves.erase(moves.begin());
		for(auto it : moves) {
			board->insert_move(it, _player);
			int current = _heuristic(*board, _player);
			if(current < std::get<1>(choice)) {
				std::get<0>(choice) = it;
				std::get<1>(choice) = current;
			}
			board->remove_move(it);
		}
		return choice;
	}
	board->insert_move(*moves.begin(), _player);
	choice = mmin(board, depth - 1);
	board->remove_move(*moves.begin());
	moves.erase(moves.begin());
	for(auto it : moves) {
		board->insert_move(it, _player);
		int current = std::get<1>(mmin(board, depth - 1));
		if(current < std::get<1>(choice)) {
			std::get<0>(choice) = it;
			std::get<1>(choice) = current;
		}
		board->remove_move(it);
	}
	return choice;
}
