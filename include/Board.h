#ifndef BOARD_H	
#define BOARD_H

#include <vector>

class Board {
	public:
		Board(unsigned int size_x, unsigned int size_y);
		~Board();
		void insert_move(std::pair<int, int> position, Moves player); // players = Moves
		void remove_move(std::pair<int, int> position);
		int get_value_position(std::pair<int, int> position)  const ; // returns a Moves;
		
		enum Moves {
			NONE = 0,
			FIRSTPLAYER = 1,
			SECONDPLAYER = 2
		};

	private:
		
		std::vector<Moves> _board; // 15 elements = first line
		

};

#endif /* BOARD_H */
