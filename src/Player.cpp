
#include "Player.h"

using std::unique_ptr;

Player::Player(const Board & board, unique_ptr<InputComponent> && input_component, Board::Moves player) :
	_point(0),
	_board{board},
	_input_component{std::move(input_component)},
	_player(player)
{}

Player::~Player() {}

std::pair<int, int> Player::make_move()
{
	return _input_component->get_move(_board, _player);
}

void Player::add_points(int p)
{
	_point += p;
}

int Player::get_points()
{
	return _point;
}

Board::Moves Player::get_player()
{
	return _player;
}
