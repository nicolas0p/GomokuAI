#include <iostream>
#include <memory>
#include <functional>

#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "HumanInput.h"
#include "Minimax.h"
#include "UserInterface.h"
#include "heuristic.h"
#include "MoveGenerator.h"

using std::unique_ptr;

int main()
{
	int difficulty = 2;
	Board board(15, 15);
	std::function<int (const Board &)> heuristic = sum_sequence_values;
	std::function<std::set<std::pair<int, int>> (const Board&)> move_generator = simple_move_generator;
	auto human = std::make_unique<Player>(board, std::make_unique<HumanInput>());
	auto computer = std::make_unique<Player>(board, std::make_unique<Minimax>(heuristic, move_generator, difficulty));
	Game game(board, std::move(human), std::move(computer), std::make_unique<UserInterface>()); //human will start playing
	game.start_game();
}
