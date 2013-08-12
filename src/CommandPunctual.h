#ifndef __MARY_COMMANDPUNCTUAL_H_
#define __MARY_COMMANDPUNCTUAL_H_

#include "Command.h"
#include <string>

namespace Mary {

	class CommandPunctual : public Command
	{
	public:
		CommandPunctual(std::string name);
		~CommandPunctual(void);

		void execute();
	};

}

#endif //__MARY_COMMANDPUNCTUAL_H_