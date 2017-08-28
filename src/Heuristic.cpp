
#include <unordered_map>
#include <set>

#include "Heuristic.h"

int points_per_length[] = {1, 300, 50000, 5000000, 400000000};

int sum_sequence_values(const Board & board, const Board::Moves & player)
{
	std::unordered_map<std::pair<int, int>, std::set<Board::Sequence>, pairhash> my_sequence;
	std::unordered_map<std::pair<int, int>, std::set<Board::Sequence>, pairhash> other_sequence;
	if(player == Board::FIRSTPLAYER) {
		my_sequence = board.first_player_sequences();
		other_sequence = board.second_player_sequences();
	} else {
		my_sequence = board.second_player_sequences();
		other_sequence = board.first_player_sequences();
	}
	int score = 0;
	int multiplier;
	for(auto seqs : my_sequence) {
		for(auto it : seqs.second) {
			multiplier = 1;
			if(it.other_is_open) {
				multiplier = 2;
			}
			//score += multiplier * points_per_length[it.length];
			score += points_per_length[it.length - 1];
		}
	}
	for(auto seqs : other_sequence) {
		for(auto it : seqs.second) {
			multiplier = 1;
			if(it.other_is_open) {
				multiplier = 2;
			}
			//score -= multiplier * points_per_length[it.length];
			score -= points_per_length[it.length - 1];
		}
	}
	return score;
}
