#include "Game.h"

Game::Game(Board board, unique_ptr<Player> && first_player, unique_ptr<Player> && second_player) :
	_board(board),
	_first_player(std::move(first_player)),
	_second_player(std::move(second_player))
{}

void start_game() {}
