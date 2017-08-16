#include <iostream>
#include <memory>

#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "HumanInput.h"
#include "Minimax.h"

using std::unique_ptr;

int main()
{
	Board board(15, 15);
	auto human{std::make_unique<Player>(board, std::make_unique<HumanInput>())};
	auto computer{std::make_unique<Player>(board, std::make_unique<Minimax>())};
	Game game(board, human, computer); //human will start playing
	game.start_game();
}
