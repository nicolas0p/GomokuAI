#ifndef PLAYER_H
#define PLAYER_H

#include <utility>
#include <memory>

#include "Board.h"
#include "InputComponent.h"

using std::unique_ptr;

class Player {
	public:
		Player(const Board & board, unique_ptr<InputComponent> && input_component);
		virtual ~Player();
		virtual std::pair<int, int> make_move();
		void add_points(int p);
		int get_points();
		
	protected:
		int _point;
		const Board _board;
		unique_ptr<InputComponent> _input_component;
};

#endif //PLAYER_H
