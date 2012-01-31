#include "flameGuy.h"
#include "timer.h" //game timer
#include "random.h" //for random number generator
#include "Sound.h"

extern CTimer g_cTimer;  //game timer
extern CRandom g_cRandom; //random number generator
extern CSoundManager* g_pSoundManager;
D3DXVECTOR3 home;
const int CLOSE_DISTANCE=150; ///< Distance for close to plane
const int FAR_DISTANCE=500; ///< Distance for "far from" plane.
const int FALLBACK_DISTANCE=150; ///< Fall back at this vertical distance from plane
const int BEHIND_DISTANCE=-5; ///< Horizontal distance considered to be behind plane

CFlameGuyObject::CFlameGuyObject(char* name, D3DXVECTOR3 location, float xspeed, float yspeed):
CIntelligentObject(FLAMEGUY_OBJECT, name, location, xspeed, yspeed){ //constructor
  home = location;
  mHeight = location.y;
  mWidth = location.x;
  m_fDistance = m_fHorizontalDistance = m_fVerticalDistance = 0.0f;
  m_eState = CRUISINGS3_STATE;
  m_nLastAiTime = 0; m_nAiDelayTime = 0;
}

/// Intelligent move function.
/// Basically, just move like a dumb object then modify your private variables "intelligently".

void CFlameGuyObject::move(){ //move object
  CGameObject::move(); //move like a dumb object
  ai(); //act intelligently
}

/// Main crow AI function.
/// The real work is done by a function for each state. Call the appropriate
/// function for the current state periodically, based on a timer.

void CFlameGuyObject::ai(){ //main AI function
  //do the following periodically
  if(g_cTimer.elapsed(m_nLastAiTime,m_nAiDelayTime)) 
    switch(m_eState){ //behavior depends on state
      case CRUISINGS3_STATE: CruisingAi(); break;
      case AVOIDINGS3_STATE: AvoidingAi(); break;
	  case CHASINGS3_STATE: ChasingAi(); break;
	  case WAITS3_STATE: WaitingAi(); break;
      default: break;
    }
}

/// Set the current state.
/// Change the current state, taking whatever actions are necessary on entering the new state.
/// \param state new state

void CFlameGuyObject::SetState(States3Type state){

  m_eState=state; //change state

  switch(m_eState){ //change behavior settings

    case CRUISINGS3_STATE:
      m_nAiDelayTime=400+g_cRandom.number(0,200);
      m_nHeightDelayTime=3000+g_cRandom.number(0,5000);
      break;

    case AVOIDINGS3_STATE:
      m_nAiDelayTime=250+g_cRandom.number(0,250); 
      m_fXspeed=-3;
      break;
	case CHASINGS3_STATE:
      //m_nAiDelayTime=400+g_cRandom.number(0,200);
      m_nHeightDelayTime=300+g_cRandom.number(0,50);
	  break;
	case WAITS3_STATE:
	  m_nAiDelayTime=250+g_cRandom.number(0,250); 
	  break;

    default: break;
  }
}

/// AI for crows in the cruising state.
/// These crows are just cruising along, periodically lokoing for the plane.

void CFlameGuyObject::CruisingAi(){ //just cruising along
int x=0;

if(g_cTimer.elapsed(m_nHeightTime, m_nHeightDelayTime))
	{
	int rand = g_cRandom.number(1,3000);
	m_nHeightDelayTime=100+g_cRandom.number(0,1000);
	
		if(rand%2 == 1)
	{
		m_fXspeed = 5;
		
	}
	else 
	{
		m_fXspeed = -5;
		
	}
	
	if(m_fDistance < CLOSE_DISTANCE){
		g_pSoundManager->play(DEADCROW14_SOUND);
		SetState(CHASINGS3_STATE);}
	}

}

/// AI for crows in the avoinding state.
/// These crows are trying to get away from the plane.

void CFlameGuyObject::AvoidingAi(){ //avoiding plane
		if(xdistance > home.x)
	{
		m_fXspeed = (float)-g_cRandom.number(7,10); 
	
	}
	if(xdistance < home.x)
	{
		m_fXspeed = (float)-g_cRandom.number(-7,-10);

	}
	if(ydistance > home.y)
	{
		m_fYspeed = (float)-g_cRandom.number(-7,-10); 
		
	}
	if(ydistance < home.y)
	{
		m_fYspeed = (float)-g_cRandom.number(7,10);
	
	}
	
	if(ydistance == home.y && xdistance == home.x)
		SetState(CRUISINGS3_STATE);

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
   // SetState(CRUISINGS3_STATE); //then back to cruising

}

void CFlameGuyObject::ChasingAi()
{

		m_nHeightDelayTime=100+g_cRandom.number(0,1000);

	if(g_cTimer.elapsed(m_nHeightTime, m_nHeightDelayTime))
	{
		m_nHeightDelayTime=100+g_cRandom.number(0,1000);
		
	


	if(m_fHorizontalDistance > 0)
	{
		m_fXspeed = -10;
	
	}
	 if(m_fHorizontalDistance < 0)
	{
		m_fXspeed = 10;

	}
	if(m_fVerticalDistance > 0)
	{
		m_fYspeed = 10;
		
	}
	if(m_fVerticalDistance < 0)
	{
		m_fYspeed = -10;
	
	}}
}
void CFlameGuyObject::WaitingAi(){ //avoiding plane
	
	
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
    SetState(CRUISINGS3_STATE);
}