#ifndef __MARY_COMMAND_H_
#define __MARY_COMMAND_H_

#include <vector>
#include <string>


#include "opencv.hpp"

#include "CommandPoint.h"
#include "SignalEmitter.h"

namespace Mary {

	/**
	 *class that interprets the activation and deactivation from a list of points
	 */
	class Command : public std::vector<CommandPoint>, public SignalEmitter
	{
	public:
		/**
		 *constructor
		 */
		Command(std::string name);
		/**
		 *destructor
		 */
		~Command(void);

		
		/**
		 *virtual method for user implements own point's interpretation
		 */
		virtual void execute() = 0;

	private:
		std::string mName;
	};

}

#endif //__MARY_COMMAND_H_
