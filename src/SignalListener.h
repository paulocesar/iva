#ifndef __MARY_SIGNALLISTENER_H_
#define __MARY_SIGNALLISTENER_H_

#include <list>
#include <string>

namespace Mary {
	/**
	 *class that listen signals
	 */
	class SignalListener
	{
	public:
		/**
		 *virtual method to do calculation with the signal emitted
		 *@param signal identifier signal name
		 *@raram data signal content
		 */
		virtual void listen(std::string &signal, void *data) = 0;
		
		/**
		 *emit the signal to all listener objects
		 *@param signal identifier signal name
		 *@param data signal content
		 */
		static void sharedSignal(std::string &signal, void *data);

	protected:
		/**
		 *constructor
		 */
		SignalListener(void);
		/**
		 *destructor
		 */
		~SignalListener(void);
	private:
		static std::list<SignalListener *> mListeners;
	};

}


#endif //__MARY_SIGNALLISTENER_H_
