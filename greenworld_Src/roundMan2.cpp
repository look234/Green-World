#include "roundMan2.h"
#include "timer.h" //game timer
#include "random.h" //for random number generator
#include "Objman.h"
#include <stdio.h>
#include <math.h>
extern CTimer g_cTimer;  //game timer
extern CRandom g_cRandom; //random number generator
extern CObjectManager g_cObjectManager;
const int CLOSE_DISTANCE=500; ///< Distance for close to plane
const int FAR_DISTANCE=150; ///< Distance for "far from" plane.
const int FALLBACK_DISTANCE=150; ///< Fall back at this vertical distance from plane
 int BEHIND_DISTANCE=0; ///< Horizontal distance considered to be behind plane
float xballSpeed;
float yballSpeed;
bool timeswap= false;
int tests = 0;
CRoundMan2Object::CRoundMan2Object(char* name, D3DXVECTOR3 location, float xspeed, float yspeed):
CIntelligentObject(ROUNDMAN2_OBJECT, name, location, xspeed, yspeed){ //constructor
	 loc = location;
  mHeight = location.y;
  mWidth = location.x;
  m_fDistance = m_fHorizontalDistance = m_fVerticalDistance = 0.0f;
  m_eState = WAITS6_STATE;
  m_nLastAiTime = 0; m_nAiDelayTime = 0;
}

/// Intelligent move function.
/// Basically, just move like a dumb object then modify your private variables "intelligently".

void CRoundMan2Object::move(){ //move object
  CGameObject::move(); //move like a dumb object
  ai(); //act intelligently
}

/// Main crow AI function.
/// The real work is done by a function for each state. Call the appropriate
/// function for the current state periodically, based on a timer.

void CRoundMan2Object::ai(){ //main AI function
  //do the following periodically
  if(g_cTimer.elapsed(m_nLastAiTime,m_nAiDelayTime)) 
    switch(m_eState){ //behavior depends on state
      case CRUISINGS6_STATE: CruisingAi(); break;
      case AVOIDINGS6_STATE: AvoidingAi(); break;
	  case CHASINGS6_STATE: ChasingAi(); break;
	  case WAITS6_STATE: WaitingAi(); break;
      default: break;
    }
}

/// Set the current state.
/// Change the current state, taking whatever actions are necessary on entering the new state.
/// \param state new state

void CRoundMan2Object::SetState(States6Type state){

  m_eState=state; //change state

  switch(m_eState){ //change behavior settings

    case CRUISINGS6_STATE:
     
      m_nHeightDelayTime=3000+g_cRandom.number(0,5000);
      break;

    case AVOIDINGS6_STATE:
      
      m_fXspeed=-3;
      break;
	case CHASINGS6_STATE:
    
      m_nHeightDelayTime=300+g_cRandom.number(0,50);
	  break;
	case WAITS6_STATE:
	
	  break;

    default: break;
  }
}

/// AI for crows in the cruising state.
/// These crows are just cruising along, periodically lokoing for the plane.

void CRoundMan2Object::CruisingAi(){ //just cruising along

	
if(g_cTimer.elapsed(m_nHeightTime, 1500))
	{
		loc.x = xdistance;
		loc.y = ydistance;
		m_fXspeed=m_fYspeed=0;
	m_nHeightDelayTime=100+g_cRandom.number(0,100);
	
	g_cObjectManager.enemyFireball(1, loc, 0, -50);
	g_cObjectManager.enemyFireball(1, loc, (float)35.35533906, (float)-35.35533906);
	g_cObjectManager.enemyFireball(1, loc, 50, 0);
	g_cObjectManager.enemyFireball(1, loc, (float)35.35533906, (float)35.35533906);
	g_cObjectManager.enemyFireball(1, loc, 0, 50);
	g_cObjectManager.enemyFireball(1, loc, (float)-35.35533906, (float)35.35533906);
	g_cObjectManager.enemyFireball(1, loc, -50, 0);
	g_cObjectManager.enemyFireball(1, loc, (float)-35.35533906, (float)-35.35533906);

	if(FAR_DISTANCE < m_fDistance)
		SetState(CHASINGS6_STATE);
	}

}

/// AI for crows in the avoinding state.
/// These crows are trying to get away from the plane.

void CRoundMan2Object::AvoidingAi(){ //avoiding plane
SetState(CRUISINGS6_STATE); //then back to cruising
}

void CRoundMan2Object::ChasingAi() {
	
	
	if(g_cTimer.elapsed(m_nHeightTime, m_nHeightDelayTime))
	{
		loc.x = xdistance;
		loc.y = ydistance;
		m_nHeightDelayTime=100+g_cRandom.number(0,2000);
		//BEHIND_DISTANCE=100+g_cRandom.number(0,5000);
		if(BEHIND_DISTANCE%3==0)
		g_cObjectManager.enemyHoming(1, loc, 0, 0);

	if(m_fHorizontalDistance > 0)
	{
		m_fXspeed = (float)-g_cRandom.number(7,10); 
	BEHIND_DISTANCE++;
	}
	 if(m_fHorizontalDistance < 0)
	{
		m_fXspeed = (float)-g_cRandom.number(-7,-10);
		BEHIND_DISTANCE++;

	}
	if(m_fVerticalDistance > 0)
	{
		m_fYspeed = (float)-g_cRandom.number(-7,-10); 
		BEHIND_DISTANCE++;
		
	}
	if(m_fVerticalDistance < 0)
	{
		m_fYspeed = (float)-g_cRandom.number(7,10);
		BEHIND_DISTANCE++;
	
	}
	if(FAR_DISTANCE > m_fDistance)
	{m_fXspeed=m_fYspeed=0;
	SetState(CRUISINGS6_STATE);
	}}
}

void CRoundMan2Object::WaitingAi(){ //avoiding plane

	m_fXspeed=m_fYspeed=0;
	if(CLOSE_DISTANCE > m_fDistance)
		SetState(CRUISINGS6_STATE);

}