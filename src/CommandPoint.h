#ifndef __MARY_COMMANDPOINT_H_
#define __MARY_COMMANDPOINT_H_

#include "opencv.hpp"

namespace Mary {

	class CommandPoint : public cv::Point
	{
	public:
		CommandPoint(int x = 0, int y = 0);
		CommandPoint(cv::Point &point);

		~CommandPoint(void);

		void setActived(bool actived);
		bool isActived();

	private:
		bool mActived;

	};

}

#endif //__MARY_COMMANDPOINT_H_
