/*
 * KinectDevice1.cpp
 *
 *  Created on: 06/07/2011
 *      Author: tiagotrocoli
 */

#include "KinectDevice.h"
#include <iostream>
#include <opencv.hpp>
#include <opencv/cxcore.h>
#include <libfreenect.h>

#include "Manager.h"

using namespace std;

KinectDevice::KinectDevice(freenect_context *_ctx, int _index) : Freenect::FreenectDevice(_ctx, _index),
		depthMat(cv::Size(640,480),CV_16UC1)
//m_new_rgb_frame(false), m_new_depth_frame(false)
{}

void KinectDevice::VideoCallback(void* _video, uint32_t timestamp) {

	{
	    boost::mutex::scoped_lock lock(Mary::Manager::mutexDevice);
	    video = static_cast<uint8_t*>(_video);
	}
}

void KinectDevice::DepthCallback(void* _depth, uint32_t timestamp) {

	{
	    boost::mutex::scoped_lock lock(Mary::Manager::mutexDevice);
		depth = static_cast<uint16_t*>(_depth);
		depthMat.data = (uchar *) depth;
	}
}

uint8_t* KinectDevice::getVideo() {
	return video;
}

cv::Mat KinectDevice::getDepth(){
	cv::Mat mat;
	if(!depthMat.empty())
		mat = depthMat;
	return mat;

}
