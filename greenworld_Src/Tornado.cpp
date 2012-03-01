//#include "Barrier.h"
//#include <math.h>
//#include <stdio.h>
//#include "timer.h" //game timer
//#include "random.h" //for random number generator
//#include "sound.h" //for sound manager
//#define PI 3.14159265
//extern CTimer g_cTimer;  //game timer
//extern CRandom g_cRandom; //random number generator
//extern CSoundManager* g_pSoundManager;
//const int CLOSE_DISTANCE=100; ///< Distance for close to plane
//const int FAR_DISTANCE=500; ///< Distance for "far from" plane.
//const int FALLBACK_DISTANCE=150; ///< Fall back at this vertical distance from plane
//const int BEHIND_DISTANCE=-5; ///< Horizontal distance considered to be behind plane
//const int radius = 25; 
//CBarrierObject::CBarrierObject(char* name, D3DXVECTOR3 location, float xspeed, float yspeed):
//CIntelligentObject(BARRIER_OBJECT, name, location, xspeed, yspeed){ //constructor
//  mHeight = location.y;
//  mWidth = location.x;
//  m_fDistance = m_fHorizontalDistance = m_fVerticalDistance = 0.0f;
//  m_nLastAiTime = 0; m_nAiDelayTime = 0;
//   if(xspeed > 0)
//	  direction = 1;
//  else if(xspeed < 0)
//	  direction = 2;
//  else if(yspeed > 0)
//	  direction = 3;
//  else if(yspeed < 0)
//	  direction = 4;
//}
//
///// Intelligent move function.
///// Basically, just move like a dumb object then modify your private variables "intelligently".
//
//void CBarrierObject::move(){ //move object
//  CGameObject::move(); //move like a dumb object
// DefendingAi(direction);
//}
//
///// Main crow AI function.
///// The real work is done by a function for each state. Call the appropriate
///// function for the current state periodically, based on a timer.
//
//void CBarrierObject::ai(){ //main AI function
// //AttackingAi(direction);
//}
//
///// Set the current state.
///// Change the current state, taking whatever actions are necessary on entering the new state.
///// \param state new state
//
//
///// AI for crows in the cruising state.
///// These crows are just cruising along, periodically lokoing for the plane.
//
//void CBarrierObject::DefendingAi(int direct)
//{
//		if(g_cTimer.elapsed(m_nHeightTime, m_nHeightDelayTime))
//	{
//	int rand = g_cRandom.number(1,1000);
//	m_nHeightDelayTime=100+g_cRandom.number(0,500);
//	
//	if(direct == 1)
//	{
//		rand = g_cRandom.number(1,1000);
//		if(rand%2 == 0)
//		{
//		//m_fXspeed = 0;
//		m_fYspeed = -8;
//		}
//		else
//		{
//		//m_fYspeed = 0;
//		m_fYspeed = 8;
//		}
//	}
//	else if(direct == 2)
//	{
//		rand = g_cRandom.number(1,1000);
//		if(rand%2 == 0)
//		{
//		//m_fXspeed = 0;
//		m_fYspeed = -8;
//		}
//		else
//		{
//		//m_fYspeed = 0;
//		m_fYspeed = 8;
//		}
//	}
//	else if(direct == 3)
//	{
//		rand = g_cRandom.number(1,1000);
//		if(rand%2 == 0)
//		{
//		//m_fXspeed = 0;
//		m_fXspeed = -8;
//		}
//		else
//		{
//		//m_fYspeed = 0;
//		m_fXspeed = 8;
//		}
//	}
//	else if(direct == 4)
//	{
//		rand = g_cRandom.number(1,1000);
//		if(rand%2 == 0)
//		{
//		//m_fXspeed = 0;
//		m_fXspeed = -8;
//		}
//		else
//		{
//		//m_fYspeed = 0;
//		m_fXspeed = 8;
//		}
//	}
//
//	}
//	/*if(m_fVerticalDistance > 0)
//		m_fYspeed = 50;
//	if(m_fVerticalDistance < 0)
//		m_fYspeed = -50;
//	if(m_fHorizontalDistance > 0)
//		m_fXspeed = -50;
//	if(m_fHorizontalDistance < 0)
//		m_fXspeed = 50;*/
//	
//	/*float rad = 0;
//	
//	for(float i = 1; i < 360; i++)
//	{
//		if(g_cTimer.elapsed(m_nHeightTime, 10))
//		{
//			
//	mHeight = yPlanePosition + radius*cos(i*PI/180);
//	mWidth = xPlanePosition + radius*sin(i*PI/180);
//	if(m_fDistance != radius)
//	{
//	if(ydistance < mHeight)
//		m_fYspeed = -5;
//	if(ydistance > mHeight)
//		m_fYspeed = 5;
//	if(xdistance > mWidth)
//		m_fXspeed = -5;
//	if(xdistance < mWidth)
//		m_fXspeed = 5;
//		}
//		}
//	}*/
//}











#include "Tornado.h"
#include "timer.h" //game timer
#include "random.h" //for random number generator
#include "sound.h" //for sound manager

extern CTimer g_cTimer;  //game timer
extern CRandom g_cRandom; //random number generator
extern CSoundManager* g_pSoundManager;
const int CLOSE_DISTANCE=100; ///< Distance for close to plane
const int FAR_DISTANCE=500; ///< Distance for "far from" plane.
const int FALLBACK_DISTANCE=150; ///< Fall back at this vertical distance from plane
const int BEHIND_DISTANCE=-5; ///< Horizontal distance considered to be behind plane

CTornadoObject::CTornadoObject(char* name, D3DXVECTOR3 location, float xspeed, float yspeed):
CIntelligentObject(TORNADO_OBJECT, name, location, xspeed, yspeed){ //constructor
  mHeight = location.y;
  mWidth = location.x;
  m_fDistance = m_fHorizontalDistance = m_fVerticalDistance = 0.0f;
  m_nLastAiTime = 0; m_nAiDelayTime = 0;
  if(xspeed > 0)
	  direction = 1;
  else if(xspeed < 0)
	  direction = 2;
  else if(yspeed > 0)
	  direction = 3;
  else if(yspeed < 0)
	  direction = 4;
}

/// Intelligent move function.
/// Basically, just move like a dumb object then modify your private variables "intelligently".

void CTornadoObject::move(){ //move object
  CGameObject::move(); //move like a dumb object
 AttackingAi(direction);
}

/// Main crow AI function.
/// The real work is done by a function for each state. Call the appropriate
/// function for the current state periodically, based on a timer.

void CTornadoObject::ai(){ //main AI function
 //AttackingAi(direction);
}

/// Set the current state.
/// Change the current state, taking whatever actions are necessary on entering the new state.
/// \param state new state


/// AI for crows in the cruising state.
/// These crows are just cruising along, periodically lokoing for the plane.

void CTornadoObject::AttackingAi(int direct)
{
	if(g_cTimer.elapsed(m_nHeightTime, m_nHeightDelayTime))
	{
	int rand = g_cRandom.number(1,1000);
	m_nHeightDelayTime=100+g_cRandom.number(0,500);
	
	if(direct == 1)
	{
		rand = g_cRandom.number(1,1000);
		if(rand%2 == 0)
		{
		//m_fXspeed = 0;
		m_fYspeed = -8;
		}
		else
		{
		//m_fYspeed = 0;
		m_fYspeed = 8;
		}
	}
	else if(direct == 2)
	{
		rand = g_cRandom.number(1,1000);
		if(rand%2 == 0)
		{
		//m_fXspeed = 0;
		m_fYspeed = -8;
		}
		else
		{
		//m_fYspeed = 0;
		m_fYspeed = 8;
		}
	}
	else if(direct == 3)
	{
		rand = g_cRandom.number(1,1000);
		if(rand%2 == 0)
		{
		//m_fXspeed = 0;
		m_fXspeed = -8;
		}
		else
		{
		//m_fYspeed = 0;
		m_fXspeed = 8;
		}
	}
	else if(direct == 4)
	{
		rand = g_cRandom.number(1,1000);
		if(rand%2 == 0)
		{
		//m_fXspeed = 0;
		m_fXspeed = -8;
		}
		else
		{
		//m_fYspeed = 0;
		m_fXspeed = 8;
		}
	}

	}
}