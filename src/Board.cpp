
#include <iostream>

#include "Board.h"
#include "traits.h"

Board::Board(unsigned int size_x, unsigned int size_y) :
	_board(size_x * size_y)
{}

Board::~Board() {}

void Board::insert_move(std::pair<int, int> position, Moves player)
{
	//_board[((position.first-1) * SIZE) + position.second] = player; // position.first = line; position.second = col
	if (get_value_position(position) != NONE) {
		std::cout << "ERRO!!! Tentativa de inserir jogada em posição já ocupada" << std::endl;
		return;
	}
	_board.at(((position.first) * SIZE) + position.second) = player;
	_available_positions.erase(position);
	if(player == Board::FIRSTPLAYER) {
		insert_sequences(_sequences_first_player, position);
		remove_sequences(_sequences_second_player, position);
	} else {
		insert_sequences(_sequences_second_player, position);
		remove_sequences(_sequences_first_player, position);
	}
}

void Board::remove_move(std::pair<int, int> position)
{
	auto player = _board[((position.first-1) * SIZE) + position.second];
	_board[((position.first) * SIZE) + position.second] = NONE;  // position.first = line; position.second = col
	_available_positions.insert(position);
	//TODO figure out how to fix the sequences data structures
}

Board::Moves Board::get_value_position(std::pair<int, int> position) const
{
	return _board[((position.first) * SIZE) + position.second]; // position.first = line; position.second = col
}

std::set<std::pair<int, int>> Board::available_positions() const
{
	return _available_positions;
}

/* Handles the sequences structures. This method should be called with the sequences of the player that is making the move now. It will increase the sequences where the move lands.
 **/
void Board::insert_sequences(std::unordered_map<std::pair<int, int>, std::set<Sequence>, pairhash>& sequences, const std::pair<int, int>& move)
{
	auto to_increase = sequences[move]; //contains the sequences that will be increased
	std::vector<Sequence> vertical, horizontal, left, right;
	for(auto seq : to_increase) {
		switch(seq.direction) {
			case VERTICAL:
				vertical.push_back(seq);
				break;
			case HORIZONTAL:
				horizontal.push_back(seq);
				break;
			case LEFT:
				left.push_back(seq);
				break;
			case RIGHT:
				right.push_back(seq);
		}
	}
	Direction dir = 0;
	for(auto it : {vertical, horizontal, left, right}) {
		if(it.size() == 1) {
			//only one sequence in this direction with this opening, just increase it
			//and update opening
			auto seq = it[0];
			seq.length++;
			seq.opening = next_opening(seq, move);
			dir++;
		} else if(it.size() == 2){
			//two sequences with the same opening in the same direction, merge them!
			//remove both of them and their other opening copies
			sequences[move].erase(it[0]);
			sequences[it[0].opening].erase(it[0]);
			sequences[move].erase(it[1]);
			sequences[it[1].opening].erase(it[1]);
			if(!it[0].other_is_open && !it[1].other_is_open) { //if both ends are closed
				//TODO check to see if it is length 5!
				continue;
			}
			auto seq1 = Sequence();
			auto seq2 = Sequence();
			unsigned short length = it[0].length + it[1].length + 1;
			seq1.length = length;
			seq2.length = length;
			seq1.opening = it[0].opening; //has to be added in sequences[it[1].opening]
			seq2.opening = it[1].opening;
			seq1.other_is_open = it[0].other_is_open;
			seq2.other_is_open = it[1].other_is_open;
			seq1.direction = it[0].direction;
			seq2.direction = it[1].direction;
			sequences[it[1].opening].insert(seq1);
			sequences[it[0].opening].insert(seq2);
			dir++;
		} else { //no sequence, create new sequence in this direction
			switch(dir) {
				case VERTICAL:
					std::pair<int, int> edge1, edge2;
					edge1 = {move.first, move.second - 1}; //to the left
					edge2 = {move.first, move.second + 1}; //to the right
					sequences[edge1].insert(Sequence(1, edge2, _board[edge2] == NONE, VERTICAL));
					sequences[edge2].insert(Sequence(1, edge1, _board[edge1] == NONE, VERTICAL));
					break;
				case HORIZONTAL:
					break;
				case LEFT:
					break;
				case RIGHT:
			}
			dir++;
		}
	}
}

/* Handles the sequences structures. This method should be called with the sequences of the player that is NOT making the move now. It will remove the openings of the sequences where the move lands.
 **/
void Board::remove_sequences(std::unordered_map<std::pair<int, int>, std::set<Sequence>, pairhash>& sequences, const std::pair<int, int>& move)
{
}

std::pair<int, int> Board::next_opening(const Sequence& sequence, const std::pair<int, int>& move)
{
	switch(sequence.direction) {
		case VERTICAL:
			if(move.first > sequence.opening.first) {
				return {move.first + 1, move.second}; //below
			}
			return {move.first - 1, move.second}; //above
			break;
		case HORIZONTAL:
			if(move.second > sequence.opening.second) {
				return {move.first, move.second + 1}; //to the right
			}
			return {move.first, move.second - 1}; //to the left
			break;
		case LEFT:
			if(move.first > sequence.opening.first) {
				return {move.first + 1, move.second + 1}; //to the right and below
			}
			return {move.first - 1, move.second - 1}; //to the left and above
			break;
		case RIGHT:
			if(move.first > sequence.opening.first) {
				return {move.first + 1, move.second - 1}; //to the left and below
			}
			return {move.first - 1, move.second + 1}; //to the right and above
	}
}
