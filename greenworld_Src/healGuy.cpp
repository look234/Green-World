#include "healGuy.h"
#include "timer.h" //game timer
#include "random.h" //for random number generator
#include "Sound.h"


extern CTimer g_cTimer;  //game timer
extern CRandom g_cRandom; //random number generator
extern CSoundManager* g_pSoundManager;
D3DXVECTOR3 cool;
const int CLOSE_DISTANCE=150; ///< Distance for close to plane
const int FAR_DISTANCE=500; ///< Distance for "far from" plane.
const int FALLBACK_DISTANCE=150; ///< Fall back at this vertical distance from plane
const int BEHIND_DISTANCE=-5; ///< Horizontal distance considered to be behind plane


CHealGuyObject::CHealGuyObject(char* name, D3DXVECTOR3 location, float xspeed, float yspeed):CIntelligentObject(HEALGUY_OBJECT, name, location, xspeed, yspeed)
{


	cool=location;
	height=location.y;
	width=location.x;
	m_fDistance = m_fHorizontalDistance = m_fVerticalDistance = 0.0f;
	 m_eState = herCRUISINGS3_STATE;
	 lastAi=0; AiDelay=0;
}

void CHealGuyObject::move()
{
	 CGameObject::move();
	 ai();


}

void CHealGuyObject::ai(){ //main AI function
  //do the following periodically
  if(g_cTimer.elapsed(lastAi,AiDelay)) 
    switch(m_eState){ //behavior depends on state
      case herCRUISINGS3_STATE: Healing(); break;
      case herAVOIDINGS3_STATE: AvoidMonster(); break;
	  case herCHASINGS3_STATE: KeepUp(); break;
	  case herWAITS3_STATE: Stay(); break;
      default: break;
    }
}


void CHealGuyObject::SetState(HealType state){

  m_eState=state; //change state

  switch(m_eState){ //change behavior settings

    case herCRUISINGS3_STATE:
      AiDelay=400+g_cRandom.number(0,200);
      DelayTime=3000+g_cRandom.number(0,5000);
      break;

    case herAVOIDINGS3_STATE:
      AiDelay=250+g_cRandom.number(0,250); 
      m_fXspeed=-3;
      break;
	case herCHASINGS3_STATE:
      //m_nAiDelayTime=400+g_cRandom.number(0,200);
      DelayTime=300+g_cRandom.number(0,50);
	  break;
	case herWAITS3_STATE:
	  AiDelay=250+g_cRandom.number(0,250); 
	  break;

    default: break;
  }
}


void CHealGuyObject::Stay(){ //just cruising along
int x=0;

if(g_cTimer.elapsed(timeHeight, DelayTime))
	{
	int rand = g_cRandom.number(1,3000);
	DelayTime=100+g_cRandom.number(0,1000);
	
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
		SetState(herCHASINGS3_STATE);}
	}

}


void CHealGuyObject::AvoidMonster(){ //avoiding plane
		if(xdistance > cool.x)
	{
		m_fXspeed = (float)-g_cRandom.number(7,10); 
	
	}
	if(xdistance < cool.x)
	{
		m_fXspeed = (float)-g_cRandom.number(-7,-10);

	}
	if(ydistance > cool.y)
	{
		m_fYspeed = (float)-g_cRandom.number(-7,-10); 
		
	}
	if(ydistance < cool.y)
	{
		m_fYspeed = (float)-g_cRandom.number(7,10);
	
	}
	
	if(ydistance == cool.y && xdistance == cool.x)
		SetState(herCRUISINGS3_STATE);

 
}


void CHealGuyObject::KeepUp()
{

		DelayTime=100+g_cRandom.number(0,1000);

	if(g_cTimer.elapsed(timeHeight, DelayTime))
	{
		DelayTime=100+g_cRandom.number(0,1000);
		
	


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

void CHealGuyObject::Healing(){ //avoiding plane
	
	
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
    SetState(herCRUISINGS3_STATE);
}