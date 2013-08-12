#ifndef __MARY_COMMANDUNIDIRECTIONAL_H_
#define __MARY_COMMANDUNIDIRECTIONAL_H_

#include "Command.h"

namespace Mary {

	class CommandUnidirectional : public Command
	{
	public:
		int mStatePoint;

		CommandUnidirectional(std::string name);
		~CommandUnidirectional(void);

		void setMidPoint(CommandPoint &point);

		void execute();
	
	private:
		CommandPoint mMidPoint;
	};

}

#endif //__MARY_COMMANDUNIDIRECTIONAL_H_