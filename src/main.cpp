#include <iostream>
#include <memory>
#include <functional>

#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "HumanInput.h"
#include "Minimax.h"
#include "CommandLineInterface.h"
#include "heuristic.h"
#include "MoveGenerator.h"

#define FIRSTPLAYER 1
#define SECONDPLAYER 2

using std::unique_ptr;

int main()
{
	int difficulty = 2;
	Board board(15, 15);
	std::function<int (const Board &)> heuristic = sum_sequence_values;
	std::function<std::set<std::pair<int, int>> (const Board&)> move_generator = simple_move_generator;
	auto human = std::make_unique<Player>(board, std::make_unique<HumanInput>());
	auto computer = std::make_unique<Player>(board, std::make_unique<Minimax>(heuristic, move_generator, difficulty, SECONDPLAYER));

	std::cout << "First Player = human          Symbol = X" << std::endl
			  << "Second Player = computer      Symbol = O" << std::endl;
	
	Game game(board, std::move(human), std::move(computer), std::make_unique<CommandLineInterface>()); //human will start playing
	game.start_game();
}
