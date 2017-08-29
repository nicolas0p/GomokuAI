
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
		//Contains a sequence length and other opening
		//other_is_open contains if the other opening is open
		struct Sequence {
			Sequence(){}
			Sequence(const unsigned short& len, const bool& ot_is_open, const Direction& dir):
				length{len},
				other_is_open{ot_is_open},
				direction{dir}
				{}

			unsigned short length;
			bool other_is_open;
			Direction direction;

			bool operator<(const Sequence& seq) const
			{
				if(length != seq.length) {
					return length < seq.length;
				}
				if(direction != seq.direction) {
					return direction < seq.direction;
				}
				return !other_is_open;
			}
		};

		typedef std::unordered_map<std::pair<int, int>, std::unordered_map<std::pair<int, int>, Board::Sequence, pairhash>, pairhash> Sequences_map;

		Sequences_map first_player_sequences() const;
		Sequences_map second_player_sequences() const;

		void insert_move(std::pair<int, int> position, Board::Moves player); // players = Moves
		void remove_move(std::pair<int, int> position);
		Moves get_value_position(std::pair<int, int> position)  const ; // returns a Moves;
		std::set<std::pair<int, int>> available_positions() const;

	private:
		std::pair<std::pair<int, int>, std::pair<int, int>> generate_sequence_len1(const std::pair<int, int>& center, const Direction& direction);
		bool is_valid_position(const std::pair<int, int> position);
		//Helper function to help find the next opening for a sequence after a move has been played
		std::pair<int, int> next_opening(const Sequence& sequence, const std::pair<int, int>& opening, const std::pair<int, int>& move);
		//these two functions take care of keeping the sequences data structures in order after insertir or removing a move
		void insert_move_self_sequences(std::unordered_map<std::pair<int, int>, std::unordered_map<std::pair<int, int>, Sequence, pairhash>, pairhash>& sequences, const std::pair<int, int>& move);
		void insert_move_other_people_sequences(std::unordered_map<std::pair<int, int>, std::unordered_map<std::pair<int, int>, Sequence, pairhash>, pairhash>& sequences, const std::pair<int, int>& move);

		std::vector<Moves> _board; // 15 elements = first line
		std::set<std::pair<int, int>> _available_positions; //positions on the board that have not being played yet
		std::unordered_map<std::pair<int, int>, std::unordered_map<std::pair<int, int>, Sequence, pairhash>, pairhash> _sequences_first_player;
		std::unordered_map<std::pair<int, int>, std::unordered_map<std::pair<int, int>, Sequence, pairhash>, pairhash> _sequences_second_player;

		std::set<std::pair<int, int>> get_neighbors(std::pair<int, int> p);
		std::pair<std::pair<int, int>, Board::Sequence> select_sequence_by_position(
		std::unordered_map<std::pair<int, int>, Board::Sequence, pairhash> seq,
		std::pair<int, int> p);
};

#endif /* BOARD_H */

