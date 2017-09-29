
#include "Player.h"

using std::unique_ptr;

Player::Player(Board * board, unique_ptr<InputComponent> && input_component, Board::Moves player) :
	_point(0),
	_board{board},
	_input_component{std::move(input_component)},
	_player(player)
{}

Player::~Player() {}

/** Ask player where to insert a move
 * @return position to insert a move
 **/
std::pair<int, int> Player::make_move()
{
	return _input_component->get_move(*_board);
}

/** Get the points accumulated by the player
 * @return the points
 **/
int Player::get_points()
{
	return _point;
}

/** Set the points accumulated by the player
 * @param the points
 **/
void Player::set_points(int p)
{
	_point = p;
}
