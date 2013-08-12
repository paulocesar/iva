/*
 * TTSFestival.h
 *
 *  Created on: 04/10/2011
 *      Author: root
 */

#ifndef __MARY_TTSFESTIVAL_H_
#define __MARY_TTSFESTIVAL_H_

#include <string>

#include <festival.h>

#include "TTS.h"

namespace Mary {

	class TTSFestival : public TTS
	{
	public:
		TTSFestival();
		virtual ~TTSFestival();

		virtual void update();

		virtual bool play(std::string message);
		virtual bool isPlaying();
		virtual void stop();

	private:
		bool mIsPlaying;
	};

}

#endif /* __MARY_TTSFESTIVAL_H_ */
