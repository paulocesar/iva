/*
 * KinectDevice1.h
 *
 *  Created on: 06/07/2011
 *      Author: tiagotrocoli
 */

#ifndef KINECTDEVICE_H_
#define KINECTDEVICE_H_

//#include <pthread.h>
#include <libfreenect.hpp>
#include <opencv/cv.h>
#include <opencv/cxcore.h>

using namespace std;

//class Mutex {
//public:
//	Mutex(){pthread_mutex_init( &m_mutex, NULL );}
//	void lock(){pthread_mutex_lock( &m_mutex );}
//	void unlock(){pthread_mutex_unlock( &m_mutex );}
//
//private:
//	pthread_mutex_t m_mutex;
//};

class KinectDevice : public Freenect::FreenectDevice {
public:
	KinectDevice(freenect_context *_ctx, int _index);

	void VideoCallback(void* _rgb, uint32_t timestamp);
	void DepthCallback(void* _depth, uint32_t timestamp);

	uint8_t* getVideo();
	cv::Mat getDepth();//cv::Mat& output);


private:
	cv::Mat depthMat;
	uint16_t* depth;
	uint8_t* video;
//	Mutex m_rgb_mutex;
//	Mutex m_depth_mutex;
};

#endif /* KinectDevice_H_ */
