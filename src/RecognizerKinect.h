#ifndef __MARY_RecognizerKinect_H_
#define __MARY_RecognizerKinect_H_

#include "Recognizer.h"

#include <opencv.hpp>

#include "SignalEmitter.h"
#include "CommandPunctual.h"

#include <libfreenect.hpp>
#include <libfreenect.h>


#include "Kinect.h"
#include "Hand.h"

#include <list>

namespace Mary {

class RecognizerKinect : public Recognizer, public SignalEmitter
	{

	public:
		RecognizerKinect(void);
		~RecognizerKinect(void);

		
	public:
		void update();
		void clear();

	public:
		unsigned char* getData();
		double getWidth();
		double getHeight();
		Ogre::PixelFormat getPixelFormat();

	protected:
		Kinect kinect;
		cv::Mat depthFrame;
		int nearest;
	};

}

#endif
