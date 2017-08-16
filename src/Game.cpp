#include "Game.h"

Game::Game(Board board, unique_ptr<Player> first_player, unique_ptr<Player> second_player, unique_ptr<UserInterface> user_interface) :
	_board(board),
	_first_player(std::move(first_player)),
	_second_player(std::move(second_player)),
	_user_interface(std::move(user_interface))
{}

Game::~Game() {}

void Game::start_game() {}
