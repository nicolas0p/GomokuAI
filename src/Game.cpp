#include "Game.h"

#include <iostream>

#define WINNER 400000000
#define MAX 256 // quantidade-1 jogadas possíveis de realizar num GUMOKU

Game::Game(Board board, unique_ptr<Player> first_player, unique_ptr<Player> second_player, unique_ptr<UserInterface> user_interface) :
	_board(board),
	_n_move(0),
	_first_player(std::move(first_player)),
	_second_player(std::move(second_player)),
	_user_interface(std::move(user_interface)),
	_line(0),
	_col(0)
{}

Game::~Game() {}

void Game::start_game() {

	// print empty board
	_user_interface->draw(_board);

	// loop until someone wins
	while(_n_move < MAX){
		///// First player turn /////
		_board.insert_move(_first_player->make_move(), Board::FIRSTPLAYER);
		// Update first player's points
		_first_player->set_points(sum_sequence_values(_board, Board::FIRSTPLAYER));
		// print turn and board
		_n_move++;
		std::cout << "---------------" << _n_move << "º Move -----------------" << std::endl;
		_user_interface->draw(_board);
		std::cout << "First Player's points: " << _first_player->get_points() << std::endl
			  	  << "Second Player's points: " << _second_player->get_points() << std::endl;
		if (_first_player->get_points() >= WINNER){
			std::cout << "--------------- FIRST PLAYER WON!!! ----------------" << std::endl;
			break;
		}

		///// Second player turn /////
		_board.insert_move(_second_player->make_move(), Board::SECONDPLAYER);
		// Update second player's points
		_second_player->set_points(sum_sequence_values(_board, Board::SECONDPLAYER));

		// print turn and board
		_n_move++;
		std::cout << "---------------" << _n_move << "º Move -----------------" << std::endl;
		_user_interface->draw(_board);
		std::cout << "First Player's points: " << _first_player->get_points() << std::endl
			 	  << "Second Player's points: " << _second_player->get_points() << std::endl;
		if (_second_player->get_points() >= WINNER){
			std::cout << "--------------- SECOND PLAYER WON!!! ----------------" << std::endl;
			break;
		}
	}

	if(_n_move == MAX)
		std::cout << "--------------- DRAW!!! ----------------" << std::endl;

}
