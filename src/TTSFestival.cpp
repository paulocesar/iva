/*
 * TTSFestival.cpp
 *
 *  Created on: 04/10/2011
 *      Author: root
 */

#include "TTSFestival.h"

namespace Mary {

	TTSFestival::TTSFestival() {
		// TODO Auto-generated constructor stub

		  festival_initialize(1,210000);
		  //festival_say_text("this is a simple test of threading situation");

		  mIsPlaying = false;
	}

	TTSFestival::~TTSFestival() {
		// TODO Auto-generated destructor stub
	}

	void TTSFestival::update() {
	}

	bool TTSFestival::play(std::string message) {
		mIsPlaying = true;
		festival_say_text(message.c_str());
		mIsPlaying = false;
		return true;
	}

	bool TTSFestival::isPlaying() {
		return mIsPlaying;
	}

	void TTSFestival::stop() {

	}
}
