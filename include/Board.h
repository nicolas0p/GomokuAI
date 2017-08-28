
#ifndef BOARD_H
#define BOARD_H

#include <unordered_map>
#include <set>
#include <vector>

#include "traits.h"

struct pairhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
	 return x.first * SIZE + x.second;
  }
};

enum Direction {
	VERTICAL = 0, HORIZONTAL = 1, LEFT = 2/* \ */, RIGHT = 3/* / */
};
Direction next_direction(const Direction& direction);

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

		//Contains a sequence length and other opening
		//other_is_open contains if the other opening is open
		struct Sequence {
			Sequence(){}
			Sequence(const unsigned short& len, const std::pair<int, int>& open, const bool& ot_is_open, const Direction& dir):
				length{len},
				opening{open},
				other_is_open{ot_is_open},
				direction{dir}
				{}

			unsigned short length;
			std::pair<int, int> opening;
			bool other_is_open;
			Direction direction;

			bool operator<(const Sequence& seq) const
			{
				if(opening != seq.opening) {
					return opening < seq.opening;
				}
				if(length != seq.length) {
					return length < seq.length;
				}
				if(direction != seq.direction) {
					return direction < seq.direction;
				}
				return !other_is_open;
			}
		};

		std::unordered_map<std::pair<int, int>, std::set<Sequence>, pairhash> first_player_sequences() const;
		std::unordered_map<std::pair<int, int>, std::set<Sequence>, pairhash> second_player_sequences() const;

	private:
		bool is_valid_position(const std::pair<int, int> position);
		//Helper function to help find the next opening for a sequence after a move has been played
		std::pair<int, int> next_opening(const Sequence& sequence, const std::pair<int, int>& move);
		//these two functions take care of keeping the sequences data structures in order after insertir or removing a move
		void insert_sequences(std::unordered_map<std::pair<int, int>, std::set<Sequence>, pairhash>& sequences, const std::pair<int, int>& move);
		void remove_sequences(std::unordered_map<std::pair<int, int>, std::set<Sequence>, pairhash>& sequences, const std::pair<int, int>& move);

		std::vector<Moves> _board; // 15 elements = first line
		std::set<std::pair<int, int>> _available_positions; //positions on the board that have not being played yet
		std::unordered_map<std::pair<int, int>, std::set<Sequence>, pairhash> _sequences_first_player;
		std::unordered_map<std::pair<int, int>, std::set<Sequence>, pairhash> _sequences_second_player;

		std::set<std::pair<int, int>> get_neighbors(std::pair<int, int> p);
};

#endif /* BOARD_H */

