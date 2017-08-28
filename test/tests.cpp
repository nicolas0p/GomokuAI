#include "gtest/gtest.h"

#include "Minimax.h"
#include "Board.h"
#include "heuristic.h"

TEST(InsertMoveInBoard, OneMove) {
	int size = 15;
	Board board(size, size);
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	EXPECT_EQ(size*size - 1, board.available_positions().size());
	EXPECT_EQ(Board::FIRSTPLAYER, board.get_value_position({0,0}));
}

//TEST(InsertMoveInBoard, OneMoveHeuristic) {
//}
