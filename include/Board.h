#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <unordered_map>
#include <boost/functional/hash.hpp>


struct pairhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
	  std::size_t result;
	  boost::hash_combine(result, x.first);
	  boost::hash_combine(result, x.second);
	  return result;
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

	private:
		//Contains a sequence length and other opening
		//if there's no other opening = (-1,-1)
		struct Sequence {
			unsigned short length;
			std::pair<int, int> opening;
		};
		std::vector<Moves> _board; // 15 elements = first line
		std::unordered_map<std::pair<int, int>, std::vector<Sequence>, pairhash> _sequences_first_player;
		std::unordered_map<std::pair<int, int>, std::vector<Sequence>, pairhash> _sequences_second_player;

};

#endif /* BOARD_H */
