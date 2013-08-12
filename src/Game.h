
#ifndef __MARY_GAME_H_
#define __MARY_GAME_H_

#include <memory>

#include "OgreHardwarePixelBuffer.h"
#include <vector>

namespace Ogre
{
	class RenderWindow;
	class RenderSystem;
	class Root;
}

namespace Mary
{
	/**
	 * Game interface. used by the Manager class 
	 */
	class Game
	{
	public:
		/**
		 *reference to all objects that can be manipulated by the Recognizer
		 *should not be used for now...
		 */
		static std::vector<Ogre::SceneNode*> mListTouchableModels;

		/**
		 *initializes the game
		 */
		bool start();
	
		/**
		 *reference to the graphic engine's root node
		 */
		std::auto_ptr<Ogre::Root> mRoot;
		/**
		 *reference to the graphic engine's window
		 */
		Ogre::RenderWindow* mWindow;

		/**
		 *constructor
		 */
		Game();
		/**
		 *destructor
		 */
		~Game();
	
		/**
		 *create a basic white sphere. Usually used in examples
		 *@param strName unique name for the sphere
		 *@param r the radius
		 *@param nRings number of rings to generate polygons for the sphere mesh. more rings, more polygons...
		 */
		void createSphere(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16);
		void createRecognizerImagePlane(std::string& name);
	
	protected:
		/**
		 *virtual method for initializations needed before the main loop 
		 */
		virtual void build() = 0;
		/**
		 *virtual method for the main game loop
		 */
		virtual void update(float elapsedSinceLastFrame) = 0;
		/**
		 *virtual method for code the runs after the main loop.
		 */
		virtual void destroy() = 0;

	private:
		Game(const Game& s);
		Game& operator=(const Game& s);
	};

}

#endif //__MARY_GAME_H_

