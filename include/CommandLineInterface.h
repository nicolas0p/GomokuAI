#ifndef COMMAND_LINE_INTERFACE_H
#define COMMAND_LINE_INTERFACE_H

#include "Board.h"
#include "UserInterface.h"

class CommandLineInterface : public UserInterface {
	public:
		UserInterface();
		virtual ~UserInterface();
		void draw(const Board & board);
};

#endif //COMMAND_LINE_INTERFACE_H
