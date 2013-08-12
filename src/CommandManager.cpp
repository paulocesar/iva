#include "CommandManager.h"
#include "CommandPoint.h"

#include "EasyDefines.h"

#include <fstream>

namespace Mary {

	CommandManager *CommandManager::mInstance = 0;

	CommandManager *CommandManager::getInstance()
	{
		if(!mInstance)
			mInstance = new CommandManager();
		return mInstance;
	}

	CommandManager::CommandManager(void) : mListCommandPunctual(0), mListCommandUnidirectional(0)
	{
	}


	CommandManager::~CommandManager(void)
	{
	}
	
	void CommandManager::loadFromFile(std::string path)
	{
		if(!mListCommandPunctual)
			mListCommandPunctual = new std::list<CommandPunctual>();
		mListCommandPunctual->clear();

		if(!mListCommandUnidirectional)
			mListCommandUnidirectional = new std::list<CommandUnidirectional>();
		mListCommandUnidirectional->clear();

		//Teste
		//CommandPunctual c1 = CommandPunctual("first");
		//c1.push_back(CommandPoint(10,10));
		//c1.push_back(CommandPoint(100,10));
		//mListCommandPunctual->push_back(c1);
		//CommandUnidirectional c2 = CommandUnidirectional("unifirst");
		//c2.push_back(CommandPoint(100,10));
		//c2.push_back(CommandPoint(100,100));
		//mListCommandUnidirectional->push_back(c2);

		std::ifstream file (path.c_str());
		if(file.is_open())
		{
			std::string type, quant;

			while(file.good())
			{
				getline(file,type);
				getline(file,quant);

				/*
					TODO: fazer a propria classe (Unidirecional ou Pontual) se
					inserir nos vetores do manager. Para adicionar os pontos,
					usar um ponteiro para Command.
					O codigo abaixo faz nao faz isso. Precisa ser otimizado.
					OS PONTOS TAMBEM PRECISAM SER ESCALONADOS
				*/

				//PONTUAL
				if(type.compare("P") == 0)
				{
					CommandPunctual cp = CommandPunctual("pontual");
					std::string content;
					for(int i = 0; i < atoi(quant.c_str()); i++)
					{
						int x, y;
						getline(file,content);
						x = atoi(content.c_str());
						getline(file,content);
						y = atoi(content.c_str());
						
						cp.push_back(CommandPoint(x/3,y/3));
						std::cout << type << ' ' << quant << ' ' << x << ',' << y << std::endl;
					}
					mListCommandPunctual->push_back(cp);
				}

				//MULTIPONTO
				if(type.compare("U") == 0 || type.compare("B") == 0)
				{
					CommandUnidirectional cp = CommandUnidirectional("multiPontual");
					std::string content;
					for(int i = 0; i < atoi(quant.c_str()); i++)
					{
						int x, y;
						getline(file,content);
						x = atoi(content.c_str());
						getline(file,content);
						y = atoi(content.c_str());
						
						cp.push_back(CommandPoint(x/3,y/3));
						std::cout << type << ' ' << quant << ' ' << x << ',' << y << std::endl;
					}
					mListCommandUnidirectional->push_back(cp);
				}
			}
			file.close();
		}
		else
			MWARNING("Command File can't be found.");

	}

	std::list<CommandPunctual> *CommandManager::getListCommandPunctual()
	{
		return mListCommandPunctual;
	}
	
	std::list<CommandUnidirectional> *CommandManager::getListCommandUnidirectional()
	{
		return mListCommandUnidirectional;
	}
}
