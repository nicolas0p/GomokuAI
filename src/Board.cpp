
#include "Board.h"
#include <iostream>

#define SIZE 15

Board::Board(unsigned int size_x, unsigned int size_y) :
	_board{new vector<Moves>(size_x * size_y, NONE)}
{}

Board::~Board() {}

void Board::insert_move(std::pair<int, int> position, Moves player){
	//_board[((position.first-1) * SIZE) + position.second] = player; // position.first = line; position.second = col
	if (get_value_position(position) == NONE)
		_board.at(((position.first-1) * SIZE) + position.second) = player; 
	else
		std::cout << "ERRO!!! Tentativa de inserir jogada sem definir jogador" << std::endl;
}

void Board::remove_move(std::pair<int, int> position){
	_board[((position.first-1) * SIZE) + position.second] = NONE;  // position.first = line; position.second = col
}

Board::Moves Board::get_value_position(std::pair<int, int> position) const { 
	return _board[((position.first-1) * SIZE) + position.second]; // position.first = line; position.second = col

}
