#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "Board.h"

class UserInterface {
	public:
		UserInterface();
		virtual ~UserInterface();
		void draw(const Board & board);
};

#endif //USER_INTERFACE_H
