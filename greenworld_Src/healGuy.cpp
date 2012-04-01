#include "healGuy.h"
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


CHealGuyObject::CHealGuyObject(char* name, D3DXVECTOR3 location, float xspeed, float yspeed):CIntelligentObject(HEALGUY_OBJECT, name, location, xspeed, yspeed)
{


	home=location;
	height=location.y;
	width=location.x;
	m_fDistance = m_fHorizontalDistance = m_fVerticalDistance = 0.0f;
	 m_eState = CRUISINGS3_STATE;
	 lastAi=0; AiDelay=0;
}

void CHealGuyObject::move()
{
	 CGameObject::move();
	 ai();


}


