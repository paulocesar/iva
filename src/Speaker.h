/*
 * Speaker.h
 *
 *  Created on: 06/10/2011
 *      Author: root
 */

#ifndef SPEAKER_H_
#define SPEAKER_H_

#include <string>
#include <map>

#include <OgreMesh.h>
#include <OgreEntity.h>
#include <OgreAnimationState.h>
#include <OgreAnimationTrack.h>

namespace Mary {
	class Speaker {
	public:
		Speaker(std::string name);
		virtual ~Speaker();

		void configureAnimMesh(unsigned int quantAnim, Ogre::MeshPtr mesh);
		void configureAnimState(Ogre::Entity *entity);

		void setMapLetters(int a, int e, int i, int o, int u);

		void speak(std::string message);
		bool isSpeaking();

		void update(float elapsedSinceLastFrame);

	private:
		std::string mName;
		std::string mMessage;

		bool mSpeakActived;

		int mLetterPos;
		int mLetterInt;
		int mLetterIntOld;

		std::map<char,int> mMapLetters;

		float mAnimValue;

		Ogre::VertexPoseKeyFrame* mManualKeyFrame;
		Ogre::AnimationState* mManualAnimState;
	};
}


#endif /* SPEAKER_H_ */
