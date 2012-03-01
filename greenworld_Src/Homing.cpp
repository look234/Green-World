#include "Homing.h"
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
const int radius = 25; 
CHomingObject::CHomingObject(char* name, D3DXVECTOR3 location, float xspeed, float yspeed):
CIntelligentObject(ENEMYHOMING_OBJECT, name, location, xspeed, yspeed){ //constructor
  mHeight = location.y;
  mWidth = location.x;
  m_fDistance = m_fHorizontalDistance = m_fVerticalDistance = 0.0f;
  m_nLastAiTime = 0; m_nAiDelayTime = 0;

}

/// Intelligent move function.
/// Basically, just move like a dumb object then modify your private variables "intelligently".

void CHomingObject::move(){ //move object
 CGameObject::move(); //move like a dumb object
 HomingAi();
}

/// Main crow AI function.
/// The real work is done by a function for each state. Call the appropriate
/// function for the current state periodically, based on a timer.

void CHomingObject::ai(){ //main AI function
 //AttackingAi(direction);
}

/// Set the current state.
/// Change the current state, taking whatever actions are necessary on entering the new state.
/// \param state new state


/// AI for crows in the cruising state.
/// These crows are just cruising along, periodically lokoing for the plane.

void CHomingObject::HomingAi()
{	if(g_cTimer.elapsed(m_nHeightTime, m_nHeightDelayTime))
	{
		m_nHeightDelayTime=100+g_cRandom.number(0,2000);
	

	if(m_fHorizontalDistance > 0)
	{
		m_fXspeed = -15; 
	
	}
	 if(m_fHorizontalDistance < 0)
	{
		m_fXspeed = 15;

	}
	if(m_fVerticalDistance > 0)
	{
		m_fYspeed = 15; 
		
	}
	if(m_fVerticalDistance < 0)
	{
		m_fYspeed = -15;
	
	}
	

}}