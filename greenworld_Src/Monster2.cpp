#include "Monster2.h"
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

CMonster2Object::CMonster2Object(char* name, D3DXVECTOR3 location, float xspeed, float yspeed):
CIntelligentObject(MONSTER2_OBJECT, name, location, xspeed, yspeed){ //constructor
  mHeight = location.y;
  mWidth = location.x;
  m_fDistance = m_fHorizontalDistance = m_fVerticalDistance = 0.0f;
  m_eState = CRUISINGS2_STATE;
  m_nLastAiTime = 0; m_nAiDelayTime = 0;
}

/// Intelligent move function.
/// Basically, just move like a dumb object then modify your private variables "intelligently".

void CMonster2Object::move(){ //move object
  CGameObject::move(); //move like a dumb object
  ai(); //act intelligently
}

/// Main crow AI function.
/// The real work is done by a function for each state. Call the appropriate
/// function for the current state periodically, based on a timer.

void CMonster2Object::ai(){ //main AI function
  //do the following periodically
  if(g_cTimer.elapsed(m_nLastAiTime,m_nAiDelayTime)) 
    switch(m_eState){ //behavior depends on state
      case CRUISINGS2_STATE: CruisingAi(); break;
      case AVOIDINGS2_STATE: AvoidingAi(); break;
	  case CHASINGS2_STATE: ChasingAi(); break;
	  case WAITS2_STATE: WaitingAi(); break;
      default: break;
    }
}

/// Set the current state.
/// Change the current state, taking whatever actions are necessary on entering the new state.
/// \param state new state

void CMonster2Object::SetState(States2Type state){

  m_eState=state; //change state

  switch(m_eState){ //change behavior settings

    case CRUISINGS2_STATE:
      m_nAiDelayTime=400+g_cRandom.number(0,200);
      m_nHeightDelayTime=3000+g_cRandom.number(0,5000);
      break;

    case AVOIDINGS2_STATE:
      m_nAiDelayTime=250+g_cRandom.number(0,250); 
      m_fXspeed=-3;
      break;
	case CHASINGS2_STATE:
			
      //m_nAiDelayTime=400+g_cRandom.number(0,200);
      m_nHeightDelayTime=300+g_cRandom.number(0,50);
	  break;
	case WAITS2_STATE:
	  m_nAiDelayTime=250+g_cRandom.number(0,250); 
	  break;

    default: break;
  }
}

/// AI for crows in the cruising state.
/// These crows are just cruising along, periodically lokoing for the plane.

void CMonster2Object::CruisingAi(){ //just cruising along
	
if(g_cTimer.elapsed(m_nHeightTime, m_nHeightDelayTime))
	{
	int rand = g_cRandom.number(1,1000);
	m_nHeightDelayTime=100+g_cRandom.number(0,500);
	
	if(rand % 3 == 0)
	{
		rand = g_cRandom.number(1,1000);
		if(rand%2 == 0)
		{
		m_fXspeed = 0;
		m_fXspeed = 8;
		}
		else
		{
		m_fYspeed = 0;
		m_fYspeed = 8;
		}
	}
	else if(rand%3 == 1)
	{
		rand = g_cRandom.number(1,1000);
		if(rand%2 == 0)
		{
		m_fXspeed = 0;
		m_fXspeed = -8;
		}
		else
		{
		m_fYspeed = 0;
		m_fYspeed = -8;
		}
	}
	else
	{
		m_fXspeed = m_fYspeed = 0;
	}
	if(CLOSE_DISTANCE > m_fDistance)
	SetState(CHASINGS2_STATE);
	}

}

/// AI for crows in the avoinding state.
/// These crows are trying to get away from the plane.

void CMonster2Object::AvoidingAi(){ //avoiding plane

  //height variation
  //if(g_cTimer.elapsed(m_nHeightTime, m_nHeightDelayTime)){
  //  m_nDesiredHeight = g_cRandom.number(100,450);
  //  if(m_nDesiredHeight < m_structLocation.y)
  //    m_fYspeed= -(float)g_cRandom.number(3,7);
  //  if(m_nDesiredHeight > m_structLocation.y)
  //    m_fYspeed = (float)g_cRandom.number(3,7);
  //  m_nHeightDelayTime = 3000+g_cRandom.number(0,2000);
  //}

  ////speed variation
  //if(g_cTimer.elapsed(m_nSpeedVariationTime, m_nSpeedVariationDuration)){
  //    m_fXspeed = (float)-g_cRandom.number(7,10); 
  //    m_nSpeedVariationDuration = 10000+g_cRandom.number(0,3000);
  //}
  //if(m_fHorizontalDistance > BEHIND_DISTANCE) //if behind
  //  m_fXspeed = -1; //slow down 

  ////look for plane, maybe leave avoiding state
  //if(m_fDistance>FAR_DISTANCE|| //if far away, or
  //  (m_fDistance<CLOSE_DISTANCE&& //close and
  //    m_fVerticalDistance>FALLBACK_DISTANCE)) //higher or lower
    SetState(CRUISINGS2_STATE); //then back to cruising

}

void CMonster2Object::ChasingAi()
{
	
	if(g_cTimer.elapsed(m_nHeightTime, m_nHeightDelayTime))
	{
		m_nHeightDelayTime=100+g_cRandom.number(0,50);
	

	if(m_fHorizontalDistance > 0)
	{
		m_fXspeed = (float)-g_cRandom.number(7,10); 
	
	}
	 if(m_fHorizontalDistance < 0)
	{
		m_fXspeed = (float)-g_cRandom.number(-7,-10);

	}
	if(m_fVerticalDistance > 0)
	{
		m_fYspeed = (float)-g_cRandom.number(-7,-10); 
		
	}
	if(m_fVerticalDistance < 0)
	{
		m_fYspeed = (float)-g_cRandom.number(7,10);
	
	}
	if(CLOSE_DISTANCE > m_fDistance)
		SetState(CRUISINGS2_STATE);
}
}
void CMonster2Object::WaitingAi(){ //avoiding plane
	/*
  //height variation
  if(g_cTimer.elapsed(m_nHeightTime, m_nHeightDelayTime)){
    m_nDesiredHeight = g_cRandom.number(100,450);
    if(m_nDesiredHeight < m_structLocation.y)
      m_fYspeed= -(float)g_cRandom.number(3,7);
    if(m_nDesiredHeight > m_structLocation.y)
      m_fYspeed = (float)g_cRandom.number(3,7);
    m_nHeightDelayTime = 3000+g_cRandom.number(0,2000);
  }

  //speed variation
  if(g_cTimer.elapsed(m_nSpeedVariationTime, m_nSpeedVariationDuration)){
      m_fXspeed = (float)-g_cRandom.number(7,10); 
      m_nSpeedVariationDuration = 10000+g_cRandom.number(0,3000);
  }
  if(m_fHorizontalDistance > BEHIND_DISTANCE) //if behind
    m_fXspeed = -1; //slow down 
  */
  //look for plane, maybe leave avoiding state
    /*if(m_fDistance < CLOSE_DISTANCE&&
      m_fHorizontalDistance+100 > BEHIND_DISTANCE)*/
    SetState(CRUISINGS2_STATE);
}
