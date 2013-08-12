#include "SignalListener.h"



namespace Mary {

	std::list<SignalListener *> SignalListener::mListeners;

	SignalListener::SignalListener(void)
	{
		mListeners.push_back(this);
	}

	SignalListener::~SignalListener(void)
	{
	}

	void SignalListener::sharedSignal(std::string &signal, void *data)
	{
		std::list<SignalListener*>::iterator it;
		for(it=mListeners.begin();it!=mListeners.end();it++)
			(*it)->listen(signal,data);
	}
}