#include "gtest/gtest.h"

#include "Minimax.h"
#include "Board.h"
#include "Heuristic.h"
#include "traits.h"

TEST(InsertMoveInBoard, OneMove) {
	Board board(SIZE, SIZE);
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	EXPECT_EQ(SIZE*SIZE - 1, board.available_positions().size());
	EXPECT_EQ(Board::FIRSTPLAYER, board.get_value_position({0,0}));
	EXPECT_EQ(3, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(InsertMoveInBoard, TwoHorizontalNextMoves) {
	Board board(SIZE, SIZE);
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	board.insert_move({0,1}, Board::FIRSTPLAYER);
	EXPECT_EQ(305, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(InsertMoveInBoard, MergingTwoSequences) {
	Board board(SIZE, SIZE);
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	board.insert_move({0,2}, Board::FIRSTPLAYER);
	board.insert_move({0,1}, Board::FIRSTPLAYER);
	EXPECT_EQ(50008, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(InsertMoveInBoard, Len1SequencesAroundBoard) {
	Board board(SIZE, SIZE);
	board.insert_move({5,5}, Board::FIRSTPLAYER); //2*4 sequences
	board.insert_move({14,0}, Board::FIRSTPLAYER); //1*3 sequences
	board.insert_move({14,5}, Board::FIRSTPLAYER); //2*1 + 1*3 sequences
	board.insert_move({9,14}, Board::FIRSTPLAYER); //2*1 + 1*3 sequences
	EXPECT_EQ(21, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(InsertMoveInBoard, 2by2SquareSequence) {
	Board board(SIZE, SIZE);
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.insert_move({6,6}, Board::FIRSTPLAYER);
	board.insert_move({5,6}, Board::FIRSTPLAYER);
	board.insert_move({6,5}, Board::FIRSTPLAYER);
	EXPECT_EQ(6*2*300 + 2*4, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(InsertMoveInBoard, InsertSecondPlayerMove) {
	Board board(SIZE, SIZE);
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.insert_move({10,10}, Board::SECONDPLAYER);
	EXPECT_EQ(0, sum_sequence_values(board, Board::FIRSTPLAYER));
}

/*TEST(RemoveMoveFromBoard, OneMove) {
	Board board(SIZE, SIZE);
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.remove_move({5,5});
	EXPECT_EQ(0, sum_sequence_values(board, Board::FIRSTPLAYER));
}*/

