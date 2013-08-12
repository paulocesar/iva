#ifndef __MARY_SIGNALEMITTER_H_
#define __MARY_SIGNALEMITTER_H_

#include <string>

namespace Mary {
	
	/**
	 *class that emit signals
	 */
	class SignalEmitter
	{

	public:
		/**
		 *emits the signal to the SignalListener class
		 *@param signal identifier signal name
		 *@param data signal content
		 */
		void emit(std::string &signal, void *data);

	protected:
		/**
		 *constructor
		 */
		SignalEmitter(void);
		/**
		 *destructor
		 */
		~SignalEmitter(void);
	};

}

#endif //__MARY_SIGNALEMITTER_H_