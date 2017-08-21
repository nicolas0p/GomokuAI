
#include <iostream>

#include "CommandLineInterface.h"
#include "Board.h"

const int SIZE = 15;

CommandLineInterface::CommandLineInterface() {}

CommandLineInterface::~CommandLineInterface() {}

void CommandLineInterface::draw(const Board & board)
{
	std::cout << "    |  0   |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |  11 |  12 |  13 |  14 |" << std::endl;

	for (int l = 0; l<SIZE; l++){ //line

		if (l<10)
			std::cout << "0" << l << "  | ";
		else 
			std::cout << l << "  | ";

		for(int c = 0; c<SIZE; c++ ){ //column
			if (board.get_value_position(std::make_pair(l,c)) == Board::NONE)
				std::cout << "     |" ;
			else if (board.get_value_position(std::make_pair(l,c)) == Board::FIRSTPLAYER)
				std::cout << "  X  |" ;
			else if (board.get_value_position(std::make_pair(l,c)) == Board::SECONDPLAYER)
				std::cout << "  O  |" ;
			else
				std::cout << "ERRO |" ;
		}

		std::cout << std::endl;
	}
}
