
#include <unordered_map>
#include <set>

#include "Heuristic.h"

int points_per_length[] = {1, 300, 50000, 5000000, 400000000};

int sum_sequence_values(const Board & board, const Board::Moves & player)
{
	Board::Sequences_map my_sequence;
	Board::Sequences_map other_sequence;
	if(player == Board::FIRSTPLAYER) {
		my_sequence = board.first_player_sequences();
		other_sequence = board.second_player_sequences();
	} else {
		my_sequence = board.second_player_sequences();
		other_sequence = board.first_player_sequences();
	}
	int score = 0;
	for(auto seqs : my_sequence) {
		for(auto it : seqs.second) {
			if(it.second.length <= 5) {
				score += points_per_length[it.second.length - 1];
			} else {
				throw std::runtime_error("Sequence [" + std::to_string(seqs.first.first) + "," + std::to_string(seqs.first.second) + "][" + std::to_string(it.first.first) + "," + std::to_string(it.first.second) + "] has invalid length = " + std::to_string(it.second.length));
			}
		}
	}
	for(auto seqs : other_sequence) {
		for(auto it : seqs.second) {
			if(it.second.length <= 5) {
				score -= points_per_length[it.second.length - 1];
			} else {
				throw std::runtime_error("OtherSequence [" + std::to_string(seqs.first.first) + "," + std::to_string(seqs.first.second) + "][" + std::to_string(it.first.first) + "," + std::to_string(it.first.second) + "] has invalid length = " + std::to_string(it.second.length));
			}
		}
	}
	return score;
}
