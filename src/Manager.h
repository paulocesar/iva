#ifndef __MARY_MANAGER_H_
#define __MARY_MANAGER_H_

#include "Game.h"
#include "Recognizer.h"
#include "TTSFestival.h"

#include "EasyDefines.h"

#include "OgreException.h"

#include "boost/thread/thread.hpp"

#include <map>
#include <string>
#include <vector>


namespace Mary {

	typedef void (*function)();
	/**
	 * the main manager class. organize the thread system between the game and CV engine
	 */
	class Manager
	{
	public:
		static boost::mutex mutexDevice;

		std::vector<function> mFunctions;

		/**
		 *get the singleton for Manager
		 *@return the Manager reference
		 */
		static Manager* getInstance();

		/**
		 *initializes and starts the game
		 *@param game a game implementation
		 *@param recognizer a CV implementation
		 */
		void start(Game* game, Recognizer* recognizer);

		/**
		 *send the emitted signal to the signal listener manager
		 *@param signal any identification to the signal emitted
		 *@param data emitted content specified by the user
		 */
		void signalShare(std::string &signal, void *data);

		/**
		 *loads image in Ogre3d from the recognizer
		 *used in the first load image, because the image reference is empty
		 *@param image empty image
		 */
		void loadImageFromRecognizer(Ogre::Image *image);
		/**
		 *update the data from Ogre3d image that contains the recognizer image
		 *@param buffer pointer to the shared data
		 */
		void loadSharedDataFromRecognizer(Ogre::HardwarePixelBufferSharedPtr buffer);

		void setActivedGameRecongizer(bool active);
		static bool isActived();

		//TODO: SEPARAR A PARTE DE SPEAK EM UMA HERANCA DO MANAGER
		void speak(std::string message);
		bool isSpeaking();
		static void setSpeakText(std::string message);
		static std::string getSpeakText();
		static bool hasSpeakText();
		static void clearSpeakText();

	private:
		/**
		 *constructor
		 */
		Manager(void);
		/**
		 *destructor
		 */
		~Manager(void);

	private:
		/**
		 * static reference to the Game implementation for thread proposes
		 */
		static void threadGame();
		/**
		 * static reference to the Recognizer implementation for thread proposes
		 */
		static void threadRecognizer();
		//static void threadTTS();

	private:
		/**
		 * variable used in the singleton pattern
		 */
		static Manager *mInstance;
		/**
		 * shared state between threads for syncronization
		 */
		bool mActived;
		bool mActivedGameRecognizer;

		/**
		 * Game implementation reference
		 */
		Game* mGame;
		
		/**
		 * Recognizer implementation reference
		 */
		Recognizer* mRecognizer;

		std::string mMessage;

	};

}

#endif //__MARY_MANAGER_H_
