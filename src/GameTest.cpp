#include "GameTest.h"


#include <cstdlib>

#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreWindowEventUtilities.h"
#include "OgreMaterialManager.h"
#include "OgreEntity.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreSubEntity.h"
#include "OgreMeshManager.h"
#include "OgreSceneNode.h"
#include "TextRenderer.h"

#include "Manager.h"


namespace Mary
{

	GameTest::GameTest() : mAvatar("Manual")
	{
		pos = 0.01f;
		press = false;
//		animValue = 0.0f;

		mDoRecognition = false;

		mState = BEGIN;
		mStatus = DONE;
		mHand = NONE;

		mTestHand = "";
	}


	GameTest::~GameTest(void)
	{
	}
	
	void GameTest::build()
	{
		mSound = 0;
		mChannel = 0;
		mSystem = 0;
		mTrack = 0;

		mResult = FMOD::System_Create(&mSystem);
		mSystem->init(32, FMOD_INIT_NORMAL, 0);
		mSystem->createSound("media/sound/playlist.m3u", FMOD_DEFAULT, 0, &mPlaylist);

		Ogre::Root* lRoot = mRoot.get();
		Ogre::RenderWindow* lWindow = mWindow;

		Ogre::SceneManager* lScene = lRoot->getSceneManager("MaryEngineSceneManager");
		Ogre::SceneNode* lRootSceneNode = lScene->getRootSceneNode();


		Ogre::Camera* lCamera = lScene->createCamera("MaryEngineCamera");

		Ogre::SceneNode* lCameraNode = lRootSceneNode->createChildSceneNode("MaryEngineCameraNode");
		lCameraNode->attachObject(lCamera);
	

		float lViewportWidth = 1.0f;//0.88f;
		float lViewportHeight = 1.0f; //0.88f;
		float lViewportLeft	= (1.0f - lViewportWidth) * 0.5f;
		float lViewportTop = (1.0f - lViewportHeight) * 0.5f;
		unsigned short lMainViewportZOrder = 100;
		Ogre::Viewport * vp = lWindow->addViewport(lCamera, lMainViewportZOrder, lViewportLeft, lViewportTop, lViewportWidth, lViewportHeight);

		vp->setAutoUpdated(true);
 
		vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

		float ratio = float(vp->getActualWidth()) / float(vp->getActualHeight());
		lCamera->setAspectRatio(ratio);

		lCamera->setNearClipDistance(1.5f);
		lCamera->setFarClipDistance(3000.0f); 

		lWindow->setActive(true);
		lWindow->setAutoUpdated(false);

		Ogre::SceneNode* lLightSceneNode = NULL;
		{
			Ogre::Light* lLight = lScene->createLight();
			lLight->setType(Ogre::Light::LT_POINT);
			lLight->setPosition(0.0f,0.0f,0.0f);

			lLight->setDiffuseColour(5.5f, 5.5f, 5.0f);
			lLight->setSpecularColour(5.0f, 5.0f, 5.0f);

			lLightSceneNode = lRootSceneNode->createChildSceneNode();
			lLightSceneNode->attachObject(lLight);
		}
	

		//Ogre::MaterialManager& lMaterialManager = Ogre::MaterialManager::getSingleton();

		Ogre::String lNameOfResourceGroup = "Mary Engine Test";
		{
			Ogre::ResourceGroupManager& lRgMgr = Ogre::ResourceGroupManager::getSingleton();
			lRgMgr.createResourceGroup(lNameOfResourceGroup);
		
			Ogre::String lDirectoryToLoadTextures = "media/mesh";
			bool lIsRecursive = false;
			lRgMgr.addResourceLocation(lDirectoryToLoadTextures, "FileSystem", lNameOfResourceGroup, lIsRecursive);
		
			lRgMgr.initialiseResourceGroup(lNameOfResourceGroup);

			lRgMgr.loadResourceGroup(lNameOfResourceGroup);


			Ogre::MaterialManager& lMaterialManager = Ogre::MaterialManager::getSingleton();
			//material from OpenCV cam
			Ogre::MaterialPtr materialOgrePtr = Ogre::MaterialManager::getSingleton().getByName("Ogre");
			{
				Ogre::MaterialPtr lMaterial = lMaterialManager.create("OpenCVImage",lNameOfResourceGroup);
				Ogre::Technique* lFirstTechnique = lMaterial->getTechnique(0);
				Ogre::Pass* lFirstPass = lFirstTechnique->getPass(0);

				Ogre::Image img;

				//getting image from OpenCV
				Manager::getInstance()->loadImageFromRecognizer(&img);

				Ogre::TexturePtr mCaptureTexture = Ogre::TextureManager::getSingleton().loadImage(
					"exemploDeTeste",
					Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
					img,
					Ogre::TEX_TYPE_2D);
				cvBuffer = mCaptureTexture->getBuffer();

				Ogre::TextureUnitState* lTextureUnit_LM = lFirstPass->createTextureUnitState();
				lTextureUnit_LM->setTextureName("exemploDeTeste", Ogre::TEX_TYPE_2D);
				lTextureUnit_LM->setTextureCoordSet(1);

			}


			//creating OpenCV screen and setting OpenCV Image Texture
			{
				Ogre::String lNameOfTheMesh = "OpenCVMesh";
				createRecognizerImagePlane(lNameOfTheMesh);
				Ogre::Entity* lEntity = lScene->createEntity(lNameOfTheMesh);

				Ogre::SceneNode* lNode = lRootSceneNode->createChildSceneNode();
				lNode->attachObject(lEntity);
				lNode->setScale(Ogre::Vector3(1.0f,1.0f,1.0f));
				lNode->setPosition(9.0f,6.5f,-20.75f);
				const Ogre::String& lMaterialName = "OpenCVImage";
				lEntity->setMaterialName(lMaterialName);

				//lNode->setScale(0.2,0.2,0.2);
			}

			//HAND NUMBER ONE
			{
				Ogre::Plane plane(Ogre::Vector3::UNIT_Z, 0);
				Ogre::MeshManager::getSingleton().createPlane("hand01", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				        plane, 4, 3, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);

				Ogre::Entity* entHand = lScene->createEntity("hand01Entity", "hand01");
				entHand->setMaterialName("Background/HandOne");
				entHand->setCastShadows(false);

				Ogre::SceneNode* lNode = lRootSceneNode->createChildSceneNode("hand01Node");
				lNode->attachObject(entHand);
				lNode->setPosition(-6.0f,-3.0f,-15.75f);
			}

			//HAND NUMBER TWO
			{
				Ogre::Plane plane(Ogre::Vector3::UNIT_Z, 0);
				Ogre::MeshManager::getSingleton().createPlane("hand02", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				        plane, 4, 3, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);

				Ogre::Entity* entHand = lScene->createEntity("hand02Entity", "hand02");
				entHand->setMaterialName("Background/HandTwo");
				entHand->setCastShadows(false);

				Ogre::SceneNode* lNode = lRootSceneNode->createChildSceneNode("hand02Node");
				lNode->attachObject(entHand);
				lNode->setPosition(6.0f,-3.0f,-15.75f);
			}

			//creating Facial Animation
			{  

				Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().load("facial.mesh", lNameOfResourceGroup);

				mAvatar.configureAnimMesh(15,mesh);
				mAvatar.setMapLetters(0,0,4,5,5);

				Ogre::Entity* lEntity = lScene->createEntity("facial.mesh");
				lEntity->getSubEntity(1)->setMaterialName("drbunsen_glasses_frame");
				lEntity->getSubEntity(2)->setMaterialName("drbunsen_eyes");
				lEntity->getSubEntity(4)->setMaterialName("drbunsen_tongue");

				mSpeakAnimState = lEntity->getAnimationState("Speak");

//				Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().load("char.mesh", lNameOfResourceGroup);
//				mAvatar.configureAnimMesh(3,mesh);
//				mAvatar.setMapLetters(0,0,1,2,2);
//
//				Ogre::Entity* lEntity = lScene->createEntity("char.mesh");


				mAvatar.configureAnimState(lEntity);

				Ogre::SceneNode* lNode = lRootSceneNode->createChildSceneNode("OgreMove");
				lNode->attachObject(lEntity);
				lNode->setScale(Ogre::Vector3(0.13f,0.13f,0.13f));
//				lNode->setScale(Ogre::Vector3(1.5f,1.5f,1.5f));
//				lNode->setPosition(0.0f,-2.0f,-3.75f);
				lNode->setPosition(0.0f,-2.0f,-15.75f);
				//mListTouchableModels.push_back(lNode);
			}
		}

		{
			new TextRenderer();
			TextRenderer::getSingleton().addTextBox("txtGreeting", "Hello", 10, 10, 100, 20, Ogre::ColourValue::Green);
		}

		//TEMP RECOGNIZER
		//Manager::getInstance()->setActivedGameRecongizer(true);
	}

	void GameTest::executeTerminalCommand(std::string command)
	{
		system(command.c_str());
	}

	void GameTest::update(float elapsedSinceLastFrame)
	{
		Ogre::Root* lRoot = mRoot.get();
		Ogre::RenderWindow* lWindow = mWindow;

		Manager::getInstance()->loadSharedDataFromRecognizer(cvBuffer);

		updateStates(elapsedSinceLastFrame);

		updateSpeak(elapsedSinceLastFrame);

		TextRenderer::getSingleton().setText("txtGreeting", mTestHand);

		lWindow->update(false);
		bool lVerticalSynchro = true;
		lWindow->swapBuffers(lVerticalSynchro);
		lRoot->renderOneFrame();
		lRoot->_updateAllRenderTargets();
	}

	void GameTest::updateStates(float elapsedSinceLastFrame)
	{
		switch(mState)
		{
		case BEGIN:
			break;
		case GREETING:
			if(mStatus == START)
			{
				//speak(0);
				//Manager::getInstance()->speak("Do the left gesture to execute firefox and the right gesture to execute gedit.");
				mAvatar.speak("Do the left gesture to execute firefox and the right gesture to execute gedit.");
				mStatus = DOING;
			}
			else
			{
				if(mStatus != DOING)
					break;
				if(!mAvatar.isSpeaking())
					mStatus = DONE;
			}
			break;
		case READING:
			if(mStatus == START)
			{
				Manager::getInstance()->setActivedGameRecongizer(true);
				mDoRecognition = true;
				mStatus = DOING;
			}
			else
			{
				if(mStatus != DOING)
					break;

				mHand = NONE;
				if(mTestHand.compare("hand01") == 0)
					mHand = ONE;
				if(mTestHand.compare("hand02") == 0)
					mHand = TWO;

				if(mHand != NONE)
				{
					mDoRecognition = false;
					mStatus = DONE;
				}
			}
			break;
		case WARNING:
			if(mStatus == START)
			{
				//TODO: add new audio for test
				Manager::getInstance()->setActivedGameRecongizer(false);
				mAvatar.speak("OK, I Understood.");
				mStatus = DOING;
			}
			else
			{
				if(!isSpeaking())
					mStatus = DONE;
			}
			break;
		case EXECUTING:

			if(mHand == ONE)
				executeTerminalCommand("firefox");
			if(mHand == TWO)
				executeTerminalCommand("gedit");

			mHand = NONE;
			mStatus = DONE;

			/*TEMP TEST*/ //exit(0);
			break;
		}

		if(mStatus == DONE)
			nextState();
	}

	void GameTest::nextState()
	{
		mStatus = START;
		switch(mState)
		{
		case GREETING:
			mState = READING;
			break;
		case READING:
			mState = WARNING;
			break;
		case WARNING:
			mState = EXECUTING;
			break;
		case BEGIN:
		case EXECUTING:
			mState = GREETING;
			break;
		}
	}

	void GameTest::updateSpeak(float elapsedSinceLastFrame)
	{
		mAvatar.update(elapsedSinceLastFrame);
	}

	void GameTest::speak(int track)
	{

        mResult = mPlaylist->getTag("FILE", track, &mTag);
        if (mResult == FMOD_OK)
        {
        	mTrack = track;

            file = "media/sound/";
            file += (char *)mTag.data;
            std::cout << file << std::endl;

            mResult = mSystem->createSound(file.c_str(), FMOD_DEFAULT, 0, &mSound);
    		mResult = mSystem->playSound(FMOD_CHANNEL_FREE, mSound, false, &mChannel);
        }
	}

	bool GameTest::isSpeaking()
	{
		if(!mChannel)
			return false;

		bool isPlaying;
		mChannel->isPlaying(&isPlaying);
		return isPlaying;
	}

	void GameTest::destroy()
	{
		mListTouchableModels.clear();
		Ogre::Root* lRoot = mRoot.get();
		Ogre::RenderWindow* lWindow = mWindow;

		Ogre::SceneManager* lScene = lRoot->getSceneManager("MaryEngineSceneManager");
		Ogre::SceneNode* lRootSceneNode = lScene->getRootSceneNode();
		
		{
			lWindow->removeAllViewports();
		}
		{
			lScene->destroyAllCameras();
			lScene->destroyAllManualObjects();
			lScene->destroyAllEntities();
			lScene->destroyAllLights();
			lRootSceneNode->removeAndDestroyAllChildren();
		}
		{
			Ogre::ResourceGroupManager& lRgMgr = Ogre::ResourceGroupManager::getSingleton();
			lRgMgr.destroyResourceGroup("Mary Engine Test");
		}
	}

	void GameTest::listen(std::string &signal, void *data)
	{
		mTestHand = signal;
	}
}
