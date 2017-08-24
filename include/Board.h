
#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <unordered_map>
#include <set>

#include "traits.h"

struct pairhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
	 return x.first * SIZE + x.second;
  }
};

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
		std::set<std::pair<int, int>> available_positions() const;

	private:
		//Contains a sequence length and other opening
		//other_is_open contains if the other opening is open
		struct Sequence {
			unsigned short length;
			std::pair<int, int> opening;
			bool other_is_open;
		};
		//these two functions take care of keeping the sequences data structures in order after insertir or removing a move
		void insert_sequences(std::unordered_map<std::pair<int, int>, std::vector<Sequence>, pairhash>, std::pair<int, int>);
		void remove_sequences(std::unordered_map<std::pair<int, int>, std::vector<Sequence>, pairhash>, std::pair<int, int>);

		std::vector<Moves> _board; // 15 elements = first line
		std::set<std::pair<int, int>> _available_positions; //positions on the board that have not being played yet
		std::unordered_map<std::pair<int, int>, std::vector<Sequence>, pairhash> _sequences_first_player;
		std::unordered_map<std::pair<int, int>, std::vector<Sequence>, pairhash> _sequences_second_player;

};

#endif /* BOARD_H */

