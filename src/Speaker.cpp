/*
 * Speaker.cpp
 *
 *  Created on: 06/10/2011
 *      Author: root
 */

#include "Speaker.h"
#include "Manager.h"

#include <OgreAnimation.h>

namespace Mary {
	Speaker::Speaker(std::string name) {
		// TODO Auto-generated constructor stub
		mAnimValue = 0.0f;
		mName = name;

		mSpeakActived = false;

	}

	void Speaker::setMapLetters(int a, int e, int i, int o, int u)
	{
		mMapLetters['a'] = a;
		mMapLetters['e'] = e;
		mMapLetters['i'] = i;
		mMapLetters['o'] = o;
		mMapLetters['u'] = u;
	}

	Speaker::~Speaker() {
		// TODO Auto-generated destructor stub
	}

	void Speaker::configureAnimMesh(unsigned int quantAnim, Ogre::MeshPtr mesh)
	{
//		bool needVertexAnimation = true;
//		bool needSkeletalAnimation = true; // assumed you need this too? If not disable
//		Ogre::VertexData* vdata = mesh->sharedVertexData;
//		Ogre::VertexDeclaration* newdcl = vdata->vertexDeclaration->getAutoOrganisedDeclaration(needSkeletalAnimation, needVertexAnimation);
//		vdata->reorganiseBuffers(newdcl);
		mManualKeyFrame = mesh->createAnimation(mName, 1)->createVertexTrack(4, Ogre::VAT_POSE)->createVertexPoseKeyFrame(0);
//        Ogre::Animation* anim = mesh->createAnimation(mName, 0);
//        /*Ogre::VertexPoseKeyFrame**/ mManualKeyFrame = anim->createVertexTrack(0, Ogre::VAT_POSE)->createVertexPoseKeyFrame(0);
		for (unsigned int i = 0; i < quantAnim; i++) mManualKeyFrame->addPoseReference(i, 0.0f);

		std::cout
		<< std::endl
		<< "!!!!!!!!!!!! TAMANHO DO VETOR DE ANIMACOES"
		<< mManualKeyFrame->getPoseReferences().size();
	}

	void Speaker::configureAnimState(Ogre::Entity *entity)
	{
		mManualAnimState = entity->getAnimationState(mName);
	}


	void Speaker::update(float elapsedSinceLastFrame)
	{
		if(Manager::getInstance()->isSpeaking() && mSpeakActived)
		{
			if(!mManualAnimState->hasEnded())
				mManualAnimState->setEnabled(true);

			mAnimValue += elapsedSinceLastFrame+0.07f;

			if(mAnimValue > 1)
			{
				if(mLetterPos >= (int) mMessage.size())
					mSpeakActived = false;

				mAnimValue = 0;
				mLetterPos++;

				mLetterIntOld = mLetterInt;
				mLetterInt= mMapLetters[mMessage[mLetterPos]];
			}

			if(mSpeakActived)
			{
//				std::cout << "Posicao Atual: " << mLetterInt << "   Posicao Antiga: " << mLetterIntOld << std::endl;
				mManualKeyFrame->updatePoseReference(mLetterInt,mAnimValue);
				if(mLetterPos)
					mManualKeyFrame->updatePoseReference(mLetterIntOld,1.0f-mAnimValue);
				mManualAnimState->getParent()->_notifyDirty();
			}
		}
		else
		{
			mAnimValue = 0.0f;
			if(mManualAnimState->hasEnded())
				mManualAnimState->setEnabled(false);
		}
	}


	void Speaker::speak(std::string message)
	{
		if(!Manager::getInstance()->isSpeaking())
		{
			mMessage = "";

			for(int i = 0; i < (int) message.size(); i++)
			{
				char letter = message[i];
				map<char,int>::iterator it = mMapLetters.begin();
				for(;it != mMapLetters.end(); it++)
				{
					if(it->first == letter)
					{
						mMessage += letter;
						break;
					}
				}
			}

			mSpeakActived = true;
			mLetterPos = 0;
			mLetterInt = 0;

			Manager::getInstance()->speak(message);
		}
	}

	bool Speaker::isSpeaking()
	{
		return Manager::getInstance()->isSpeaking();
	}
}
