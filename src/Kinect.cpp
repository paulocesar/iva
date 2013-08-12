/*
 * Kinect.cpp
 *
 *  Created on: 23/09/2011
 *      Author: paulo
 */

#include "Kinect.h"
#include "KinectDevice.h"

Kinect::Kinect() {
	mImage = cv::Mat(cv::Size(640,480),CV_8UC3,cv::Scalar(0));
	mDepth = cv::Mat(cv::Size(640,480),CV_8UC3);
	mDepthConversor = cv::Mat(cv::Size(640,480),CV_16UC1);

	device = &freenect.createDevice(0);

	device->startVideo();
	device->startDepth();
}

Kinect::~Kinect() {
	// TODO Auto-generated destructor stub
	device->stopVideo();
	device->stopDepth();
	freenect.deleteDevice(0);
}

cv::Mat Kinect::getImage()
{
	mImage.data = device->getVideo();
	return mImage;
}

cv::Mat Kinect::getDepth(int type)
{

	if(type == CV_16UC1)
		return device->getDepth();

	mDepthConversor = device->getDepth();
	mDepthConversor.convertTo(mDepth,CV_8UC3,0.12);
	return mDepth;
}
