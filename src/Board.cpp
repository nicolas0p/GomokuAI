
#include "Board.h"
#include <iostream>

#define SIZE 15

Board::Board(unsigned int size_x, unsigned int size_y) :
	_board(size_x * size_y)
{}

Board::~Board() {}

void Board::insert_move(std::pair<int, int> position, int player){
	//_board[((position.first-1) * SIZE) + position.second] = player; // position.first = line; position.second = col
	if (player == 1)
		_board.at(((position.first-1) * SIZE) + position.second) = FIRSTPLAYER; 
	else if (player == 2)
		_board.at(((position.first-1) * SIZE) + position.second) = SECONDPLAYER;
	else
		std::cout << "ERRO!!! Tentatica de inserir jogada sem definir jogador" << std::endl;
}

void Board::remove_move(std::pair<int, int> position){
	_board[((position.first-1) * SIZE) + position.second] = NONE;  // position.first = line; position.second = col
}

int Board::get_value_position(std::pair<int, int> position) const { 
	return _board[((position.first-1) * SIZE) + position.second]; // position.first = line; position.second = col

}
