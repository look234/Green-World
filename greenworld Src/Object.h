/// \file object.h
/// Interface for the game object class CGameObject.
/// Copyright Ian Parberry, 2004
/// Last updated July 14, 2010

#pragma once

#include "sprite.h"
#include "defines.h"
#include "tinyxml.h"

/// The game object. Game objects are responsible for remembering information about 
/// themselves - including their image, location, and speed - and for moving and
/// drawing themselves.

class CGameObject{ //class for a game object

	friend class CObjectManager;

	protected:
		D3DXVECTOR3 m_structLocation; ///< Current location.
		float m_fXspeed,m_fYspeed; ///< Current speed.
		int m_nLastMoveTime; ///< Last time moved.
		int m_nMinXSpeed,m_nMaxXSpeed; ///< Min, max horizontal speeds.
		int m_nMinYSpeed,m_nMaxYSpeed; ///< Min,max vertical speeds.
		int m_nCurrentFrame; ///< Frame to be displayed.
		int m_nFrameCount; ///< Number of frames in animation.
		int m_nLastFrameTime; ///< Last time the frame was changed.
		int m_nFrameInterval; ///< Interval between frames.
		int* m_pAnimation; ///< Sequence of frame numbers to be repeated.
		int m_nAnimationFrameCount; ///< Number of entries in m_pAnimation.
		int m_nBounceCount; ///< Number of bounces.
		ObjectType m_nObjectType; ///< Object type.
		int m_nWidth,m_nHeight; ///< Width and height of sprite.
		int m_nBirthTime; ///< Time of creation.
		int m_nLifeTime; //< Time that object lives.
		int m_nHP; ///< HP for object if it has any.
		int m_nEXP; ///< EXP for hero only and the amount each enemy gives.
		BOOL m_bVulnerable; ///< Vulnerable to bullets.
		BOOL m_bIntelligent; ///< TRUE for an intelligent object.
		BOOL m_bCanFly; ///< TRUE for an object that can fly, all other fall.
		BOOL m_bCycleSprite; ///< TRUE to cycle sprite frames, otherwise play once.
		void LoadSettings(char* name); //< Load object-dependent settings from XML element.

		int waitTime;
		void wait();

	public:
		CGameObject(ObjectType object, char* name, D3DXVECTOR3 location,
			float xspeed, float yspeed); ///< Constructor.
		~CGameObject(); //< Destructor.
		void draw(); ///< Draw at current location.
		void draw(int key); ///< Draw at current location.
		virtual void move();
		virtual void move2();
		void draw(char letter); ///< Draw at current location.
		void draw2(int key); ///< Draw at current location.


		//virtual void move(float &x, float &y);
		///< Change location depending on time and speed
		void accelerate(int xdelta,int ydelta=0); //change speed
		void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight();
		void noMove();

		int width;

		float step;
};