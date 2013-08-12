/*
 * ProjectionArray.cpp
 *
 *  Created on: 25/09/2011
 *      Author: root
 */

#include "ProjectionArray.h"

#include <fstream>

ProjectionArray::ProjectionArray(int size) : std::vector<Projection>() {
	// TODO Auto-generated constructor stub
	for(int i = 0; i < size; i++)
		push_back(Projection(-1,-1));

	mMin = -1;
	mMax = -1;
	mDepthMin = -1;
	mDepthMax = -1;

}

ProjectionArray::~ProjectionArray() {
	// TODO Auto-generated destructor stub
}

void ProjectionArray::incrementProjection(int pos, int value, int depth)
{
	if( pos < 0 || value < 0 || depth < 0)
		return;

	if(pos >= (int) size())
		return;

	if(mMin == -1)
	{
		mMin = value;
		mMax = value;
	}
	else
	{
		if(value < mMin)
			mMin = value;
		if(value > mMax)
			mMax = value;
	}

	if(mDepthMin == -1)
	{
		mDepthMin = depth;
		mDepthMax = depth;
		mDepthAvg = depth;
	}
	else
	{
		if(depth > mDepthMax)
			mDepthMax = depth;
		if(depth < mDepthMin)
			mDepthMin = depth;
		mDepthAvg = (mDepthAvg+depth)/2;
	}

	setPointMinOrMax(pos,value);
}

void ProjectionArray::setPointMinOrMax(int pos, int value)
{
	if(at(pos).x == -1)
	{
		(*this)[pos] = cv::Point(value,value);
	}
	else
	{
		if(at(pos).x > value)
			(*this)[pos] = cv::Point(value,at(pos).y);
		if(at(pos).y < value)
			(*this)[pos] = cv::Point(at(pos).x,value);
	}
}

int ProjectionArray::getPointMin(int pos)
{
	return at(pos).x;
}

int ProjectionArray::getPointMax(int pos)
{
	return at(pos).y;
}

int ProjectionArray::getPointValue(int pos)
{
	return at(pos).y - at(pos).x;
}

int ProjectionArray::getMin()
{
	return mMin;
}

int ProjectionArray::getMax()
{
	return mMax;
}

int ProjectionArray::getDepthMin()
{
	return mDepthMin;
}

int ProjectionArray::getDepthMax()
{
	return mDepthMax;
}

int ProjectionArray::getDepthAvg()
{
	return mDepthAvg;
}

float ProjectionArray::getDepthAvgNormalized()
{
	return (float) (mDepthAvg-mDepthMin)/(mDepthMax-mDepthMin);
}

float ProjectionArray::getDifferenceNormalized(ProjectionArray &proj)
{
	float diff = 0.0f;
	for(int i = 0; i < (int) proj.size(); i++)
		diff += getDifferenceProjections(i,proj);
	return diff/proj.size();
}

float ProjectionArray::getDifferenceProjections(int pos, ProjectionArray &proj)
{
	float p1Normalized = (float) proj.getPointValue(pos)/proj.mMax;

	int ownPos = getCompatiblePosition(pos,proj);
	float p2Normalized = (float) getPointValue(ownPos)/mMax;

	float diff = p1Normalized - p2Normalized;
	if(diff < 0)
		diff *= (-1);
	return diff;
}

int ProjectionArray::getCompatiblePosition(int pos, ProjectionArray &proj)
{
	float normalized = (float)(pos)/(proj.size()-1);
	return normalized*(size()-1);
}

void ProjectionArray::DEBUG_draw(cv::Mat *image, bool inBegin)
{
	int width = size();
	int heigth = mMax;
	for(int x = 0; x < width; x++)
	{
		int value = getPointValue(x);
		for(int y = 0; y < heigth; y++)
		{

			if(value > 0)
			{
				value--;
				if(inBegin)
					image->at<uint8_t>(x,y) = 128;
				else
					image->at<uint8_t>((image->rows-width)+x,y) = 128;
			}
			else
			{
				if(inBegin)
					image->at<uint8_t>(x,y) = 0;
				else
					image->at<uint8_t>((image->rows-width)+x,y) = 0;
			}
		}
	}
}

void ProjectionArray::load(std::string filename)
{
	std::ifstream file(filename.c_str());
	std::string line;
	int x, y;

	if(file.is_open())
	{
		getline(file,line);
		mMin = stringToInt(line);
		getline(file,line);
		mMax = stringToInt(line);
		getline(file,line);
		mDepthMin = stringToInt(line);
		getline(file,line);
		mDepthMax = stringToInt(line);
		getline(file,line);
		mDepthAvg = stringToInt(line);
		while(file.good())
		{
			getline(file,line);
			x = stringToInt(line);
			getline(file,line);
			y = stringToInt(line);
			push_back(Projection(x,y));
		}
	}

	file.close();
}

void ProjectionArray::save(std::string filename)
{
	std::ofstream file(filename.c_str());
	if(file.is_open())
	{
		file << mMin << std::endl
			 << mMax << std::endl
			 << mDepthMin << std::endl
			 << mDepthMax << std::endl
			 << mDepthAvg;

		std::vector<Projection>::iterator it;
		for(it = begin(); it != end(); it++)
			file << std::endl << it->x << std::endl << it->y;
	}
	file.close();
}

int ProjectionArray::stringToInt(std::string value)
{
	std::stringstream ss(value);
	int valRet;
	ss >> valRet;
	return valRet;
}
