#include "gtest/gtest.h"

#include "Minimax.h"
#include "Board.h"
#include "Heuristic.h"
#include "MoveGenerator.h"
#include "traits.h"

/* Insert_move */

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

/* Remove_move */
TEST(RemoveMoveFromBoard, OneMove) {
	Board board;
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.remove_move({5,5});
	EXPECT_EQ(0, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, TwoMove) {
	Board board;
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.insert_move({5,6}, Board::FIRSTPLAYER);
	board.remove_move({5,5});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, ThreeMoves_Horizontal) {
	Board board;
	board.insert_move({5,4}, Board::FIRSTPLAYER);
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.insert_move({5,6}, Board::FIRSTPLAYER);
	board.remove_move({5,5});
	EXPECT_EQ(16, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, ThreeMoves_Vertical) {
	Board board;
	board.insert_move({4,5}, Board::FIRSTPLAYER);
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.insert_move({6,5}, Board::FIRSTPLAYER);
	board.remove_move({5,5});
	EXPECT_EQ(16, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, ThreeMoves_Left) {
	Board board;
	board.insert_move({4,4}, Board::FIRSTPLAYER);
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.insert_move({6,6}, Board::FIRSTPLAYER);
	board.remove_move({5,5});
	EXPECT_EQ(16, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, ThreeMoves_Right) {
	Board board;
	board.insert_move({4,6}, Board::FIRSTPLAYER);
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.insert_move({6,4}, Board::FIRSTPLAYER);
	board.remove_move({5,5});
	EXPECT_EQ(16, sum_sequence_values(board, Board::FIRSTPLAYER));
}

// Extremety
TEST(RemoveMoveFromBoard, Extremity_OneMove_UpLeft) {
	Board board;
	board.insert_move({0,0}, Board::SECONDPLAYER);
	board.remove_move({0,0});
	EXPECT_EQ(0, sum_sequence_values(board, Board::SECONDPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_TwoMoves_UpLeft1) {
	Board board;
	board.insert_move({1,1}, Board::FIRSTPLAYER);
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	board.remove_move({0,0});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_TwoMoves_UpLeft2) {
	Board board;
	board.insert_move({1,1}, Board::FIRSTPLAYER);
	board.insert_move({2,0}, Board::FIRSTPLAYER);
	board.remove_move({2,0});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_TwoMoves_UpLeft3) {
	Board board;
	board.insert_move({0,1}, Board::FIRSTPLAYER);
	board.insert_move({1,0}, Board::FIRSTPLAYER);
	board.remove_move({1,0});
	EXPECT_EQ(5, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_TwoMoves_UpLeft4) {
	Board board;
	board.insert_move({1,0}, Board::FIRSTPLAYER);
	board.insert_move({2,1}, Board::FIRSTPLAYER);
	board.remove_move({1,0});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_ThreeMoves_UpLeft1) {
	Board board;
	board.insert_move({0,1}, Board::FIRSTPLAYER);
	board.insert_move({1,0}, Board::FIRSTPLAYER);
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	board.remove_move({1,0});
	EXPECT_EQ(305, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_ThreeMoves_UpLeft2) {
	Board board;
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	board.insert_move({1,0}, Board::FIRSTPLAYER);
	board.insert_move({2,0}, Board::FIRSTPLAYER);
	board.remove_move({1,0});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_ThreeMoves_UpLeft3) {
	Board board;
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	board.insert_move({1,1}, Board::FIRSTPLAYER);
	board.insert_move({2,2}, Board::FIRSTPLAYER);
	board.remove_move({1,1});
	EXPECT_EQ(11, sum_sequence_values(board, Board::FIRSTPLAYER));
}

//

TEST(RemoveMoveFromBoard, Extremity_OneMove_UpRight) {
	Board board;
	board.insert_move({0,14}, Board::SECONDPLAYER);
	board.remove_move({0,14});
	EXPECT_EQ(0, sum_sequence_values(board, Board::SECONDPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_TwoMoves_UpRight1) {
	Board board;
	board.insert_move({1,13}, Board::FIRSTPLAYER);
	board.insert_move({0,14}, Board::FIRSTPLAYER);
	board.remove_move({0,14});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_TwoMoves_UpRight2) {
	Board board;
	board.insert_move({2,13}, Board::FIRSTPLAYER);
	board.insert_move({1,14}, Board::FIRSTPLAYER);
	board.remove_move({1,14});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_TwoMoves_UpRight3) {
	Board board;
	board.insert_move({0,13}, Board::FIRSTPLAYER);
	board.insert_move({1,14}, Board::FIRSTPLAYER);
	board.remove_move({0,13});
	EXPECT_EQ(5, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_TwoMoves_UpRight4) {
	Board board;
	board.insert_move({1,13}, Board::FIRSTPLAYER);
	board.insert_move({2,14}, Board::FIRSTPLAYER);
	board.remove_move({2,14});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_ThreeMoves_UpRight1) {
	Board board;
	board.insert_move({0,14}, Board::FIRSTPLAYER);
	board.insert_move({1,14}, Board::FIRSTPLAYER);
	board.insert_move({2,14}, Board::FIRSTPLAYER);
	board.remove_move({1,14});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_ThreeMoves_UpRight2) {
	Board board;
	board.insert_move({1,13}, Board::FIRSTPLAYER);
	board.insert_move({0,14}, Board::FIRSTPLAYER);
	board.insert_move({2,12}, Board::FIRSTPLAYER);
	board.remove_move({1,13});
	EXPECT_EQ(11, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_ThreeMoves_UpRight3) {
	Board board;
	board.insert_move({0,13}, Board::FIRSTPLAYER);
	board.insert_move({0,14}, Board::FIRSTPLAYER);
	board.insert_move({1,14}, Board::FIRSTPLAYER);
	board.remove_move({1,14});
	EXPECT_EQ(305, sum_sequence_values(board, Board::FIRSTPLAYER));
}

//

TEST(RemoveMoveFromBoard, Extremity_OneMove_DownLeft) {
	Board board;
	board.insert_move({14,0}, Board::SECONDPLAYER);
	board.remove_move({14,0});
	EXPECT_EQ(0, sum_sequence_values(board, Board::SECONDPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_TwoMoves_DownLeft1) {
	Board board;
	board.insert_move({14,0}, Board::FIRSTPLAYER);
	board.insert_move({13,1}, Board::FIRSTPLAYER);
	board.remove_move({14,0});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_TwoMoves_DownLeft2) {
	Board board;
	board.insert_move({12,1}, Board::FIRSTPLAYER);
	board.insert_move({13,0}, Board::FIRSTPLAYER);
	board.remove_move({13,0});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_TwoMoves_DownLeft3) {
	Board board;
	board.insert_move({12,0}, Board::FIRSTPLAYER);
	board.insert_move({13,1}, Board::FIRSTPLAYER);
	board.remove_move({12,0});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_TwoMoves_DownLeft4) {
	Board board;
	board.insert_move({13,0}, Board::FIRSTPLAYER);
	board.insert_move({14,1}, Board::FIRSTPLAYER);
	board.remove_move({14,1});
	EXPECT_EQ(5, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_ThreeMoves_DownLeft1) {
	Board board;
	board.insert_move({13,0}, Board::FIRSTPLAYER);
	board.insert_move({14,0}, Board::FIRSTPLAYER);
	board.insert_move({14,1}, Board::FIRSTPLAYER);
	board.remove_move({14,0});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_ThreeMoves_DownLeft2) {
	Board board;
	board.insert_move({14,0}, Board::FIRSTPLAYER);
	board.insert_move({13,0}, Board::FIRSTPLAYER);
	board.insert_move({12,0}, Board::FIRSTPLAYER);
	board.remove_move({13,0});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_ThreeMoves_DownLeft3) {
	Board board;
	board.insert_move({14,0}, Board::FIRSTPLAYER);
	board.insert_move({13,1}, Board::FIRSTPLAYER);
	board.insert_move({12,2}, Board::FIRSTPLAYER);
	board.remove_move({13,1});
	EXPECT_EQ(11, sum_sequence_values(board, Board::FIRSTPLAYER));
}

//

TEST(RemoveMoveFromBoard, Extremity_OneMove_DownRight) {
	Board board;
	board.insert_move({14,14}, Board::SECONDPLAYER);
	board.remove_move({14,14});
	EXPECT_EQ(0, sum_sequence_values(board, Board::SECONDPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_TwoMoves_DownRight1) {
	Board board;
	board.insert_move({12,13}, Board::FIRSTPLAYER);
	board.insert_move({13,14}, Board::FIRSTPLAYER);
	board.remove_move({13,14});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_TwoMoves_DownRight2) {
	Board board;
	board.insert_move({13,13}, Board::FIRSTPLAYER);
	board.insert_move({14,14}, Board::FIRSTPLAYER);
	board.remove_move({14,14});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_TwoMoves_DownRight3) {
	Board board;
	board.insert_move({13,13}, Board::FIRSTPLAYER);
	board.insert_move({12,14}, Board::FIRSTPLAYER);
	board.remove_move({12,14});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_ThreeMoves_DownRight1) {
	Board board;
	board.insert_move({12,14}, Board::FIRSTPLAYER);
	board.insert_move({14,14}, Board::FIRSTPLAYER);
	board.insert_move({13,14}, Board::FIRSTPLAYER);
	board.remove_move({13,14});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_ThreeMoves_DownRight2) {
	Board board;
	board.insert_move({12,12}, Board::FIRSTPLAYER);
	board.insert_move({13,13}, Board::FIRSTPLAYER);
	board.insert_move({14,14}, Board::FIRSTPLAYER);
	board.remove_move({13,13});
	EXPECT_EQ(11, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_ThreeMoves_DownRight3) {
	Board board;
	board.insert_move({14,13}, Board::FIRSTPLAYER);
	board.insert_move({14,14}, Board::FIRSTPLAYER);
	board.insert_move({13,14}, Board::FIRSTPLAYER);
	board.remove_move({14,14});
	EXPECT_EQ(8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

// With Adversery's moves

TEST(RemoveMoveFromBoard, OneMove_OneAdvers) {
	Board board;
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.insert_move({4,4}, Board::SECONDPLAYER);
	board.remove_move({5,5});
	EXPECT_EQ(-8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, TwoMoves_OneAdvers) {
	Board board;
	board.insert_move({5,5}, Board::FIRSTPLAYER);
	board.insert_move({5,3}, Board::SECONDPLAYER);
	board.insert_move({5,4}, Board::FIRSTPLAYER);
	board.remove_move({5,4});
	EXPECT_EQ(0, sum_sequence_values(board, Board::SECONDPLAYER));
}

TEST(RemoveMoveFromBoard, Advers_One) {
	Board board;
	board.insert_move({7,7}, Board::FIRSTPLAYER);
	board.insert_move({7,8}, Board::FIRSTPLAYER);
	board.insert_move({7,9}, Board::FIRSTPLAYER);
	board.insert_move({8,7}, Board::FIRSTPLAYER);
	board.insert_move({8,8}, Board::SECONDPLAYER);
	board.insert_move({8,9}, Board::FIRSTPLAYER);
	board.insert_move({9,7}, Board::FIRSTPLAYER);
	board.insert_move({9,8}, Board::FIRSTPLAYER);
	board.insert_move({9,9}, Board::FIRSTPLAYER);
	board.remove_move({9,7});
	EXPECT_EQ((50000*4 + 300*12 + 13 - 1), sum_sequence_values(board, Board::FIRSTPLAYER));
}

// Extremity + adversery's moves
TEST(RemoveMoveFromBoard, Extremity_Advers_TwoMoves) {
	Board board;
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	board.insert_move({1,1}, Board::SECONDPLAYER);
	board.remove_move({0,0});
	EXPECT_EQ(-8, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(RemoveMoveFromBoard, Extremity_Advers_ThreeMoves) {
	Board board;
	board.insert_move({0,0}, Board::FIRSTPLAYER);
	board.insert_move({1,0}, Board::FIRSTPLAYER);
	board.insert_move({1,1}, Board::SECONDPLAYER);
	board.remove_move({0,0});
	EXPECT_EQ(-3, sum_sequence_values(board, Board::FIRSTPLAYER));
}


/* MinMax*/
TEST(Minimax, Depth1_OnePlayer) {
	Board board;
	board.insert_move({1,1}, Board::FIRSTPLAYER);
	Minimax minimax(sum_sequence_values, simple_move_generator, 1, Board::FIRSTPLAYER);
	auto move = minimax.get_move(board);
	board.insert_move(move, Board::FIRSTPLAYER);
	EXPECT_EQ(300*2 + 6*2, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(Minimax, Depth1_TwoPlayers) {
	Board board;
	board.insert_move({7,6}, Board::FIRSTPLAYER);
	Minimax minimax(sum_sequence_values, simple_move_generator, 1, Board::SECONDPLAYER);
	auto move = minimax.get_move(board);
	board.insert_move(move, Board::SECONDPLAYER);
	EXPECT_EQ(0, sum_sequence_values(board, Board::FIRSTPLAYER));
}

TEST(Minimax, Depth2_OnePlayer) {
	Board board;
	board.insert_move({7,6}, Board::FIRSTPLAYER);
	Minimax minimax(sum_sequence_values, simple_move_generator, 2, Board::FIRSTPLAYER);
	auto move = minimax.get_move(board);
	board.insert_move(move, Board::FIRSTPLAYER);
	EXPECT_EQ(300*2 + 6*2, sum_sequence_values(board, Board::FIRSTPLAYER));
}

