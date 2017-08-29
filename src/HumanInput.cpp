
#include "HumanInput.h"
#include <iostream>

HumanInput::HumanInput() :
	_line(0),
	_col(0)
{}

HumanInput::~HumanInput() {}


std::pair<int, int> HumanInput::get_move(const Board board, Board::Moves player)
{
	
	std::cout << "=====================" << std::endl << 	player << "º Player, it's your turn:" << std::endl << "Enter a line:";
	std::cin >> _line;
	std::cout << "Enter a column:";
	std::cin >> _col;
	
	return std::make_pair(_line,_col);
}
