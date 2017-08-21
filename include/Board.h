#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board {
	public:
		enum Moves {
			NONE = 0,
			FIRSTPLAYER = 1,
			SECONDPLAYER = 2
		};

		Board(unsigned int size_x, unsigned int size_y);
		~Board();
		void insert_move(std::pair<int, int> position, Board::Moves player); // players = Moves
		void remove_move(std::pair<int, int> position);
		Moves get_value_position(std::pair<int, int> position)  const ; // returns a Moves;


	private:
		std::vector<Moves> _board; // 15 elements = first line


};

#endif /* BOARD_H */
