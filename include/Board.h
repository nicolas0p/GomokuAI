
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

		Board();
		~Board() {}
		//Contains a sequence length and direction
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
		std::pair<std::pair<int, int>, std::pair<int, int>> generate_sequence_len1(const std::pair<int, int>& center, const Direction& direction) const;
		bool is_valid_position(const std::pair<int, int> position) const;
		//Helper function to help find the next opening for a sequence after a move has been played
		std::pair<int, int> next_opening(const Sequence& sequence, const std::pair<int, int>& opening, const std::pair<int, int>& move) const;
		//these two functions take care of keeping the sequences data structures in order after insertir or removing a move
		void insert_move_self_sequences(std::unordered_map<std::pair<int, int>, std::unordered_map<std::pair<int, int>, Sequence, pairhash>, pairhash>& sequences, const std::pair<int, int>& move);
		void insert_move_other_player_sequences(std::unordered_map<std::pair<int, int>, std::unordered_map<std::pair<int, int>, Sequence, pairhash>, pairhash>& sequences, const std::pair<int, int>& move);

		std::vector<Moves> _board; // 15 elements = first line
		std::set<std::pair<int, int>> _available_positions; //positions on the board that have not being played yet
		Sequences_map _sequences_first_player;
		Sequences_map _sequences_second_player;

		void remove_move_sequences(Board::Sequences_map& sequences, Board::Sequences_map& seq_advers, const std::pair<int, int>& position);
		std::set<std::pair<int, int>> get_neighbors(std::pair<int, int> p);

		Moves winner() const;
		Moves _winner;
		std::pair<std::pair<int, int>, Board::Sequence> select_sequence_by_position(std::pair<int, int> b, std::unordered_map<std::pair<int, int>, Board::Sequence, pairhash> seq,std::pair<int, int> p);
		std::pair<std::pair<int,int>, Direction> get_opposite_position(std::pair<int,int> n, std::pair<int,int> p);
		std::pair<int, int> find_begin_sequence(Sequences_map& s, std::pair<int,int> p, Direction d);
		int calculate_lenght(std::pair<int, int> begin, std::pair<int, int> end, Direction d);
		void aux_remove_move(Sequences_map& sequences, std::pair<int, int> it, std::pair<int, int> position);


		void remove_move_self_sequences(Sequences_map& sequences, const std::pair<int, int>& move);
		std::pair<std::pair<int, int>, std::pair<int, int>> get_sequence_in_direction(const Sequences_map& sequences, const std::pair<int, int>& move, const Direction& direction) const;
		std::pair<int, int> get_next_in_direction(const std::pair<int, int>& position, const Direction& direction) const;
		std::pair<int, int> direction_max(const Direction& direction, const std::pair<int, int>& position) const;
		bool is_position_in_sequence(const std::pair<int, int>& opening1, const std::pair<int, int>& opening2, const Direction& direction, const std::pair<int, int>& position) const;
		std::pair<int, int> closest_opening(const std::pair<int, int>& position, const std::pair<int, int>& opening1, const std::pair<int, int>& opening2) const;
		bool is_on_the_edge_of_sequence(const std::pair<int, int>& position, const std::pair<int, int>& opening1, const std::pair<int, int>& opening2) const;
		unsigned short distance(const std::pair<int, int>& position1, const std::pair<int, int>& position2, const Direction& direction) const;
};

#endif /* BOARD_H */

