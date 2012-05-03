/// \file object.cpp
/// Code for the game object class CGameObject.
/// Copyright Ian Parberry, 2004.
/// Last updated July 14, 2010.

#include "object.h"

#include "defines.h" 
#include "timer.h" 
#include "spriteman.h" 
#include "Objman.h"
#include "sound.h"


extern CTimer g_cTimer;
extern int g_nScreenWidth;
extern int g_nScreenHeight;
extern CSpriteManager g_cSpriteManager;
extern CObjectManager g_cObjectManager;
extern TiXmlElement* g_xmlSettings;
extern CSoundManager* g_pSoundManager;
extern int theCount;
extern BOOL attackfoo = FALSE;
extern BOOL AttackCount = FALSE;

extern BOOL MenuUp;

/// Initializes a game object. Gets object-dependent settings from g_xmlSettings
/// from the "object" tag that has the same "name" attribute as parameter name.
/// Assumes that the sprite manager has loaded the sprites already.
/// \param object object type
/// \param name object name in XML settings file object tag
/// \param location initial location of object in 3D space
/// \param xspeed speed along x axis
/// \param yspeed speed along y axis

CGameObject::CGameObject(ObjectType object, char* name, D3DXVECTOR3 location,
                         float xspeed, float yspeed){ //constructor
 
  //defaults
  m_nCurrentFrame = 0; 
  m_nLastFrameTime = g_cTimer.time();
  m_nFrameInterval = 30; 
  m_nHP = 1; //negative means immortalz
  m_nEXP = 0; //negative means immortal
  m_nLifeTime = -1; //negative means immortal
  m_bVulnerable = FALSE; 
  m_bIntelligent = FALSE;
  m_nMinXSpeed = -20; m_nMaxXSpeed = 20;
  m_nMinYSpeed = -20; m_nMaxYSpeed = 20;
  m_bCanFly = FALSE;
  m_bCycleSprite = TRUE;
  m_pAnimation = NULL;
  m_nAnimationFrameCount = 0;
  m_nBounceCount = 0;

  waitTime = 0;

  step = 3.0f;

  //common values
  m_nObjectType = object; //type of object
  m_nLastMoveTime = g_cTimer.time(); //time
  m_structLocation = location; //location
  m_fXspeed = xspeed; m_fYspeed = yspeed; //speed
  m_nFrameCount = g_cSpriteManager.FrameCount(m_nObjectType);
  m_nHeight = g_cSpriteManager.Height(m_nObjectType);
  m_nWidth = g_cSpriteManager.Width(m_nObjectType);
  width = g_cSpriteManager.Width(m_nObjectType) + 100; //m_nWidth;
  m_nBirthTime = g_cTimer.time(); //time of creation
  
  //object-dependent settings loaded from XML
  LoadSettings(name);

     switch(object){
    case LINKATK_OBJECT:
     // g_pSoundManager->play(FIRE_SOUND); //sound of gun firing
      break;
    case EXPLODINGCROW_OBJECT:
      //g_pSoundManager->play(BOOM_SOUND); //sound of explosion
      break;
  }


} 

CGameObject::~CGameObject(){
  delete [] m_pAnimation;
}

/// Draw game object.
/// Draws the current sprite frame at the current position, then
/// computes which frame is to be drawn next time.

void CGameObject::draw(){ //draw

  int t = m_nFrameInterval;
  
  if(m_nObjectType != EXPLODINGCROW_OBJECT)
    t = (int)((float)m_nFrameInterval/(1.5f+fabs(m_fXspeed))); //frame interval

  float fOrientation = atan2f(m_fYspeed, 20.0f); //rotation around Z axis

  if(1==1 || m_nObjectType != MENU_OBJECT){
	  		if(1==1 || m_nObjectType != POINTER_OBJECT){
  if(m_pAnimation != NULL){ //if there's an animation sequence
    //draw current frame
    g_cSpriteManager.Draw(m_nObjectType, m_structLocation, fOrientation, m_pAnimation[m_nCurrentFrame]);
    //advance to next frame
	if(!MenuUp){
    if(g_cTimer.elapsed(m_nLastFrameTime, t)) //if enough time passed
      //increment and loop if necessary
      if(++m_nCurrentFrame >= m_nAnimationFrameCount && m_bCycleSprite) 
        m_nCurrentFrame = 0;
	}
  }else{
    g_cSpriteManager.Draw(m_nObjectType, m_structLocation, fOrientation, 0); //assume only one frame
				}
		}else{
			g_cSpriteManager.Draw(POINTER_OBJECT, m_structLocation, fOrientation, 0); //assume only one frame

  }
  }else{
  g_cSpriteManager.Draw(MENU_OBJECT, m_structLocation, fOrientation, 0); //assume only one frame
  }
}

void CGameObject::draw(int key){ //draw

  int t = m_nFrameInterval;
  
  if(m_nObjectType != EXPLODINGCROW_OBJECT)
    t = (int)((float)m_nFrameInterval/(2.0f+fabs(m_fXspeed))); //frame interval

  float fOrientation = atan2f(m_fYspeed, 20.0f); //rotation around Z axis
  int testFrame = 0;
  int get = 0;
  m_nCurrentFrame = ((key*6) - 6); 
  testFrame = (key*6);

  		if(g_cTimer.elapsed(m_nLastFrameTime, t+10.0f)) //if enough time passed
			theCount++;
	switch(theCount){
	case 0: g_cSpriteManager.Draw(LINK_OBJECT, m_structLocation, fOrientation, 0+m_nCurrentFrame);break;
	case 1: g_cSpriteManager.Draw(LINK_OBJECT, m_structLocation, fOrientation, 1+m_nCurrentFrame);break;
	case 2: g_cSpriteManager.Draw(LINK_OBJECT, m_structLocation, fOrientation, 2+m_nCurrentFrame);break;
	case 3: g_cSpriteManager.Draw(LINK_OBJECT, m_structLocation, fOrientation, 3+m_nCurrentFrame);break;
	case 4: g_cSpriteManager.Draw(LINK_OBJECT, m_structLocation, fOrientation, 4+m_nCurrentFrame);break;
	case 5: g_cSpriteManager.Draw(LINK_OBJECT, m_structLocation, fOrientation, 5+m_nCurrentFrame);break;
	default: theCount = 0;m_nCurrentFrame = (key*6) - 6;g_cSpriteManager.Draw(LINK_OBJECT, m_structLocation, fOrientation, m_nCurrentFrame);break;
		}

}

void CGameObject::draw2(int key){ //draw

	  int t = m_nFrameInterval;
  
  if(m_nObjectType != EXPLODINGCROW_OBJECT)
    t = (int)((float)m_nFrameInterval/(1.5f+fabs(m_fXspeed))); //frame interval

  float fOrientation = atan2f(m_fYspeed, 20.0f); //rotation around Z axis

  int testFrame = 0;

  int get = 0;

	//takes in the key info from player and converts it to frames for animation
		m_nCurrentFrame = ((key*6) - 6); 
		//takes in the key info from player and converts it to max frames for animation
		testFrame = (key*6);

	if(g_cTimer.elapsed(m_nLastFrameTime, t+10.0f)) //if enough time passed
			theCount++;
	if(++m_nCurrentFrame >= testFrame) 
		m_nCurrentFrame = (key*6) - 6;

	switch(theCount){
	case 0: g_cSpriteManager.Draw(LINK_OBJECT, m_structLocation, fOrientation, 0+m_nCurrentFrame+23);break;
	case 1: g_cSpriteManager.Draw(LINK_OBJECT, m_structLocation, fOrientation, 1+m_nCurrentFrame+23);break;
	case 2: g_cSpriteManager.Draw(LINK_OBJECT, m_structLocation, fOrientation, 2+m_nCurrentFrame+23);break;
	case 3: g_cSpriteManager.Draw(LINK_OBJECT, m_structLocation, fOrientation, 3+m_nCurrentFrame+23);break;
	case 4: g_cSpriteManager.Draw(LINK_OBJECT, m_structLocation, fOrientation, 4+m_nCurrentFrame+23);break;
	case 5: g_cSpriteManager.Draw(LINK_OBJECT, m_structLocation, fOrientation, 5+m_nCurrentFrame+23);break;
	default: m_nCurrentFrame = 0; testFrame = 0; theCount = 0; AttackCount = TRUE; break;
	}

}

void CGameObject::draw(char letter){ //draw

  int t = m_nFrameInterval;
  
  float fOrientation = atan2f(m_fYspeed, 20.0f); //rotation around Z axis

  switch(letter){
	case 'A': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 0); break;
	case 'B': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 1); break;
	case 'C': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 2); break;
	case 'D': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 3); break;
	case 'E': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 4); break;
	case 'F': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 5); break;
	case 'G': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 6); break;
	case 'H': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 7); break;
	case 'I': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 8); break;
	case 'J': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 9); break;
	case 'K': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 10); break;
	case 'L': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 11); break;
	case 'M': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 12); break;
	case 'N': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 13); break;
	case 'O': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 14); break;
	case 'P': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 15); break;
	case 'Q': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 16); break;
	case 'R': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 17); break;
	case 'S': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 18); break;
	case 'T': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 19); break;
	case 'U': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 20); break;
	case 'V': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 21); break;
	case 'W': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 22); break;
	case 'X': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 23); break;
	case 'Y': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 24); break;
	case 'Z': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 25); break;
	case ',': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 26); break;
	case '.': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 27); break;
	case '?': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 28); break;
	case '!': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 29); break;
	case '0': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 30); break;
	case '1': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 31); break;
	case '2': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 32); break;
	case '3': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 33); break;
	case '4': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 34); break;
	case '5': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 35); break;
	case '6': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 36); break;
	case '7': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 37); break;
	case '8': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 38); break;
	case '9': g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 39); break;
	default: g_cSpriteManager.Draw(ALPHA_OBJECT, m_structLocation, fOrientation, 40); break;
  }
}



/// Load settings.
/// Loads settings for object from g_xmlSettings.
/// \param name name of object as found in name tag of XML settings file

void CGameObject::LoadSettings(char* name){

  if(g_xmlSettings){ //got "settings" tag

    //get "objects" tag
    TiXmlElement* objSettings = 
      g_xmlSettings->FirstChildElement("objects"); //objects tag

    if(objSettings){ //got "objects" tag

      //set obj to the first "object" tag with the correct name
      TiXmlElement* obj = objSettings->FirstChildElement("object");
      while(obj && strcmp(name, obj->Attribute("name"))){
        obj = obj->NextSiblingElement("object");
      }

      if(obj){ //got "object" tag with right name
        //get object information from tag
        obj->Attribute("minxspeed",&m_nMinXSpeed);
        obj->Attribute("maxxspeed",&m_nMaxXSpeed);
        obj->Attribute("minyspeed",&m_nMinYSpeed);
        obj->Attribute("maxyspeed",&m_nMaxYSpeed);
        obj->Attribute("frameinterval",&m_nFrameInterval);
        obj->Attribute("vulnerable",&m_bVulnerable);
        obj->Attribute("fly",&m_bCanFly);
        obj->Attribute("cycle",&m_bCycleSprite);
        obj->Attribute("lifetime",&m_nLifeTime);   
		obj->Attribute("hp",&m_nHP);  
		obj->Attribute("exp",&m_nEXP);  

        //parse animation sequence
        if(obj->Attribute("animation")){ //sequence present

          //get sequence length
          int length=strlen(obj->Attribute("animation"));
          m_nAnimationFrameCount=1; //one more than number of commas
          for(int i=0; i<length; i++) //for each character
            if(obj->Attribute("animation")[i] == ',')
              m_nAnimationFrameCount++; //count commas

          m_pAnimation = new int[m_nAnimationFrameCount]; //memory for animation sequence

          int i = 0; //character index
          int count = 0; //number of frame numbers input
          int num; //frame number
          char c = obj->Attribute("animation")[i]; //character in sequence string
          while(i<length){
            //get next frame number
            num = 0;
            while(i<length && c >= '0' && c <= '9'){
              num = num*10 + c - '0';
              c = obj->Attribute("animation")[++i];
            }
            //process frame number
            c = obj->Attribute("animation")[++i]; //skip over comma
            m_pAnimation[count++] = num; //record frame number
          }
        }
      }
    }
  }
}

/// Move object. 
/// The distance that an object moves depends on its speed, 
/// and the amount of time since it last moved.

void CGameObject::move(){ //move object
  int time = g_cTimer.time();

  if(time > waitTime){
	  if(!MenuUp){
		  bool moved = false;
		  //do{
			  const float XSCALE = 100.0f; //to scale back horizontal motion
			  const float YSCALE = 100.0f; //to scale back vertical motion
			  const float MARGIN = 100.0f; //margin on top of page

			  float xdelta = 0.0f, ydelta = 0.0f; //change in position
			   //current time
			  int tfactor = time-m_nLastMoveTime; //time since last move

			  //compute xdelta and ydelta, horizontal and vertical distance
			  xdelta = (m_fXspeed * (float)tfactor)/XSCALE; //x distance moved  
			  if(m_bCanFly)
				ydelta = (m_fYspeed * (float)tfactor)/YSCALE; //y distance moved
			  else{
				float t = (float)(time-m_nBirthTime);
				ydelta = 3.0f + m_fYspeed * 2.0f + t*t/10000.0f; //y distance moved
			  }

			  //delta motion

			 D3DXVECTOR3 v = D3DXVECTOR3(m_structLocation.x + xdelta, m_structLocation.y - ydelta, 500.0f);
			 CGameObject* temp = new CGameObject(DUMMY_OBJECT,"dummy",v,0,0);

			 //Check and see if object is a spell

			 switch(m_nObjectType){
			 case FIREBALL_OBJECT: 
			 case LIGHTNING_OBJECT: 
			 case TORNADO_OBJECT: 
			 case BARRIER_OBJECT: 
			 case ENEMYFIREBALL_OBJECT: 
			 case ENEMYHOMING_OBJECT: moved = true; break;

			 case CROW_OBJECT:
			 case MONSTER_OBJECT:
			 case MONSTER2_OBJECT:
			 case FLAMEGUY_OBJECT:
			 case HEALGUY_OBJECT:
			 case FLAMEGUY2_OBJECT:
			 case ROUNDMAN_OBJECT:
			 case ROUNDMAN2_OBJECT: if(g_cObjectManager.monsterSmartMoveCheck(temp)) moved = true; break;
			 default: moved = true;
			 }

			 /*
			 if(isSpell)
				 moved = true;
			 else if( g_cObjectManager.distance(g_cObjectManager.m_pPlayerObject,temp) < 700.0f) //Can we see the monster? {
				   if(g_cObjectManager.monsterSmartMoveCheck(temp))
						moved = true;
			  else
				  moved = true; //Who cares if the monster is on a tree
			*/

		   if(moved){
			   m_structLocation.x += xdelta; //x motion
			   m_structLocation.y -= ydelta; //y motion
		   }


		   if(m_nBounceCount>=3)m_nLifeTime = 1; //force cull by making lifetime tiny
				m_nLastMoveTime = time;

		 // }while(!moved);

		}
	  else
		  m_nLastMoveTime = time;
	}
  else
	  m_nLastMoveTime = time;
  
}

void CGameObject::move2(){ //move object
  if(!MenuUp){
	  bool moved = false;
	  //do{
		  const float XSCALE = 100.0f; //to scale back horizontal motion
		  const float YSCALE = 100.0f; //to scale back vertical motion
		  const float MARGIN = 100.0f; //margin on top of page

		  float xdelta = 0.0f, ydelta = 0.0f; //change in position
		  int time = g_cTimer.time(); //current time
		  int tfactor = time-m_nLastMoveTime; //time since last move

		  //compute xdelta and ydelta, horizontal and vertical distance
		  xdelta = (m_fXspeed * (float)tfactor)/XSCALE; //x distance moved  
		  if(m_bCanFly)
			ydelta = (m_fYspeed * (float)tfactor)/YSCALE; //y distance moved
		  else{
			float t = (float)(time-m_nBirthTime);
			ydelta = 3.0f + m_fYspeed * 2.0f + t*t/10000.0f; //y distance moved
		  }

		  
	   m_structLocation.x += xdelta; //x motion
	   m_structLocation.y -= ydelta; //y motion
	

	   if(m_nBounceCount>=3)m_nLifeTime = 1; //force cull by making lifetime tiny
			m_nLastMoveTime = time;

	 // }while(!moved);


  }
}

/// Change speed.
/// Change the object's speed, and check against speed limits to make
/// sure that the object doesn't break them.
/// \param xdelta change in horizontal speed
/// \param ydelta change in vertical speed

void CGameObject::accelerate(int xdelta, int ydelta){ //change speed

  //horizontal
  m_fXspeed += xdelta;
  //check bounds
  if(m_fXspeed < m_nMinXSpeed) m_fXspeed = (float)m_nMinXSpeed;
  if(m_fXspeed > m_nMaxXSpeed) m_fXspeed = (float)m_nMaxXSpeed;

  //vertical
  m_fYspeed+=ydelta;
  //check bounds
  if(m_fYspeed < m_nMinYSpeed) m_fYspeed = (float)m_nMinYSpeed;
  if(m_fYspeed > m_nMaxYSpeed) m_fYspeed = (float)m_nMaxYSpeed;
}
void CGameObject::noMove()
{

}
void CGameObject::moveDown()
{
	m_structLocation.y -= step;
}
void CGameObject::moveUp()
{
	m_structLocation.y += step;
}
void CGameObject::moveRight()
{
	m_structLocation.x += step;
}
void CGameObject::moveLeft()
{
	m_structLocation.x -= step;
}
