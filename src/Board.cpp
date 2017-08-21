
#include "Board.h"
#include <iostream>

#define SIZE 15

Board::Board(unsigned int size_x, unsigned int size_y) :
	_board(size_x * size_y)
{
	for(int i = 0; i < size_x; ++i) {
		for(int j = 0; j < size_y; ++j) {
			_available_positions.insert(std::pair<int, int>(i, j));
		}
	}
}

Board::~Board() {}

void Board::insert_move(std::pair<int, int> position, Moves player)
{
	//_board[((position.first-1) * SIZE) + position.second] = player; // position.first = line; position.second = col
	if (get_value_position(position) != NONE) {
		std::cout << "ERRO!!! Tentativa de inserir jogada em posição já ocupada" << std::endl;
		return;
	}
	_board.at(((position.first-1) * SIZE) + position.second) = player;
	_available_positions.erase(position);
	//insert_sequences(position, player);
}

void Board::remove_move(std::pair<int, int> position)
{
	_board[((position.first-1) * SIZE) + position.second] = NONE;  // position.first = line; position.second = col
	_available_positions.insert(position);
}

Board::Moves Board::get_value_position(std::pair<int, int> position) const
{
	return _board[((position.first-1) * SIZE) + position.second]; // position.first = line; position.second = col

}
