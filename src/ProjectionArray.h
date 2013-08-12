/*
 * ProjectionArray.h
 *
 *  Created on: 25/09/2011
 *      Author: root
 */

#ifndef PROJECTION_ARRAY_H_
#define PROJECTION_ARRAY_H_

#include <opencv.hpp>
#include <vector>
#include <string>

#define Projection cv::Point

class ProjectionArray : protected std::vector<Projection>{
public:
	void DEBUG_draw(cv::Mat *image, bool inBegin = true);

	void load(std::string filename);
	void save(std::string filename);

	ProjectionArray(int size = 0);
	virtual ~ProjectionArray();

	void incrementProjection(int pos, int value, int depth);

	int getPointMin(int pos);
	int getPointMax(int pos);
	int getPointValue(int pos);

	int getMin();
	int getMax();

	int getDepthMin();
	int getDepthMax();
	int getDepthAvg();

	float getDifferenceNormalized(ProjectionArray &proj);
	float getDepthAvgNormalized();

protected:
	float getDifferenceProjections(int pos, ProjectionArray &proj);
	int getCompatiblePosition(int pos, ProjectionArray &proj);

	void setPointMinOrMax(int pos, int value);

	int stringToInt(std::string value);

private:
	int mDepthMin;
	int mDepthMax;
	int mDepthAvg;

	int mMin;
	int mMax;
};

#endif /* PROJECTION_H_ */
