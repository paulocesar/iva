#ifndef __MARY_RECOGNIZER_H_
#define __MARY_RECOGNIZER_H_

#include "OgrePixelFormat.h"
#include <string>

namespace Mary {
	
	/**
	 * Recognizer interface. used by the Manager class 
	 */
	class Recognizer
	{
	public:
		/**
		 *constructor
		 */
		Recognizer(void);
		/**
		 *destructor
		 */
		~Recognizer(void);

	public:
		/**
		 *virtual method for recognizer capturing and image calculations
		 */
		virtual void update() = 0;
		virtual void clear() = 0;

	public:
		
		/**
		 *virtual method to get the image unformated data
		 */
		virtual unsigned char* getData() = 0;
		/**
		 *virtual method to get the image width
		 */
		virtual double getWidth() = 0;
		/**
		 *virtual method to get the image height
		 */
		virtual double getHeight() = 0;
		/**
		 *virtual method to get the pixel format
		 */
		virtual Ogre::PixelFormat getPixelFormat() = 0;
	
	};

}

#endif //__MARY_RECOGNIZER_H_
