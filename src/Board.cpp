
#include <iostream>

#include "Board.h"
#include "traits.h"

Board::Board(unsigned int size_x, unsigned int size_y) : _board(size_x * size_y, NONE)
{
	for(unsigned int i = 0; i < size_x; ++i) {
		for(unsigned int j = 0; j < size_y; ++j) {
			_available_positions.insert(std::pair<int, int>{i, j});
		}
	}
}

Board::~Board() {}

// position need to be empty
void Board::insert_move(std::pair<int, int> position, Moves player)
{
	//_board[((position.first) * SIZE) + position.second] = player; // position.first = line; position.second = col
	if (get_value_position(position) != NONE) {
		std::cout << "ERRO!!! Tentativa de inserir jogada em posição já ocupada" << std::endl;
		return;
	}
	_board.at(((position.first) * SIZE) + position.second) = player;
	_available_positions.erase(position);
	if(player == Board::FIRSTPLAYER) {
		insert_move_self_sequences(_sequences_first_player, position);
		insert_move_other_people_sequences(_sequences_second_player, position);
	} else {
		insert_move_self_sequences(_sequences_second_player, position);
		insert_move_other_people_sequences(_sequences_first_player, position);
	}
}


// position can't be empty
void Board::remove_move(std::pair<int, int> position)
{
	auto player = _board[((position.first) * SIZE) + position.second];
	
	if (player == FIRSTPLAYER){
		remove_move_sequences(_sequences_first_player, _sequences_second_player, position);
	}
	else if (player == SECONDPLAYER){
		remove_move_sequences(_sequences_second_player, _sequences_first_player, position);
	}
	else if (player == NONE) // check if position set a place on board that is empty
		std::cout << "ERRO!!! Tentativa de apagar uma posição sem jogada!" << std::endl;

}

void Board::remove_move_sequences(Board::Sequences_map& sequences, Board::Sequences_map& seq_advers, const std::pair<int, int>& position)
{
	auto player = _board[((position.first) * SIZE) + position.second];

	// get the positions around the position
	std::set<std::pair<int, int>> neighbors = get_neighbors(position);
	for(auto it : neighbors){
		Board::Moves v_neighbor = _board[((it.first) * SIZE) + it.second];
		// the position  is a limit/point/extremity of a sequence;
		if (v_neighbor == NONE)
		{
			aux_remove_move(sequences, it, position);
		}
		// not empty; maybe in the middle of a sequence (check opposite position by direction)
		else if (v_neighbor == player) // neighbor is a position where there is a player' move
		{
			std::pair<std::pair<int,int>, Direction> opposite = get_opposite_position(it,position);
			if (opposite.first.first >= 0 && opposite.first.second >= 0) // validating opposite
			{
				if (_board[((opposite.first.first) * SIZE) + opposite.first.second] == player)
				{ // PROBLEMA: essa parte PODE ser executada duas vezes, gerando redundancia no BD. Devido duplicação das direções da BD.
					// call recursive function to find the sequence's extremity; 
					std::pair<int, int> begin = find_begin_sequence(sequences, opposite.first, opposite.second);
					auto end_seq = select_sequence_by_position(sequences[begin], position); 
					// than create two new sequences 
					sequences[begin][position] = Sequence(calculate_lenght(begin,position,opposite.second), true,  opposite.second);
					sequences[position][end_seq.first] = Sequence(calculate_lenght(position,end_seq.first,opposite.second),true, opposite.second);
					// and delete the old ones;
					sequences[begin].erase(end_seq.first);
					sequences[end_seq.first].erase(begin);
				}
			}
			else // the opposite will not be used, because goes out of board...
			{
				// create a new sequence' player with sequence reduced and delete the old one;
				aux_remove_move(sequences, it, position);
			}
		}
		else // it is a position where there is a adverser's move // need to set true other_is_open some adverser's sequences
		{
			if ( seq_advers.find(it) != seq_advers.end())
			{
				auto adver = select_sequence_by_position(seq_advers[position], it); 
				seq_advers[adver.first][position].other_is_open = true;
			}
			else
			{
				// investigar sequencia para inclui-la novamente no BD do adversário;
				
			}
		}
	}
	_board[((position.first) * SIZE) + position.second] = NONE;
	_available_positions.insert(position);
}

void Board::aux_remove_move(Board::Sequences_map sequences, std::pair<int, int> it, std::pair<int, int> position)
{
	auto s = select_sequence_by_position(sequences[it], position); 
	if (s.second.length > 1)
	{
		// create new sequence with position as the begin.
		auto temp = s.second;
		temp.length = s.second.length-1;
		sequences[position][s.first] = temp;
		// change other side.
		sequences[s.first][position] = temp;
		sequences[s.first].erase(it);
	}
	// Just do it if s.second.length == 1
	sequences[it].erase(s.first);
}

int Board::calculate_lenght(std::pair<int, int> begin, std::pair<int, int> end, Direction direction)
{
	int l;
	switch(direction) {
		case VERTICAL:
			l = (begin.first - end.first - 1);
		case HORIZONTAL:
			l = (begin.second - end.second - 1);
		case LEFT:
			l = (begin.second - end.second - 1);
		case RIGHT:
			l = (begin.first - end.first - 1);
	}
	
	if (l<0)
		throw ("Error! lenght less than 0.");

	return l;
}

std::pair<int, int> Board::find_begin_sequence(Board::Sequences_map& s, std::pair<int,int> p, Direction d)
{
	if (d == VERTICAL)
		p = std::make_pair(p.first-1, p.second);
	else if (d == HORIZONTAL)
		p = std::make_pair(p.first, p.second-1);
	else if (d == LEFT) /* \ */
		p = std::make_pair(p.first-1, p.second-1);
	else // if (d == RIGHT)/* / */
		p = std::make_pair(p.first-1, p.second+1);

	//std::unordered_map<std::string,double>::const_iterator got = s.find(p);
	if ( s.find(p) != s.end() )  //if ( got != s.end() ) 
		return p;

	//"not found"
	return find_begin_sequence(s, p, d);
}

std::pair<std::pair<int,int>, Direction> Board::get_opposite_position(std::pair<int,int> neighbor, std::pair<int,int> position)
{
	if ((neighbor.first == position.first) && (neighbor.second > position.second) ) // HORIZONTAL -->
		return std::make_pair(std::make_pair(position.first,position.second-1), HORIZONTAL);
	else if ((neighbor.first == position.first) && (neighbor.second < position.second) ) // HORIZONTAL <--
		return std::make_pair(std::make_pair(position.first,position.second+1), HORIZONTAL);
	else if ((neighbor.first < position.first) && (neighbor.second < position.second) ) // \ down
		return std::make_pair(std::make_pair(position.first+1,position.second+1), LEFT);
	else if ((neighbor.first > position.first) && (neighbor.second > position.second) ) // \ up
		return std::make_pair(std::make_pair(position.first-1,position.second-1), LEFT);
	else if ((neighbor.first > position.first) && (neighbor.second == position.second) ) // VERTICAL ^
		return std::make_pair(std::make_pair(position.first-1,position.second), VERTICAL);
	else if ((neighbor.first < position.first) && (neighbor.second == position.second) ) // VERTICAL v
		return std::make_pair(std::make_pair(position.first+1,position.second), VERTICAL);
	else if ((neighbor.first < position.first) && (neighbor.second > position.second) ) // / down
		return std::make_pair(std::make_pair(position.first+1,position.second-1), RIGHT);
	else if ((neighbor.first > position.first) && (neighbor.second < position.second) ) // / up
		return std::make_pair(std::make_pair(position.first-1,position.second+1), RIGHT);
	throw "error 404: opposite position not found";
}

// auxilia remove_move
// seq = conjunto de sequencias que partem de uma posição
// p = posição de uma EXTREMIDADE de uma sequencia
// objetivo: retornar a sequencia de seq que contem p
std::pair<std::pair<int, int>, Board::Sequence> Board::select_sequence_by_position(
			std::unordered_map<std::pair<int, int>, Board::Sequence, pairhash> seq,
		    std::pair<int, int> p)
{
	for (auto s : seq)
	{
		if ((s.second.direction == HORIZONTAL || s.second.direction == LEFT) && /* \ */
			(s.first.second == (p.second + s.second.length) || s.first.second == (p.second - s.second.length)))
			return s;
		else if ((s.second.direction == VERTICAL || s.second.direction == RIGHT) && 	/* / */
				 (s.first.first == (p.first + s.second.length) ||  s.first.first == (p.first - s.second.length)))
			return s;
	}
	throw "ERRO!!! Conjunto de Sequencias não contem a posição procurada!";
}

std::set<std::pair<int, int>> Board::get_neighbors(std::pair<int, int> p)
{
	if (p.first > 0 && p.first < 14 && p.second > 0 && p.second < 14)
		return {std::make_pair(p.first,p.second-1),
				std::make_pair(p.first-1,p.second-1),
				std::make_pair(p.first-1,p.second),
				std::make_pair(p.first-1,p.second+1),
				std::make_pair(p.first,p.second+1),
				std::make_pair(p.first+1,p.second+1),
				std::make_pair(p.first+1,p.second),
				std::make_pair(p.first+1,p.second-1)};
	else if (p.first == 0 && p.second > 0 && p.second < 14)
		return {std::make_pair(p.first,p.second-1),
				std::make_pair(p.first,p.second+1),
				std::make_pair(p.first+1,p.second+1),
				std::make_pair(p.first+1,p.second),
				std::make_pair(p.first+1,p.second-1)};
	else if (p.first > 0 && p.first < 14 && p.second == 14)
		return {std::make_pair(p.first,p.second-1),
				std::make_pair(p.first-1,p.second-1),
				std::make_pair(p.first-1,p.second),
				std::make_pair(p.first+1,p.second),
				std::make_pair(p.first+1,p.second-1)};
	else if (p.first == 14 && p.second > 0 && p.second < 14)
		return {std::make_pair(p.first,p.second-1),
				std::make_pair(p.first-1,p.second-1),
				std::make_pair(p.first-1,p.second),
				std::make_pair(p.first-1,p.second+1),
				std::make_pair(p.first,p.second+1)};
	else if (p.first > 0 && p.first < 14 && p.second == 0)
		return {std::make_pair(p.first-1,p.second),
				std::make_pair(p.first-1,p.second+1),
				std::make_pair(p.first,p.second+1),
				std::make_pair(p.first+1,p.second+1),
				std::make_pair(p.first+1,p.second)};
	else if (p.first == 0 && p.second == 0)
		return {std::make_pair(0,1), std::make_pair(1,1), std::make_pair(1,0)};
	else if (p.first == 0 && p.second == 14)
		return {std::make_pair(0,13), std::make_pair(1,14), std::make_pair(1,13)};
	else if (p.first == 14 && p.second == 14)
		return {std::make_pair(14,13), std::make_pair(13,13), std::make_pair(13,14)};
	else if (p.first == 14 && p.second == 0)
		return {std::make_pair(13,0), std::make_pair(13,1), std::make_pair(14,1)};
	throw "Error 404: neighbor not found";
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
			bool is_open = is_valid_position(edge2) && get_value_position(edge2) == NONE;
			if(is_valid_position(edge1)) {
				//sequences[edge1].insert(Sequence(1, edge2, is_open, dir));
				sequences[edge1][edge2] = Sequence(1, is_open, dir);
			}
			is_open = is_valid_position(edge1) && get_value_position(edge1) == NONE;
			if(is_valid_position(edge2)) {
				//sequences[edge2].insert(Sequence(1, edge1, is_open, dir));
				sequences[edge2][edge1] = Sequence(1, is_open, dir);
			}
			dir = next_direction(dir);
		}
	}
}

/*This method generate the length 1 sequence in the direction
 * center the center position for the sequence
 * direction the direction of the sequence
 * */
std::pair<std::pair<int, int>, std::pair<int, int>> Board::generate_sequence_len1(const std::pair<int, int>& center, const Direction& direction)
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

bool Board::is_valid_position(const std::pair<int, int> position)
{
	return !(position.first < 0 || position.second < 0 || position.first >= SIZE || position.second >= SIZE );
}

std::vector<Direction> _possible_directions = {HORIZONTAL, LEFT, RIGHT, VERTICAL};
Direction next_direction(const Direction& direction)
{
	return _possible_directions[direction];
}

/* Handles the sequences structures. This method should be called with the sequences of the player that is NOT making the move now. It will remove the openings of the sequences where the move lands.
 **/
void Board::insert_move_other_people_sequences(Board::Sequences_map& sequences, const std::pair<int, int>& move)
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

std::pair<int, int> Board::next_opening(const Sequence& sequence, const std::pair<int, int>& opening, const std::pair<int, int>& move)
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
			throw "error 404: direction not found";
	}
}

Board::Sequences_map Board::first_player_sequences() const
{
	return _sequences_first_player;
}

Board::Sequences_map Board::second_player_sequences() const
{
	return _sequences_second_player;
}
