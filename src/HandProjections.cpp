/*
 * HandProjections.cpp
 *
 *  Created on: 25/09/2011
 *      Author: root
 */

#include "HandProjections.h"

HandProjections::HandProjections(int sizeProjX, int sizeProjY) : mProjX(sizeProjX), mProjY(sizeProjY) {
	// TODO Auto-generated constructor stub

}

HandProjections::~HandProjections() {
	// TODO Auto-generated destructor stub
}

void HandProjections::loadFromFile(std::string name)
{
	mProjX = ProjectionArray();
	mProjY = ProjectionArray();

	std::string filename;

	filename = name;
	filename += "ProjX.txt";
	mProjX.load(filename);

	filename = name;
	filename += "ProjY.txt";
	mProjY.load(filename);
}

void HandProjections::saveToFile(std::string name)
{
	std::string filename;

	filename = name;
	filename += "ProjX.txt";
	mProjX.save(filename);

	filename = name;
	filename += "ProjY.txt";
	mProjY.save(filename);
}

void HandProjections::setProjectionArray(ProjectionArray &projX, ProjectionArray &projY)
{
	mProjX = projX;
	mProjY = projY;
}

float HandProjections::compare(HandProjections &handProj)
{
	return compare(handProj.mProjX,handProj.mProjY);
}

float HandProjections::compare(ProjectionArray &projX, ProjectionArray &projY)
{
	float diffX = mProjX.getDifferenceNormalized(projX);
	float diffY = mProjY.getDifferenceNormalized(projY);
	float depth = mProjX.getDepthAvg() - projX.getDepthAvg();
	if(depth < 0)
		depth *=(-1);
	return /*depth + */diffX + diffY;
}

ProjectionArray& HandProjections::getProjectionX()
{
	return mProjX;
}

ProjectionArray& HandProjections::getProjectionY()
{
	return mProjY;
}

void HandProjections::addPointToProjections(int x, int y, int depth)
{
	mProjX.incrementProjection(x,y,depth);
	mProjY.incrementProjection(y,x,depth);
}

void HandProjections::DEBUG_draw(cv::Mat *image)
{

	mProjX.DEBUG_draw(image,true);
	mProjY.DEBUG_draw(image,false);
}
