#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "Board.h"

class UserInterface {
	public:
		UserInterface();
		virtual ~UserInterface();
		virtual void draw(const Board & board) = 0;
};

#endif //USER_INTERFACE_H
