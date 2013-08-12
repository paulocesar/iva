#ifndef __MARY_TEXTRENDERER_H_
#define __MARY_TEXTRENDERER_H_

#include "Ogre.h"
#include "OgreSingleton.h"
 
namespace Mary 
{
	class TextRenderer : public Ogre::Singleton<Mary::TextRenderer>
	{
	private:
 
		Ogre::OverlayManager*    _overlayMgr;
		Ogre::Overlay*           _overlay;
		Ogre::OverlayContainer*  _panel;
 
	public:
 
		TextRenderer();
		~TextRenderer();
 
		void addTextBox(
			const std::string& ID,
			const std::string text,
			Ogre::Real x, Ogre::Real y,
			Ogre::Real width, Ogre::Real height,
			const Ogre::ColourValue& color = Ogre::ColourValue(1.0, 1.0, 1.0, 1.0));
 
		void removeTextBox(const std::string& ID);
 
		void setText(const std::string& ID, const std::string Text);
		const std::string& getText(const std::string& ID);
	};
}

#endif //__MARY_TEXTRENDERER_H_
