
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <functional>
#include <string>

#include "Board.h"
#include "traits.h"

Board::Board() : _board(SIZE * SIZE, NONE),	_winner(NONE)
{
	for(unsigned int i = 0; i < SIZE; ++i) {
		for(unsigned int j = 0; j < SIZE; ++j) {
			_available_positions.insert(std::pair<int, int>{i, j});
		}
	}
}

/**
 * Indicate where the position is on board
 * @param position position to be searched
 * @return where the position is on board
 * 
 */
Board::Moves Board::get_value_position(std::pair<int, int> position) const
{
	return _board[((position.first) * SIZE) + position.second]; // position.first = line; position.second = col
}

/** 
 * Insert a move in a position and recalculate the sequences results
 * @param position position where will be inserted a move, so it needs to be empty
 * @param player it indicates which player is making the move
 */
void Board::insert_move(const std::pair<int, int>& position, const Moves& player)
{
	if (get_value_position(position) != NONE) {
		throw std::runtime_error("ERRO!!! Tentativa de inserir jogada em posição já ocupada");
	}
	if(player == Board::FIRSTPLAYER) {
		insert_move_self_sequences(_sequences_first_player, position);
		insert_move_other_player_sequences(_sequences_second_player, position);
	} else {
		insert_move_self_sequences(_sequences_second_player, position);
		insert_move_other_player_sequences(_sequences_first_player, position);
	}
	_board.at(((position.first) * SIZE) + position.second) = player;
	_available_positions.erase(position);
}

/**
 * Removes a move in a position and recalculate the sequences results
 * @param position where will be remove a move, so it need to be occupied by a move
 */
void Board::remove_move(const std::pair<int, int>& position)
{
	auto player = _board[((position.first) * SIZE) + position.second];
	if (player == FIRSTPLAYER){
		remove_move_self_sequences(_sequences_first_player, position);
		remove_move_other_player_sequences(_sequences_second_player, position);
	}
	else if (player == SECONDPLAYER){
		remove_move_self_sequences(_sequences_second_player, position);
		remove_move_other_player_sequences(_sequences_first_player, position);
	}
	else if (player == NONE) // check if position set a place on board that is empty
		throw std::runtime_error("ERRO!!! Tentativa de apagar uma posição sem jogada! Position=(" + std::to_string(position.first) + "," + std::to_string(position.second) + ")");
	_board[((position.first) * SIZE) + position.second] = NONE;
	_available_positions.insert(position);
}

/**
 * Walk the given position in the given direction length units
 * @param direction direction to walk in
 * @param position initial position to walk from
 * @param length distance to walk
 * @return initil position + length in direction
 */
std::pair<int, int> walk_in_direction(const Direction& direction, const std::pair<int, int>& position, const int& length)
{
	switch(direction) {
		case VERTICAL: //to the bottom
			return {position.first + length, position.second};
		case HORIZONTAL: //to the right
			return {position.first, position.second + length};
		case LEFT: //to the bottom right
			return {position.first + length, position.second + length};
		case RIGHT: //to the bottom left
			return {position.first + length, position.second - length};
	}
}

/* 
 * Get the set of pairs that stores all the available postions from board
 * @return set of pairs that stores all the available postions from board
 */
std::set<std::pair<int, int>> Board::available_positions() const
{
	return _available_positions;
}

/**
 * Set the adversary's actual player
 * @param player actual player 
 * @return adversary
 */
Board::Moves get_other_player(const Board::Moves& player)
{
	if(player == Board::FIRSTPLAYER) {
		return Board::SECONDPLAYER;
	}
	return Board::FIRSTPLAYER;
}

/**
 * Method responsible for updating the sequences of a player when a move from the adversary is removed
 * @param sequences the sequences map structure of the player 
 * @param move the move was removed from the board
 */
void Board::remove_move_other_player_sequences(Sequences_map& sequences, const std::pair<int, int>& move)
{
	static constexpr auto limit = std::pair<int, int>{-1,-1};
	for (Direction direction : {VERTICAL, HORIZONTAL, LEFT, RIGHT}) {
		auto other_edges = get_other_edges_sequence_in_direction(sequences, move, direction);
		if(other_edges[0] != limit) {
			for(auto edge : other_edges) {
				sequences[edge][move].other_is_open = true;
				unsigned short length = sequences[edge][move].length;
				sequences[move][edge] = Sequence(length, true, direction);
			}
		} else {
			static const std::function<std::pair<int, int> (const Direction&, const std::pair<int, int>&)> dir_min = direction_min;
			static const std::function<std::pair<int, int> (const Direction&, const std::pair<int, int>&)> dir_max = direction_max;
			static const std::function<std::pair<int, int> (const std::pair<int, int>&, const Direction&)> next = get_next_in_direction;
			static const std::function<std::pair<int, int> (const std::pair<int, int>&, const Direction&)> previous = get_previous_in_direction;
			const auto player = get_other_player(get_value_position(move));
			auto length_to_min = get_length_to_direction(direction, move, dir_min, previous, player);
			if(length_to_min > 0) {
				auto seq = Sequence(length_to_min, false, direction);
				sequences[move][walk_in_direction(direction, move, -length_to_min - 1)] = seq;
			}
			auto length_to_max = get_length_to_direction(direction, move, dir_max, next, player);
			if(length_to_max > 0) {
				auto seq = Sequence(length_to_max, false, direction);
				sequences[move][walk_in_direction(direction, move, length_to_max + 1)] = seq;
			}
		}
	}
}

/**
 * @param sequences
 * @param position
 * @param direction
 * @return 
 */
std::vector<std::pair<int, int>> Board::get_other_edges_sequence_in_direction(const Board::Sequences_map& sequences, const std::pair<int, int>& position, const Direction &direction) const
{
	std::vector<std::pair<int, int>> results;
	auto dir_max = direction_max(direction, position);
	for(auto it = direction_min(direction, position); it != dir_max; it = get_next_in_direction(it, direction)) {
		if(sequences.find(it) != sequences.end() && sequences.at(it).find(position) != sequences.at(it).end()) {
			results.push_back(it);
		}
	}
	if(results.size() == 0) {
		results.push_back({-1, -1});
	}
	return results;
}

/* Helper method to get the lenght of a sequence starting in position in a given direction
 * @param direction The direction the sequence must be in
 * @param position The position in which the sequences we are looking for should begin
 * @param limit Function to generate the limit in this direction. Max or min
 * @param next Function to generate the next position in this direction. Next, towards max, or previous, towards min
 * @param player The player
 * @return Lenght of a sequence starting in position, going in the direction towards the position returned by the limit function
 * */
unsigned short Board::get_length_to_direction(const Direction& direction, const std::pair<int, int>& position, const std::function<std::pair<int, int> (const Direction&, const std::pair<int, int>&)>& limit, const std::function<std::pair<int, int> (const std::pair<int, int>&, const Direction&)>& next, const Moves& player) const
{
	auto boundary = limit(direction, position);
	unsigned short length = 0;
	for(auto it = next(position, direction); it != boundary; it = next(it, direction)) {
		if(get_value_position(it) != player) {
			break;
		}
		length++;
	}
	return length;
}

/**
 * Method responsible for updating the sequences of a player when a move from the same player is removed
 * @param sequences the sequences map structure of the player which move is being removed
 * @param move the move being removes from the board
 */
void Board::remove_move_self_sequences(Board::Sequences_map& sequences, const std::pair<int, int>& move)
{
	static constexpr auto limit = std::pair<int, int>{-1,-1};
	for (Direction direction : {VERTICAL, HORIZONTAL, LEFT, RIGHT}) {
		//discover the sequence I am a part of in this direction
		auto edges = get_sequence_part_of_in_direction(sequences, move, direction);
		if(edges.first == limit && edges.second == limit) {
			//should we create a sequence that begins in move and ends in direction_max or direction_min?
			static const std::function<std::pair<int, int> (const Direction&, const std::pair<int, int>&)> dir_min = direction_min;
			static const std::function<std::pair<int, int> (const Direction&, const std::pair<int, int>&)> dir_max = direction_max;
			static const std::function<std::pair<int, int> (const std::pair<int, int>&, const Direction&)> next = get_next_in_direction;
			static const std::function<std::pair<int, int> (const std::pair<int, int>&, const Direction&)> previous = get_previous_in_direction;
			const auto player = get_value_position(move);
			auto length_to_min = get_length_to_direction(direction, move, dir_min, previous, player);
			if(length_to_min > 0) {
				auto seq = Sequence(length_to_min, false, direction);
				sequences[move][walk_in_direction(direction, move, length_to_min)] = seq;
			}
			auto length_to_max = get_length_to_direction(direction, move, dir_max, next, player);
			if(length_to_max > 0) {
				auto seq = Sequence(length_to_max, false, direction);
				sequences[move][walk_in_direction(direction, move, length_to_max)] = seq;
			}
			continue;
		}
		unsigned short length = sequences[edges.first][edges.second].length;
		if(length == 1) {
			sequences[edges.first].erase(edges.second);
			sequences[edges.second].erase(edges.first);
		} else { //am I on the edge of a sequence or in its middle?
			if(is_on_the_edge_of_sequence(move, edges.first, edges.second)) {
				//reduce its length and update the nearest opening to 'move'
				auto near_open = closest_opening(move, edges.first, edges.second);
				auto far_open = (near_open == edges.second) ? edges.first : edges.second;
				bool is_open = sequences[near_open][far_open].other_is_open;
				sequences[edges.first].erase(edges.second);
				sequences[edges.second].erase(edges.first);
				bool is_valid_and_open = is_valid_position(far_open) && get_value_position(far_open) == NONE;
				if(is_valid_and_open) {
					sequences[far_open][move] = Sequence(length - 1, true, direction);
				}
				sequences[move][far_open] = Sequence(length - 1, is_open, direction);
			} else {
				//break the sequence in two, the new opening is 'move'
				sequences[edges.first].erase(edges.second);
				sequences[edges.second].erase(edges.first);
				unsigned short length1, length2;
				length1 = distance(edges.first, move, direction) - 1;
				length2 = distance(edges.second, move, direction) - 1;
				bool is_open1 = is_valid_position(edges.first) && get_value_position(edges.first) == NONE;
				bool is_open2 = is_valid_position(edges.second) && get_value_position(edges.second) == NONE;
				if(is_open1) {
					sequences[edges.first][move] = Sequence(length1, true, direction);
				}
				if(is_open2) {
					sequences[edges.second][move] = Sequence(length2, true, direction);
				}
				sequences[move][edges.first] = Sequence(length1, is_open1, direction);
				sequences[move][edges.second] = Sequence(length2, is_open2, direction);
			}
		}
	}
}

/**
 * Calculates the distance between two positions in the board in a given direction.
 * @param position1 first position to calculate the distance from.
 * @param position2 second position to calculate the distance to.
 * @param direction.
 * @return the distance between position1 and position2 in direction.
 */
unsigned short Board::distance(const std::pair<int, int>& position1, const std::pair<int, int>& position2, const Direction& direction) const
{
	if(direction == HORIZONTAL) {
		return abs(position1.second - position2.second);
	}
	return abs(position1.first - position2.first);
	/*switch(direction) {
		case VERTICAL:
			return abs(position1.first - position2.first);
		case HORIZONTAL:
			return abs(position1.second - position2.second);
		case LEFT:
			return abs(position1.first - position2.first);
		case RIGHT:
			return abs(position1.first - position2.first);
	}
	throw std::runtime_error("error 404: direction not found");*/
}

/** Returns the closest opening of a sequence to a position.
 * Position MUST be in between openings AND distance 1 to one of them!
 * @param position position on the board, must be in between the openings and distance 1 to one of them!
 * @param opening1 one opening of a sequence
 * @param opening2 another opening of that sequence
 * @return std::pair<int, int> opening1 or opening2 depending on which one is the closest to position
 * */
std::pair<int, int> Board::closest_opening(const std::pair<int, int>& position, const std::pair<int, int>& opening1, const std::pair<int, int>& opening2) const
{
	if(abs(position.first - opening1.first) <= 1 && abs(position.second - opening1.second) <= 1) {
		 return opening1;
	}
	return opening2;
}

/** Method used to discover if a position is on the edge of a sequence, that is, if it is 'beside' a opening
 * @param position position from which it is wanted to know if it is on the edge of a sequence
 * @param opening1 opening of a sequence
 * @param opening2 another opening of the same sequence
 * @return if position is 'beside'(distance 1) one of the openings
 * */
bool Board::is_on_the_edge_of_sequence(const std::pair<int, int>& position, const std::pair<int, int>& opening1, const std::pair<int, int>& opening2) const
{
	for (auto open : {opening1, opening2}) {
		if(abs(position.first - open.first) <= 1 && abs(position.second - open.second) <= 1) {
			 return true;
		}
	}
	return false;
}

/** Returns the edges of the sequence this positions belongs to in the given direction
 * @param sequences the map containig the sequences from which the sequence will be taken
 * @param move the position in the board that is a part of a sequence
 * @param direction the direction in which the sequence must be in
 * @return
 * */
std::pair<std::pair<int, int>, std::pair<int, int>> Board::get_sequence_part_of_in_direction(const Board::Sequences_map& sequences, const std::pair<int, int>& move, const Direction& direction) const
{
	auto dir_max = direction_max(direction, move);
	for(auto it = direction_min(direction, move); it != dir_max; it = get_next_in_direction(it, direction)) {
		if(sequences.find(it) == sequences.end()) {
			continue;
		}
		for(auto other_end : sequences.at(it)) {
			if(is_position_in_sequence(it, other_end.first, direction, move)) {
				return {it, other_end.first};
			}
		}
	}
	return {{-1, -1}, {-1, -1}};
}

/** Get the next position in the same direction
 * @param position the initial position
 * @param direction the direction in which the next position must be in
 * @return the next position in the same direction
 * */
std::pair<int, int> get_next_in_direction(const std::pair<int, int>& position, const Direction& direction)
{
	switch(direction) {
		case VERTICAL:
			return {position.first + 1, position.second};
		case HORIZONTAL:
			return {position.first, position.second + 1};
		case LEFT:
			return {position.first + 1, position.second + 1};
		case RIGHT:
			return {position.first + 1, position.second - 1};
	}
	throw std::runtime_error("error 404: direction not found");
}

/** Get the previus position in the same direction
 * @param position the initial position
 * @param direction the direction in which the sequence must be in
 * @return the previous position in the same direction
 * */
std::pair<int, int> get_previous_in_direction(const std::pair<int, int>& position, const Direction& direction)
{
	switch(direction) {
		case VERTICAL:
			return {position.first - 1, position.second};
		case HORIZONTAL:
			return {position.first, position.second - 1};
		case LEFT:
			return {position.first - 1, position.second - 1};
		case RIGHT:
			return {position.first - 1, position.second + 1};
	}
	throw std::runtime_error("error 404: direction not found");
}

/* Calculates the maximum position, just outside the board, for a position in a direction
 * @param direction The direction of the max
 * @param position The position used to calculate the max
 * @return The maximum position in the direction regarding the position.
 * */
std::pair<int, int> direction_max(const Direction& direction, const std::pair<int, int>& position)
{
	switch(direction) {
		case VERTICAL: //bottom
			return {SIZE, position.second};
		case HORIZONTAL: //right
			return {position.first, SIZE};
		case LEFT: //bottom right
			if(position.first > position.second) {
				return {SIZE, SIZE - position.first + position.second};
			} else {
				return {SIZE - position.second + position.first, SIZE};
			}
		case RIGHT: //bottom left
			if(position.first + position.second < SIZE -1 ) { //above secondary diagonal
				return {position.first + position.second + 1, -1};
			} else {
				return {SIZE, position.second - SIZE + position.first};
			}
	}
	throw std::runtime_error("error 404: direction not found");
}


/* Calculates the minimum position, just outside the board, for a position in a direction
 * @param direction The direction of the min
 * @param position The position used to calculate the min
 * @return The minimum position in the direction regarding the position.
 * */
std::pair<int, int> direction_min(const Direction& direction, const std::pair<int, int>& position)
{
	switch(direction) {
		case VERTICAL: //top
			return {-1, position.second};
		case HORIZONTAL: //left
			return {position.first, -1};
		case LEFT: //top left
			if(position.first > position.second) { //below main diagonal
				return {position.first - position.second - 1, -1};
			} else {
				return {-1, position.second - position.first - 1};
			}
		case RIGHT: //top right
			if(position.first + position.second < SIZE -1 ) { //above secondary diagonal
				return {-1, position.first + position.second + 1};
			} else {
				return {position.first - SIZE + position.second, SIZE};
			}
	}
	throw std::runtime_error("error 404: direction not found");
}

/* Calculates if the position is between opening1 and opening2 in the direction
 * @param opening1 One opening of a sequence
 * @param opening2 The other opening of the sequence
 * @param direction The direction the sequence is in
 * @param position The position that is wanted to know if it is inside the sequence
 * @return true if the position is in the sequence
 * */
bool Board::is_position_in_sequence(const std::pair<int, int>& opening1, const std::pair<int, int>& opening2, const Direction& direction, const std::pair<int, int>& position) const
{
	switch(direction) {
		case VERTICAL:
			return position.second == opening1.second && position.second == opening2.second &&
					(( position.first < opening1.first && position.first > opening2.first ) || ( position.first < opening2.first && position.first > opening1.first ));
		case HORIZONTAL:
			return position.first == opening1.first && position.first == opening2.first &&
					(( position.second < opening1.second && position.second > opening2.second ) || ( position.second < opening2.second && position.second > opening1.second ));
		case LEFT:
			return ( position.first < opening1.first && position.second < opening1.second && position.first > opening2.first && position.second > opening2.second ) ||
					( position.first > opening1.first && position.second > opening1.second && position.first < opening2.first && position.second < opening2.second );
		case RIGHT:
			return ( position.first < opening1.first && position.second > opening1.second && position.first > opening2.first && position.second < opening2.second ) ||
					( position.first > opening1.first && position.second < opening1.second && position.first < opening2.first && position.second > opening2.second );
	}
	throw std::runtime_error("error 404: direction not found");
}

/* Handles the sequences structures. This method should be called with the sequences of the player that is making the move now. It will increase the sequences where the move lands.
 * @param sequences The sequences map structure containing all one player sequences
 * @param move Given player move
 **/
void Board::insert_move_self_sequences(Board::Sequences_map& sequences, const std::pair<int, int>& move)
{
	auto to_increase = sequences[move]; //contains the sequences that will be increased
	std::vector<std::pair<std::pair<int, int>, Sequence>> vertical, horizontal, left, right;
	for(auto seq : to_increase) {
		switch(seq.second.direction) {
			case VERTICAL:
				vertical.push_back({seq.first, seq.second});
				break;
			case HORIZONTAL:
				horizontal.push_back({seq.first, seq.second});
				break;
			case LEFT:
				left.push_back({seq.first, seq.second});
				break;
			case RIGHT:
				right.push_back({seq.first, seq.second});
		}
	}
	Direction dir = VERTICAL;
	for(auto it : {vertical, horizontal, left, right}) {
		if(it.size() == 1) {
			//only one sequence in this direction with this opening, just increase it
			//and update opening
			auto seq = it[0].second;
			seq.length++;
			sequences[move].erase(it[0].first);
			auto opening = next_opening(seq, it[0].first, move);
			bool this_is_open = is_valid_position(opening) && get_value_position(opening) == NONE;
			if(this_is_open) { //only insert it if it is open
				sequences[opening][it[0].first] = seq;
			}
			if(seq.other_is_open) { //update the other side
				seq = sequences[it[0].first][move];
				seq.length++;
				seq.other_is_open = this_is_open;
				sequences[it[0].first][opening] = seq;
				sequences[it[0].first].erase(move);
			}
			//TODO remove sequence if both side are closed if(!seq.other_is_open && sequences[])
			dir = next_direction(dir);
		} else if(it.size() == 2) {
			//two sequences with the same opening in the same direction, merge them!
			//remove both of them and their other opening copies
			sequences[move].erase(it[0].first);
			sequences[move].erase(it[1].first);
			sequences[it[0].first].erase(move);
			sequences[it[1].first].erase(move);
			if(!it[0].second.other_is_open && !it[1].second.other_is_open) { //if both ends are closed
				//TODO check to see if it is length 5!
				continue;
			}
			auto seq1 = Sequence();
			auto seq2 = Sequence();
			unsigned short length = it[0].second.length + it[1].second.length + 1;
			seq1.length = length;
			seq2.length = length;
			seq1.other_is_open = it[0].second.other_is_open;
			seq2.other_is_open = it[1].second.other_is_open;
			seq1.direction = it[0].second.direction;
			seq2.direction = it[1].second.direction;
			if(is_valid_position(it[1].first) && get_value_position(it[1].first) == NONE) {
				sequences[it[1].first][it[0].first] = seq1;
			} if(is_valid_position(it[0].first) && get_value_position(it[0].first) == NONE) {
				sequences[it[0].first][it[1].first] = seq2;
			}
			dir = next_direction(dir);
		} else { //no sequence, create new sequence in this direction
			std::pair<int, int> edge1, edge2;
			auto a = generate_sequence_len1(move, dir);
			edge1 = a.first;
			edge2 = a.second;
			bool is_open2 = is_valid_position(edge2) && get_value_position(edge2) == NONE;
			bool is_open1 = is_valid_position(edge1) && get_value_position(edge1) == NONE;
			if(is_open1) {
				//sequences[edge1].insert(Sequence(1, edge2, is_open, dir));
				sequences[edge1][edge2] = Sequence(1, is_open2, dir);
			}
			if(is_open2) {
				//sequences[edge2].insert(Sequence(1, edge1, is_open, dir));
				sequences[edge2][edge1] = Sequence(1, is_open1, dir);
			}
			dir = next_direction(dir);
		}
	}
}

/* This method generates the length 1 sequence in the direction
 * @param center the center position for the sequence, around which the edges will be
 * @param center the center position for the sequence
 * @param direction the direction of the sequence
 * @return The two edges of the sequence around center in the direction
 * */
std::pair<std::pair<int, int>, std::pair<int, int>> Board::generate_sequence_len1(const std::pair<int, int>& center, const Direction& direction) const
{
	std::pair<int, int> edge1, edge2;
	switch(direction) {
		case VERTICAL:
			edge1 = {center.first - 1, center.second}; //above
			edge2 = {center.first + 1, center.second}; //below
			break;
		case HORIZONTAL:
			edge1 = {center.first, center.second - 1}; //to the left
			edge2 = {center.first, center.second + 1}; //to the right
			break;
		case LEFT:
			edge1 = {center.first - 1, center.second - 1}; //above and left
			edge2 = {center.first + 1, center.second + 1}; //below and right
			break;
		case RIGHT:
			edge1 = {center.first - 1, center.second + 1}; //above and right
			edge2 = {center.first + 1, center.second - 1}; //below and left
			break;
	}
	return {edge1, edge2};
}

/** Checks if the position is inside the limits of the board
 * @param position the position to be checked
 * @return true if the position is inside the limits of the board
 * */
bool Board::is_valid_position(const std::pair<int, int> position) const
{
	return !(position.first < 0 || position.second < 0 || position.first >= SIZE || position.second >= SIZE );
}

/* Function used to iterate over all the possible directions
 * @param direction The current direction
 * @return The next direction
 * */
std::vector<Direction> _possible_directions = {HORIZONTAL, LEFT, RIGHT, VERTICAL};
Direction next_direction(const Direction& direction)
{
	return _possible_directions[direction];
}

/* Handles the sequences structures. This method should be called with the sequences of the player that is NOT making the move now. It will remove the openings of the sequences where the move lands.
 * @param sequences The sequences map structure containing all the sequences of the player that is not making a move this turn.
 * @param move Move made this turn.
 **/
void Board::insert_move_other_player_sequences(Board::Sequences_map& sequences, const std::pair<int, int>& move)
{
	auto to_decrease = sequences[move]; //contains the sequences that will be decreased
	for(auto other : to_decrease)
	{
		// set other_is_open, from sequences that have open = move, as false
		auto seq = sequences[other.first];
		for(auto f : seq)
		{
			if (f.first == move){
				f.second.other_is_open = false;
			}
		}
	}
	// remove the sequence
	sequences.erase(move);
}

/* Calculates the next opening of a sequence, being move the current sequence opening
 * @param sequence The sequence that is going to be extended
 * @param opening The other opening of the sequence, the side that will not be extended
 * @param move The current opening of the sequence that is going to be extended
 * @return The new opening of the sequence considering it was already extended
 **/
std::pair<int, int> Board::next_opening(const Sequence& sequence, const std::pair<int, int>& opening, const std::pair<int, int>& move) const
{
	switch(sequence.direction) {
		case VERTICAL:
			if(move.first > opening.first) {
				return {move.first + 1, move.second}; //below
			}
			return {move.first - 1, move.second}; //above
		case HORIZONTAL:
			if(move.second > opening.second) {
				return {move.first, move.second + 1}; //to the right
			}
			return {move.first, move.second - 1}; //to the left
		case LEFT:
			if(move.first > opening.first) {
				return {move.first + 1, move.second + 1}; //to the right and below
			}
			return {move.first - 1, move.second - 1}; //to the left and above
		case RIGHT:
			if(move.first > opening.first) {
				return {move.first + 1, move.second - 1}; //to the left and below
			}
			return {move.first - 1, move.second + 1}; //to the right and above
		default:
			throw std::runtime_error("error 404: direction not found");
	}
}

/** Get the first player sequences
 * @return the first player sequences
 **/
Board::Sequences_map Board::first_player_sequences() const
{
	return _sequences_first_player;
}

/** Get the second player sequences
 * @return the second player sequences
 **/
Board::Sequences_map Board::second_player_sequences() const
{
	return _sequences_second_player;
}

/** Get the winner
 * @return the player who is the winner
 **/
Board::Moves Board::winner() const
{
	return _winner;
}
