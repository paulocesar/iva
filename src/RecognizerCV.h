#ifndef __MARY_RECOGNIZERCV_H_
#define __MARY_RECOGNIZERCV_H_

#include "Recognizer.h"

#include "opencv.hpp"

#include "SignalEmitter.h"

#include <list>

namespace Mary {

	class RecognizerCV : public Recognizer, public SignalEmitter
	{

	public:
		RecognizerCV(void);
		~RecognizerCV(void);

		static const int REDUCTION = 3;
		
	public:
		void update();
		void clear();

	public:
		unsigned char* getData();
		double getWidth();
		double getHeight();
		Ogre::PixelFormat getPixelFormat();

	protected:

		cv::VideoCapture mCVCapture;
		cv::Mat mImage;
		cv::Mat mImageGray;
		cv::Mat mCVImageOld;
		
		int time;
	};

}

#endif
