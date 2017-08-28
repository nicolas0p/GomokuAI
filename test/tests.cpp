#include "gtest/gtest.h"

#include "Minimax.h"
#include "Board.h"
#include "Heuristic.h"

TEST(InsertMoveInBoard, OneMove) {
	int size = 15;
	Board board(size, size);
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	EXPECT_EQ(size*size - 1, board.available_positions().size());
	EXPECT_EQ(Board::FIRSTPLAYER, board.get_value_position({0,0}));
	EXPECT_EQ(3, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(InsertMoveInBoard, TwoMoves) {
	int size = 15;
	Board board(size, size);
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	board.insert_move({0,1}, Board::FIRSTPLAYER);
	EXPECT_EQ(305, sum_sequence_values(board, Board::FIRSTPLAYER));
}
