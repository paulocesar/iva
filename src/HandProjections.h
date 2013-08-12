/*
 * HandProjections.h
 *
 *  Created on: 25/09/2011
 *      Author: root
 */

#ifndef HANDPROJECTIONS_H_
#define HANDPROJECTIONS_H_

#include "ProjectionArray.h"
#include <opencv.hpp>

class HandProjections {
public:
	void DEBUG_draw(cv::Mat *image);

	HandProjections(int sizeProjX = 0, int sizeProjY = 0);
	virtual ~HandProjections();

	void addPointToProjections(int x, int y, int depth);

	void setProjectionArray(ProjectionArray &projX, ProjectionArray &projY);

	void loadFromFile(std::string name);
	void saveToFile(std::string name);

	float compare(HandProjections &handProj);
	float compare(ProjectionArray &projX, ProjectionArray &projY);

	ProjectionArray& getProjectionX();
	ProjectionArray& getProjectionY();

private:
	ProjectionArray mProjX;
	ProjectionArray mProjY;
};

#endif /* HANDPROJECTIONS_H_ */
