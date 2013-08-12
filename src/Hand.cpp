/*
 * Hand.cpp
 *
 *  Created on: 24/09/2011
 *      Author: root
 */

#include "Hand.h"

Hand *Hand::mInstance = 0;

Hand *Hand::getInstance()
{
	if(!mInstance)
		mInstance = new Hand();
	return mInstance;
}

Hand::Hand() {
	// TODO Auto-generated constructor stub
	isFoundHand = false;
	foundCount = 0;

	mMessage = "";

	loadDataSet();
}

Hand::~Hand() {
	// TODO Auto-generated destructor stub
}

void Hand::reset() {
	mListResultHand1.clear();
	mListResultHand2.clear();
	mListResultHand2_1.clear();
	mListResultHand2_2.clear();
	mListResultHand2_3.clear();
	mListResultHand5.clear();
	mListTrackPoints.clear();
	isFoundHand = false;
	foundCount = 0;
	mMessage = "none";
}

void Hand::findHand(cv::Mat image, int distance)
{
	if(image.empty())
		return;
	mImage = &image;

	if(distance < 58)
		return;
	if(distance > 99)
		return;

	distance *= 1.5f;
	float proportionCalibration = 73;
	int mixHandX = ((HAND_X_MIN*proportionCalibration)/(distance));
	int maxHandX = ((HAND_X_MAX*proportionCalibration)/(distance));
	mHandMin = cv::Point(mixHandX,mixHandX*HAND_Y_PROPORTION);
	mHandMax = cv::Point(maxHandX,maxHandX*HAND_Y_PROPORTION);

	if((int)mListTrackPoints.size() > HAND_TRACK_POINTS)
		mListTrackPoints.pop_front();
	mListTrackPoints.push_back(cv::Point(oldPoint));

	if(isFoundHand)
	{
		bool found = false;
		for(int y = oldPoint.y - HAND_X_MAX/2; y < oldPoint.y + HAND_X_MAX/2 && y < image.rows; y++)
		{
			if(y < 0)
				y = 0;
			for(int x = oldPoint.x - HAND_X_MAX/2; x < oldPoint.x + HAND_X_MAX/2 && x < image.cols; x++)
			{
				if(x < 0)
					x = 0;
				if(isInterest(x,y))
				{
					if(selectInterestRegion(x, y))
					{
						found = true;
						if(foundCount <= HAND_TIMES_FOUND)
							foundCount++;
						oldPoint = cv::Point(x,y);
						paintTrack();
						break;
					}
					else
					{
						foundCount--;
						if(foundCount > 0)
							return;
						if(foundCount == 0)
						{
							isFoundHand = false;
							break;
						}
					}
				}
			}
			if(found) return;
		}
	}

	bool found = false;
	for(int y = 0; y < mImage->rows; y += 3)
	{
		for(int x = 0; x < mImage->cols; x += 3)
		{
			if(isInterest(x,y))
			{
				if(selectInterestRegion(x, y))
				{
					found = true;
					if(isNearOldPoint(x,y))
					{
						foundCount++;
						oldPoint = cv::Point(x,y);
						if(foundCount > HAND_TIMES_FOUND)
							isFoundHand = true;
					}
					else
					{
						foundCount--;
						if(foundCount <= 0)
						{
							foundCount = 0;
							oldPoint = cv::Point(x,y);
						}
					}
				}
				x += mHandMax.x;
				break;
			}
		}
		if(found) break;
	}
}

bool Hand::isNearOldPoint(int x, int y)
{
	x = abs(oldPoint.x - x);
	y = abs(oldPoint.y - y);
	return (x < HAND_X_MAX && y < HAND_X_MAX) ? true : false;
}

bool Hand::selectInterestRegion(int x, int y)
{
	int startX = x - mHandMax.x/2;
	int startY = y;

	if(startX < 0 || startY < 0)
		return false;

	if(startX + mHandMax.x > mImage->cols || startY + mHandMax.y > mImage->rows )
		return false;

	int minX = -1;
	int maxX = 0;

	for(int x = startX - HAND_TEST_BORDER; x < startX + mHandMax.x + HAND_TEST_BORDER; x++)
		for(int y = startY - HAND_TEST_BORDER; y < startY + mHandMax.y + HAND_TEST_BORDER; y++)
		{
			if(isInterest(x,y))
			{
				if(minX == -1)
					minX = x;
				maxX = x;
				break;
			}
		}

	if( (maxX-minX) > mHandMax.x || (maxX-minX) < mHandMin.x )
		return false;

	HandProjections projs(mHandMax.x,mHandMax.y);
	projs.DEBUG_draw(mImage);

	for(int x = startX; x < startX + mHandMax.x; x++)
		for(int y = startY; y < startY + mHandMax.y; y++)
		{
			if(isInterest(x,y))
			{
				projs.addPointToProjections(x-startX,y-startY,mImage->at<uint8_t>(y,x));
			}
			paint(x,y);
		}

	projs.DEBUG_draw(mImage);

	if(mListResultHand1.size() < 3)
	{
		float result1 = getDataSetHand("hand1")->compare(projs);
		float result2 = getDataSetHand("hand2")->compare(projs);
		float result2_1 = getDataSetHand("hand2_1")->compare(projs);
		float result2_2 = getDataSetHand("hand2_2")->compare(projs);
		float result2_3 = getDataSetHand("hand2_3")->compare(projs);
		float result5 = getDataSetHand("hand5")->compare(projs);
		mListResultHand1.push_back(result1);
		mListResultHand2.push_back(result2);
		mListResultHand2_1.push_back(result2_1);
		mListResultHand2_2.push_back(result2_2);
		mListResultHand2_3.push_back(result2_3);
		mListResultHand5.push_back(result5);
	}
	else
	{
		float result1 = 0.0f;
		for(int i = 0; i < (int) mListResultHand1.size(); i++)
		{
			if(i != 0)
				result1 = (float) (result1+mListResultHand1[i])/2;
			else
				result1 = mListResultHand1[i];
		}

		float result2 = 0.0f;
		for(int i = 0; i < (int) mListResultHand2.size(); i++)
		{
			if(i != 0)
				result2 = (float) (result2+mListResultHand2[i])/2;
			else
				result2 = mListResultHand2[i];
		}

		float result2_1 = 0.0f;
		for(int i = 0; i < (int) mListResultHand2_1.size(); i++)
		{
			if(i != 0)
				result2_1 = (float) (result2_1+mListResultHand2_1[i])/2;
			else
				result2_1 = mListResultHand2_1[i];
		}

		float result2_2 = 0.0f;
		for(int i = 0; i < (int) mListResultHand2_2.size(); i++)
		{
			if(i != 0)
				result2_2 = (float) (result2_2+mListResultHand2_2[i])/2;
			else
				result2_2 = mListResultHand2_2[i];
		}

		float result2_3 = 0.0f;
		for(int i = 0; i < (int) mListResultHand2_3.size(); i++)
		{
			if(i != 0)
				result2_3 = (float) (result2_3+mListResultHand2_3[i])/2;
			else
				result2_3 = mListResultHand2_3[i];
		}

		float result5 = 0.0f;
		for(int i = 0; i < (int) mListResultHand5.size(); i++)
		{
			if(i != 0)
				result5 = (float) (result5+mListResultHand5[i])/2;
			else
				result5 = mListResultHand5[i];
		}

//		std::stringstream ss;
//		ss << result2 << " " << result2_1 << " ";
//		mMessage = ss.str();

//		mMessage = "none";
//		if(result1 < 0.20f)
//			mMessage = "hand01";
//		if(result2 < 0.20f)
//			mMessage = "hand02";

		std::string selected = "";

//		if(result2 < 0.2f &&
//			result2_1 > 0.7f &&
//			result2_2 > 0.6f)
//		{
//			selected = "hand01";
//		}
//		if(result2 > 0.15f && result2 < 0.37f &&
//		   result2_1 < 0.59f &&
//		   result2_2 < 0.60f)
//		{
//			selected = "hand02";
//		}
//		if(result2 > 0.25f && result2 < 0.32f &&
//		   result2_1 < 0.81f && result2_1 > 0.59f &&
//		   result2_2 > 0.64f)
//		{
//			selected = "hand03";
//		}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>UM DEDO<<<<<<<<<<<<<<<<<<<<<<<<
		if(result2 < 0.27f &&
			result2_1 > 0.7f &&
			result2_2 > 0.6f)
		{
			selected = "hand01";
		}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>DOIS DEDOS<<<<<<<<<<<<<<<<<<<<<<<<
		if(result2 > 0.15f && result2 < 0.35f &&
		   result2_1 < 0.69f &&
		   result2_2 < 0.63f &&
		   result5 < 0.57f)
		{
			selected = "hand02";
		}

		if(result2 > 0.15f && result2 < 0.49f &&
		   result2_1 < 0.45f &&
		   result2_2 < 0.45f &&
		   result5 < 0.58f)
		{
			selected = "hand02";
		}

		if(result2 > 0.15f && result2 < 0.37f &&
		   result2_1 < 0.80f &&
		   result2_2 < 0.55f &&
		   result5 < 0.50f)
		{
			selected = "hand02";
		}


		if(result2 > 0.15f && result2 < 0.47f &&
		   result2_1 < 0.50f &&
		   result2_2 < 0.50f &&
		   result5 < 0.55f)
		{
			selected = "hand02";
		}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>TRES DEDOS<<<<<<<<<<<<<<<<<<<<<<<<
		if(result2 > 0.25f && result2 < 0.36f &&
		   result2_1 < 0.91f && result2_1 > 0.60f &&
		   result2_2 > 0.64f &&
		   result5 > 0.25f && result5 < 0.33f)
		{
			selected = "hand03";
		}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>QUATRO DEDOS<<<<<<<<<<<<<<<<<<<<<<<<
		if( result2 > 0.36f && result2 < 0.46f &&
		   result2_1 > 0.80f &&
		   result2_2 > 0.74f &&  result2_2 < 0.85f &&
		   result5 > 0.24f && result5 < 0.47f)
		{
			selected = "hand04";
		}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>CINCO DEDOS<<<<<<<<<<<<<<<<<<<<<<<<

		if(result5 < 0.22f)
		{
			selected = "hand05";
		}


		mMessage = selected;

		std::cout << "hand2:" << result2 << std::endl
				<< "hand2_1:" << result2_1 << std::endl
				<< "hand2_2:" << result2_2 << std::endl;

		mListResultHand1.clear();
		mListResultHand2.clear();
		mListResultHand2_1.clear();
		mListResultHand2_2.clear();
	}

//	int diffX = (mHandMax.x - mHandMin.x) / 2;
	//int diffY = (mHandMax.y - mHandMin.y) / 2;
//	for(int x = startX + diffX; x < startX + diffX + mHandMin.x; x++)
//		for(int y = startY*1.09f; y < startY + mHandMin.y; y++)
//			paint(x,y);

	return true;
}

void Hand::paint(int x, int y)
{
	if(mImage->at<uint8_t>(y,x) < 64)
		mImage->at<uint8_t>(y,x) = 64;
	if(mImage->at<uint8_t>(y,x) > 64)
		mImage->at<uint8_t>(y,x) = 0;
}
void Hand::paintTrack()
{
	std::list<cv::Point>::iterator it;
	for(it = mListTrackPoints.begin(); it != mListTrackPoints.end(); it++)
	{
		for(int x = it->x - 5; x < it->x + 5; x++)
			for(int y = it->y - 5; y < it->y + 5; y++)
			{
				paint(x,y + mHandMax.y/2);
			}
	}
}

bool Hand::isInterest(int x, int y)
{
	if(mImage->at<uint8_t>(y,x) < 64)
		return false;
	return true;
}

void Hand::loadDataSetHand(std::string name)
{
	HandProjections projs;
	projs.loadFromFile(name);
	mMapHandProjections[name] = projs;
}

void Hand::loadDataSet()
{
	loadDataSetHand("hand1");
	loadDataSetHand("hand2");
	loadDataSetHand("hand2_1");
	loadDataSetHand("hand2_2");
	loadDataSetHand("hand2_3");
	loadDataSetHand("hand5");
}

HandProjections *Hand::getDataSetHand(std::string name)
{
	return &(mMapHandProjections[name]);
}

std::string Hand::getMessage()
{
	return mMessage;
}
