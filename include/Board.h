
#ifndef BOARD_H
#define BOARD_H

#include <unordered_map>
#include <set>
#include <vector>
#include <functional>

#include "traits.h"

/* Hash function for a pair of values. Needed by the unordered map structure.
 * */
struct pairhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
	 return x.first * SIZE + x.second;
  }
};
/* Represents all four directions a sequence can be in.
 * Left represents the sequences that begin on the upper left corner and end on the bottom right.
 * Right represents the sequences that begin on the upper right corner and en on the bottom left.
 * */
enum Direction {
	VERTICAL = 0, HORIZONTAL = 1, LEFT = 2/* \ */, RIGHT = 3/* / */
};
Direction next_direction(const Direction& direction);

std::pair<int, int> direction_max(const Direction& direction, const std::pair<int, int>& position);
std::pair<int, int> direction_min(const Direction& direction, const std::pair<int, int>& position);
std::pair<int, int> get_next_in_direction(const std::pair<int, int>& position, const Direction& direction);
std::pair<int, int> get_previous_in_direction(const std::pair<int, int>& position, const Direction& direction);

class Board {
	public:
		/* Represents all the possible state a position on the board could be.
		 * NONE means no move has been played there yet.
		 * FIRSTPLAYER means the first player has played in this position.
		 * SECONDPLAYER means the second player has played in this position.
		 * */
		enum Moves {
			NONE = 0,
			FIRSTPLAYER = 1,
			SECONDPLAYER = 2
		};

		Board();
		~Board() {}
		/* Contains a sequence length, direction and if the other opening of the sequence is open or not
		*/
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

		void insert_move(const std::pair<int, int>& position, const Board::Moves& player); // players = Moves
		void remove_move(const std::pair<int, int>& position);
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

		Moves winner() const;
		Moves _winner;


		void remove_move_self_sequences(Sequences_map& sequences, const std::pair<int, int>& move);
		void remove_move_other_player_sequences(Sequences_map& sequences, const std::pair<int, int>& move);
		std::pair<std::pair<int, int>, std::pair<int, int>> get_sequence_part_of_in_direction(const Sequences_map& sequences, const std::pair<int, int>& move, const Direction& direction) const;
		bool is_position_in_sequence(const std::pair<int, int>& opening1, const std::pair<int, int>& opening2, const Direction& direction, const std::pair<int, int>& position) const;
		std::pair<int, int> closest_opening(const std::pair<int, int>& position, const std::pair<int, int>& opening1, const std::pair<int, int>& opening2) const;
		bool is_on_the_edge_of_sequence(const std::pair<int, int>& position, const std::pair<int, int>& opening1, const std::pair<int, int>& opening2) const;
		unsigned short distance(const std::pair<int, int>& position1, const std::pair<int, int>& position2, const Direction& direction) const;
		std::vector<std::pair<int, int>> get_other_edges_sequence_in_direction(const Sequences_map& sequences, const std::pair<int, int>& position, const Direction &direction) const;
		unsigned short get_length_to_direction(const Direction& direction, const std::pair<int, int>& position, const std::function<std::pair<int, int> (const Direction&, const std::pair<int, int>&)>& limit, const std::function<std::pair<int, int> (const std::pair<int, int>&, const Direction&)>& next, const Moves& player) const;
};

Board::Moves get_other_player(const Board::Moves& player);

#endif /* BOARD_H */
