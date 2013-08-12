#include "CommandPoint.h"

namespace Mary {

	CommandPoint::CommandPoint(int x, int y) : cv::Point(x,y), mActived(false)
	{
	}
	
	CommandPoint::CommandPoint(cv::Point &point) : cv::Point(point), mActived(false)
	{

	}

	CommandPoint::~CommandPoint(void)
	{
	}

	void CommandPoint::setActived(bool actived)
	{
		mActived = actived;
	}

	bool CommandPoint::isActived()
	{
		return mActived;
	}

}