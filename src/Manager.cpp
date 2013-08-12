#include "Manager.h"

#include "SignalListener.h"
#include "TTSFestival.h"

namespace Mary {

	Manager* Manager::mInstance = 0;
	boost::mutex Manager::mutexDevice;

	Manager* Manager::getInstance()
	{
		if(!mInstance)
			mInstance = new Manager();
		return mInstance;
	}
	
	void Manager::start(Game* game, Recognizer* recognizer)
	{
		mActived = true;
		mActivedGameRecognizer = false;

		mGame = game;
		mRecognizer = recognizer;

		boost::thread_group mThreads;
		mThreads.create_thread(&threadRecognizer);
		mThreads.create_thread(&threadGame);
		//mThreads.create_thread(&threadTTS);
		for(int i = 0; i < (int)mFunctions.size(); i++)
			mThreads.create_thread(mFunctions[i]);

		mThreads.join_all();
	}

	void Manager::threadGame()
	{
		getInstance()->mGame->start();
		getInstance()->mActived = false;
	}
	
	void Manager::threadRecognizer()
	{
		while(getInstance()->mActived)
		{
			if(getInstance()->mActivedGameRecognizer)
				getInstance()->mRecognizer->update();
		}
	}

//	void Manager::threadTTS()
//	{
//		TTSFestival *mTTS = new TTSFestival();
//		while(getInstance()->mActived)
//		{
//			if(hasSpeakText())
//			{
//				boost::thread::yield();
//			}
//			else
//			{
//				mTTS->play(getSpeakText());
//				clearSpeakText();
//			}
//		}
//	}

	void Manager::setSpeakText(std::string message)
	{
		getInstance()->mMessage = message;
	}

	std::string Manager::getSpeakText()
	{
		return getInstance()->mMessage;
	}

	bool Manager::hasSpeakText()
	{
		return (getInstance()->mMessage.size() == 0) ? true : false;
	}

	void Manager::clearSpeakText()
	{
		getInstance()->mMessage = "";
	}

	Manager::Manager(void)
	{
		mMessage = "";
	}

	Manager::~Manager(void)
	{
	}
	
	bool Manager::isActived()
	{
		return getInstance()->mActived;
	}

	void Manager::signalShare(std::string &signal, void *data)
	{
		SignalListener::sharedSignal(signal,data);
	}

	void Manager::loadImageFromRecognizer(Ogre::Image *image)
	{
		while(!mRecognizer->getData())
			std::cout << "IMAGE FROM CAMERA NOT FOUND" << std::endl;
		image->loadDynamicImage(
						mRecognizer->getData(), 
						(size_t) mRecognizer->getWidth(), 
						(size_t) mRecognizer->getHeight(), 
						mRecognizer->getPixelFormat());
	}
	
	void Manager::loadSharedDataFromRecognizer(Ogre::HardwarePixelBufferSharedPtr buffer)
	{
		while(!mRecognizer->getData())
			std::cout << "IMAGE FROM CAMERA NOT FOUND" << std::endl;
		buffer->blitFromMemory(
			Ogre::PixelBox(
				(size_t) mRecognizer->getWidth(),  
				(size_t) mRecognizer->getHeight(), 
				1, 
				mRecognizer->getPixelFormat(),
				mRecognizer->getData()));
	}

	void Manager::setActivedGameRecongizer(bool active)
	{
		getInstance()->mRecognizer->clear();
		mActivedGameRecognizer = active;
	}

	void Manager::speak(std::string message)
	{
		mMessage = message;
	}

	bool Manager::isSpeaking()
	{
		return (mMessage.size() != 0) ? true : false;
	}
}
