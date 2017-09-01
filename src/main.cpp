#include <iostream>
#include <memory>
#include <functional>

#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "HumanInput.h"
#include "Minimax.h"
#include "CommandLineInterface.h"
#include "Heuristic.h"
#include "MoveGenerator.h"
#include "traits.h"

using std::unique_ptr;


int main()
{

	unsigned int funcao = 1;
	int difficulty = 2;
	unsigned int temp = 0;
	Board board;

	std::function<int (const Board &, const Board::Moves &)> heuristic = sum_sequence_values;
	std::function<std::set<std::pair<int, int>> (const Board&)> move_generator = simple_move_generator;

	// Menu
	std::cout << "=========== UNIVERSIDADE FEDERAL DE SANTA CATARINA ===============" << std::endl
		 << "==============  INTELIGÊNCIA ARTIFICIAL 2017.2 ===================" << std::endl
		 << "==================== Trabalho Pratico 01  ========================" << std::endl
		 << "=========================== GOMOKU  ==============================" << std::endl
		 << "================== KARLA APARECIDA JUSTEN ========================" << std::endl
		 << "===================== NÍCOLAS PFEIFER ============================" << std::endl
		 << "==================================================================" << std::endl << std::endl;


	while(funcao==1 || funcao==2 )
	{
		std::cout << "======================== MENU ============================" << std::endl
			 //<< " 1 - Help " << std::endl
			 << " 2 - Iniciar jogo " << std::endl
			 << " Pressione qualquer outra tecla para sair " << std::endl
			 << "====================================================" << std::endl;
		std::cin >> funcao;
		switch (funcao)
		{
			case 1:
			{
				break;
			}
			case 2:
			{
				// set difficult
				std::cout << "Definir Dificuldade: " << std::endl;
				std::cin >> difficulty;

				std::unique_ptr<Player> first_player , second_player;

				// set players
				std::cout << "============= DEFINIR JOGADORES: =================" << std::endl
						  << " 1 - Humano " << std::endl 
						  << " 2 - Computador " << std::endl
						  << " Definir primeiro jogador: ";
				std::cin >> temp;

				if (temp == 1)
					first_player = std::make_unique<Player>(board, std::make_unique<HumanInput>(Board::FIRSTPLAYER), Board::FIRSTPLAYER);
				else
					first_player = std::make_unique<Player>(board, std::make_unique<Minimax>(heuristic, move_generator, difficulty, Board::FIRSTPLAYER),  Board::FIRSTPLAYER);
	
				std::cout << std::endl << " Definir segundo jogador: ";
				std::cin >> temp;
				std::cout << std::endl;
				if (temp == 1)
					second_player = std::make_unique<Player>(board, std::make_unique<HumanInput>(Board::SECONDPLAYER), Board::SECONDPLAYER);
				else
					second_player = std::make_unique<Player>(board, std::make_unique<Minimax>(heuristic, move_generator, difficulty, Board::SECONDPLAYER), Board::SECONDPLAYER);

				// FIRST player starts				
				Game game(board, std::move(first_player), std::move(second_player), std::make_unique<CommandLineInterface>()); 
				
				game.start_game();
			}
		}
	}
}
