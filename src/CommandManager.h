#ifndef __MARY_COMMANDMANAGER_H_
#define __MARY_COMMANDMANAGER_H_

#include <string>
#include <list>

#include "CommandPunctual.h"
#include "CommandUnidirectional.h"

namespace Mary {

	/**
	 * organize and execute the commands
	 */
	class CommandManager
	{
	public:
		/**
		 *get the singleton
		 *@return reference to the singleton
		 */
		static CommandManager *getInstance();

		/**
		 *load commands from a .mry text file
		 *@param path string with the file path
		 */
		void loadFromFile(std::string path);

		/**
		 *get commands that have just 1 point execution
		 *@return list of commands
		 */
		std::list<CommandPunctual> *getListCommandPunctual();
		/**
		 *get commands that have more than 1 point of execution and needs the point between points
		 *@return list of commands
		 */
		std::list<CommandUnidirectional> *getListCommandUnidirectional();

	private:
		CommandManager(void);
		~CommandManager(void);

	private:
		static CommandManager *mInstance;
		std::list<CommandPunctual> *mListCommandPunctual;
		std::list<CommandUnidirectional> *mListCommandUnidirectional;
	};

}

#endif //__MARY_COMMANDMANAGER_H_