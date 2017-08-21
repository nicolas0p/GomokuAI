
#include "CommandLineInterface.h"
#include <iostream>

#define SIZE 15

CommandLineInterface::CommandLineInterface() {}

CommandLineInterface::~CommandLineInterface() {}

void CommandLineInterface::draw(const Board & board) 
{
	std::cout << "  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10  |  11  |  12  |  13  |  14  |  15  |" << std::endl;

	for (int l = 1; l<SIZE; l++){ //line
		std::cout << l << "  | ";
		for(int c = 1; c<SIZE; c++ ){ //column
			if (board.get_value_position(std::make_pair(l,c)) == NONE)
				std::cout << "     |" ;
			else if (board.get_value_position(std::make_pair(l,c)) == FIRSTPLAYER)
				std::cout << "  X  |" ;
			else //board->get_value_position(std::make_pair(l,c) == 2
				std::cout << "  O  |" ;
		}
		std::cout << std::endl;		
	}
}
