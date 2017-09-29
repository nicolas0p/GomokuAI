
#include "HumanInput.h"
#include <iostream>

HumanInput::HumanInput(const Board::Moves& player) :
	InputComponent(player),
	_line(0),
	_col(0)
{}

HumanInput::~HumanInput() {}

/**
 * Ask user where to insert a move
 * @return position to insert a move
 **/
std::pair<int, int> HumanInput::get_move(const Board board)
{
	
	std::cout << "=====================" << std::endl << 	_player << "º Player, it's your turn:" << std::endl << "Enter a line:";
	std::cin >> _line;
	std::cout << "Enter a column:";
	std::cin >> _col;
	
	return std::make_pair(_line,_col);
}
