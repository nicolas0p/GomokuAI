#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "Board.h"

class UserInterface {
	public:
		UserInterface(unsigned int size_x, unsigned int size_y);
		virtual ~UserInterface();
		void draw(Board board);
}

#endif //USER_INTERFACE_H
