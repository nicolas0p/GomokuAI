
#include "Player.h"

using std::unique_ptr;

Player::Player(const Board & board, unique_ptr<InputComponent> && input_component) :
	_board{board},
	_input_component{std::move(input_component)}
{}

Player::~Player() {}

std::pair<int, int> Player::make_move() {}
