#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board {
	public:
		Board(unsigned int size_x, unsigned int size_y);
		~Board();
		void insert_move(std::pair<int, int> position);
		void remove_move(std::pair<int, int> position);

	private:
		enum Moves {
			NONE = 0,
			HUMAN = 1,
			COMPUTER = 2
		};
		std::vector<Moves> _board;

};

#endif /* BOARD_H */
