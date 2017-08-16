
#include "Board.h"

Board::Board(unsigned int size_x, unsigned int size_y) :
	_board(size_x * size_y)
{}

Board::~Board() {}

void Board::insert_move(std::pair<int, int> position) {}

void Board::remove_move(std::pair<int, int> position) {}
