#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include <set>

#include "Board.h"

std::set<std::pair<int, int>> simple_move_generator(const Board& board);

#endif /* MOVE_GENERATOR_H */
