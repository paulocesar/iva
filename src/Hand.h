/*
 * Hand.h
 *
 *  Created on: 24/09/2011
 *      Author: root
 */

#ifndef HAND_H_
#define HAND_H_

#include <opencv.hpp>
#include <list>
#include <vector>
#include <map>

#include "ProjectionArray.h"
#include "HandProjections.h"

class Hand {
public:

	static const int HAND_TIMES_FOUND = 100;

	static const int HAND_TRACK_POINTS = 50;

	static const int HAND_X_MIN = 80;
	static const int HAND_X_MAX = 220;
	static const float HAND_Y_PROPORTION = 1;//1.5714;

	static const int HAND_TEST_BORDER = 3;


	static Hand *getInstance();

	HandProjections *getDataSetHand(std::string name);

	std::string getMessage();

	void findHand(cv::Mat image, int distance);

	void reset();

private:
	bool selectInterestRegion(int x, int y);
	bool isInterest(int x, int y);
	bool isNearOldPoint(int x, int y);

	void paint(int x, int y);
	void paintTrack();

	void loadDataSetHand(std::string name);
	void loadDataSet();

private:
	static Hand* mInstance;

	bool isFoundHand;
	int foundCount;
	cv::Point oldPoint;

	std::list<cv::Point> mListTrackPoints;

	Hand();
	virtual ~Hand();

	std::string mMessage;

	cv::Mat *mImage;

	cv::Point mHandMin;
	cv::Point mHandMax;

	std::vector<float> mListResultHand1;
	std::vector<float> mListResultHand2;
	std::vector<float> mListResultHand2_1;
	std::vector<float> mListResultHand2_2;
	std::vector<float> mListResultHand2_3;
	std::vector<float> mListResultHand5;
	std::map<std::string, HandProjections> mMapHandProjections;

};

#endif /* HAND_H_ */
