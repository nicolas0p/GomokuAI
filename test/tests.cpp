#include "gtest/gtest.h"

#include "Minimax.h"
#include "Board.h"
#include "Heuristic.h"
#include "MoveGenerator.h"
#include "traits.h"

TEST(InsertMoveInBoard, OneMove) {
	Board board;
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	EXPECT_EQ(SIZE*SIZE - 1, board.available_positions().size());
	EXPECT_EQ(Board::FIRSTPLAYER, board.get_value_position({0,0}));
	EXPECT_EQ(3, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(InsertMoveInBoard, TwoHorizontalNextMoves) {
	Board board;
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	board.insert_move({0,1}, Board::FIRSTPLAYER);
	EXPECT_EQ(305, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(InsertMoveInBoard, TwoHorizontalNextMovesNotEdge) {
	Board board;
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.insert_move({5,6}, Board::FIRSTPLAYER);
	EXPECT_EQ(2*300+12, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(InsertMoveInBoard, MergingTwoSequences) {
	Board board;
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	board.insert_move({0,2}, Board::FIRSTPLAYER);
	board.insert_move({0,1}, Board::FIRSTPLAYER);
	EXPECT_EQ(50008, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(InsertMoveInBoard, Len1SequencesAroundBoard) {
	Board board;
	board.insert_move({5,5}, Board::FIRSTPLAYER); //2*4 sequences
	board.insert_move({14,0}, Board::FIRSTPLAYER); //1*3 sequences
	board.insert_move({14,5}, Board::FIRSTPLAYER); //2*1 + 1*3 sequences
	board.insert_move({9,14}, Board::FIRSTPLAYER); //2*1 + 1*3 sequences
	EXPECT_EQ(21, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(InsertMoveInBoard, 2by2SquareSequence) {
	Board board;
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.insert_move({6,6}, Board::FIRSTPLAYER);
	board.insert_move({5,6}, Board::FIRSTPLAYER);
	board.insert_move({6,5}, Board::FIRSTPLAYER);
	EXPECT_EQ(6*2*300 + 2*4, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(InsertMoveInBoard, InsertSecondPlayerMoveFar) {
	Board board;
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.insert_move({10,10}, Board::SECONDPLAYER);
	EXPECT_EQ(0, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(InsertMoveInBoard, InsertSecondPlayerMoveClose) {
	Board board;
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.insert_move({5,6}, Board::FIRSTPLAYER);
	board.insert_move({5,7}, Board::SECONDPLAYER);
	EXPECT_EQ(300+12-7, sum_sequence_values(board, Board::FIRSTPLAYER));
}

/*TEST(RemoveMoveFromBoard, OneMove) {
	Board board;
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.remove_move({5,5});
	EXPECT_EQ(0, sum_sequence_values(board, Board::FIRSTPLAYER));
}*/

TEST(Minimax, Depth1OnePlayer) {
	Board board;
	board.insert_move({1,1}, Board::FIRSTPLAYER);
	Minimax minimax(sum_sequence_values, simple_move_generator, 1, Board::FIRSTPLAYER);
	auto move = minimax.get_move(board);
	board.insert_move(move, Board::FIRSTPLAYER);
	EXPECT_EQ(300*2 + 6*2, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, TwoMove) {
	Board board;
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.insert_move({5,6}, Board::FIRSTPLAYER);
	board.remove_move({5,5});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, ThreeMoves) {
	Board board;
	board.insert_move({5,4}, Board::FIRSTPLAYER);
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.insert_move({5,6}, Board::FIRSTPLAYER);
	board.remove_move({5,5});
	EXPECT_EQ(16, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, OneMove_OneAdvers) {
	Board board;
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.insert_move({4,4}, Board::SECONDPLAYER);
	board.remove_move({5,5});
	EXPECT_EQ(0, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, OneMove_Extremity) {
	Board board;
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	board.remove_move({0,0});
	EXPECT_EQ(0, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, TwoMoves_Extremity) {
	Board board;
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	board.insert_move({1,0}, Board::FIRSTPLAYER);
	board.remove_move({0,0});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, TwoMoves_Extremity_OneAdvers) {
	Board board;
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	board.insert_move({1,0}, Board::FIRSTPLAYER);
	board.insert_move({1,1}, Board::SECONDPLAYER);
	board.remove_move({0,0});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, TwoMoves_OneAdvers) {
	Board board;
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.insert_move({5,4}, Board::SECONDPLAYER);
	board.insert_move({5,3}, Board::FIRSTPLAYER);
	board.remove_move({5,5});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}



