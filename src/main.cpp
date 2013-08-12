#include <memory>
#include <exception>

#include <opencv.hpp>


#include "EasyDefines.h"

#include "GameTest.h"
#include "Manager.h"
#include "RecognizerCV.h"
#include "RecognizerKinect.h"

#include "TTSFestival.h"

#define MODE_KINECT 1


void threadTTS()
{
	Mary::TTSFestival *mTTS = new Mary::TTSFestival();
	while(Mary::Manager::isActived())
	{
		if(Mary::Manager::hasSpeakText())
		{
			boost::thread::yield();
		}
		else
		{
			mTTS->play(Mary::Manager::getSpeakText());
			Mary::Manager::clearSpeakText();
		}
	}
}


int main()
{

	Mary::Manager::getInstance()->mFunctions.push_back(&threadTTS);

#if MODE_KINECT

	Mary::RecognizerKinect *kinect = new Mary::RecognizerKinect();
	Mary::GameTest *maryGame = new Mary::GameTest();

	Mary::Manager::getInstance()->start(maryGame,kinect);

	kinect->~RecognizerKinect();

#else

	Mary::RecognizerCV *recCV = new Mary::RecognizerCV();
	Mary::GameTest *maryGame = new Mary::GameTest();

	Mary::Manager::getInstance()->start(maryGame,recCV);

	recCV->~RecognizerCV();

#endif

	return 0;
}

