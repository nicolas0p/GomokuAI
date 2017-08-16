#ifndef PLAYER_H
#define PLAYER_H

#include <pair>

#include "Board.h"

class Player {
	public:
		Player(Board * board);
		virtual ~Player();
		virtual std::pair<int, int> make_move() = 0;

	protected:
		Board * _board;
}

class HumanPlayer : public Player {
	public:
		HumanPlayer(Board * board);
		virtual ~HumanPlayer();
		std::pair<int, int> make_move();

	private:
		UserInterface * _user_interface;
}

class ComputerPlayer : public Player {
	public:
		ComputerPlayer(Board * board, unique_ptr<AI_Predictor> ai_predictor);
		virtual ~ComputerPlayer();
		std::pair<int, int> make_move();

	private:
		unique_ptr<AI_Predictor> _ai_predictor;
}

#endif //PLAYER_H
