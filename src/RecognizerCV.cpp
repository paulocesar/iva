#include "RecognizerCV.h"

#include "GameTest.h"

#include "OgreSceneNode.h"


namespace Mary {

	RecognizerCV::RecognizerCV(void)
	{
		mCVCapture = cv::VideoCapture(CV_CAP_ANY);
		time = 0;
	}


	RecognizerCV::~RecognizerCV(void)
	{
		mCVCapture.release();
	}

	void RecognizerCV::clear()
	{

	}

	void RecognizerCV::update()
	{
		if(mCVCapture.isOpened())
		{
			cv::Mat frame;
			cv::Mat reduceImg;
			cv::Mat imgGray;
			mCVCapture >> frame;
			
			//Resizing and Inverting
			int width = frame.size().width/REDUCTION;
			int height = frame.size().height/REDUCTION;
			cv::resize(frame,frame,cv::Size(width,height));
			cv::flip(frame,frame,1);

			cv::erode(frame,frame,cv::Mat());
			cv::dilate(frame,frame,cv::Mat());
			cv::GaussianBlur(frame,frame,cv::Size(21,21),0,0);
			cv::threshold(frame,frame,100,255,CV_THRESH_BINARY_INV);
			//cv::cvtColor(frame,frame, CV_BGR2GRAY);


			frame.copyTo(mImage);

		}
	}
	
	unsigned char* RecognizerCV::getData()
	{
		return (unsigned char *) mImage.data;
	}

	double RecognizerCV::getWidth()
	{
		return mImage.size().width;
	}
	
	double RecognizerCV::getHeight()
	{
		return mImage.size().height;
	}
	
	Ogre::PixelFormat RecognizerCV::getPixelFormat()
	{
		return Ogre::PF_R8G8B8;
	}

}
