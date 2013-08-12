/*
 * TTS.h
 *
 *  Created on: 04/10/2011
 *      Author: root
 */

#ifndef __MARY_TTS_H_
#define __MARY_TTS_H_

#include <string>

namespace Mary {

	class TTS {
	public:
		TTS();
		virtual ~TTS();

		virtual void update() = 0;

		virtual bool play(std::string message) = 0;
		virtual bool isPlaying() = 0;
		virtual void stop() = 0;
	};

}

#endif /* __MARY_TTS_H_ */
