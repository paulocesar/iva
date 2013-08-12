#ifndef __MARY_GAMETEST_H_
#define __MARY_GAMETEST_H_

#include <fmod.hpp>

#include "Game.h"
#include "Speaker.h"
#include "Recognizer.h"
#include "SignalListener.h"

#include <OgreAnimationState.h>
#include <OgreAnimationTrack.h>
#include <OgreKeyFrame.h>

namespace Mary {

	class GameTest : public Game, public SignalListener
	{

	/*
	 * automata
	 */
	public:
		enum Status {START, DOING, DONE};
		enum States {BEGIN, GREETING, READING, WARNING, EXECUTING};
	private:
		Status mStatus;
		States mState;
		void updateStates(float elapsedSinceLastFrame);
		void nextState();

	public:
		GameTest();
		~GameTest(void);

	/*
	 * game engine inheritance
	 */
	protected:
		void build();
		void update(float elapsedSinceLastFrame);
		void destroy();

		void listen(std::string &signal, void *data);
		
		void executeTerminalCommand(std::string command);

		void speak(int track);
		bool isSpeaking();
		void updateSpeak(float elapsedSinceLastFrame);

	/*
	 * hand recognition
	 */
	public:
		enum Hand {NONE, ONE, TWO};
	protected:
		bool mDoRecognition;
		Hand mHand;

		std::string mTestHand;
	/*
	 * ogre3d stuffs
	 */
	private:
		float pos;
		bool press;
		Ogre::HardwarePixelBufferSharedPtr cvBuffer;

	private:
//		float animValue;
//		Ogre::VertexPoseKeyFrame* mManualKeyFrame;
//		Ogre::AnimationState* mManualAnimState;
		Speaker mAvatar;
		Ogre::AnimationState* mSpeakAnimState;

	/*
	 * sound system
	 */
	private:
		FMOD_RESULT mResult;
		FMOD::Channel *mChannel;
		FMOD::System *mSystem;

		std::string file;
		FMOD::Sound *mPlaylist;
	    FMOD_TAG mTag;
	    int mTrack;

		FMOD::Sound *mSound;
	};

}

#endif //__MARY_GAMETEST_H_
