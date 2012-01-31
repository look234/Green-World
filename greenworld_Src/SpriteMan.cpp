/// \file SpriteMan.cpp
/// Code for the sprite manager class CSpriteManager.
/// Copyright Ian Parberry, 2004.
/// Last updated July 14, 2010.

#include <stdio.h>

#include "SpriteMan.h"
#include "abort.h"

extern TiXmlElement* g_xmlSettings; //global XML settings

CSpriteManager::CSpriteManager(){ //constructor
  for(int i=0; i<NUM_SPRITES; i++)m_pSprite[i]=NULL; 
}

CSpriteManager::~CSpriteManager(){ //destructor
  for(int i=0; i<NUM_SPRITES; i++)delete m_pSprite[i]; 
}

/// Load sprite from files.
/// Given a file name and extension such as "foo" and "bmp", reads
/// in sprite frames from "foo1.bmp", "foo2.bmp," etc.
/// \param object object type
/// \param file file name prefix
/// \param ext file extension
/// \param frames number of frames
/// \return TRUE if load succeeded

BOOL CSpriteManager::Load(ObjectType object,
                          const char* file, const char* ext, int frames){

  BOOL success = TRUE; //are reading sucessfully

  m_pSprite[object] = new C3DSprite(frames); //get space in array for new sprite

  for(int i=0; i<frames && success; i++){ //for each frame
    sprintf_s(m_pBuffer, MAX_PATH, "%s%d.%s", file, i, ext); //assemble file name
    success = m_pSprite[object]->Load(m_pBuffer,i); //load from that file name
  }

  return success; // return success, obviously some work needs to be done here
}

/// Load sprite from info in XML settings.
/// Loads information about the sprite from global variable g_xmlSettings, then
/// loads the sprite images as per that information. Aborts if something goes wrong.
/// \param object object type
/// \param name object name in XML file

void CSpriteManager::Load(ObjectType object, char* name){

  int frames = 0; //number of frames in sprite
  BOOL success = FALSE; //TRUE if loaded successfully

  if(g_xmlSettings){ //got "settings" tag
    //get "sprites" tag
    TiXmlElement* spriteSettings = g_xmlSettings->FirstChildElement("sprites"); //sprites tag
    if(spriteSettings){ //got "sprites" tag
      //load "sprite" tag with correct name attribute
      TiXmlElement* spr; //sprite element
      //set spr to the first "sprite" tag with the correct name
      spr = spriteSettings->FirstChildElement("sprite");
      while(spr && strcmp(name, spr->Attribute("name")) ){
        spr = spr->NextSiblingElement("sprite");
      }
      if(spr){ //got "sprite" tag with right name
        //get sprite information from tag
        spr->Attribute("frames",&frames);
        //now load the sprite from the information loaded
        success = Load(object, spr->Attribute("file"), spr->Attribute("ext"), frames);
      }
    }
  }

  if(!success) ABORT("Cannot load sprite \"%s\".\n",name);
}

/// Draw object.
/// Draw a frame of an object of a given type, at a given place.
/// \param object object type
/// \param p position
/// \param angle orientation
/// \param frame frame number

void CSpriteManager::Draw(ObjectType object, D3DXVECTOR3 p, float angle, int frame){ //draw sprite
  if(m_pSprite[object])
    m_pSprite[object]->Draw(p, angle, frame);
}

/// Reader function for frame count.
/// \param object object type
/// \return number of frames of animation for that object

int CSpriteManager::FrameCount(ObjectType object){ //number of frames
  if(m_pSprite[object])return m_pSprite[object]->m_nFrameCount;
  else return 0;
}

/// Reader function for height.
/// \param object object type
/// \return height of that object

int CSpriteManager::Height(ObjectType object){ //frame height
  if(m_pSprite[object]) return m_pSprite[object]->m_structImageInfo.Height;
  else return 0;
}

/// Reader function for width.
/// \param object object type
/// \return width of that object

int CSpriteManager::Width(ObjectType object){ //frame width
  if(m_pSprite[object]) return m_pSprite[object]->m_structImageInfo.Width;
  else return 0;
}

/// Release function.
/// Releases all textures from all sprites.

void CSpriteManager::Release(){ //release textures from sprites
  for(int i=0; i<NUM_SPRITES; i++)
    if(m_pSprite[i])m_pSprite[i]->Release();
}