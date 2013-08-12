
#include "Game.h"

#include <exception>

#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreRenderWindow.h"
#include "OgreWindowEventUtilities.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreManualObject.h"

#include "EasyDefines.h"

namespace Mary
{
	std::vector<Ogre::SceneNode*> Game::mListTouchableModels;

	// the constructor uses the initialisation list to have a proper state.
	Game::Game()//Recognizer *recog)
		:mRoot(NULL), mWindow(NULL)//, mRecognizer(recog)
	{
	}

	// the destructor frees memory allocated by the class. 
	Game::~Game()
	{
		mWindow = NULL;
		//mRoot.reset();// I was not obliged to do that...
	}


	bool Game::start()
	{
		bool result = false;
		try
		{
			// STEP 1/ First, we will need to create the Ogre::Root object.
			Ogre::String lConfigFileName = "";
			Ogre::String lPluginsFileName = "";
			Ogre::String lLogFileName = "Ogre.log";

			mRoot = std::auto_ptr<Ogre::Root>(
				new Ogre::Root(lConfigFileName, lPluginsFileName, lLogFileName));

			// STEP 2/ Then we need to load plugins. It means that there are functions that are stored inside dynamic libraries.
			{
				typedef std::vector<Ogre::String> Strings;

				Strings lPluginNames;
				lPluginNames.push_back("RenderSystem_GL");
				//lPluginNames.push_back("RenderSystem_Direct3D9");
				//lPluginNames.push_back("Plugin_ParticleFX");
				//lPluginNames.push_back("Plugin_CgProgramManager");
				//lPluginNames.push_back("Plugin_PCZSceneManager");
				//lPluginNames.push_back("Plugin_OctreeZone");
				lPluginNames.push_back("Plugin_OctreeSceneManager");
				//lPluginNames.push_back("Plugin_BSPSceneManager");

				{
					Strings::iterator lIter = lPluginNames.begin();
					Strings::iterator lIterEnd = lPluginNames.end();
					for(;lIter != lIterEnd; lIter++)
					{
						Ogre::String& lPluginName = (*lIter);
						bool lIsInDebugMode = OGRE_DEBUG_MODE;
						if(lIsInDebugMode)
						{
							lPluginName.append("_d");
						}
						mRoot->loadPlugin(lPluginName);
					}
				}
			}

			// STEP 3/ Then, we can select from the loaded plugins the unique RenderSystem we want to use.
			{
				const Ogre::RenderSystemList& lRenderSystemList = mRoot->getAvailableRenderers();
				if( lRenderSystemList.size() == 0 )
				{
					MWARNING("Sorry, no rendersystem was found.");
					return result;
				}

				Ogre::RenderSystem *lRenderSystem = lRenderSystemList[0];
				mRoot->setRenderSystem(lRenderSystem);
			}

			// STEP 4/ When the RenderSystem is selected, we can initialise the Root. The root can be initialised only when a rendersystem has been selected.
			{
				bool lCreateAWindowAutomatically = false;
				Ogre::String lWindowTitle = "";
				Ogre::String lCustomCapacities = "";
				mRoot->initialise(lCreateAWindowAutomatically, lWindowTitle, lCustomCapacities);
			}

			// STEP 5/ Then we can ask to the RenderSystem to create a window.
			{
				Ogre::String lWindowTitle = "Hello Ogre World";
				unsigned int lSizeX = 800;
				unsigned int lSizeY = 600;
				bool lFullscreen = false; 
				// This is just an example of parameters that we can put. Check the API for more details.
				Ogre::NameValuePairList lParams;
				// fullscreen antialiasing. (check wikipedia if needed).
				lParams["FSAA"] = "0"; 
				// vertical synchronisation will prevent some image-tearing, but also
				// will provide smooth framerate in windowed mode.(check wikipedia if needed).
				lParams["vsync"] = "true";
				mWindow = mRoot->createRenderWindow(lWindowTitle, lSizeX, lSizeY, lFullscreen, &lParams);
			}
			result = true;
			

			mRoot.get()->createSceneManager(Ogre::ST_GENERIC, "MaryEngineSceneManager");

			build();

			mRoot->clearEventTimes();
			
			unsigned long lastUpdateMs = mRoot.get()->getTimer()->getMilliseconds();
			while(!mWindow->isClosed())
			{
				unsigned long timeNowMs = mRoot.get()->getTimer()->getMilliseconds();
				float deltaTimeSecs = (timeNowMs - lastUpdateMs) * 0.001f;
				lastUpdateMs = timeNowMs;
				update(deltaTimeSecs);
				Ogre::WindowEventUtilities::messagePump();
			}

			destroy();
			exit(0);
		}catch(Ogre::Exception &e)
		{
			MWARNING("!!!!Ogre::Exception!!!!" << e.what());
			result = false;
		}catch(std::exception &e)
		{
			MWARNING("!!!!std::exception!!!!" << e.what());
			result = false;
		}
		return result;
	}
	
	void Game::createSphere(const std::string& strName, const float r, const int nRings, const int nSegments) 
	{ 
		Ogre::SceneManager *sceneMgr = mRoot->getSceneManager("MaryEngineSceneManager");
		Ogre::ManualObject * manual = sceneMgr->createManualObject(strName); 
		manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);  
		float fDeltaRingAngle = (Ogre::Math::PI / nRings); 
		float fDeltaSegAngle = (2 * Ogre::Math::PI / nSegments); 
		unsigned short wVerticeIndex = 0 ;  
		// Generate the group of rings for the sphere 
		for( int ring = 0; ring <= nRings; ring++ ) 
		{ 
			float r0 = r * sinf (ring * fDeltaRingAngle); float y0 = r * cosf (ring * fDeltaRingAngle);  
			// Generate the group of segments for the current ring 
			for(int seg = 0; seg <= nSegments; seg++) 
			{ 
				float x0 = r0 * sinf(seg * fDeltaSegAngle); 
				float z0 = r0 * cosf(seg * fDeltaSegAngle);  
				// Add one vertex to the strip which makes up the sphere 
				manual->position( x0, y0, z0); manual->normal(Ogre::Vector3(x0, y0, z0).normalisedCopy()); 
				manual->textureCoord((float) seg / (float) nSegments, (float) ring / (float) nRings);  if (ring != nRings) 
				{ 
					// each vertex (except the last) has six indicies pointing to it 
					manual->index(wVerticeIndex + nSegments + 1); manual->index(wVerticeIndex);  
					manual->index(wVerticeIndex + nSegments); manual->index(wVerticeIndex + nSegments + 1); 
					manual->index(wVerticeIndex + 1); manual->index(wVerticeIndex); wVerticeIndex ++; 
				} 
			}; // end for seg 
		} // end for ring 
		manual->end(); 
		Ogre::MeshPtr mesh = manual->convertToMesh(strName); 
		mesh->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3(-r, -r, -r), Ogre::Vector3(r, r, r) ), false ); 
		mesh->_setBoundingSphereRadius(r); 
		unsigned short src, dest; 
		if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest)) 
		{ 
			mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest); 
		} 
	}

	
	void Game::createRecognizerImagePlane(std::string& name)
	{
		Ogre::ManualObject * lManualObject = NULL;
		Ogre::String lManualObjectName = name;
		lManualObject = mRoot.get()->getSceneManager("MaryEngineSceneManager")->createManualObject(lManualObjectName);

		bool lDoIWantToUpdateItLater = false;
		lManualObject->setDynamic(lDoIWantToUpdateItLater);

		float lSize = 2.0f;
		float lYDist = 0.5f;

		lManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
		{
			float cp = 1.0f * lSize ;
			float cm = -1.0f * lSize;
			float lDiffuseScale = 3.0f;
			float lLightmapScale = 1.0f;

			lManualObject->position(cm, cp-lYDist, 0.0f);// a vertex
			lManualObject->colour(Ogre::ColourValue(0.0f,1.0f,0.0f,1.0f));
			lManualObject->normal(0.0, 0.0, 1.0f);
			lManualObject->textureCoord(0.0f, 0.0f);
			lManualObject->textureCoord(0.0f, 0.0f);

			lManualObject->position(cp, cp-lYDist, 0.0f);// a vertex
			lManualObject->colour(Ogre::ColourValue(1.0f,1.0f,0.0f,1.0f));
			lManualObject->normal(0.0, 0.0, 1.0f);
			lManualObject->textureCoord(lDiffuseScale, 0.0f);
			lManualObject->textureCoord(lLightmapScale, 0.0f);

			lManualObject->position(cp, cm+lYDist, 0.0f);// a vertex
			lManualObject->colour(Ogre::ColourValue(1.0f,0.0f,0.0f,1.0f));
			lManualObject->normal(0.0, 0.0, 1.0f);
			lManualObject->textureCoord(lDiffuseScale, lDiffuseScale);
			lManualObject->textureCoord(lLightmapScale, lLightmapScale);

			lManualObject->position(cm, cm+lYDist, 0.0f);// a vertex
			lManualObject->colour(Ogre::ColourValue(0.0f,0.0f,0.0f,1.0f));
			lManualObject->normal(0.0, 0.0, 1.0f);
			lManualObject->textureCoord(0.0, lDiffuseScale);
			lManualObject->textureCoord(0.0, lLightmapScale);

			lManualObject->triangle(2,1,0);
			lManualObject->triangle(0,3,2);
		}
		lManualObject->end();
		
		lManualObject->convertToMesh(name);
	}

}


