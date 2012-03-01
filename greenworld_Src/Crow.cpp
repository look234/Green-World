/// \file crow.cpp
/// Code for the crow intelligent object class CCrowObject.
/// Copyright Ian Parberry, 2004.
/// Last updated August 18, 2004.

#include "crow.h"
#include "timer.h" //game timer
#include "random.h" //for random number generator
#include "sound.h"
#include "Objman.h"
#include "debug.h"


extern CTimer g_cTimer;  //game timer
extern CRandom g_cRandom; //random number generator
extern CSoundManager* g_pSoundManager;
extern CObjectManager g_cObjectManager;
extern BOOL MenuUp;

const int CLOSE_DISTANCE=300; ///< Distance for close to plane
const int FAR_DISTANCE=500; ///< Distance for "far from" plane.
const int FALLBACK_DISTANCE=150; ///< Fall back at this vertical distance from plane
const int BEHIND_DISTANCE=-5; ///< Horizontal distance considered to be behind plane
int Key=0;
CCrowObject::CCrowObject(char* name, D3DXVECTOR3 location, float xspeed, float yspeed):
CIntelligentObject(CROW_OBJECT, name, location, xspeed, yspeed){ //constructor
  loc = location;
  m_nDesiredHeight = 240; 
  m_nHeightTime = 100;  m_nHeightDelayTime = 0;
  m_nSpeedVariationTime = m_nSpeedVariationDuration = 0;
  m_fDistance = m_fHorizontalDistance = m_fVerticalDistance = 0.0f;
  m_eState = CRUISING_STATE;
  m_nLastAiTime = 0; m_nAiDelayTime = 0;
  xspeeed = xspeed;
  yspeeed = yspeed;
}

/// Intelligent move function.
/// Basically, just move like a dumb object then modify your private variables "intelligently".

void CCrowObject::move(){ //move object
	if(!MenuUp){

		//D3DXVECTOR3 v = D3DXVECTOR3(m_structLocation.x, m_structLocation.y, 500.0f);
	//	CGameObject* temp = new CGameObject(DUMMY_OBJECT,"dummy",v,0,0);
	//	if(!g_cObjectManager.monsterSmartMoveCheck(temp))
	//		SetState(WAIT_STATE);
			

		CGameObject::move();
		ai();
		 //move like a dumb object
	
		 //act intelligently
	}
	else 
		xspeeed=yspeeed=0;
		
}

/// Main crow AI function.
/// The real work is done by a function for each state. Call the appropriate
/// function for the current state periodically, based on a timer.

void CCrowObject::ai(){ //main AI function
  //do the following periodically
  if(MenuUp)SetState(WAIT_STATE);
  else if(g_cTimer.elapsed(m_nLastAiTime,m_nAiDelayTime)) 
    switch(m_eState){ //behavior depends on state
      case CRUISING_STATE: CruisingAi(); break;
      case AVOIDING_STATE: AvoidingAi(); break;
	  case CHASING_STATE: ChasingAi(); break;
	  case WAIT_STATE: WaitingAi(); break;
      default: break;
    }
}

/// Set the current state.
/// Change the current state, taking whatever actions are necessary on entering the new state.
/// \param state new state

void CCrowObject::SetState(StateType state){

  m_eState=state; //change state
  switch(m_eState){ //change behavior settings

    case CRUISING_STATE:
      m_nAiDelayTime=400+g_cRandom.number(0,200);
      //m_fXspeed=(float)-g_cRandom.number(1,4);
      m_nHeightDelayTime=3000+g_cRandom.number(0,5000);
      break;

    case AVOIDING_STATE:
      m_nAiDelayTime=250+g_cRandom.number(0,250); 
      m_fXspeed=-3;
      m_nDesiredHeight=g_cRandom.number(100,400);
      m_nHeightDelayTime=3000+g_cRandom.number(0,2000);
      m_nSpeedVariationDuration=5000+g_cRandom.number(0,2000);
      break;
	case CHASING_STATE:
      m_nAiDelayTime=250+g_cRandom.number(0,250); 
	  m_fXspeed=-3;
	  break;
	case WAIT_STATE:
	  m_nAiDelayTime=250+g_cRandom.number(0,250); 
	  m_fXspeed=-3;
	  break;

    default: break;
  }
}

/// AI for crows in the cruising state.
/// These crows are just cruising along, periodically lokoing for the plane.

void CCrowObject::CruisingAi(){ //just cruising along
	
  //height variation
		
	if(g_cTimer.elapsed(m_nHeightTime, m_nHeightDelayTime))
	{
		loc.x = xdistance;
		loc.y = ydistance;


			if(Key == 1){//left attack
			g_cObjectManager.enemyFireball(Key, loc, -50, 0);
			
			
		}
		if(Key == 2){//right attack
		g_cObjectManager.enemyFireball(Key, loc, 50, 0);
		
			
		}
		if(Key == 3){
			g_cObjectManager.enemyFireball(Key, loc, 0, 50);
		
		
		}
		if(Key == 4){
		g_cObjectManager.enemyFireball(Key, loc, 0, -50);
			
		}




	
		Key = 5;
		//g_pSoundManager->play(CAW_SOUND);
	int rand = g_cRandom.number(1,1000);
	m_nHeightDelayTime=3000+g_cRandom.number(0,2000);
	if(rand % 2 == 0)
	{
		rand = g_cRandom.number(1,1000);
		if(rand%2 == 0)
		{
			
		m_fYspeed = 0;
		m_fXspeed = 5;
		Key=2;
		}
		else
		{
			Key=4;
		m_fXspeed = 0;
		m_fYspeed = 5;
		}
	}
	else if(rand%2 == 1)
	{
		rand = g_cRandom.number(1,1000);
		if(rand%2 == 0)
		{
			Key=1;
		m_fYspeed = 0;
		m_fXspeed = -5;
		}
		else
		{
			Key=3;
		m_fXspeed = 0;
		m_fYspeed = -5;
		}
	}
	else
	{
		Key = 5;

		m_fXspeed = m_fYspeed = 0;
		m_nHeightDelayTime=100+g_cRandom.number(0,50);
	}
	}

  /*if(m_nDesiredHeight > (int)m_structLocation.y+20)
    m_fYspeed = -(float)g_cRandom.number(1,4);
  else if(m_nDesiredHeight < m_structLocation.y-20)
    m_fYspeed=(float)g_cRandom.number(1,4);
  else m_fYspeed=0.0f;
  if(g_cTimer.elapsed(m_nHeightTime, m_nHeightDelayTime)){
    m_nDesiredHeight = g_cRandom.number(150,400);
    m_nHeightDelayTime = 15000+g_cRandom.number(0,5000);*/
  //}

  //look for plane
 /* if(m_fDistance < CLOSE_DISTANCE&&
  m_fHorizontalDistance < BEHIND_DISTANCE)
    SetState(CHASING_STATE);*/
}

/// AI for crows in the avoinding state.
/// These crows are trying to get away from the plane.

void CCrowObject::AvoidingAi(){ //avoiding plane
	SetState(CRUISING_STATE);
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

  //look for plane, maybe leave avoiding state
  if(m_fDistance>FAR_DISTANCE|| //if far away, or
    (m_fDistance<CLOSE_DISTANCE&& //close and
      m_fVerticalDistance>FALLBACK_DISTANCE)) //higher or lower
    SetState(CRUISING_STATE); //then back to cruising

}

void CCrowObject::ChasingAi()
{
	SetState(CRUISING_STATE);
	if(m_fHorizontalDistance > 0)
		m_fXspeed = (float)-g_cRandom.number(7,10); 
	if(m_fHorizontalDistance < 0)
		m_fXspeed = (float)-g_cRandom.number(-7,-10);
	if(m_fVerticalDistance < m_structLocation.y)
	{
      m_fYspeed= (float)g_cRandom.number(1,7) + 3;
	  m_fVerticalDistance += 100;
	}
    if(m_fVerticalDistance > m_structLocation.y)
	{
      m_fYspeed = -(float)g_cRandom.number(1,7) -3;
	  m_fVerticalDistance -= 100;
	}
}

void CCrowObject::WaitingAi(){ //avoiding plane
	m_fXspeed = -m_fXspeed;
	m_fYspeed = -m_fYspeed;
	//if(!MenuUp)
	//SetState(CRUISING_STATE);
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
   /* if(m_fDistance < CLOSE_DISTANCE&&
      m_fHorizontalDistance+100 > BEHIND_DISTANCE)
    SetState(CRUISING_STATE);*/
}
