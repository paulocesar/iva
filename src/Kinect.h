/*
 * Kinect.h
 *
 *  Created on: 23/09/2011
 *      Author: paulo
 */

#ifndef KINECT_H_
#define KINECT_H_

#include <libfreenect.hpp>
#include <libfreenect.h>
#include <opencv.hpp>

#include "KinectDevice.h"

class Kinect {
public:
	Kinect();
	virtual ~Kinect();

	cv::Mat getImage();

	//CV_8UC3 or CV_16UC1
	cv::Mat getDepth(int type = CV_8UC3);

protected:
	Freenect::Freenect<KinectDevice> freenect;
	KinectDevice* device;

	cv::Mat mImage;
	cv::Mat mDepth;

private:
	cv::Mat mDepthConversor;
};

#endif /* KINECT_H_ */
