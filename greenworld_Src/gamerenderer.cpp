/// \file gamerenderer.cpp
/// Direct3D rendering.
/// DirectX stuff that won't change much is hidden away in this file
/// so you won't have to keep looking at it.
/// Copyright Ian Parberry, 2010.
/// Last updated September 3, 2010.

#include <d3d9.h>

#include "gamerenderer.h"
#include "defines.h" 
#include "abort.h"
#include "tinyxml.h"
#include "imagefilenamelist.h"
#include "sprite.h"
#include "object.h"
#include "objman.h"
#include "spriteman.h"

extern TiXmlElement* g_xmlSettings;
extern BOOL g_bWindowed;
extern int g_nScreenWidth;
extern int g_nScreenHeight;
extern CImageFileNameList g_cImageFileName;
extern BOOL g_bImagesLoaded;
extern CGameObject g_cPlane; 
extern CObjectManager g_cObjectManager;
extern CSpriteManager g_cSpriteManager;
extern BOOL g_bWireFrame;
int currentKey = 1;
extern int VFrameCount;
extern int theCount;
extern BOOL attackfoo;
extern BOOL AttackCount;


CGameRenderer::CGameRenderer(): m_bCameraDefaultMode(TRUE){
}

void CGameRenderer::InitBackgroundVertexBuffer(){
	//create vertex buffer for background
	m_d3ddevice->CreateVertexBuffer( 
		4*sizeof(BILLBOARDVERTEX), D3DUSAGE_WRITEONLY, BILLBOARDVERTEX::FVF,
		D3DPOOL_MANAGED, &m_pBackgroundVB, NULL);
  
	//load vertex buffer
	BILLBOARDVERTEX* v; //vertex buffer data
	float w = 2.0f * g_nScreenWidth;
	float h = 2.0f * g_nScreenHeight; 

	if(SUCCEEDED(m_pBackgroundVB->Lock(0, 0, (void**)&v, 0))){ //lock buffer
		//vertex information, first triangle in clockwise order
		v[0].p = D3DXVECTOR3(4000, -4000, 500); 
		v[0].tu = 1.0f; v[0].tv = (float)g_nScreenHeight/g_nScreenWidth;

		v[1].p = D3DXVECTOR3(-4000, -4000, 500);
		v[1].tu = 0.0f; v[1].tv = (float)g_nScreenHeight/g_nScreenWidth;

		v[2].p = D3DXVECTOR3(4000, 4000, 500);
		v[2].tu = 1.0f; v[2].tv = 0.0f;

		v[3].p = D3DXVECTOR3(-4000, 4000, 500);
		v[3].tu = 0.0f; v[3].tv = 0.0f;

		m_pBackgroundVB->Unlock();
	}
} //InitBackgroundVertexBuffer

/// Draw the game background.

void CGameRenderer::DrawBackground(float x){
  
	const float delta = 2.0f * g_nScreenWidth;
	float fQuantizeX = delta * (int)(100/delta - 1.0f) + g_nScreenWidth; ///< Quantized x coordinate

	//set vertex buffer to background
	m_d3ddevice->SetStreamSource(0, m_pBackgroundVB, 0, sizeof(BILLBOARDVERTEX));
	m_d3ddevice->SetFVF(BILLBOARDVERTEX::FVF); //flexible vertex format

	//draw floor
	if(g_bWireFrame)m_d3ddevice->SetTexture(0, m_pWireframeTexture); //set wireframe texture
	else m_d3ddevice->SetTexture(0, m_pBackgroundTexture); //set floor texture

	SetWorldMatrix(fQuantizeX);
	m_d3ddevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	SetWorldMatrix(fQuantizeX-delta);
	m_d3ddevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	/*draw backdrop
	if(!g_bWireFrame)
		m_d3ddevice->SetTexture(0, m_pBackgroundTexture); //set cloud texture
		SetWorldMatrix(fQuantizeX);
		m_d3ddevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 2, 2);
		SetWorldMatrix(fQuantizeX-delta);
		m_d3ddevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 2, 2);
	*/
} //DrawBackground


/// Load the textures required for the game. 
/// Calls function LoadTexture to load in the textures needed for the game.
/// The texture names are hard-coded.

void CGameRenderer::LoadTextures(){ //create textures for image storage
	//LoadTexture(m_pBackgroundTexture, g_cImageFileName[0]);
	//LoadTexture(m_pFloorTexture, g_cImageFileName[1]);
	LoadTexture(m_pWireframeTexture, g_cImageFileName[2]);
	
	///Sprites

	///Hero
	g_cSpriteManager.Load(LINK_OBJECT,"link");
	g_cSpriteManager.Load(LINKATK_OBJECT,"linkATK");
	//g_cSpriteManager.Load(NPC_OBJECT,"NPC");
	//g_cSpriteManager.Load(HEALTH_PIECE,"healthpiece");
	//g_cSpriteManager.Load(HEALTH_FRAME,"healthframe");

	//UI Objects
	g_cSpriteManager.Load(HEALTH_OBJECT,"healthPiece");
	g_cSpriteManager.Load(MANA_OBJECT,"manaPiece");
	g_cSpriteManager.Load(HEALTHBOX_OBJECT,"healthBox");

	g_cSpriteManager.Load(HEALTH_PIECEMOB,"healthpiecemob");
	g_cSpriteManager.Load(HEALTH_FRAMEMOB,"healthframemob");

	g_cSpriteManager.Load(HURT_OBJECT,"hurt");

	///Monsters
	//Crow
	g_cSpriteManager.Load(CROW_OBJECT,"crow");
	g_cSpriteManager.Load(EXPLODINGCROW_OBJECT,"explodingcrow");
	g_cSpriteManager.Load(DEADCROW_OBJECT,"deadcrow");
	//Monster
	g_cSpriteManager.Load(MONSTER_OBJECT,"monster");
	g_cSpriteManager.Load(EXPLODINGMONSTER_OBJECT,"explodingmonster");
	g_cSpriteManager.Load(DEADMONSTER_OBJECT,"deadmonster");
	//Monster2
	g_cSpriteManager.Load(MONSTER2_OBJECT,"monster2");
	g_cSpriteManager.Load(EXPLODINGMONSTER2_OBJECT,"explodingmonster2");
	g_cSpriteManager.Load(DEADMONSTER2_OBJECT,"deadmonster2");
	//FlameGuy
	g_cSpriteManager.Load(FLAMEGUY_OBJECT,"flameguy");
	g_cSpriteManager.Load(EXPLODINGFLAMEGUY_OBJECT,"explodingflameguy");
	g_cSpriteManager.Load(DEADFLAMEGUY_OBJECT,"deadflameguy");
	//FlameGuy2
	g_cSpriteManager.Load(FLAMEGUY2_OBJECT,"flameguy2");
	g_cSpriteManager.Load(EXPLODINGFLAMEGUY2_OBJECT,"explodingflameguy2");
	g_cSpriteManager.Load(DEADFLAMEGUY2_OBJECT,"deadflameguy2");
	//RoundMan
	g_cSpriteManager.Load(ROUNDMAN_OBJECT,"roundman");
	g_cSpriteManager.Load(EXPLODINGROUNDMAN_OBJECT,"explodingroundman");
	g_cSpriteManager.Load(DEADROUNDMAN_OBJECT,"deadroundman");
	//RoundMan2
	g_cSpriteManager.Load(ROUNDMAN2_OBJECT,"roundman2");
	g_cSpriteManager.Load(EXPLODINGROUNDMAN2_OBJECT,"explodingroundman2");
	g_cSpriteManager.Load(DEADROUNDMAN2_OBJECT,"deadroundman2");

    ///Spells
	g_cSpriteManager.Load(FIREBALL_OBJECT,"fireball");
	g_cSpriteManager.Load(LIGHTNING_OBJECT,"lightning");
	g_cSpriteManager.Load(TORNADO_OBJECT,"tornado");
	g_cSpriteManager.Load(BARRIER_OBJECT, "barrier");

	///Enemy Spells
	g_cSpriteManager.Load(ENEMYFIREBALL_OBJECT, "enemyfireball");
	g_cSpriteManager.Load(ENEMYHOMING_OBJECT, "enemyhoming");


	///Items
	g_cSpriteManager.Load(LOOT_OBJECT,"loot");
	g_cSpriteManager.Load(LOOT_OBJECTSHOW,"loot");
	g_cSpriteManager.Load(INVENTORY_OBJECT,"inventory");
  
	///Static Objects
	g_cSpriteManager.Load(TREE_OBJECT,"tree");
	g_cSpriteManager.Load(TREE2_OBJECT,"treea");
	g_cSpriteManager.Load(TREE3_OBJECT,"treeb");
	g_cSpriteManager.Load(ROCK_OBJECT,"rock");
	g_cSpriteManager.Load(CAVE1_OBJECT,"cave1");
	g_cSpriteManager.Load(TALK_OBJECT,"talkBubble");
	g_cSpriteManager.Load(ALPHA_OBJECT,"alpha");
	g_cSpriteManager.Load(ITEMSMENU_OBJECT,"itemsMenu");
	g_cSpriteManager.Load(SPELLSMENU_OBJECT,"spellsMenu");
	g_cSpriteManager.Load(OPTIONSMENU_OBJECT,"optionsMenu");
	g_cSpriteManager.Load(POINTER_OBJECT,"pointer");
	g_cSpriteManager.Load(TEXTBOX_OBJECT,"textBox");
	g_cSpriteManager.Load(ITEMCURSOR_OBJECT,"itemCursor");
	g_cSpriteManager.Load(DEADTEXT_OBJECT,"dead");




	//g_cSpriteManager.Load(HOUSE_OBJECT,"house");

	///Tiles
	g_cSpriteManager.Load(FOREST1_OBJECT,"forest1");
	g_cSpriteManager.Load(GRASS1_OBJECT,"grass1");
	g_cSpriteManager.Load(GRASS2_OBJECT,"grass2");
	g_cSpriteManager.Load(FLOWER1_OBJECT,"flower1");
	g_cSpriteManager.Load(DESERT1_OBJECT,"desert1");
	g_cSpriteManager.Load(DESERT2_OBJECT,"desert2");
	g_cSpriteManager.Load(DUNGEON1_OBJECT,"dungeon1");
	g_cSpriteManager.Load(DUNGEON2_OBJECT,"dungeon2");
	g_cSpriteManager.Load(DUNGEON3_OBJECT,"dungeon3");
	g_cSpriteManager.Load(DUNGEON4_OBJECT,"dungeon4");
	g_cSpriteManager.Load(DUNGEON5_OBJECT,"dungeon5");
	g_cSpriteManager.Load(DUNGEON6_OBJECT,"dungeon6");
	g_cSpriteManager.Load(DUNGEON1COCKBLOCK,"bigdungeoncockblock1");
	g_cSpriteManager.Load(WALL1_OBJECT,"wall1");
	g_cSpriteManager.Load(WALL2_OBJECT,"wall2");
	g_cSpriteManager.Load(EXIT1_OBJECT,"exit1");
	g_cSpriteManager.Load(EXIT2_OBJECT,"exit2");

	//House
	g_cSpriteManager.Load(HOUSET1P1_OBJECT,"houset1p1");
	g_cSpriteManager.Load(HOUSET1P2_OBJECT,"houset1p2");
	g_cSpriteManager.Load(HOUSET1P3_OBJECT,"houset1p3");

	//Weather
	g_cSpriteManager.Load(NIGHT_OBJECT,"night");
	g_cSpriteManager.Load(MENU_OBJECT,"menu");

	//Environment Stuff
	g_cSpriteManager.Load(PLANT1_OBJECT,"plant1");

	//Blank Object
	g_cSpriteManager.Load(NULL_OBJECT,"null1");

	//Inventory Objects
	g_cSpriteManager.Load(TREASURE_OBJECT, "treasure");
	g_cSpriteManager.Load(TREASUREOPEN_OBJECT, "treasureopen");

	g_cSpriteManager.Load(GOLD1_OBJECT,"gold1");
	g_cSpriteManager.Load(STICK1_OBJECT,"stick1");

	g_cSpriteManager.Load(POTION1_OBJECT,"potion1");
	g_cSpriteManager.Load(POTION2_OBJECT,"potion2");
	g_cSpriteManager.Load(POTION3_OBJECT,"potion3");

	g_cSpriteManager.Load(KEY1_OBJECT,"key1");

	//Minimap
	g_cSpriteManager.Load(MINIWALL,"miniwall");
	g_cSpriteManager.Load(MINIEXIT,"miniexit");
	g_cSpriteManager.Load(MINIPLAYER,"miniplayer");

} //LoadTextures


/// Release game textures.
/// All textures used in the game are released - the release function is kind
/// of like a destructor for DirectX entities, which are COM objects.

void CGameRenderer::Release(){ //release D3D textures and stuff, then D3D itself 
	if(m_pBackgroundTexture)m_pBackgroundTexture->Release();
	if(m_pFloorTexture)m_pFloorTexture->Release();
	if(m_pBackgroundVB)m_pBackgroundVB->Release(); //release background vertex buffer
	g_cSpriteManager.Release(); 

	ReleaseD3D(); //release Direct3D
} //Release


/// Compose a frame of animation.
/// Draws the next frame of animation to the back buffer and flips it to the front.
/// \return TRUE if it succeeded

void CGameRenderer::ComposeFrame(){  
	m_d3ddevice->Clear(0L, NULL, D3DCLEAR_TARGET, 0xFFFFFF, 1.0f, 0L); //clear render buffer
	if(SUCCEEDED(m_d3ddevice->BeginScene())){ //can start rendering
		//move objects
		g_cObjectManager.move();
		//set camera location
		float x, y; //plane's current location
		g_cObjectManager.GetPlayerLocation(x, y); //get plane's lcoation
		if(m_bCameraDefaultMode)  
			SetViewMatrix(x+500, y, -350.0f);
		else SetViewMatrix(x + g_nScreenWidth/2.0f, 1000.0f, -5000.0f);

		//draw background 
		DrawBackground(x + g_nScreenWidth/2);

		//draw objects
		g_cObjectManager.draw();

		if(currentKey > 4){
			for(VFrameCount = 0; VFrameCount < 6; VFrameCount++){ 
				g_cObjectManager.draw2(currentKey-4);

			}
		if(AttackCount)
			currentKey = currentKey - 4; 
		VFrameCount = 0;
		}else{
			AttackCount = FALSE;
			g_cObjectManager.draw(currentKey);
		}
	    //draw text on the HUD
		g_cObjectManager.DrawTextHeader();

		m_d3ddevice->EndScene(); //done rendering
	}
} //ComposeFrame

/// Process a frame of animation. 
/// Compose a frame, present it, then check for 
/// lost device and restore it if necessary.

void CGameRenderer::ProcessFrame(){ //process a frame of animation
	if(g_bImagesLoaded){ //wait for the image loading thread to finish
		//check for lost graphics device
		if(DeviceLost())RestoreDevice(); //if device lost, restore it
		//process a frame of animation
		ComposeFrame(); //compose a frame of animation
		PageFlip(); //flip to front
	}
} //ProcessFrame

/// Toggle camera mode.
/// Toggle between eagle-eye camera (camera pulled back far enough to see
/// backdrop) and the normal game camera.

void CGameRenderer::FlipCameraMode(){ //pull the camera back
	m_bCameraDefaultMode = !m_bCameraDefaultMode; 
}

//Gets direction key pressed for character rendering
void CGameRenderer::GetKey(int key){
	currentKey = key; 
}