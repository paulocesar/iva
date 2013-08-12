#include "RecognizerKinect.h"

#include "GameTest.h"
#include "Manager.h"

#include "OgreSceneNode.h"

#include "CommandManager.h"

#include <string>
#include <sstream>

namespace Mary {

	RecognizerKinect::RecognizerKinect(void)
	{
//		depthFrame = cv::Mat(cv::Size(640,480),CV_8UC3);
	}


	RecognizerKinect::~RecognizerKinect(void)
	{
	}
	

	void RecognizerKinect::update()
	{
		{
		    boost::mutex::scoped_lock lock(Manager::mutexDevice);
			depthFrame = kinect.getDepth();
		}

		if(depthFrame.empty())
		{
			cv::waitKey(30);
			return;
		}

		nearest = 254;
		for(int x = 0; x < depthFrame.cols; x+=5)
			for(int y = 0; y < depthFrame.rows; y+=5)
				if(depthFrame.at<uint8_t>(y,x) < nearest)
					nearest = depthFrame.at<uint8_t>(y,x);

		cv::threshold(kinect.getDepth(),depthFrame,nearest+7,255,CV_THRESH_BINARY_INV);
		cv::flip(depthFrame,depthFrame,1);
		cv::GaussianBlur(depthFrame,depthFrame,cv::Size(3,3),1);

		Hand::getInstance()->findHand(depthFrame,nearest);

		std::string message = Hand::getInstance()->getMessage();
		emit(message,NULL);

		//cv::imshow("result",depthFrame);

		cv::waitKey(30);

	}

	void RecognizerKinect::clear()
	{
		Hand::getInstance()->reset();
	}
	
	unsigned char* RecognizerKinect::getData()
	{
		return (unsigned char *) kinect.getImage().data;
	}

	double RecognizerKinect::getWidth()
	{
		return kinect.getImage().size().width;
	}
	
	double RecognizerKinect::getHeight()
	{
		return kinect.getImage().size().height;
	}
	
	Ogre::PixelFormat RecognizerKinect::getPixelFormat()
	{
		return Ogre::PF_R8G8B8;
	}





}
