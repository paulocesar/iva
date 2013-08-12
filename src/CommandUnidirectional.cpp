#include "CommandUnidirectional.h"

#include <iostream>

namespace Mary {

	CommandUnidirectional::CommandUnidirectional(std::string name) : Command(name)
	{
		mStatePoint = 0;
	}


	CommandUnidirectional::~CommandUnidirectional(void)
	{
	}

	void CommandUnidirectional::execute()
	{
		if(at(0).isActived())
			std::cout << "Funcionou" << std::endl;
	}
	
	void CommandUnidirectional::setMidPoint(CommandPoint &point)
	{
		mMidPoint = point;
	}
}