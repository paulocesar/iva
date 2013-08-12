#include "SignalEmitter.h"

#include "Manager.h"

namespace Mary {

	SignalEmitter::SignalEmitter(void)
	{
	}


	SignalEmitter::~SignalEmitter(void)
	{
	}

	void SignalEmitter::emit(std::string &signal, void *data)
	{
		if(Manager::getInstance())
			Manager::getInstance()->signalShare(signal,data);
	}

}