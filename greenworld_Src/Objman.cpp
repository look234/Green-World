#include "objman.h"
#include "defines.h"
#include "timer.h"
#include "Sound.h"
#include "crow.h"
#include "Monster.h"
#include <math.h>
#include "Monster2.h"
#include "flameGuy.h"
#include "flameGuy2.h"
#include "roundMan.h"
#include "roundMan2.h"
#include "healGuy.h"
#include "gamerenderer.h"
#include <cstring>
#include "Tornado.h"
#include "Barrier.h"
#include <iostream>
#include "Homing.h"
#include <string.h>

#define PI 3.14159265
#define length(a) ( sizeof ( a ) / sizeof ( *a ) )

//extern CTimer g_cTimer; //game timer
extern int g_nScreenWidth;
extern int g_nScreenHeight;
extern CGameRenderer GameRenderer; ///< The game renderer.
extern CSoundManager* g_pSoundManager;

bool dead= false;
extern BOOL MenuUp;
extern BOOL SubMenuUp;
extern BOOL TalkBubble;
int menuState = 1;
extern int theKey;
extern int menuON;
int foo = 0;
BOOL Talk = FALSE;
extern BOOL textBoxON;
int itemsMenuON = 0;
int spellsMenuON = 0;
int optionsMenuON = 0;
int baseValue = 0;
int HealMana = 10;
extern int currentItemSelect;

CObjectManager::CObjectManager(){ //constructor 

	//SETTING ALL OBJECTS TO NULL OR ZERO
	cooliding = false;
	m_pObjectList = NULL;
	m_Dead = NULL;
	damageDone = 0;
	
	monsterAmount = 0;
	collisionCount =0; 
	HP = 0;
	totalHP =0; 
	MP =0;
	totalMP =0;
	LV = 0;
	XP = 0;
	nextXP =0;
	playerGold =0;
	zoneHeight =0;
	zoneWidth =0;
	spawnX =0;
	spawnY =	0;
    m_pPlayerObject = NULL;
    m_nCount = 0;
    m_nMaxCount = 0;
    m_nLastGunFireTime =0;
	m_nLastFireball =0;
	m_nLastLightning =0;
	m_nLastBarrier =0;
	m_nLastTornado =0;

	monsterInfo = NULL;

    m_tileObjects = NULL;
	m_nTileCount =0;
	m_nMaxTileCount =0;

	m_wallObjects = NULL;
	m_nWallCount =0;
	m_nMaxWallCount =0;

	timeHit =0;
	timeToBeHitAgain =0;
	
	m_spellObjects = NULL;
	spellTimers =0;
	m_nSpellCount =0;
	m_nMaxSpellCount =0;
	
	m_exitObjects = NULL;
	m_nExitCount =0;
	m_nMaxExitCount =0;
	exitInformation *exitInfo = NULL;
	inAnExit =0;
	zoneIds = NULL;

	m_impasibleObjects = NULL;
	m_nImpassibleCount =0;
	m_nMaxImpassibleCount =0;

	m_impasibleObjects32 = NULL;
	m_nImpassibleCount32 =0;
	m_nMaxImpassibleCount32 =0;

	night = NULL;

	m_menuObjects = NULL;
	m_nMenuCount =0;
	m_nMaxMenuCount =0;

	m_itemCursorObjects = NULL;
	m_nItemCursorCount =0;
	m_nMaxItemCursorCount =0;

	m_NPCObjects = NULL;
	m_nNPCCount =0;
	m_nMaxNPCCount =0;
	m_nNPCName = NULL;

	m_pointerObjects = NULL;
	m_nPointerCount =0;
	m_nMaxPointerCount =0;
	m_itemsMenuObjects = NULL;
	m_nItemsMenuCount =0;
	m_nMaxItemsMenuCount =0;
	m_spellsMenuObjects = NULL;
	m_nSpellsMenuCount =0;
	m_nMaxSpellsMenuCount =0;
	m_optionsMenuObjects = NULL;
	m_nOptionsMenuCount =0;
	m_nMaxOptionsMenuCount =0;

	m_talkBubbleObjects = NULL;
	m_nTalkBubbleCount =0;
	m_nMaxTalkBubbleCount =0;

	m_textBoxObjects = NULL;
	m_nTextBoxCount =0;
	m_nMaxTextBoxCount =0;
	m_alphaObjects = NULL;
	m_nAlphaCount =0;
	m_nMaxAlphaCount =0;

	isSafe = false;

	playerLoot = NULL;
	lootOnGround = NULL;
	m_groundLootObjects = NULL;
	m_invintoryLootObjects = NULL;
	maxGroundLoot =0;

	m_equipmentObjects[0] = NULL;
	m_equipmentObjects[1] = NULL;
	m_equipmentObjects[2] = NULL;

	playerLootWidth =0;
	playerLootHeight =0;
	m_playerLootObjects = NULL;
	m_nMaxPlayerLootObjects = 0;
	m_nPlayerLootCount =0;

	timeEnd = 0;
	speedOn = false;
	zoneName =0;

	m_nMaxHealthCountMob =0;
	m_healthObjects = NULL;
	m_nHealthCount =0;
	m_nMaxHealthCount =0;
	m_manaObjects = NULL;
	m_nManaCount =0;
	m_nMaxManaCount =0;

	m_healthBoxObjects = NULL;
	m_nHealthBoxCount =0;
	m_nMaxHealthBoxCount =0;

	m_nUiCount =0;
	m_nMaxUiCount =0;

	monsterHealthBars = NULL;

	m_miniMapObjects =0;
	m_mapObjectsCount =0;

	loadedSaved = false;

	prevCoords.x = 0;
	prevCoords.y = 0;
	prevCoords.z = 0;

	playerHurt = NULL;
	mobHurt = NULL;
	mobIndex =0;
	playerHurtTime =0;
	playerIsHurt = false;
	mobHurtTime =0;
	mobIsHurt = false;
	//END SETTING ALL OBJECTS TO NULL OR ZERO

	m_nMaxCount = MAX_OBJECTS;
	m_nCount = 0;
	m_nLastGunFireTime = 0; m_pPlayerObject = NULL;
	m_pObjectList = new CGameObject*[m_nMaxCount]; //object list
	m_Dead = NULL;
	for(int i = 0; i < m_nMaxCount; i++) 
		m_pObjectList[i] = NULL;

	//Variables for text output, probably will make them global later
		collisionCount = 0;//keeps track of how many times the hero has collided with a monster
		HP = 500; //Hero's Health
		totalHP = 500; //Max Helath
		MP = 500;
		totalMP = 500;
		LV; //Hero's current level
		XP = 0; //Hero's current Experience
		isSafe=false;

	m_menuObjects = new CGameObject*[1];
	m_menuObjects[0] = NULL;

	spawnX = 100; //This will come from the player save file
	spawnY = 100; 

	m_nMaxSpellCount = 200;
	m_nSpellCount = 0;

	m_spellObjects = new CGameObject*[m_nMaxSpellCount];
	spellTimers = new int[m_nMaxSpellCount];

	for(int i=0;i<m_nMaxSpellCount;i++){
		m_spellObjects[i] = NULL;
		spellTimers[i] = 0;
	}

	m_pointerObjects = new CGameObject*[1];
		m_pointerObjects[0] = NULL;
	m_itemsMenuObjects = new CGameObject*[1];
		m_itemsMenuObjects[0] = NULL;
	m_spellsMenuObjects = new CGameObject*[1];
		m_spellsMenuObjects[0] = NULL;
	m_optionsMenuObjects = new CGameObject*[1];
		m_optionsMenuObjects[0] = NULL;
	m_talkBubbleObjects = new CGameObject*[7];
	for(int i = 0; i < 7; i++) 
		m_talkBubbleObjects[i] = NULL;
	m_textBoxObjects = new CGameObject*[7];
	for(int i = 0; i < 7; i++) 
		m_textBoxObjects[i] = NULL;
	m_alphaObjects = new CGameObject*[7];
	for(int i = 0; i < 7; i++) 
		m_alphaObjects[i] = NULL;

	m_invintoryLootObjects = new CGameObject*[m_nMaxPlayerLootObjects];

	for(int i=0;i<m_nMaxPlayerLootObjects;i++)
		m_invintoryLootObjects[i] = NULL;

	m_itemCursorObjects = new CGameObject*[1];
		m_itemCursorObjects[0] = NULL;

	m_nMaxHealthCountMob = 33;

  LV = 1; //Default, will change in user loadfile
  defineNextXP();
  defineLoot();

  //Character Weapon
  m_equipmentObjects[0] = new weapon("A Weak Training Sword","sword", SWORD_OBJECT,1,1,1,true,2,4);
}

CObjectManager::~CObjectManager(){ //destructor
	for(int i=0; i<m_nMaxCount; i++) //for each object
		delete m_pObjectList[i]; //delete it

	delete[]m_pObjectList; //delete object list
	delete m_pPlayerObject; //delete player object
}

//Creates the zone

void CObjectManager::createZone(int zId){

	create(LINK_OBJECT, "link", D3DXVECTOR3((float)spawnX, (float)spawnY, 450), -3, 0);
	create(HEALGUY_OBJECT,"healguy", D3DXVECTOR3((float)spawnX+10, (float)spawnY+10, 100),-3,0);

	timeToBeHitAgain = 1000; //ms
	timeHit = 0; //Starting at the default

	
	
	//File Name
	const char* g_xmlFileNameZone;
	switch(zId) {
	case 4: g_xmlFileNameZone = "zone4.xml"; break;
	case 3: g_xmlFileNameZone = "zone3.xml"; break;
	case 2: g_xmlFileNameZone = "zone2.xml"; break;
	default: g_xmlFileNameZone = "zone1.xml";
	}

	zoneHeight = 6400;
	zoneWidth = 6400;
	zoneName = "Default";
	int tileSet = 1;

	TiXmlDocument g_xmlDocumentZone(g_xmlFileNameZone); ///< TinyXML document for settings.
	TiXmlElement* g_xmlSettingsZone = NULL; ///< TinyXML element for settings tag..	
	g_xmlDocumentZone.LoadFile();
	g_xmlSettingsZone = g_xmlDocumentZone.FirstChildElement("settings");

	
	if(g_xmlSettingsZone){

		
		//Zone Info
		TiXmlElement* ist = g_xmlSettingsZone->FirstChildElement("zoneinfo");
		if(ist) {
			ist->Attribute("width", &zoneWidth);
			ist->Attribute("height", &zoneHeight);
			ist->Attribute("tileSet", &tileSet);
		}
	
		ist = g_xmlSettingsZone->FirstChildElement("name");
		if(ist){
			//Effing rediculous to convert a const char * to char*
			const char* temp = ist->GetText();
			zoneName = new char[strlen(temp)];
			strcpy(zoneName, temp);
		}

		ist = g_xmlSettingsZone->FirstChildElement("exits");
		if(ist)
			ist->Attribute("size",&m_nMaxExitCount);
	
		m_nExitCount = 0;
		m_exitObjects = new CGameObject*[m_nMaxExitCount];
		for(int i = 0; i < m_nMaxExitCount; i++)
				m_exitObjects[i] = NULL;
		exitInfo = new exitInformation[m_nMaxExitCount];

		for(int r=0; r<m_nMaxExitCount;r++){
			exitInfo[r].keyId = 0;
			exitInfo[r].spawnX = 0;
			exitInfo[r].spawnY = 0;
			exitInfo[r].x = -1;
			exitInfo[r].y = -1;
			exitInfo[r].zoneId = 0;
		}

		int exitCounter = 0;
		ist = g_xmlSettingsZone->FirstChildElement("exit");
		while(ist){
				ist->Attribute("x",&exitInfo[exitCounter].x);
				ist->Attribute("y",&exitInfo[exitCounter].y);
				ist->Attribute("zoneId",&exitInfo[exitCounter].zoneId);
				ist->Attribute("spawnX",&exitInfo[exitCounter].spawnX);
				ist->Attribute("spawnY",&exitInfo[exitCounter].spawnY);
				ist->Attribute("keyId",&exitInfo[exitCounter].keyId);
				exitCounter++;
				ist = ist->NextSiblingElement("exit"); }


		createForest(tileSet,zoneHeight,zoneWidth);
		createTile(tileSet,zoneHeight,zoneWidth);
		createWall(tileSet,zoneHeight,zoneWidth);

		 int minA = 0;
		 int maxA = 0;
		 int numHouses = 0;
		 int plantTotal = 0;
		 int rockTotal = 0;
		 int specificObjects32 = 0;
		 int specificObjects64 = 0;

	     ist = g_xmlSettingsZone->FirstChildElement("plants");
			if(ist){
				ist->Attribute("min",&minA);
				ist->Attribute("max",&maxA);
			}

		plantTotal = g_cRandom.number(minA,maxA);
		minA = 0;
		maxA = 0;

		 ist = g_xmlSettingsZone->FirstChildElement("rocks");
			if(ist){
				ist->Attribute("min",&minA);
				ist->Attribute("max",&maxA);
			}
		 rockTotal = g_cRandom.number(minA,maxA);

		 ist = g_xmlSettingsZone->FirstChildElement("houses");
			if(ist)
				ist->Attribute("size",&numHouses);

		 ist = g_xmlSettingsZone->FirstChildElement("objects32");
			if(ist)
				ist->Attribute("size",&specificObjects32);

	     ist = g_xmlSettingsZone->FirstChildElement("objects64");
		    if(ist)
			    ist->Attribute("size",&specificObjects64);

		 m_nMaxImpassibleCount32 = plantTotal + rockTotal + numHouses + specificObjects32;
		 m_nImpassibleCount32 = 0;
		 m_impasibleObjects32 = new CGameObject*[m_nMaxImpassibleCount32];
			for(int i=0; i<m_nMaxImpassibleCount32; i++)
				m_impasibleObjects32[i] = NULL;

		 m_nMaxImpassibleCount = 2*numHouses + specificObjects64;
		 m_nImpassibleCount = 0;
		 m_impasibleObjects = new CGameObject*[m_nMaxImpassibleCount];
			 for(int i=0; i<m_nMaxImpassibleCount; i++)
				m_impasibleObjects[i] = NULL;

		 ist = g_xmlSettingsZone->FirstChildElement("object32");
		  while(ist) {
			  int x =0;
			  int y =0;
			  int type = 0;
			  ObjectType object = DUMMY_OBJECT;
			  char* name = "dumb";

			  ist->Attribute("x",&x);
			  ist->Attribute("y",&y);
			  ist->Attribute("type",&type);

			  //Add the specific id's of 32 X 32 here
			  switch(type){
			  case 2: object = ROCK_OBJECT; name = "rock"; break;
			  case 1: object = PLANT1_OBJECT; name = "plant1"; break;
			  default: object = DUMMY_OBJECT; name = "dummy";
			  }

			  D3DXVECTOR3 v = D3DXVECTOR3((float)x,(float)y,500.0f);

			  m_impasibleObjects32[m_nImpassibleCount32++] = new CGameObject(object, name, v, 0, 0);
			  ist = ist->NextSiblingElement("object32"); 
		  }
		  
		  ist = g_xmlSettingsZone->FirstChildElement("object64");
			  while(ist) {
				   int x =0;
				   int y =0;
				   int type = 0;
			       ObjectType object = DUMMY_OBJECT;
			 	   char* name = "dumb";

				  ist->Attribute("x",&x);
				  ist->Attribute("y",&y);
				  ist->Attribute("type",&type);

				  //Add the specific id's of 64 X 64 here
				  switch(type){
				  case 2: object = ROCK_OBJECT; name = "rock"; break;
				  case 1: object = PLANT1_OBJECT; name = "plant1"; break;
				  default: object = DUMMY_OBJECT; name = "dummy";
				  }

				  D3DXVECTOR3 v = D3DXVECTOR3((float)x,(float)y,500.0f);

				  m_impasibleObjects[m_nImpassibleCount++] = new CGameObject(object, name, v, 0, 0);
				  ist = ist->NextSiblingElement("object64"); 
			  }
			
		    
			ist = g_xmlSettingsZone->FirstChildElement("house");
			while(ist) {
					int x = 0;
					int y = 0;

					ist->Attribute("x",&x);
					ist->Attribute("y",&y);
					ist->Attribute("zoneId",&exitInfo[exitCounter].zoneId);
					ist->Attribute("spawnX",&exitInfo[exitCounter].spawnX);
					ist->Attribute("spawnY",&exitInfo[exitCounter].spawnY);
					ist->Attribute("keyId",&exitInfo[exitCounter].keyId);
					exitCounter++;
					createHouse((float)x,(float)y,1);
					ist = ist->NextSiblingElement("house");
					}
			
			//Generating the 32 by 32 plants
			for(int i=0; i<plantTotal; i++) {
				  bool inAnotherObject = false;
				  D3DXVECTOR3 v;
				    v.z = 500.0f;
					v.x = (float)g_cRandom.number(0, (int)(zoneWidth/2.0f - 590));
					if(g_cRandom.number(0, 100) < 50)	
						v.x = -v.x;
					v.y = (float)g_cRandom.number(0, (int)(zoneHeight/2.0f - 590));
					if(g_cRandom.number(0, 100) < 50)
						v.y = -v.y;
				
					//smart placement
					
					CGameObject* temp = new CGameObject(DUMMY_OBJECT, "dummy", v, 0,0);
					for(int i=0; i<m_nImpassibleCount32; i++) {
						if(distanceRect(temp, m_impasibleObjects32[i]) < 25) {
							v.x = (float)g_cRandom.number(0, (int)(zoneWidth/2.0f - 590));
							if(g_cRandom.number(0, 100) < 50)	
								v.x = -v.x;
							v.y = (float)g_cRandom.number(0, (int)(zoneHeight/2.0f - 590));
							if(g_cRandom.number(0, 100) < 50)
								v.y = -v.y;

							temp->m_structLocation = v;
							i = 0;
						}
					}
						for(int i=0; i<m_nImpassibleCount; i++) {
							if(distanceRect(temp, m_impasibleObjects[i]) < 45) {
							v.x = (float)g_cRandom.number(0, (int)(zoneWidth/2.0f - 590));
							if(g_cRandom.number(0, 100) < 50)	
								v.x = -v.x;
							v.y = (float)g_cRandom.number(0, (int)(zoneHeight/2.0f - 590));
							if(g_cRandom.number(0, 100) < 50)
								v.y = -v.y;

							temp->m_structLocation = v;
							i = 0;
							}
						}

			  m_impasibleObjects32[m_nImpassibleCount32++] = new CGameObject(PLANT1_OBJECT, "plant1", v, 0, 0); 
			}
			
			//Generating the 32 by 32 rocks
			for(int i=0; i<rockTotal; i++) {
				  bool inAnotherObject = false;
				  D3DXVECTOR3 v;
				    v.z = 500.0f;
					v.x = (float)g_cRandom.number(0, (int)(zoneWidth/2.0f - 590));
					if(g_cRandom.number(0, 100) < 50)	
						v.x = -v.x;
					v.y = (float)g_cRandom.number(0, (int)(zoneHeight/2.0f - 590));
					if(g_cRandom.number(0, 100) < 50)
						v.y = -v.y;
				
					//smart placement
					
					CGameObject* temp = new CGameObject(DUMMY_OBJECT, "dummy", v, 0,0);
					for(int i=0; i<m_nImpassibleCount32; i++) {
						if(distanceRect(temp, m_impasibleObjects32[i]) < 25) {
							v.x = (float)g_cRandom.number(0, (int)(zoneWidth/2.0f - 590));
							if(g_cRandom.number(0, 100) < 50)	
								v.x = -v.x;
							v.y = (float)g_cRandom.number(0, (int)(zoneHeight/2.0f - 590));
							if(g_cRandom.number(0, 100) < 50)
								v.y = -v.y;

							temp->m_structLocation = v;
							i = 0;
						}
					}
						for(int i=0; i<m_nImpassibleCount; i++) {
							if(distanceRect(temp, m_impasibleObjects[i]) < 45) {
							v.x = (float)g_cRandom.number(0, (int)(zoneWidth/2.0f - 590));
							if(g_cRandom.number(0, 100) < 50)	
								v.x = -v.x;
							v.y = (float)g_cRandom.number(0, (int)(zoneHeight/2.0f - 590));
							if(g_cRandom.number(0, 100) < 50)
								v.y = -v.y;

							temp->m_structLocation = v;
							i = 0;
							}
						}
				if(m_nImpassibleCount32 < m_nMaxImpassibleCount32)
					m_impasibleObjects32[m_nImpassibleCount32++] = new CGameObject(ROCK_OBJECT, "rock", v, 0, 0);
		}

		ist = g_xmlSettingsZone->FirstChildElement("monster");

		//monsterInfo 
		monsterInfo = new monsterInformation[MAX_OBJECTS];
		for(int r=0; r<MAX_OBJECTS;r++){
			monsterInfo[r].xp = 0;
			monsterInfo[r].bossId = 0;
			monsterInfo[r].HP = 0;
			monsterInfo[r].LV = 0;
			monsterInfo[r].tClass = 0;
			monsterInfo[r].totalHP = 0;
		}
		int tempMCount = 0; 

		while(ist)
		{
			int id = 0;
			int min = 0;
			int max = 0;
			int mLV = 1;
			int mHP = 10;
			int mTC = 1;
			int mX = 0;
			int mY = 0;
			int mXp = 0;
			int bId = 0;

			ist->Attribute("id",&id);
			ist->Attribute("min",&min);
			ist->Attribute("max",&max);
			ist->Attribute("lv",&mLV);
			ist->Attribute("hp",&mHP);
			ist->Attribute("tc",&mTC);
			ist->Attribute("x",&mX);
			ist->Attribute("y",&mY);
			ist->Attribute("xp",&mXp);
			ist->Attribute("bossId",&bId);

			//Gather other information here

			ObjectType monster = CROW_OBJECT;
			char* monsterName = "crow";

			switch(id) {
				//Monsters go here, case corresponds to monster information in zone xml file
			case 0: monster = CROW_OBJECT; monsterName = "crow"; break;
			case 1: monster = MONSTER_OBJECT; monsterName = "monster"; break;
			case 2: monster = MONSTER2_OBJECT; monsterName = "monster2"; break;
			case 3: monster = FLAMEGUY_OBJECT; monsterName = "flameguy"; break;
			case 4: monster = FLAMEGUY2_OBJECT; monsterName = "flameguy2"; break;
			case 5: monster = HEALGUY_OBJECT; monsterName="healguy";break;//healing person
			case 6: monster = ROUNDMAN_OBJECT; monsterName = "roundman"; break;
			case 7: monster = ROUNDMAN2_OBJECT; monsterName = "roundman2"; break;
			case 8: monster = TREASURE_OBJECT; monsterName = "treasure"; break;
			}

			D3DXVECTOR3 v;

			min = g_cRandom.number(min,max);
			for(int i = 0; i < min; i++) {

			if(mX == 0 && mY == 0){

				CGameObject* temp = NULL;
				do{
					v.x = (float)g_cRandom.number(0, (int)(zoneWidth/2.0f - 590));
					if(g_cRandom.number(0, 100) < 50)
						v.x = -v.x;
					v.y = (float)g_cRandom.number(0, (int)(zoneHeight/2.0f - 590));
					if(g_cRandom.number(0, 100) < 50)
						v.y = -v.y;
					v.z = 500.0f;

				 CGameObject* temp = new CGameObject(DUMMY_OBJECT,"dummy",v,0,0);
				}while(monsterSmartMoveCheck(temp));

			//delete temp;

				}
				else{
					v.x = (float)mX;
					v.y = (float)mY;
					v.z = 500.0f;
				}

			monsterInfo[m_nCount].HP = mHP;
			monsterInfo[m_nCount].totalHP = mHP;
			monsterInfo[m_nCount].LV = mLV;
			monsterInfo[m_nCount].tClass = mTC;
			monsterInfo[m_nCount].xp = mXp;
			monsterInfo[m_nCount].bossId = bId;

			create(monster, monsterName, v, -1, 0);
			tempMCount++;
			}

			ist = ist->NextSiblingElement("monster");
			
		}

		monsterAmount = tempMCount;
		monsterAmount = 3;

		monsterHealthBars = new monsterHealthBarsList[monsterAmount];
		for(int i=0; i<monsterAmount; i++)
		{
			monsterHealthBars[i].m_monsterHealthFrameObjects = new CGameObject*[m_nMaxHealthCountMob];
			monsterHealthBars[i].m_monsterHealthObjects = new CGameObject*[m_nMaxHealthCountMob];
			monsterHealthBars[i].show = false;
			monsterHealthBars[i].m_level = NULL;
			monsterHealthBars[i].mLV = 0;
			monsterHealthBars[i].currentMonster = -1;

			for(int b=0; b<m_nMaxHealthCountMob; b++)
			{
				monsterHealthBars[i].m_monsterHealthFrameObjects[b] = NULL;
				monsterHealthBars[i].m_monsterHealthObjects[b] = NULL;
			}
		}

	}//End main if

/////////////////////////////////Generate Smart NPCs/////////////////////////////////////////////
	ObjectType NPC[1];
	NPC[0] = NPC_OBJECT;

	char* NPCNames[1];
	NPCNames[0] = "NPC";

	D3DXVECTOR3 vG;
	vG.x = 250;
	vG.y =250;
	vG.z = 500.0f;

	//NPC[1] = NPC_OBJECT;
	//NPCNames[1] = "NPC";

	//D3DXVECTOR3 vG2;
	//vG2.x = 150;
	//vG2.y = 250;
	//vG2.z = 500.0f;

	create(NPC[0], NPCNames[0], vG, 0, 0);

	//create(NPC[1], NPCNames[1], vG2, 0, 0);
/////////////////////////////////Generate Smart NPCs/////////////////////////////////////////////
 
  m_mapObjectsCount = 2*(zoneHeight/64 + zoneWidth/64) + m_nMaxExitCount + 1;
  m_miniMapObjects = new CGameObject*[m_mapObjectsCount];

  for(int i=0; i<m_mapObjectsCount; i++){
		m_miniMapObjects[i] = NULL;
	}

  if(!loadedSaved)
	 loadSavedGame();
	
  createUi();
  healthBox();
  healthBar();
  manaBar();
  
  
} //End

void CObjectManager::createHouse(float x, float y, int type)
{
	int size2; // = 32;
	ObjectType objects[4];
	char* names[4];

	//For different houses we will define the object types and chars here
	switch(type)
	{
	default:
		size2 = 32;
		objects[0] = HOUSET1P1_OBJECT;
		objects[1] = HOUSET1P2_OBJECT;
		objects[2] = HOUSET1P3_OBJECT;
		objects[3] = HOUSET1P1_OBJECT;

		names[0] = "houset1p1";
		names[1] = "houset1p2";
		names[2] = "houset1p3";
		names[3] = "houset1p1";
	}

  D3DXVECTOR3 v = D3DXVECTOR3(x, y, 500.0f);

  m_impasibleObjects[m_nImpassibleCount++] = new CGameObject(objects[0], names[0], v, 0, 0);
  v.x = (float)(v.x + size2 * 1.5);
  v.y = (float)(v.y - size2 * 0.5);
  //The door
  // create(objects[1], names[1], v, 0, 0);
  zoneIds[m_nExitCount] = exitInfo[m_nExitCount].zoneId;
  m_exitObjects[m_nExitCount++] = new CGameObject(objects[1], names[1], v, 0,0);
  //LV = m_nExitCount;
  v.y = v.y + size2;
  m_impasibleObjects32[m_nImpassibleCount32++] = new CGameObject(objects[2], names[2], v, 0, 0);
  v.x = (float)(v.x + size2 * 1.5);
  v.y = (float)(v.y - size2 * 0.5);
  m_impasibleObjects[m_nImpassibleCount++] = new CGameObject(objects[3], names[3], v, 0, 0);
}

void CObjectManager::create(ObjectType object, char* name, D3DXVECTOR3 location,
                           float xspeed, float yspeed){

	if(object == MENU_OBJECT){ //intelligent object
		m_pObjectList[m_nMenuCount++] = new CGameObject(MENU_OBJECT,name, location, 0, 0);
	}else if(object == POINTER_OBJECT){ //intelligent object
		m_pObjectList[m_nMenuCount++] = new CGameObject(POINTER_OBJECT,name, location, 0, 0);

	}else if(object != LINK_OBJECT){
		//not the player object
		if(m_nCount < m_nMaxCount){ //if room, create object
			int i=0;
			while(m_pObjectList[i]) ++i; //find first free slot
				if(object == CROW_OBJECT){ //intelligent object
					m_pObjectList[i] = new CCrowObject(name, location, xspeed, yspeed);
				}else if(object == MONSTER_OBJECT){ //intelligent object
					m_pObjectList[i] = new CMonsterObject(name, location, xspeed, yspeed);
				}else if(object == MONSTER2_OBJECT){ //intelligent object
					m_pObjectList[i] = new CMonster2Object(name, location, xspeed, yspeed);
				}else if(object == FLAMEGUY_OBJECT){ //intelligent object
					m_pObjectList[i] = new CFlameGuyObject(name, location, xspeed, yspeed);
				}else if(object == FLAMEGUY2_OBJECT){ //intelligent object
					m_pObjectList[i] = new CFlameGuy2Object(name, location, xspeed, yspeed);
				}else if(object == ROUNDMAN_OBJECT){ //intelligent object
					m_pObjectList[i] = new CRoundManObject(name, location, xspeed, yspeed);
				}
				
				else if(object == ROUNDMAN2_OBJECT){ //intelligent object
					m_pObjectList[i] = new CRoundMan2Object(name, location, xspeed, yspeed);

				}
				else if(object == HEALGUY_OBJECT)
				{
					m_pObjectList[i] = new CHealGuyObject(name, D3DXVECTOR3(500,500,500), 0, 0);

				}
				
				else if(object == INVENTORY_OBJECT){ //intelligent object
					m_pObjectList[i] = new CGameObject(INVENTORY_OBJECT,name, location, 0, 0);
				}else //dumb object
					m_pObjectList[i] = new CGameObject(object, name, location, xspeed, yspeed);
			m_nCount++; //one more object
		}
	}else
		m_pPlayerObject = new CGameObject(LINK_OBJECT, name, location, xspeed, yspeed);
}

void CObjectManager::createSpells(ObjectType object, char* name, D3DXVECTOR3 location, float xspeed, float yspeed)
{
	if(m_nSpellCount < m_nMaxSpellCount){
		int i=0;
		while(m_spellObjects[i]) i++;

		switch(object){
		case BARRIER_OBJECT: m_spellObjects[i] = new CBarrierObject(name,location,xspeed,yspeed); break;
		case TORNADO_OBJECT: m_spellObjects[i] = new CTornadoObject(name,location,xspeed,yspeed); break;
		case ENEMYHOMING_OBJECT: m_spellObjects[i] = new CHomingObject(name,location,xspeed,yspeed); break;
		case LINKATK_OBJECT: m_spellObjects[i] = new CGameObject(LINKATK_OBJECT, "linkATK", location, xspeed, yspeed);break;
		default: m_spellObjects[i] = new CGameObject(object, name, location, xspeed, yspeed);break;
		}

		spellTimers[i] = g_cTimer.time() + 50; //Spells will last for 2 seconds
		m_nSpellCount++;
		collisionCount++;			
	}
}

void CObjectManager::createForest(int tileSet, int zoneHeight, int zoneWidth) {
		//Tile
	m_nMaxTileCount = zoneHeight * zoneWidth / 64;
	m_nTileCount = 0;
	m_tileObjects = new CGameObject*[m_nMaxTileCount];
	for(int i = 0; i < m_nMaxTileCount; i++) 
		m_tileObjects[i] = NULL;

	int zHeight = zoneHeight / 64;
	int zWidth = zoneWidth / 64;

	// Class Tile

		D3DXVECTOR3 v;
		v.z = 500.0f;

		v.y = zoneHeight / 2.0f;
		float vOriginalX = - zoneWidth / 2.0f;

		CRandom g_cRandom;
		
			for(int r = 0; r < zHeight; r++) {
				v.y = v.y - 64.0;
				v.x = vOriginalX;

				for(int c = 0; c < zWidth; c++) {

					bool drawThis = true;

					if(r > 7 && r < (zHeight - 8)){
						if(c == 7){
							c = zWidth-9;
							drawThis = false;
						}
					}

					if(drawThis){
						v.x = vOriginalX + (c+1)*64.0;		
						if(tileSet == 1)
							m_tileObjects[m_nTileCount] = new CGameObject(FOREST1_OBJECT, "forest1", v, 0,0);
						if(tileSet == 2)
							m_tileObjects[m_nTileCount] = new CGameObject(DUNGEON1COCKBLOCK, "bigdungeoncockblock1", v, 0,0);
						if(tileSet == 3)
							m_tileObjects[m_nTileCount] = new CGameObject(DESERT1_OBJECT, "desert1", v, 0,0);
						m_nTileCount++;
					}
					
				}
			}	
}
void CObjectManager::createTile(int tileSet, int zoneHeight, int zoneWidth)
{
	//Tile
	m_nMaxTileCount = zoneHeight * zoneWidth / 64;

	int zHeight = zoneHeight / 64;
	int zWidth = zoneWidth / 64;

		D3DXVECTOR3 v;
		v.z = 500.0f;

		v.y = zoneHeight / 2.0f - 448;
		float vOriginalX = - zoneWidth / 2.0f + 448;

		CRandom g_cRandom;

			for(int r = 0; r < zHeight-16; r++)
			{
				v.y = (float)(v.y - 64.0);
				v.x = vOriginalX;
				for(int c = 0; c< zWidth-16; c++)
				{	
					v.x = (float)(v.x + 64.0);

					if(tileSet == 1){ // Class Tile
						if(g_cRandom.number(0,20) <= 19)
							m_tileObjects[m_nTileCount] = new CGameObject(GRASS1_OBJECT, "grass1", v, 0,0);
						else
							if(g_cRandom.number(0,1) <= 0)
								m_tileObjects[m_nTileCount] = new CGameObject(FLOWER1_OBJECT, "flower1", v, 0,0);
							else
								m_tileObjects[m_nTileCount] = new CGameObject(GRASS2_OBJECT, "grass2", v, 0,0);
								
						m_nTileCount++;
					}
					else if(tileSet == 2){
						if(g_cRandom.number(0,20) <= 19)
							m_tileObjects[m_nTileCount] = new CGameObject(DUNGEON1_OBJECT,"dungeon1",v,0,0);
						else 
							if(g_cRandom.number(0,10) <= 3)
							m_tileObjects[m_nTileCount] = new CGameObject(DUNGEON2_OBJECT,"dungeon2",v,0,0);
								else 
									if (g_cRandom.number(0,7) <= 2)
									m_tileObjects[m_nTileCount] = new CGameObject(DUNGEON3_OBJECT,"dungeon3",v,0,0);
										else 
											if(g_cRandom.number(0,7) <= 2)
											m_tileObjects[m_nTileCount] = new CGameObject(DUNGEON4_OBJECT,"dungeon4",v,0,0);
												else 
													if(g_cRandom.number(0,1) <= 0)
													m_tileObjects[m_nTileCount] = new CGameObject(DUNGEON5_OBJECT,"dungeon5",v,0,0);
														else
															m_tileObjects[m_nTileCount] = new CGameObject(DUNGEON6_OBJECT,"dungeon6",v,0,0);
						m_nTileCount++;
					}
					if(tileSet == 3){ // Class Tile
						if(g_cRandom.number(0,20) <= 19)
							m_tileObjects[m_nTileCount] = new CGameObject(DESERT1_OBJECT, "desert1", v, 0,0);
						else
							m_tileObjects[m_nTileCount] = new CGameObject(DESERT2_OBJECT, "desert2", v, 0,0);

						m_nTileCount++;
					}
				}
			}
	//}
}


void CObjectManager::createWall(int tileSet, int zoneHeight, int zoneWidth){
	//Wall
	int zHeight = zoneHeight / 64;
	int zWidth = zoneWidth / 64;
	m_nMaxWallCount = 2*(zHeight - 13) + 2*(zWidth - 13);
	m_nWallCount = 0;
	m_wallObjects = new CGameObject*[m_nMaxWallCount];

	for(int i = 0; i < m_nMaxWallCount; i++) 
		m_wallObjects[i] = NULL;
	
	if(tileSet == 1 || tileSet == 2 || tileSet == 3){ // Class Tile

		D3DXVECTOR3 v;
		v.z = 500.0f;
		
		//float vOriginalX = -zoneWidth/2.0f + 64*6;
		
		CRandom g_cRandom;

		//Top Left
		v.y = zoneHeight/2.0f - 64*8;// - 64*8;
		v.x = -zoneWidth/2.0f + 64*8;// + 64*8;
		m_wallObjects[m_nWallCount] = new CGameObject(WALL2_OBJECT, "wall2", v, 0,0);
		m_nWallCount++;

		//Top Right
		v.y = zoneHeight/2.0f - 64*8;// - 64*8;
		v.x = zoneWidth/2.0f - 64*8;// - 64*16;
		m_wallObjects[m_nWallCount] = new CGameObject(WALL2_OBJECT, "wall2", v, 0,0);
		m_nWallCount++;

		//Bottom Left
		v.y = -zoneHeight/2.0f + 64*8;// - 64*8;
		v.x = -zoneWidth/2.0f + 64*8;// - 64*16;
		m_wallObjects[m_nWallCount] = new CGameObject(WALL2_OBJECT, "wall2", v, 0,0);
		m_nWallCount++;

		//Bottom Right
		v.y = -zoneHeight/2.0f + 64*8;// - 64*8;
		v.x = zoneWidth/2.0f - 64*8;// - 64*16;
		m_wallObjects[m_nWallCount] = new CGameObject(WALL2_OBJECT, "wall2", v, 0,0);
		m_nWallCount++;

		//New ZoneIds
		zoneIds = new int[m_nMaxExitCount];

		//Top
		v.x = -zoneWidth/2.0f + 64*8;
		v.y = zoneHeight/2.0f - 64*8;
		for(int r=0; r < zWidth-17; r++){
			v.x = v.x + 64.0f;

			m_wallObjects[m_nWallCount] = new CGameObject(WALL1_OBJECT, "wall1", v, 0,0);

			for(int i=0; i<m_nMaxExitCount; i++){
				if(exitInfo[i].y == 0)
					if(r == exitInfo[i].x){
						delete m_wallObjects[m_nWallCount--];
						zoneIds[m_nExitCount] = exitInfo[i].zoneId;
						if(exitInfo[i].keyId > 0)
							m_exitObjects[m_nExitCount++] = new CGameObject(EXIT2_OBJECT, "exit2", v, 0,0);
						else
							m_exitObjects[m_nExitCount++] = new CGameObject(EXIT1_OBJECT, "exit1", v, 0,0);
					}
			}

			//m_wallObjects[m_nWallCount] = new CGameObject(WALL1_OBJECT, "wall1", v, 0,0);
			m_nWallCount++;
		}

		//Bottom
		v.x = -zoneWidth/2.0f + 64*8;
		v.y = -zoneHeight/2.0f + 64*8;
		for(int r=0; r < zWidth-17; r++){
			v.x = v.x + 64.0f;

			m_wallObjects[m_nWallCount] = new CGameObject(WALL1_OBJECT, "wall1", v, 0,0);

			for(int i=0; i<m_nMaxExitCount; i++){
				if(exitInfo[i].y == (zHeight-18))
					if(r == exitInfo[i].x){
						delete m_wallObjects[m_nWallCount--];
						zoneIds[m_nExitCount] = exitInfo[i].zoneId;
						if(exitInfo[i].keyId > 0)
							m_exitObjects[m_nExitCount++] = new CGameObject(EXIT2_OBJECT, "exit2", v, 0,0);
						else
							m_exitObjects[m_nExitCount++] = new CGameObject(EXIT1_OBJECT, "exit1", v, 0,0);
					}
			}
			m_nWallCount++;
		}

		//Left
		v.x = -zoneWidth/2.0f + 64*8;
		v.y = zoneHeight/2.0f - 64*8;
		for(int r=0; r < zHeight-17; r++){
			v.y = v.y - 64.0f;
			m_wallObjects[m_nWallCount] = new CGameObject(WALL1_OBJECT, "wall1", v, 0,0);


			for(int i=0; i<m_nMaxExitCount; i++){
				if(exitInfo[i].x == 0)
					if(r == exitInfo[i].y){
						delete m_wallObjects[m_nWallCount--];
						zoneIds[m_nExitCount] = exitInfo[i].zoneId;
						if(exitInfo[i].keyId > 0)
							m_exitObjects[m_nExitCount++] = new CGameObject(EXIT2_OBJECT, "exit2", v, 0,0);
						else
							m_exitObjects[m_nExitCount++] = new CGameObject(EXIT1_OBJECT, "exit1", v, 0,0);
					}
			}

			//m_wallObjects[m_nWallCount] = new CGameObject(WALL1_OBJECT, "wall1", v, 0,0);
			m_nWallCount++;
		}

		//Right
		v.x = zoneWidth/2.0f - 64*8;
		v.y = zoneHeight/2.0f - 64*8;
		for(int r=0; r < zHeight-17; r++){
			v.y = v.y - 64.0f;
			m_wallObjects[m_nWallCount] = new CGameObject(WALL1_OBJECT, "wall1", v, 0,0);

			for(int i=0; i<m_nMaxExitCount; i++){
				if(exitInfo[i].x == (zWidth-18))
					if(r == exitInfo[i].y){
						delete m_wallObjects[m_nWallCount--];
						zoneIds[m_nExitCount] = exitInfo[i].zoneId;
						if(exitInfo[i].keyId > 0)
							m_exitObjects[m_nExitCount++] = new CGameObject(EXIT2_OBJECT, "exit2", v, 0,0);
						else
							m_exitObjects[m_nExitCount++] = new CGameObject(EXIT1_OBJECT, "exit1", v, 0,0);
					}
			}

			m_nWallCount++;
		}
	}

	//Finish creating exits
	//zoneIds[m_nExitCount] = exitInfo[].zoneId;
}

void CObjectManager::destroyWorld()
{
	for(int i=0; i<m_nMaxTileCount; i++) //for each object slot
		if(m_tileObjects[i]) //if there's an object there
			m_tileObjects[i] = NULL; //draw it
	for(int i=0; i<m_nMaxWallCount; i++) //for each object slot
		if(m_wallObjects[i]) //if there's an object there
			m_wallObjects[i] = NULL; //draw it
	for(int i=0; i<m_nMaxExitCount; i++) //for each object slot
		if(m_exitObjects[i]) //if there's an object there
			m_exitObjects[i] = NULL; //draw it
	for(int i=0; i<m_nMaxCount; i++) //for each object slot
		if(m_pObjectList[i]) //if there's an object there
			m_pObjectList[i] = NULL; //draw it
	for(int i=0; i<m_nMaxImpassibleCount; i++)
		if(m_impasibleObjects[i])
			m_impasibleObjects[i] = NULL;
	for(int i=0; i<m_nMaxImpassibleCount32; i++)
		if(m_impasibleObjects32[i])
			m_impasibleObjects32[i] = NULL;
	for(int i=0; i<maxGroundLoot; i++)
		if(m_groundLootObjects[i])
			m_groundLootObjects[i] = NULL;
	for(int i=0; i<maxGroundLoot; i++)
			lootOnGround[i] = NULL;
	for(int i=0; i<m_nMaxSpellCount; i++)
			m_spellObjects[i] = NULL;
	

	m_nMaxTileCount = 0;
	m_nMaxWallCount = 0;
	m_nMaxExitCount = 0;
	m_nMaxMenuCount = 0;
	m_nMaxImpassibleCount = 0;
	m_nMaxImpassibleCount32 = 0;

	m_nCount = 0; ///< How many objects in list.
    m_nTileCount = 0;
	m_nWallCount = 0;
	m_nExitCount = 0;
	m_nImpassibleCount = 0;
	m_nImpassibleCount32 = 0;
	m_nSpellCount = 0;

	inAnExit = false;

//	night = NULL;
}

void CObjectManager::move(){ 
		if(!MenuUp && !dead){
	// m_pPlayerObject->move();
	for(int i=0; i<m_nMaxCount; i++){ //for each slot
		if(m_pObjectList[i]){ //if there's an object there
			if(m_pObjectList[i]->m_nObjectType != TREASURE_OBJECT){
				if(m_pObjectList[i]->m_bIntelligent) //if intelligent, tell it about plane
					((CIntelligentObject*)m_pObjectList[i])->plane(m_pPlayerObject->m_structLocation, distance(m_pObjectList[i], m_pPlayerObject));
					 m_pObjectList[i]->move();
			}
		}
	}

	for(int i=0; i<m_nMaxSpellCount; i++){ //for each slot
		if(m_spellObjects[i]){ //if there's an object there
			if(m_spellObjects[i]->m_bIntelligent) //if intelligent, tell it about plane
				((CIntelligentObject*)m_spellObjects[i])->plane(m_pPlayerObject->m_structLocation, distance(m_spellObjects[i], m_pPlayerObject));
				 m_spellObjects[i]->move();
				 if(m_spellObjects[i]->m_nObjectType != BARRIER_OBJECT) //This checks if it's a barrier object, we don't want that to go away when it hits a wall
					 if(!spellSmartMoveCheck(m_spellObjects[i]))
							killSpell(i);
		}
	}
	if(HP<=0)
		Dead(3000);

	//collision detection
	if(mobIsHurt){
		if(g_cTimer.time() > mobHurtTime){
			delete mobHurt;
			mobHurt = NULL;
			mobIsHurt = false;
			if(m_pObjectList[mobIndex])
				m_pObjectList[mobIndex]->waitTime = 0;
		}
	}

	CollisionDetection();
	MonsterDetection(); //Detects hero/monster collisions for damage calculations
	NPCDetection();
	pickUpLoot();


	//cull old objects
	cull();
	}
}

void CObjectManager::Invincible() {
	isSafe=true;
	if(g_cTimer.elapsed(m_nLastGunFireTime, 10))
		isSafe=false;

}
void CObjectManager::draw(){ 
	// m_pPlayerObject->draw();

	for(int i=0; i<m_nMaxTileCount; i++) //for each object slot
		if(m_tileObjects[i]) //if there's an object there
			m_tileObjects[i]->draw(); //draw it
	for(int i=0; i<m_nMaxWallCount; i++) //for each object slot
		if(m_wallObjects[i]) //if there's an object there
			m_wallObjects[i]->draw(); //draw it
	for(int i=0; i<m_nMaxCount; i++) //for each object slot
		if(m_pObjectList[i]) //if there's an object there
			m_pObjectList[i]->draw(); //draw it
	if(mobHurt)
		mobHurt->draw();
	for(int i=0; i<m_nMaxImpassibleCount; i++)
		if(m_impasibleObjects[i])
			m_impasibleObjects[i]->draw();
	for(int i=0; i<m_nMaxImpassibleCount32; i++)
		if(m_impasibleObjects32[i])
			m_impasibleObjects32[i]->draw();
	for(int i=0; i<m_nMaxExitCount; i++) //for each object slot
		if(m_exitObjects[i]) //if there's an object there
			m_exitObjects[i]->draw(); //draw it
	for(int i=0; i<m_nMaxSpellCount; i++)
		if(m_spellObjects[i])
			m_spellObjects[i]->draw();
	for(int i=0; i<maxGroundLoot; i++)
		if(m_groundLootObjects[i])
			m_groundLootObjects[i]->draw();

	int healthBarsToDraw = 0;

	for(int i=0; i<monsterAmount; i++){
		
		if(monsterHealthBars[i].m_level)
			monsterHealthBars[i].m_level->draw(convertChar(monsterHealthBars[i].mLV));

			for(int b=0; b<m_nMaxHealthCountMob; b++){
				int temp = b;
				if(monsterHealthBars[i].m_monsterHealthFrameObjects[b])
					monsterHealthBars[i].m_monsterHealthFrameObjects[b]->draw();
				else
					temp = m_nMaxHealthCountMob + 1;

				if(monsterHealthBars[i].m_monsterHealthObjects[b])
						monsterHealthBars[i].m_monsterHealthObjects[b]->draw();
				b = temp;
			}
		}


	if(m_healthBoxObjects[0])
		m_healthBoxObjects[0]->draw();
	for(int i=0; i<m_nMaxHealthCount; i++) //for each object slot
		if(m_healthObjects[i]) //if there's an object there
			m_healthObjects[i]->draw();
	for(int i=0; i<m_nMaxManaCount; i++) //for each object slot
		if(m_manaObjects[i]) //if there's an object there
			m_manaObjects[i]->draw();
	
	if(m_menuObjects[0])
		m_menuObjects[0]->draw();
	if(m_pointerObjects[0])
		m_pointerObjects[0]->draw();
	if(m_itemsMenuObjects[0])
		m_itemsMenuObjects[0]->draw();
	if(m_spellsMenuObjects[0])
		m_spellsMenuObjects[0]->draw();
	if(m_optionsMenuObjects[0])
		m_optionsMenuObjects[0]->draw();
	if(m_talkBubbleObjects[0])
		m_talkBubbleObjects[0]->draw();
	if(m_textBoxObjects[0])
		m_textBoxObjects[0]->draw();


		if(Talk){
	if(TalkBubble){
		ShowTextBox();
			if(textBoxON){
				ShowText();
			}
		}
	}

	if(textBoxON){
		ShowText();
	}
	if(itemsMenuON == 1){
	if(SubMenuUp){
	for(int i=0; i<m_nMaxPlayerLootObjects; i++) //for each object slot
		
		if(playerLoot[i] != NULL){
		if(m_invintoryLootObjects[i]) //if there's an object there
			m_invintoryLootObjects[i]->draw();
		ItemTextBox();
		
		}

	}
	if(m_itemCursorObjects[0])
		m_itemCursorObjects[0]->draw();
	}
	if(optionsMenuON == 1)
		HELPBox();

	for(int i=0; i<m_mapObjectsCount; i++)
		if(m_miniMapObjects[i])
			m_miniMapObjects[i]->draw();

	if(m_Dead)
		m_Dead->draw();

	//if(isNight)
//	night->draw();
}

void CObjectManager::nightMove(){
	float x = 0.0f;
	float y = 0.0f;

	GetPlayerLocation(x,y);

	night->m_structLocation.x = x;
	night->m_structLocation.y = y;
}

/// Get the plane's location.
/// \param x returns the plane's x coordinate
/// \param y returns the plane's y coordinate

void CObjectManager::GetPlayerLocation(float &x, float &y){ //get location
	x = m_pPlayerObject->m_structLocation.x;
	y = m_pPlayerObject->m_structLocation.y;
}

/// Change the speed of the plane object up or down.
/// \param xdelta change in planes's horizontal speed
/// \param ydelta change in planes's vertical speed

void CObjectManager::inExit(){
	D3DXVECTOR3 v;
			v.x = m_pPlayerObject->m_structLocation.x;
			v.y = m_pPlayerObject->m_structLocation.y;
			v.z = 500.0f;
			//create(LOOT_OBJECT, "loot", v, 0, 0);

	for(int i=0; i < m_nExitCount; i++){
		float distanceLength = 30.0f;
		if(m_exitObjects[i]->m_nObjectType == HOUSET1P2_OBJECT)
			distanceLength = 15.0f;

		if(distance(m_pPlayerObject, m_exitObjects[i]) < distanceLength){

			bool canExit = false;
			bool noKey = true;
			bool neededKey = false;

			if(exitInfo[i].keyId > 0){
				//Need a key?
				neededKey = true;
				for(int b=0; b<m_nMaxPlayerLootObjects; b++){
					if(playerLoot[b]){
						if(playerLoot[b]->type == 5){ //This is a key
							key* temp = (key*)playerLoot[b];
								if(exitInfo[i].keyId == temp->exitDoor){
									canExit = true;
									playerLoot[b] = NULL;
									noKey = false;
									break;
								}
						}
					}
				}

				if(neededKey && noKey){
					m_pPlayerObject->m_structLocation.x = prevCoords.x;
					m_pPlayerObject->m_structLocation.y = prevCoords.y;
					healthBox();
					healthBar();
					manaBar();
				}
			}
			else
				canExit = true;	

			if(canExit){
				D3DXVECTOR3 v;
				v.x = m_pPlayerObject->m_structLocation.x;
				v.y = m_pPlayerObject->m_structLocation.y;
				v.z = 500.0f;
				inAnExit = true;

				int zoneId = zoneIds[i];
				spawnX = exitInfo[i].spawnX;
				spawnY = exitInfo[i].spawnY;
				destroyWorld();
				createZone(zoneId);
			}
		}
		else{
			inAnExit = false;		
		}
	}
}

bool CObjectManager::CanMove(float angle) {
	float step = m_pPlayerObject->step;
	float xDirection = step * cos(angle);
	float yDirection = step * sin(angle);

	D3DXVECTOR3 v;
		v.x = m_pPlayerObject->m_structLocation.x + xDirection;
		v.y = m_pPlayerObject->m_structLocation.y + yDirection;
		v.z = 500.0f;

	CGameObject *tempPlayerObject = new CGameObject(DUMMY_OBJECT, "dumb", v, 0, 0);

	for(int i=0; i < m_nWallCount; i++){
		if(distanceRect(tempPlayerObject, m_wallObjects[i]) < 35.0f){
			delete tempPlayerObject;
			return false;
		}
	}

	for(int i=0; i < m_nImpassibleCount; i++){
		if(distanceRect(tempPlayerObject, m_impasibleObjects[i]) < 36.0f){
			delete tempPlayerObject;
			return false;
		}
	}

	for(int i=0; i < m_nImpassibleCount32; i++){
		float theDistance = 21.0f;

		if(m_impasibleObjects32[i]->m_nObjectType == ROCK_OBJECT)
			theDistance = 15.0f;

		if(distanceRect(tempPlayerObject, m_impasibleObjects32[i]) < theDistance){
			delete tempPlayerObject;
			return false;
		}
	}

	prevCoords = m_pPlayerObject->m_structLocation;

	delete tempPlayerObject;
	return true;
}

bool CObjectManager::spellSmartMoveCheck(CGameObject* tempPlayerObject)
{
	for(int i=0; i < m_nWallCount; i++){
		if(distanceRect(tempPlayerObject, m_wallObjects[i]) < 35.0f){
			delete tempPlayerObject;
			return false;
		}
	}

	for(int i=0; i < m_nImpassibleCount; i++){
		if(distanceRect(tempPlayerObject, m_impasibleObjects[i]) < 35.0f){
			delete tempPlayerObject;
			return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////no monster walk through
	for(int i=0; i < m_nCount; i++){
		if(distanceRect(tempPlayerObject, m_pObjectList[i]) < 55.0f){
			delete tempPlayerObject;
			return false;
		}
	}

	for(int i=0; i < m_nImpassibleCount32; i++){
		float theDistance = 20.0f;

		if(m_impasibleObjects32[i]->m_nObjectType == ROCK_OBJECT)
			theDistance = 40.0f;

		if(distanceRect(tempPlayerObject, m_impasibleObjects32[i]) < theDistance){
			delete tempPlayerObject;
			return false;
		}
	}

	for(int i=0; i < m_nExitCount; i++){
		float theDistance = 35.0f;

		if(distanceRect(tempPlayerObject, m_exitObjects[i]) < theDistance){
			delete tempPlayerObject;
			return false;
		}
	}

	return true;
}

bool CObjectManager::monsterSmartMoveCheck(CGameObject* tempPlayerObject){
	for(int i=0; i < m_nWallCount; i++){
		if(distanceRect(tempPlayerObject, m_wallObjects[i]) < 35.0f){
			delete tempPlayerObject;
			return false;
		}
	}
	return true;
}


void CObjectManager::Accelerate(int xdelta, int ydelta){ 
	m_pPlayerObject->accelerate(xdelta, ydelta);
}

void CObjectManager::MoveDown(int move){
	if(CanMove((float)(3*PI/2))){
		m_pPlayerObject->moveDown();
		healthBox();
		healthBar();
		manaBar();
		inExit();
		//nightMove();
	}
}

void CObjectManager::MoveUp(int move){
	if(CanMove((float)(PI/2))){
		m_pPlayerObject->moveUp();
		healthBox();
		healthBar();
		manaBar();
		inExit();
		//nightMove();
		
	}
}

void CObjectManager::MoveLeft(int move){
	if(CanMove((float)(PI))){
		m_pPlayerObject->moveLeft();
		healthBox();
		healthBar();
		manaBar();
		inExit();
		//nightMove();
		
	}
}

void CObjectManager::MoveRight(int move){
	if(CanMove(0.0f)){
		m_pPlayerObject->moveRight();
		healthBox();
		healthBar();
		manaBar();
		inExit();
		//nightMove();
		//Am I in an Exit?	
	}
}

void CObjectManager::stepUp(float step){
	m_pPlayerObject->step = step;
}

float CObjectManager::distanceRect(CGameObject *first, CGameObject *second){ //return distance between objects
  //const float fWorldWidth = 2.0f * (float)g_nScreenWidth; //world width
  if(first == NULL || second == NULL)return -1; //bail if bad pointer

  //first information
  float centerX1 = first->m_structLocation.x;
  float centerY1 = first->m_structLocation.y;

  //second information
  float centerX2 = second->m_structLocation.x;
  float centerY2 = second->m_structLocation.y;

  float distanceX1 = abs(centerX1 - centerX2);
  float distanceY1 = abs(centerY1 - centerY2);

  if(distanceY1 > distanceX1) distanceX1 = distanceY1;

  return distanceX1;

}

float CObjectManager::distance(CGameObject *first, CGameObject *second){ //return distance between objects
	//const float fWorldWidth = 2.0f * (float)g_nScreenWidth; //world width

	if(first == NULL || second == NULL)return -1; //bail if bad pointer

	float x = (float)fabs(first->m_structLocation.x - second->m_structLocation.x); //x distance
	float y = (float)fabs(first->m_structLocation.y - second->m_structLocation.y); //y distance

	//return result
	return (float)sqrt(x*x + y*y);
}


/// Remove an object from the object list.
/// Assumes that there is an object there to be deleted.
/// \param index index of the object to be deleted.

void CObjectManager::kill(int index){ //remove object
	m_nCount--;
	delete m_pObjectList[index];
	m_pObjectList[index] = NULL;
}

void CObjectManager::killSpell(int index){ //remove object
	m_nSpellCount--;
	//delete m_spellObjects[index];
	if(m_spellObjects[index]){
		m_spellObjects[index] = NULL;
	}
	spellTimers[index] = 0;
}

///Activate hero's attack

void CObjectManager::HeroAttack(int key){ 
	if(g_cTimer.elapsed(m_nLastGunFireTime, 40)){ //slow down attack rate //400
		D3DXVECTOR3 v; //location of hero
		v.x = m_pPlayerObject->m_structLocation.x;
		v.y = m_pPlayerObject->m_structLocation.y;
		v.z = 500.0f;

		switch(key) {
		case 1: v.x -= 50; break; //v.x -= 50
		case 2: v.x += 50; break; //v.x += 50
		case 3: v.y -= 50; break; //v.y -= 50
		case 4: v.y += 50; break; //v.y += 50
		}
		createSpells(LINKATK_OBJECT, "linkATK", v, 0, 0);
		
	}
}

//Barrier spell
void CObjectManager::Barrier(int key){	
	int spellSpeed = 700; //getto level up for spells

	//checks level for power up
/*	if(LV >=3 ){
		spellSpeed = 400;
	}*/

	if(g_cTimer.elapsed(m_nLastBarrier, spellSpeed) && (MP-manaCost(2))>0){ //slow down firing rate
		D3DXVECTOR3 v; //location of hero
		v.x = m_pPlayerObject->m_structLocation.x;
		v.y = m_pPlayerObject->m_structLocation.y;
		v.z = 500.0f;

		//create fireball
		if(key == 1){//left attack
			v.x = m_pPlayerObject->m_structLocation.x-6;
			v.y = m_pPlayerObject->m_structLocation.y;
			createSpells(BARRIER_OBJECT, "barrier", v, -5, 0);
			MP-=manaCost(2);

		}
		if(key == 2){//right attack
			v.x = m_pPlayerObject->m_structLocation.x+6;
			v.y = m_pPlayerObject->m_structLocation.y;
			createSpells(BARRIER_OBJECT, "barrier", v, 5, 0);
			MP-=manaCost(2);

		}
		if(key == 3){
			v.x = m_pPlayerObject->m_structLocation.x;
			v.y = m_pPlayerObject->m_structLocation.y;
			createSpells(BARRIER_OBJECT, "barrier", v, 0, 5);
			MP-=manaCost(2);

		}
		if(key == 4){
			v.x = m_pPlayerObject->m_structLocation.x;
			v.y = m_pPlayerObject->m_structLocation.y;
			createSpells(BARRIER_OBJECT, "barrier", v, 0, -5);
			MP-=manaCost(2);
		}
	}
	else if(MP<75)
		g_pSoundManager->play(GUN_SOUND);
}

//Tornado spell
void CObjectManager::Tornado(int key)
{
	
	int spellSpeed = 1000; //getto level up for spells

	//checks level for power up
	if(LV >=3 ){
		spellSpeed = 400;
	}

	if(g_cTimer.elapsed(m_nLastTornado, spellSpeed)&& MP-50>0){ //slow down firing rate
		D3DXVECTOR3 v; //location of hero
		v.x = m_pPlayerObject->m_structLocation.x;
		v.y = m_pPlayerObject->m_structLocation.y;
		v.z = 100.0f;

		//create fireball
		if(key == 1){//left attack
			v.x = m_pPlayerObject->m_structLocation.x-6;
			v.y = m_pPlayerObject->m_structLocation.y;
			createSpells(TORNADO_OBJECT, "tornado", v, -5, 0);
			MP-=50;
		}
		if(key == 2){//right attack
			v.x = m_pPlayerObject->m_structLocation.x+6;
			v.y = m_pPlayerObject->m_structLocation.y;
			createSpells(TORNADO_OBJECT, "tornado", v, 5, 0);
			MP-=50;
		}
		if(key == 3){
			v.x = m_pPlayerObject->m_structLocation.x;
			v.y = m_pPlayerObject->m_structLocation.y;
			createSpells(TORNADO_OBJECT, "tornado", v, 0, 5);
			MP-=50;
		}
		if(key == 4){
			v.x = m_pPlayerObject->m_structLocation.x;
			v.y = m_pPlayerObject->m_structLocation.y;
			createSpells(TORNADO_OBJECT, "tornado", v, 0, -5);
			MP-=50;
		}
	}
	else if(MP<50)
		g_pSoundManager->play(GUN_SOUND);
}

void CObjectManager::FanofFire(){
	Fireball(6);
}

void CObjectManager::Lightning(int key){
	
	int spellSpeed = 600; //getto level up for spells

	//checks level for power up
/*	if(LV >=3 ){
		spellSpeed = 400;
	}*/

	if(g_cTimer.elapsed(m_nLastLightning, spellSpeed) && (MP-manaCost(3))>0){ //slow down firing rate
		D3DXVECTOR3 v; //location of hero
		v.x = m_pPlayerObject->m_structLocation.x;
		v.y = m_pPlayerObject->m_structLocation.y;
		v.z = 500.0f;

		//create fireball
		if(key == 1){//left attack
			v.x = m_pPlayerObject->m_structLocation.x-6;
			v.y = m_pPlayerObject->m_structLocation.y;
			createSpells(LIGHTNING_OBJECT, "lightning", v, -50, -15);
			createSpells(LIGHTNING_OBJECT, "lightning", v, -50, 0);
			createSpells(LIGHTNING_OBJECT, "lightning", v, -50, 15);
			MP-=manaCost(3);
		}
		if(key == 2 ){//right attack
			v.x = m_pPlayerObject->m_structLocation.x+6;
			v.y = m_pPlayerObject->m_structLocation.y;
			createSpells(LIGHTNING_OBJECT, "lightning", v, 50, -15);
			createSpells(LIGHTNING_OBJECT, "lightning", v, 50, 0);
			createSpells(LIGHTNING_OBJECT, "lightning", v, 50, 15);
			MP-=manaCost(3);
		}
		if(key == 3){
			v.x = m_pPlayerObject->m_structLocation.x;
			v.y = m_pPlayerObject->m_structLocation.y;
			createSpells(LIGHTNING_OBJECT, "lightning", v, -15, 50);
			createSpells(LIGHTNING_OBJECT, "lightning", v, 0, 50);
			createSpells(LIGHTNING_OBJECT, "lightning", v, 15, 50);
			MP-=manaCost(3);
		}
		if(key == 4){
			v.x = m_pPlayerObject->m_structLocation.x;
			v.y = m_pPlayerObject->m_structLocation.y;
			createSpells(LIGHTNING_OBJECT, "lightning", v, -15, -50);
			createSpells(LIGHTNING_OBJECT, "lightning", v, 0, -50);
			createSpells(LIGHTNING_OBJECT, "lightning", v, 15, -50);
			MP-=manaCost(3);
		}
}
}

///Fire hero's fireball

void CObjectManager::Fireball(int key){ 

	int spellSpeed = 500; //getto level up for spells

	//checks level for power up
/*	if(LV >=3 ){
		spellSpeed = 400;
	}*/

	if(g_cTimer.elapsed(m_nLastFireball, spellSpeed) && (MP-manaCost(1))>0){ //slow down firing rate
		D3DXVECTOR3 v; //location of hero
		v.x = m_pPlayerObject->m_structLocation.x;
		v.y = m_pPlayerObject->m_structLocation.y;
		v.z = 500.0f;

		//create fireball
		if(key == 1){//left attack
			v.x = m_pPlayerObject->m_structLocation.x-6;
			v.y = m_pPlayerObject->m_structLocation.y;
			createSpells(FIREBALL_OBJECT, "fireball", v, -50, 0);
			MP-=manaCost(1);
		}
		if(key == 2){//right attack
			v.x = m_pPlayerObject->m_structLocation.x+6;
			v.y = m_pPlayerObject->m_structLocation.y;
			createSpells(FIREBALL_OBJECT, "fireball", v, 50, 0);
			MP-=manaCost(1);
		}
		if(key == 3){
			v.x = m_pPlayerObject->m_structLocation.x;
			v.y = m_pPlayerObject->m_structLocation.y;
			createSpells(FIREBALL_OBJECT, "fireball", v, 0, 50);
			MP-=manaCost(1);
		}
		if(key == 4){
			v.x = m_pPlayerObject->m_structLocation.x;
			v.y = m_pPlayerObject->m_structLocation.y;
			createSpells(FIREBALL_OBJECT, "fireball", v, 0, -50);
			MP-=manaCost(1);
		}
		if(key == 6){
			if(MP - manaCost(4) > 0){
				v.x = m_pPlayerObject->m_structLocation.x;
				v.y = m_pPlayerObject->m_structLocation.y;
				createSpells(FIREBALL_OBJECT, "fireball", v, 0, -50);
				createSpells(FIREBALL_OBJECT, "fireball", v, (float)35.35533906, (float)-35.35533906);
				createSpells(FIREBALL_OBJECT, "fireball", v, 50, 0);
				createSpells(FIREBALL_OBJECT, "fireball", v, (float)35.35533906, (float)35.35533906);
				createSpells(FIREBALL_OBJECT, "fireball", v, 0, 50);
				createSpells(FIREBALL_OBJECT, "fireball", v, (float)-35.35533906, (float)35.35533906);
				createSpells(FIREBALL_OBJECT, "fireball", v, -50, 0);
				createSpells(FIREBALL_OBJECT, "fireball", v, (float)-35.35533906, (float)-35.35533906);
				MP-=manaCost(4);
			}
		}
		

	}
	else if(MP<50)
		g_pSoundManager->play(GUN_SOUND);

}
void CObjectManager::cull(){ //cull old objects

	CGameObject *object;

	for(int i=0; i<m_nMaxCount; i++){ //for each object
		object = m_pObjectList[i]; //current object

		if(object){ //if there's really an object there
			//died of old age
			if(object->m_nLifeTime > 0 && //if mortal and ...
				(g_cTimer.time() - object->m_nBirthTime > object->m_nLifeTime)) //...old...
				kill(i); //...then kill it 
			//one shot animation 
			if(object->m_nFrameCount > 1 && !object->m_bCycleSprite && //if plays one time...
				object->m_nCurrentFrame >= object->m_nAnimationFrameCount) //and played once already...
				replace(i); //...then replace the object
		}
	}
}

void CObjectManager::replace(int index){ //replace object by next in series
	if(m_pObjectList[index]){

		CGameObject *object = m_pObjectList[index]; //current object
		ObjectType newtype;
		BOOL successor=TRUE; //assume it has a successor
		const int namelength = 64;
		char name[namelength]; //name for new object in XML tag

		bool destroyMonsterHealth = false;
		int destroyMonsterIndex = 0;
		for(int c=0; c<monsterAmount; c++)
			if(monsterHealthBars[c].currentMonster == index) {
				destroyMonsterHealth = true;
				destroyMonsterIndex = c;
				break;
			}

		newtype = DUMMY_OBJECT;
		//decide on new object type
		switch(object->m_nObjectType){
			case TREASURE_OBJECT:
				newtype = TREASUREOPEN_OBJECT;
				strncpy_s(name, namelength, "treasureopen", sizeof(name));
				break;
			case CROW_OBJECT: 
				newtype = EXPLODINGCROW_OBJECT;
				strncpy_s(name, namelength, "explodingcrow", sizeof(name));
				break;	
			case EXPLODINGCROW_OBJECT:
				newtype = DEADCROW_OBJECT; 
				strncpy_s(name, namelength, "deadcrow", sizeof(name));
				break;
			case DEADCROW_OBJECT:
				newtype = NULL_OBJECT; 
				strncpy_s(name, namelength, "null", sizeof(name));
				if(destroyMonsterHealth)
					monsterHealthBarDestroy(destroyMonsterIndex);
				break;
			case MONSTER_OBJECT: 
				newtype = EXPLODINGMONSTER_OBJECT;
				strncpy_s(name, namelength, "explodingmonster", sizeof(name));
				break;
			case EXPLODINGMONSTER_OBJECT:
				newtype = DEADMONSTER_OBJECT; 
				strncpy_s(name, namelength, "deadmonster", sizeof(name));
				break;
			case DEADMONSTER_OBJECT:
				newtype = NULL_OBJECT; 
				strncpy_s(name, namelength, "null", sizeof(name));
				if(destroyMonsterHealth)
					monsterHealthBarDestroy(destroyMonsterIndex);
				break;
			case MONSTER2_OBJECT: 
				newtype = EXPLODINGMONSTER2_OBJECT;
				strncpy_s(name, namelength, "explodingmonster2", sizeof(name));
				break;
			case EXPLODINGMONSTER2_OBJECT:
				newtype = DEADMONSTER2_OBJECT; 
				strncpy_s(name, namelength, "deadmonster2", sizeof(name));
				break;
			case DEADMONSTER2_OBJECT:
				newtype = NULL_OBJECT; 
				strncpy_s(name, namelength, "null", sizeof(name));
				if(destroyMonsterHealth)
					monsterHealthBarDestroy(destroyMonsterIndex);
				break;
			case FLAMEGUY_OBJECT: 
				newtype = EXPLODINGFLAMEGUY_OBJECT;
				strncpy_s(name, namelength, "explodingflameguy", sizeof(name));
				break;
			case EXPLODINGFLAMEGUY_OBJECT:
				newtype = DEADFLAMEGUY_OBJECT; 
				strncpy_s(name, namelength, "deadflameguy", sizeof(name));
				break;
			case DEADFLAMEGUY_OBJECT:
				newtype = NULL_OBJECT; 
				strncpy_s(name, namelength, "null", sizeof(name));
				if(destroyMonsterHealth)
					monsterHealthBarDestroy(destroyMonsterIndex);
				break;
			case FLAMEGUY2_OBJECT: 
				newtype = EXPLODINGFLAMEGUY2_OBJECT;
				strncpy_s(name, namelength, "explodingflameguy2", sizeof(name));
				break;
			case EXPLODINGFLAMEGUY2_OBJECT:
				newtype = DEADFLAMEGUY2_OBJECT; 
				strncpy_s(name, namelength, "deadflameguy2", sizeof(name));
				break;
			case DEADFLAMEGUY2_OBJECT:
				newtype = NULL_OBJECT; 
				strncpy_s(name, namelength, "null", sizeof(name));
				if(destroyMonsterHealth)
					monsterHealthBarDestroy(destroyMonsterIndex);
				break;
			case ROUNDMAN_OBJECT: 
				newtype = EXPLODINGROUNDMAN_OBJECT;
				strncpy_s(name, namelength, "explodingroundman", sizeof(name));
				break;
			case EXPLODINGROUNDMAN_OBJECT:
				newtype = DEADROUNDMAN_OBJECT; 
				strncpy_s(name, namelength, "deadroundman", sizeof(name));
				break;
			case DEADROUNDMAN_OBJECT:
				newtype = NULL_OBJECT; 
				strncpy_s(name, namelength, "null", sizeof(name));
				if(destroyMonsterHealth)
					monsterHealthBarDestroy(destroyMonsterIndex);
				break;
			case ROUNDMAN2_OBJECT: 
				newtype = EXPLODINGROUNDMAN2_OBJECT;
				strncpy_s(name, namelength, "explodingroundman2", sizeof(name));
				break;
			case EXPLODINGROUNDMAN2_OBJECT:
				newtype = DEADROUNDMAN2_OBJECT; 
				strncpy_s(name, namelength, "deadroundman2", sizeof(name));
				break;
			case DEADROUNDMAN2_OBJECT:
				newtype = NULL_OBJECT; 
				strncpy_s(name, namelength, "null", sizeof(name));
				if(destroyMonsterHealth)
					monsterHealthBarDestroy(destroyMonsterIndex);
				break;
			default: successor = FALSE; break; //has no successor
		}

		if(successor) {
			D3DXVECTOR3 location=object->m_structLocation; //location
			float xspeed = object->m_fXspeed;
			float yspeed = object->m_fYspeed;
			kill(index);
			create(newtype, name, location, 0, 0); //create new one
		}

	}
}

void CObjectManager::CollisionDetection(){

	manaBar();

	for(int i = 0; i < m_nMaxSpellCount; i++){ //for each object slot
		if(m_spellObjects[i]){ //if object exists
			switch (m_spellObjects[i]->m_nObjectType){
				case LINKATK_OBJECT: CollisionDetection(i); break;
				case FIREBALL_OBJECT: CollisionDetection(i); break;
				case LIGHTNING_OBJECT: CollisionDetection(i); break;
				case TORNADO_OBJECT: CollisionDetection(i); break;
				case BARRIER_OBJECT: CollisionDetection(i); break;
			}
		}
	}
}

void CObjectManager::NPCDetection(){ 

	D3DXVECTOR3 whee; //location of hero
	whee.x = 0;
	whee.y = 0;
	whee.z = 500.0f;

    for(int i = 0; i < m_nMaxCount; i++){ //for each object slot
		if(m_pObjectList[i]){ //if object exists
			switch (m_pObjectList[i]->m_nObjectType){
				case NPC_OBJECT:{
					if(distance(m_pObjectList[i], m_pPlayerObject) < 65.0f){ //distance to NPC //25

						whee.x = m_pObjectList[i]->m_structLocation.x-3;
						whee.y = m_pObjectList[i]->m_structLocation.y + 70;

						m_talkBubbleObjects[0] = new CGameObject(TALK_OBJECT, "talkBubble", whee, 0, 0);
						TalkBubble = TRUE;

					}else{
						TalkBubble = FALSE;
						ShowTextBox();
						if(m_talkBubbleObjects[0] != NULL){
							delete m_talkBubbleObjects[0];
							m_talkBubbleObjects[0] = NULL;
						}
					}
				}
			}
		}
	}
}

void CObjectManager::MonsterDetection(){ 
	int currentTime = g_cTimer.time();
	bool playerIsHit = false;
	int mobMeleeDamage = 0; 
	int mobSpellDamage = 0;
	
    for(int i = 0; i < m_nMaxCount && !playerIsHit; i++){ //for each object slot
		if(m_pObjectList[i]){ //if object exists
			
			if(currentTime > timeHit){

				switch (m_pObjectList[i]->m_nObjectType){
					case CROW_OBJECT:{
					if(distance(m_pObjectList[i], m_pPlayerObject) < 25.0f){ //distance to hurt player
							playerIsHit = true;
							mobMeleeDamage = g_cRandom.number(monsterInfo[i].LV,monsterInfo[i].LV+3);
							mobMeleeDamage = 3*mobMeleeDamage;
					}
					break;
					}
					case MONSTER_OBJECT:{
						if(distance(m_pObjectList[i], m_pPlayerObject) < 25.0f){ //distance to hurt player
							playerIsHit = true;
							mobMeleeDamage = g_cRandom.number(monsterInfo[i].LV,monsterInfo[i].LV+3);
							mobMeleeDamage = 3*mobMeleeDamage;
					}
					break;
					}					 
					case MONSTER2_OBJECT:{
						if(distance(m_pObjectList[i], m_pPlayerObject) < 25.0f){ //distance to hurt player
							playerIsHit = true;
							mobMeleeDamage = g_cRandom.number(monsterInfo[i].LV,monsterInfo[i].LV+3);
							mobMeleeDamage = 4*mobMeleeDamage;
					}
					break;
					}
					case FLAMEGUY_OBJECT:{
						if(distance(m_pObjectList[i], m_pPlayerObject) < 25.0f){ //distance to hurt player
							playerIsHit = true;
							mobMeleeDamage = g_cRandom.number(monsterInfo[i].LV,monsterInfo[i].LV+3);
							mobMeleeDamage = 4*mobMeleeDamage;

					}
					break;
					}

					case HEALGUY_OBJECT:{
						if(HealMana>=1){
						if(distance(m_pObjectList[i], m_pPlayerObject) < 125.0f){ //distance to hurt player
							playerIsHit = true;
							mobMeleeDamage = g_cRandom.number(monsterInfo[i].LV,monsterInfo[i].LV+3);
							mobMeleeDamage = -4*mobMeleeDamage;
							HealMana--;
						}
						else
						{
							mobMeleeDamage = 0;
						}

					}
					break;
					}

					case FLAMEGUY2_OBJECT:{
						if(distance(m_pObjectList[i], m_pPlayerObject) < 25.0f){ //distance to hurt player
							playerIsHit = true;
							mobMeleeDamage = g_cRandom.number(monsterInfo[i].LV,monsterInfo[i].LV+3);
							mobMeleeDamage = 4*mobMeleeDamage;
					}
					break;
					}
					case ROUNDMAN_OBJECT:{
						if(distance(m_pObjectList[i], m_pPlayerObject) < 25.0f){ //distance to hurt player
							playerIsHit = true;
							mobMeleeDamage = g_cRandom.number(monsterInfo[i].LV,monsterInfo[i].LV+3);
							mobMeleeDamage = 5*mobMeleeDamage;
					}
					break;
					}
					case ROUNDMAN2_OBJECT:{
						if(distance(m_pObjectList[i], m_pPlayerObject) < 25.0f){ //distance to hurt player
							playerIsHit = true;
							mobMeleeDamage = g_cRandom.number(monsterInfo[i].LV,monsterInfo[i].LV+3);
							mobMeleeDamage = 8*mobMeleeDamage;
					}
					break;
					}
				}
			}
		}
	}

	for(int i=0; i<m_nMaxSpellCount && !playerIsHit; i++){
		if(m_spellObjects[i]){
			if(currentTime > timeHit){
				switch(m_spellObjects[i]->m_nObjectType){
					case ENEMYFIREBALL_OBJECT:
						if(distance(m_spellObjects[i], m_pPlayerObject) < 25.0f){
							playerIsHit = true;
							killSpell(i);
							//m_spellObjects[i] = NULL;
							//m_nSpellCount--;
							mobSpellDamage = 15;
							}
						break;
					 case ENEMYHOMING_OBJECT:
							if(distance(m_spellObjects[i], m_pPlayerObject) < 25.0f){
							playerIsHit = true;
							killSpell(i);
							//m_spellObjects[i] = NULL;
							//m_nSpellCount--;
							mobSpellDamage = 50 + 20*(monsterInfo[i].LV%2 + 1) ;
							}
						break;
					}
		
				}
			}
		}

	if(playerIsHit){

		HP -= mobMeleeDamage + mobSpellDamage;
		healthBar();
		timeHit = g_cTimer.time() + timeToBeHitAgain;
	}
}

void CObjectManager::ObjectCollision(int index){

}
//Modified Method
void CObjectManager::CollisionDetection(int index){ 

	float distanceToCheck = 25.0f;//35
	if(m_spellObjects[index]->m_nObjectType == LINKATK_OBJECT)
		distanceToCheck = 75.0f; //140

	if(m_spellObjects[index]){
		for(int i=0; i < m_nMaxCount; i++){
			if(m_pObjectList[i]){ //if i is a valid object index
				if(m_pObjectList[i]->m_bVulnerable && distance(m_spellObjects[index], m_pObjectList[i]) < distanceToCheck){
					if(m_pObjectList[i]->m_nObjectType == TREASURE_OBJECT && m_spellObjects[index]->m_nObjectType == LINKATK_OBJECT){
							//Treasure chest object and opening with a sword attack
							monsterInfo[i].HP = 0;
							D3DXVECTOR3 mobLocation = m_pObjectList[i]->m_structLocation;
							determineLoot(monsterInfo[i].tClass,monsterInfo[i].LV, mobLocation);
							replace(i);
					}
					else{
						int playerDamage = 0; //10 + 4*(LV%3);
						playerDamage = playerDamageDealt(m_spellObjects[index]->m_nObjectType);

						if(monsterInfo[i].LV - LV >= 3)
							playerDamage = 0; //You can't hurt that which you can't touch!
						else{
							mobIsHurt = true;
							mobHurtTime = g_cTimer.time() + 0;//350
							m_pObjectList[i]->waitTime = mobHurtTime;
							mobHurt = new CGameObject(HURT_OBJECT,"hurt",m_pObjectList[i]->m_structLocation,0,0);
							mobIndex = i;
						}

						damageDone = playerDamage;
						monsterInfo[i].HP -= playerDamage;

						if(monsterInfo[i].HP < 0)
							monsterInfo[i].HP = 0;

						if(monsterInfo[i].HP == 0){

							D3DXVECTOR3 mobLocation = m_pObjectList[i]->m_structLocation;

							determineLoot(monsterInfo[i].tClass,monsterInfo[i].LV, mobLocation);
							if(monsterInfo[i].bossId > 0)
								dropBossLoot(mobLocation,monsterInfo[i].bossId);

							if((LV - monsterInfo[i].LV) <= 2){
								//You can still get XP for lower level mobs up to two levels lower
							float xpAmount = monsterInfo[i].xp; //* 1/(LV%(monsterInfo[i].LV + 1)+1);
							expInc((int)xpAmount);
								}
							replace(i);
						}
					}
					//Object is hit
					killSpell(index); //Remove the spell
					break;
					
				}
			}
		}

		if(m_spellObjects[index]){
			switch (m_spellObjects[index]->m_nObjectType){
				case LINKATK_OBJECT: killSpell(index); break;
			}
		}
	}


}

/// Draw method for hero

void CObjectManager::draw(int key){ 
	m_pPlayerObject->draw(key);
	//night->draw();
}

void CObjectManager::draw(char letter){ 
	
	m_alphaObjects[0]->draw(letter);
	//night->draw();
}

void CObjectManager::draw2(int key){ 
	m_pPlayerObject->draw2(key);
	//night->draw();
}

///Dont worry about this guy, it is what it is
void CObjectManager::DrawValue(char* title, int value, int x, int y){ 
	const int bufsize=256;
	char buffer[bufsize];
  
	GameRenderer.TextMoveTo(x, y);
	sprintf_s(buffer, bufsize, "%s %d", title, value);
	GameRenderer.TextWrite(buffer);
}

void CObjectManager::DrawValue2(char* title, const char* value, int x, int y){ 
	const int bufsize=256;
	char buffer[bufsize];
  
	GameRenderer.TextMoveTo(x, y);
	sprintf_s(buffer, bufsize, "%s %s", title, value);
	GameRenderer.TextWrite(buffer);
}
/// Draw the text header with characters, lives etc. to the the back buffer.
/// Positions of the text strings on the screen are hard-coded.

void CObjectManager::DrawTextHeader(){ //draw header with score, lives, etc.
  
    D3DXVECTOR3 v; //location of hero
    v.x = m_pPlayerObject->m_structLocation.x;
    v.y = m_pPlayerObject->m_structLocation.y;
    v.z = 500.0f;

	GameRenderer.GetTextDC();

  //Working on spell Loading
  /*if(g_cTimer.elapsed(m_nLastGunFireTime, 2000)){
	  DrawValue("FIRE!!!", NULL, 10, 150);
  }else{
	  DrawValue("POWERING UP...", NULL, 10, 150);
  }*/

	///Displays HP above hero and switches when below 0
	/*
	if(HP < 0){
		DrawValue("YOU ARE DEAD!", HP, 480, 334);
	}else{
		DrawValue(" ", HP, 490, 334);// displays current health
		DrawValue("/", totalHP, 525, 334); //displays total health
	}*/

	/*
	DrawValue(" ", MP, 490, 280);// displays current health
	DrawValue("/", totalMP, 525, 280); //displays total health
	*/

	//DrawValue("Health:", HP, 10, 10);

	////////////////////////////////Original Show//////////////////////////////////


	//DrawValue2("Zone:", zoneName, 10, 10);
  	//DrawValue("LV:", LV, 10, 30);
	//DrawValue("EXP:", XP, 10, 50);
	//DrawValue(" / ", nextXP, 65,50);

	//DrawValue("Gold:",playerGold,10,70);
	//DrawValue("Damage Done:", damageDone, 10, 90);
	////////////////////////////////Original Show//////////////////////////////////


		//DrawValue("Collisions:", collisionCount, 10, 90);
	//DrawValue("Monsters Left:", enemies(), 10, 90);
	//DrawValue("MainCharXCoord:", v.x, 10, 110);
	//DrawValue("MainCharYCoord:", v.y, 10, 130);

	/*for(int i=0; i<5; i++)
	{
		if(playerLoot[i]){
		DrawValue("Item Slot",i,600,10+i*20);
		DrawValue2(":",playerLoot[i]->title,680,10+i*20);
		DrawValue("Amount",playerLoot[i]->stack,870,10+i*20);
		}
	}*/

	if(inAnExit)
		DrawValue("EXIT", 1, 10, 200);

	GameRenderer.ReleaseTextDC();
} //DrawTextHeader

int CObjectManager::enemies(){ //return number of enemies
	int count = 0; //how many enemies left
	for(int i=0; i<m_nMaxCount; i++)
		if(m_pObjectList[i] != NULL)
			switch(m_pObjectList[i]->m_nObjectType){
				case CROW_OBJECT: count++; break;
				case MONSTER_OBJECT: count++; break;
				case MONSTER2_OBJECT: count++; break;
				case FLAMEGUY_OBJECT: count++; break;
				case HEALGUY_OBJECT:count++; break;
				case FLAMEGUY2_OBJECT: count++; break;
				case ROUNDMAN_OBJECT: count++; break;
				case ROUNDMAN2_OBJECT: count++; break;
			}
	return count;
}


void CObjectManager::ShowMenu(int key){

	SubMenuUp = FALSE;

	D3DXVECTOR3 v2; //location of hero
    v2.x = m_pPlayerObject->m_structLocation.x - 65;
    v2.y = m_pPlayerObject->m_structLocation.y - 200;
    v2.z = 300.0f;

	int y1 = 220;
	int y2 = 320;
	int x1 = 500;
	int x2 = 250;

	D3DXVECTOR3 v3; //location of hero
	v3.x = m_pPlayerObject->m_structLocation.x-x1;
	v3.y = m_pPlayerObject->m_structLocation.y-y1;
	switch(menuState){
	case 1:
		v3.x = m_pPlayerObject->m_structLocation.x-x1;
		v3.y = m_pPlayerObject->m_structLocation.y-y1;
		switch(key){
		case 3: menuState = 2;v3.x = m_pPlayerObject->m_structLocation.x-x1;
				v3.y = m_pPlayerObject->m_structLocation.y-y2; break;
		case 4: menuState = 2;v3.x = m_pPlayerObject->m_structLocation.x-x1;
				v3.y = m_pPlayerObject->m_structLocation.y-y2; break;
		case 1: menuState = 3;v3.x = m_pPlayerObject->m_structLocation.x-x2;
				v3.y = m_pPlayerObject->m_structLocation.y-y1; break;
		case 2: menuState = 3;v3.x = m_pPlayerObject->m_structLocation.x-x2;
				v3.y = m_pPlayerObject->m_structLocation.y-y1; break;
		}break;
	case 2:
		v3.x = m_pPlayerObject->m_structLocation.x-x1;
		v3.y = m_pPlayerObject->m_structLocation.y-y2;
		switch(key){
		case 3: menuState = 1;v3.x = m_pPlayerObject->m_structLocation.x-x1;
				v3.y = m_pPlayerObject->m_structLocation.y-y1; break;
		case 4: menuState = 1;v3.x = m_pPlayerObject->m_structLocation.x-x1;
				v3.y = m_pPlayerObject->m_structLocation.y-y1; break;
		case 1: menuState = 4;v3.x = m_pPlayerObject->m_structLocation.x-x2;
				v3.y = m_pPlayerObject->m_structLocation.y-y2; break;
		case 2: menuState = 4;v3.x = m_pPlayerObject->m_structLocation.x-x2;
				v3.y = m_pPlayerObject->m_structLocation.y-y2; break;
		}break;
	case 3:
		v3.x = m_pPlayerObject->m_structLocation.x-x2;
		v3.y = m_pPlayerObject->m_structLocation.y-y1;
		switch(key){
		case 3: menuState = 4;v3.x = m_pPlayerObject->m_structLocation.x-x2;
				v3.y = m_pPlayerObject->m_structLocation.y-y2; break;
		case 4: menuState = 4;v3.x = m_pPlayerObject->m_structLocation.x-x2;
				v3.y = m_pPlayerObject->m_structLocation.y-y2; break;
		case 1: menuState = 1;v3.x = m_pPlayerObject->m_structLocation.x-x1;
				v3.y = m_pPlayerObject->m_structLocation.y-y1; break;
		case 2: menuState = 1;v3.x = m_pPlayerObject->m_structLocation.x-x1;
				v3.y = m_pPlayerObject->m_structLocation.y-y1; break;
		}break;
	case 4:
		v3.x = m_pPlayerObject->m_structLocation.x-x2;
		v3.y = m_pPlayerObject->m_structLocation.y-y2;
		switch(key){
		case 3: menuState = 3;v3.x = m_pPlayerObject->m_structLocation.x-x2;
				v3.y = m_pPlayerObject->m_structLocation.y-y1; break;
		case 4: menuState = 3;v3.x = m_pPlayerObject->m_structLocation.x-x2;
				v3.y = m_pPlayerObject->m_structLocation.y-y1; break;
		case 1: menuState = 2;v3.x = m_pPlayerObject->m_structLocation.x-x1;
				v3.y = m_pPlayerObject->m_structLocation.y-y2; break;
		case 2: menuState = 2;v3.x = m_pPlayerObject->m_structLocation.x-x1;
				v3.y = m_pPlayerObject->m_structLocation.y-y2; break;
		}break;
	}

	v3.z = 650.0f;

	if(MenuUp){
		if(menuON == 0){
			m_menuObjects[0] = new CGameObject(MENU_OBJECT, "menu", v2, 0, 0);
			//create(MENU_OBJECT, "menu", v2, 0, 0, menuID);
			menuON = 1;
		}
		m_pointerObjects[0] = new CGameObject(POINTER_OBJECT, "pointer", v3, 0, 0);		
	}else{
		menuON = 0;
		delete m_optionsMenuObjects[0];
		delete m_spellsMenuObjects[0];
		delete m_itemsMenuObjects[0];
		delete m_pointerObjects[0];
		delete m_menuObjects[0];
		m_optionsMenuObjects[0] = NULL;
		m_spellsMenuObjects[0] = NULL;
		m_itemsMenuObjects[0] = NULL;
		m_pointerObjects[0] = NULL;
		m_menuObjects[0] = NULL;
		if(m_itemCursorObjects[0] != NULL){
			delete m_itemCursorObjects[0];
			m_itemCursorObjects[0] = NULL;
		}
	}
}

void CObjectManager::MenuYes(){
	if(!MenuUp){
		MenuUp = TRUE;
		ShowMenu(1);
	}else{
		MenuUp = FALSE;
		ShowMenu(1);
	}
}

void CObjectManager::pickUpLoot()
{
	for(int i=0;i<maxGroundLoot;i++)
	{
		if(m_groundLootObjects[i]){
			if(distance(m_pPlayerObject,m_groundLootObjects[i]) < 35.0f) {
				addLoot(lootOnGround[i]);
				lootOnGround[i] = NULL;
				m_groundLootObjects[i] = NULL;
		  
			}
		}
	}

}

void CObjectManager::determineLoot(int tClass, int mLevel, D3DXVECTOR3 mobV){
	int maxLoot = tClass + 1;
	int minLoot = 1;

	maxLoot = g_cRandom.number(minLoot,maxLoot);

	for(int i=0; i<maxLoot; i++){
		int b=0;
		while(m_groundLootObjects[b]) b++;
		 
		//Now to determine the randomLoot
		lootPeiceInformation lootInfo;
		lootInfo.type = 1;
		lootInfo.amount = 0;
		lootInfo.maxStack = 0;
		lootInfo.object = DUMMY_OBJECT;
		lootInfo.stack = 0;

		cout << "made it here" << endl;
		randLoot(tClass,mLevel,lootInfo);
		cout << "and here!" << endl;

		if(lootInfo.stack > 0){

			int negative = (i%2)*-1;
			if(negative == 0)
				negative = 1;
			mobV.x = mobV.x + i*10*negative;
			mobV.y = mobV.y + i*10*negative;

		//	LV = v.x;
		
			switch(lootInfo.object) {
				case GOLD1_OBJECT: m_groundLootObjects[b] = new CGameObject(lootInfo.object,lootInfo.name,mobV,0,0);
					lootOnGround[b] = new item(lootInfo.title,lootInfo.name,lootInfo.object,lootInfo.type,lootInfo.maxStack,lootInfo.stack); break;
				//Potions
				case POTION1_OBJECT:
				case POTION2_OBJECT:
				case POTION3_OBJECT: m_groundLootObjects[b] = new CGameObject(lootInfo.object,lootInfo.name,mobV,0,0); 
					lootOnGround[b] = new potion(lootInfo.title,lootInfo.name,lootInfo.object,lootInfo.type,lootInfo.maxStack,lootInfo.stack,lootInfo.amount); break;
			}
		}
	}
}

void CObjectManager::randLoot(int tClass, int mLevel, lootPeiceInformation &inf){
	int rarity = g_cRandom.number(1,2000);

	const char* g_xmlFileNameItems = "items.xml";
	TiXmlDocument g_xmlDocumentLoot(g_xmlFileNameItems); ///< TinyXML document for settings.
	TiXmlElement* g_xmlItems = NULL; ///< TinyXML element for settings tag..	
	g_xmlDocumentLoot.LoadFile();
	g_xmlItems = g_xmlDocumentLoot.FirstChildElement("items");

	if(g_xmlItems) {
		int itemsThatMatch = 0;
		TiXmlElement* ist = g_xmlItems->FirstChildElement("item");		
		
		int maxMatches = 10;
		int* index = new int[maxMatches];

		int icount = 0; //Total Items
		int imatch = 0; //Items that actually meet the criteria
		while(ist){

			int tempTClass = 0;
			int tempRarity = 0;

			ist->Attribute("rarity",&tempRarity);
			ist->Attribute("tclass",&tempTClass);

			if(tempRarity >= rarity && tempTClass == tClass) {
				index[imatch] = icount;
				imatch++;

				if(imatch%9 == 0) {
					//Dynamicall allocate new array	
					int* tempInts = new int[maxMatches+10];
					for(int d=0;d<maxMatches;d++) {
						tempInts[d] = index[d];
					}

					maxMatches+=10;
					delete [] index;
					index = NULL;
					index = tempInts;					
				}
			}
			icount++;
			ist = ist->NextSiblingElement("item");
		}//End first while

		if(imatch > 0) //is there any match?
		{
			imatch--;
			icount = 0;
			int itemNum = g_cRandom.number(0,imatch);
			ist = g_xmlItems->FirstChildElement("item");
			//Find the item that matches in the index list

				while(ist){
					if(icount == index[itemNum]) {
						//We have the item, now do the rest

						int objId;
						ist->Attribute("objId",&objId);
						inf.object = anObject(objId);
						ist->Attribute("type",&inf.type);
						ist->Attribute("maxStack",&inf.maxStack);
						ist->Attribute("stack",&inf.stack);

						

						//ObjType
					//	const char* temp = ist->Attribute("objType");
					/*	char *objType;
						objType = new char[strlen(temp)];
						strcpy(objType,temp);
						inf.objType = objType;
						zoneName = inf.objType;*/

						inf.objType = new char[64];
						const char* temp = ist->Attribute("objType");
						strncpy_s(inf.objType,64,temp,64);

						inf.title = new char[512];
						const char* temp2 = ist->Attribute("title");
						strncpy_s(inf.title,512,temp2,512);

						inf.name = new char[64];
						const char* temp3 = ist->Attribute("name");
						strncpy_s(inf.name,64,temp3,64);

		/*
						//Title
						const char* temp2 = ist->Attribute("title");
						char *objType2;
						objType2 = new char[strlen(temp2)];
						strcpy(objType2,temp2);
						inf.title = objType2;
		
						//Name
						const char* temp3 = ist->Attribute("name");
						char *objType3;
						objType3 = new char[strlen(temp3)];
						strcpy(objType3,temp3);
						inf.name = objType3;
					*/
		
						if(strcmp(inf.objType,"pot") == 0){
							ist->Attribute("amount",&inf.amount);
							}

						if(strcmp(inf.objType,"gold") == 0){
							inf.stack = g_cRandom.number(inf.stack,inf.maxStack);
							}

						break;
					}

					icount++;
					ist = ist->NextSiblingElement("item");
				}
		}
	}
}

ObjectType CObjectManager::anObject(int id) {
	ObjectType temp = DUMMY_OBJECT;

	switch(id) {
	case 0: temp = GOLD1_OBJECT; break;
	case 1: temp = POTION1_OBJECT; break;
	case 2: temp = POTION2_OBJECT; break;
	case 3: temp = POTION3_OBJECT; break;
	case 4: temp = KEY1_OBJECT; break;
	}

	return temp;
}

void CObjectManager::dropBossLoot(D3DXVECTOR3 mobV, int bossID){
	const char* g_xmlFileNameItems = "items.xml";
	TiXmlDocument g_xmlDocumentLoot(g_xmlFileNameItems); ///< TinyXML document for settings.
	TiXmlElement* g_xmlItems = NULL; ///< TinyXML element for settings tag..	
	g_xmlDocumentLoot.LoadFile();
	g_xmlItems = g_xmlDocumentLoot.FirstChildElement("items");

	if(g_xmlItems) {
		int itemsThatMatch = 0;
		TiXmlElement* ist = g_xmlItems->FirstChildElement("BossItem");

		int bossCount = 0;

		while(ist){

			int bId;
			ist->Attribute("bossID",&bId);

			if(bId == bossID){ //Item Exists!
				int objId;
				int stack;
				int maxStack;
				int type;
				int keyId;
				ObjectType obj;

				ist->Attribute("objId",&objId);
				obj = anObject(objId);

				ist->Attribute("type",&type);
				ist->Attribute("maxStack",&maxStack);
				ist->Attribute("stack",&stack);
				ist->Attribute("keyId",&keyId);

				const char* temp = ist->Attribute("title");
				char *title = new char[512];
				strncpy_s(title,512,temp,512);
		//		title = new char[strlen(temp)];
		//		strcpy(title,temp);


				const char* temp2 = ist->Attribute("name");
				char *name = new char[64];
				strncpy_s(name,64,temp2,64);
			//	name = new char[strlen(temp2)];
			//	strcpy(name,temp2);

		/*		const char* temp3 = ist->Attribute("objType");
				char *objType;
				objType = new char[strlen(temp3)];
				strcpy(objType,temp3);*/

				int b=0;
				while(m_groundLootObjects[b]) b++;

				int negative = (bossCount%2)*-1;
				if(negative == 0)
					negative = 1;
				mobV.x = mobV.x + bossCount*15*negative;
				mobV.y = mobV.y + bossCount*15*negative;
		
				m_groundLootObjects[b] = new CGameObject(obj,name,mobV,0,0);
			//	if(strcmp(objType,"key") == 0)
					lootOnGround[b] = new key(title,name,obj,type,maxStack,stack,keyId);
			}	
			ist = ist->NextSiblingElement("BossItem");
		}
	}
}


//Loot (For Real-Real)

void CObjectManager::defineLoot(){
	//Ground Loot
	maxGroundLoot = 200;
	lootOnGround = new item*[maxGroundLoot];
	m_groundLootObjects = new CGameObject*[maxGroundLoot];
	for(int i=0;i<maxGroundLoot;i++){
		lootOnGround[i] = NULL;
		m_groundLootObjects[i] = NULL;
	}

	//Actual loot itself
	playerLootWidth = 3;
	playerLootHeight = 3;
	m_nMaxPlayerLootObjects = playerLootWidth * playerLootHeight;

	//NULL items in inventory
	playerLoot = new item*[m_nMaxPlayerLootObjects];
	m_playerLootObjects = new CGameObject*[m_nMaxPlayerLootObjects];
	for(int i=0;i<m_nMaxPlayerLootObjects;i++){
			playerLoot[i] = NULL;
			m_playerLootObjects[i] = NULL;
	}

}

bool CObjectManager::addLoot(item* a){
	//Is the item stackable to more than 1?

	int stackMax = a->maxStack;
	int stack = a->stack;

	if(a->object == GOLD1_OBJECT){
		playerGold += a->stack;
	}
	else if(stackMax > 1 && stack <= stackMax) {
		//Attempt to add to same item that has not reached it's max stack
		//Find an item that corresponds to this stack.

		for(int i=0;i<m_nMaxPlayerLootObjects;i++) {
			int leftOvers = 0;

			if(playerLoot[i] != NULL) //Is it not empty?
				if(strcmp(playerLoot[i]->title,a->title) == 0) //Is it the same item
					if(playerLoot[i]->stackAmount() < playerLoot[i]->maxStack){ //is the item not full?

						int objStack = playerLoot[i]->stack;
						int maxStack = playerLoot[i]->maxStack;
						int otherStack = a->stack;

						playerLoot[i]->stack += otherStack;

						if(playerLoot[i]->stack > maxStack) {
							leftOvers = playerLoot[i]->stack - maxStack;
							playerLoot[i]->stack = maxStack;
						}

						a->stack = leftOvers;
						}

					if(a->stack == 0)
						break;
		}
		if(a->stackAmount() > 0) {
			//Still have more of the item? Attempt to add it to the next empty spot
			for(int i=0;i<m_nMaxPlayerLootObjects;i++) {
				if(playerLoot[i] == NULL){
					playerLoot[i] = a;
					return true;
				}
			}
		}
	}
	else{ //Just a single item?
		for(int i=0;i<m_nMaxPlayerLootObjects;i++) {
				if(playerLoot[i] == NULL){
					playerLoot[i] = a;
					return true;
				}
			}
	}

	return false;
}



bool CObjectManager::useItem(int index)
{
	//Attempts to use an item from the users inventory
	if(playerLoot[index] == NULL)
		return false; //Item doesn't exist

	if(playerLoot[index]->type == 2) {
		//Useable
		potion* temp;

		switch(playerLoot[index]->object) {
		case POTION1_OBJECT: 
			temp = (potion*)playerLoot[index]; 
			temp->consume();
			reduceItem(index,1); healthBar(); return true; break;
		case POTION2_OBJECT:
			temp = (potion*)playerLoot[index];
			temp->consumeMana();
			reduceItem(index,1); manaBar(); return true; break;
		case POTION3_OBJECT:
			temp = (potion*)playerLoot[index]; 
			temp->speed();
			reduceItem(index,1); return true; break;
		default: return true;
		}
	}

	return false;
}

void CObjectManager::reduceItem(int index, int amount)
{
	if(playerLoot[index]->stack > amount)
		playerLoot[index]->stack -= amount;
	else {
		playerLoot[index] = NULL;
	}
}


void CObjectManager::speedDuration() {
		int time = g_cTimer.time();

			if(time > timeEnd){
				speedOn = false;
			}
			else
				speedOn = true;
				
}

int CObjectManager::getTime() {
	return g_cTimer.time();
}

void CObjectManager::UpdateMP() {
	if(g_cTimer.elapsed(m_nLastGunFireTime, 500) && MP<totalMP)
		MP++;
}

void CObjectManager::enemyFireball(int key, D3DXVECTOR3 location, float xspeed, float yspeed){ 
		D3DXVECTOR3 v; //location of hero
		v.x = location.x;
		v.y = location.y;
		v.z = 750.0f;

		//create fireball
		if(key == 1){//left attack
			v.x = location.x-4;
			v.y = location.y;
			createSpells(ENEMYFIREBALL_OBJECT, "enemyfireball", v, xspeed, yspeed);
			
		}
		if(key == 2){//right attack
			v.x = location.x+6;
			v.y = location.y;
			createSpells(ENEMYFIREBALL_OBJECT, "enemyfireball", v, xspeed, yspeed);
			
		}
		if(key == 3){
			v.x = location.x;
			v.y = location.y;
			createSpells(ENEMYFIREBALL_OBJECT, "enemyfireball", v, xspeed, yspeed);

		
		}
		if(key == 4){
		v.x = location.x;
			v.y = location.y;
			createSpells(ENEMYFIREBALL_OBJECT, "enemyfireball", v, xspeed, yspeed);

		}
	
}


void CObjectManager::enemyHoming(int key, D3DXVECTOR3 location, float xspeed, float yspeed){ 

	
		D3DXVECTOR3 v; //location of hero
		v.x = location.x;
		v.y = location.y;
		v.z = 750.0f;

		//create fireball
		if(key == 1){//left attack
			v.x = location.x-4;
			v.y = location.y;
			createSpells(ENEMYHOMING_OBJECT, "enemyhoming", v, xspeed, yspeed);

		}
		if(key == 2){//right attack
			v.x = location.x+6;
			v.y = location.y;
			createSpells(ENEMYHOMING_OBJECT, "enemyhoming", v, xspeed, yspeed);

			
		}
		if(key == 3){
			v.x = location.x;
			v.y = location.y;
			createSpells(ENEMYHOMING_OBJECT, "enemyhoming", v, xspeed, yspeed);

		
		}
		if(key == 4){
		v.x = location.x;
			v.y = location.y;
			createSpells(ENEMYHOMING_OBJECT, "enemyhoming", v, xspeed, yspeed);
		}
	
}


void CObjectManager::SubMenuYes(){
	if(!SubMenuUp){
		SubMenuUp = TRUE;
	}else{
		SubMenuUp = FALSE;
	}
}

void CObjectManager::TextBoxYes(){
	if(!Talk){
		Talk = TRUE;
	}else{
		Talk = FALSE;
	}
}

void CObjectManager::SubMenu(){

	D3DXVECTOR3 v4; //location of hero
    v4.x = m_pPlayerObject->m_structLocation.x-190;
    v4.y = m_pPlayerObject->m_structLocation.y + 10;
    v4.z = 300.0f;

	itemsMenuON = 0;
	spellsMenuON = 0;
	optionsMenuON = 0;

	if(menuON == 1){
		if(SubMenuUp){
				switch(menuState){
				case 1:
					if(m_optionsMenuObjects[0] != NULL){
						delete m_optionsMenuObjects[0];
						m_optionsMenuObjects[0] = NULL;
					}
					if(m_spellsMenuObjects[0] != NULL){
						delete m_spellsMenuObjects[0];
						m_spellsMenuObjects[0] = NULL;
					}

					if(itemsMenuON == 0){
						m_itemsMenuObjects[0] = new CGameObject(ITEMSMENU_OBJECT, "itmesMenu", v4, 0, 0);
						itemsMenuON = 1;
					}
					ItemTextBox();break;
				case 3:
					if(m_optionsMenuObjects[0] != NULL){
						delete m_optionsMenuObjects[0];
						m_optionsMenuObjects[0] = NULL;
					}
					if(m_itemsMenuObjects[0] != NULL){
						delete m_itemsMenuObjects[0];
						m_itemsMenuObjects[0] = NULL;
					}
					if(spellsMenuON == 0){
						//m_spellsMenuObjects[0] = new CGameObject(SPELLSMENU_OBJECT, "spellsMenu", v4, 0, 0);
						spellsMenuON = 1;
					}break;
				case 2:
					if(m_itemsMenuObjects[0] != NULL){
						delete m_itemsMenuObjects[0];
						m_itemsMenuObjects[0] = NULL;
					}
					if(m_spellsMenuObjects[0] != NULL){
						delete m_spellsMenuObjects[0];
						m_spellsMenuObjects[0] = NULL;
					}
					if(optionsMenuON == 0){
						m_optionsMenuObjects[0] = new CGameObject(OPTIONSMENU_OBJECT, "optionsMenu", v4, 0, 0);
						optionsMenuON = 1;
					}HELPBox();break;
				case 4: SubMenuUp = FALSE; MenuUp = FALSE; ShowMenu(4);break;
				default: break;
				}
		}else{
		if(m_itemCursorObjects[0] != NULL){
			delete m_itemCursorObjects[0];
			m_itemCursorObjects[0] = NULL;
		}
		if(m_optionsMenuObjects[0] != NULL){
			delete m_optionsMenuObjects[0];
			m_optionsMenuObjects[0] = NULL;
		}
		if(m_spellsMenuObjects[0] != NULL){
			delete m_spellsMenuObjects[0];
			m_spellsMenuObjects[0] = NULL;
		}
		if(m_itemsMenuObjects[0] != NULL){
			delete m_itemsMenuObjects[0];
			m_itemsMenuObjects[0] = NULL;
		}
		}
	}

	}

char CObjectManager::AmountConveter(int bah){

	char thechar = '0';

	switch(playerLoot[bah]->stack){
	case 0: thechar = '0'; break;
	case 1: thechar = '1'; break;
	case 2: thechar = '2'; break;
	case 3: thechar = '3'; break;
	case 4: thechar = '4'; break;
	case 5: thechar = '5'; break;
	case 6: thechar = '5'; break;
	case 7: thechar = '6'; break;
	case 8: thechar = '7'; break;
	case 9: thechar = '9'; break;
	case 10: thechar = '10'; break;
	case 11: thechar = '11'; break;
	default: thechar = '-1';
	}


	return thechar;
}

char CObjectManager::convertChar(int num){
	char thechar = '0';
	switch(num){
	case 0: thechar = '0'; break;
	case 1: thechar = '1'; break;
	case 2: thechar = '2'; break;
	case 3: thechar = '3'; break;
	case 4: thechar = '4'; break;
	case 5: thechar = '5'; break;
	case 6: thechar = '5'; break;
	case 7: thechar = '6'; break;
	case 8: thechar = '7'; break;
	case 9: thechar = '9'; break;
	}
	return thechar;
}

void CObjectManager::ItemTextBox(){

	D3DXVECTOR3 v4; //location of hero
    v4.x = m_pPlayerObject->m_structLocation.x-313;
    v4.y = m_pPlayerObject->m_structLocation.y + 160;
    v4.z = 300.0f;

	D3DXVECTOR3 v3; //location of hero
    v3.x = m_pPlayerObject->m_structLocation.x-590;
    v3.y = m_pPlayerObject->m_structLocation.y + 290;
    v3.z = 900.0f;

	D3DXVECTOR3 v5; //location of hero
    v5.x = m_pPlayerObject->m_structLocation.x-166;
    v5.y = m_pPlayerObject->m_structLocation.y + 79;
    v5.z = 0.0f;
	//int bahcount = 0;
		for(int i = 0; i < m_nMaxPlayerLootObjects; i++){
			
			if(playerLoot[i] != NULL){
				//if(bahcount > 2 ){
					//v5.y += 28;
			switch(playerLoot[i]->object){
				case POTION1_OBJECT:
					m_alphaObjects[i] = new CGameObject(ALPHA_OBJECT, "alpha", v3, 0, 0);
					m_alphaObjects[i]->draw('X');
					v3.x +=20;
					m_alphaObjects[i] = new CGameObject(ALPHA_OBJECT, "alpha", v3, 0, 0);
					m_alphaObjects[i]->draw(AmountConveter(i));
					//m_itemCursorObjects[0] = new CGameObject(ITEMCURSOR_OBJECT, "itemCursor", v5, 0, 0);
					m_invintoryLootObjects[i] = new CGameObject(POTION1_OBJECT, "potion1", v4, 0, 0);break;
					//bahcount++;
				case POTION2_OBJECT:
					m_alphaObjects[i] = new CGameObject(ALPHA_OBJECT, "alpha", v3, 0, 0);
					m_alphaObjects[i]->draw('X');
					v3.x +=20;
					m_alphaObjects[i] = new CGameObject(ALPHA_OBJECT, "alpha", v3, 0, 0);
					m_alphaObjects[i]->draw(AmountConveter(i));
					m_invintoryLootObjects[i] = new CGameObject(POTION2_OBJECT, "potion2", v4, 0, 0);break;
					//bahcount++;
				case POTION3_OBJECT:
					m_alphaObjects[i] = new CGameObject(ALPHA_OBJECT, "alpha", v3, 0, 0);
					m_alphaObjects[i]->draw('X');
					v3.x +=20;
					m_alphaObjects[i] = new CGameObject(ALPHA_OBJECT, "alpha", v3, 0, 0);
					m_alphaObjects[i]->draw(AmountConveter(i));
					m_invintoryLootObjects[i] = new CGameObject(POTION3_OBJECT, "potion3", v4, 0, 0);break;
					//bahcount++;
				case KEY1_OBJECT:
					m_alphaObjects[i] = new CGameObject(ALPHA_OBJECT, "alpha", v3, 0, 0);
					m_alphaObjects[i]->draw('X');
					v3.x +=20;
					m_alphaObjects[i] = new CGameObject(ALPHA_OBJECT, "alpha", v3, 0, 0);
					m_alphaObjects[i]->draw(AmountConveter(i));
					m_invintoryLootObjects[i] = new CGameObject(KEY1_OBJECT, "key1", v4, 0, 0);break;
					//bahcount++;
				default: break;
			//}
				}
			}else{
				break;
			}
			v3.x += 72;
			v4.x += 39;
		}
		
		//m_itemCursorObjects[0]->draw();
}

void CObjectManager::HELPBox(){

	D3DXVECTOR3 v2; //location of hero
    v2.x = m_pPlayerObject->m_structLocation.x- 700;
    v2.y = m_pPlayerObject->m_structLocation.y + 350;
    v2.z = 1000.0f;


	char* strings[11];
	strings[0] = "SPACE BAR .. ";
	strings[1] = "ACTION BUTTON";
	strings[2] = "1 .. FIREBALL";
	strings[3] = "2 .. LIGHTNING";
	strings[4] = "3 .. BARRIER";
	strings[5] = "4 .. FAN OF FIRE";
	strings[6] = "M .. MENU";
	strings[7] = "V .. USE ITEM";
	strings[8] = "S .. SAVE";
	strings[9] = "ARROW KEYS ..";
	strings[10] = "MOVEMENT";


	for(int h = 0; h < 11; h++){	
		for(int g = 0; g < 50; g++){
			v2.x += 20;
			m_alphaObjects[0] = new CGameObject(ALPHA_OBJECT, "alpha", v2, 0, 0);
			if(m_alphaObjects[0])
				if(strings[h][g] == NULL){
					break;
				}else if(strings[h][g] == ' '){
					v2.x += 0;
				}else{
					m_alphaObjects[0]->draw(strings[h][g]);
				}
		}
		v2.x = m_pPlayerObject->m_structLocation.x- 700;
		v2.y -= 50;
	}
}


void CObjectManager::ItemSelect(int key){

	D3DXVECTOR3 v5; //location of hero
    v5.x = m_pPlayerObject->m_structLocation.x-166;
    v5.y = m_pPlayerObject->m_structLocation.y + 79;
    v5.z = 0.0f;

	int y1 = 28;
	int y2 = 56;
	int y3 = 84;
	int y4 = 112;
	int x1 = 23;
	int x2 = 46;
	int x3 = 69;

	m_itemCursorObjects[0] = new CGameObject(ITEMCURSOR_OBJECT, "itemCursor", v5, 0, 0);

	//currentItemSelect = 0;

	//int itemArray[3][3] = NULL;
	//for(int i =0; i < 20; i++){
		//if(m_invintoryLootObjects[currentItemSelect] != NULL){
			switch(currentItemSelect){
			case 0:{
					switch(key){
						case 1: currentItemSelect = 3; v5.x += x3; v5.y;break; 
						case 2: currentItemSelect = 1; v5.x += x1; v5.y;break; 
						case 3: currentItemSelect = 4; v5.x; v5.y -= y1; break;
						case 4: currentItemSelect = 16; v5.x; v5.y -= y4; break;//
					}
				}break;
			case 1:{
					switch(key){
						case 1: currentItemSelect = 0; v5.x; v5.y;break; 
						case 2: currentItemSelect = 2; v5.x += x2; v5.y;break; 
						case 3: currentItemSelect = 5; v5.x += x1; v5.y -= y1; break;
						case 4: currentItemSelect = 17;  v5.x += x1; v5.y -= y4; break;
					}
				}break;
			case 2:{
					switch(key){
						case 1: currentItemSelect = 1; v5.x += x1; v5.y;break; 
						case 2: currentItemSelect = 3; v5.x += x3; v5.y;break; 
						case 3: currentItemSelect = 6; v5.x += x2; v5.y -= y1; break;
						case 4: currentItemSelect = 18; v5.x += x2; v5.y -= y4; break;
					}
				}break;
			case 3:{
					switch(key){
						case 1: currentItemSelect = 2; v5.x += x2; v5.y;break; 
						case 2: currentItemSelect = 0; v5.x; v5.y;break; 
						case 3: currentItemSelect = 7; v5.x += x3; v5.y -= y1; break;
						case 4: currentItemSelect = 19; v5.x += x3; v5.y -= y4; break;
					}
				}break;
			case 4:{
					switch(key){
						case 1: currentItemSelect = 7; v5.x += x3; v5.y -= y1; break;
						case 2: currentItemSelect = 5; v5.x += x1; v5.y -= y1; break;
						case 3: currentItemSelect = 8; v5.x;v5.y -= y2;break;
						case 4: currentItemSelect = 0; v5.x;v5.y;break;
					}
				}break;
			case 5:{
					switch(key){
						case 1: currentItemSelect = 4;v5.x; v5.y -= y1;break;
						case 2: currentItemSelect = 6;v5.x += x2; v5.y -= y1;break; 
						case 3: currentItemSelect = 9;v5.x += x1;v5.y -= y2;break;
						case 4: currentItemSelect = 1;v5.x += x1;v5.y;break;
					}
				}break;
			case 6:{
					switch(key){
						case 1: currentItemSelect = 5;v5.x += x1;v5.y -= y1;break; 
						case 2: currentItemSelect = 7;v5.x += x3;v5.y -= y1;break; 
						case 3: currentItemSelect = 10;v5.x += x2;v5.y -= y2;break;
						case 4: currentItemSelect = 2;v5.x += x2;v5.y;break;
					}
				}break;
			case 7:{
					switch(key){
						case 1: currentItemSelect = 6;v5.x += x2;v5.y -= y1;break;
						case 2: currentItemSelect = 4;v5.x;v5.y -= y1;break;
						case 3: currentItemSelect = 11;v5.x += x3;v5.y -= y2;break;
						case 4: currentItemSelect = 3;v5.x += x3;v5.y;break;
					}
				}break;
			case 8:{
					switch(key){
						case 1: currentItemSelect = 11;v5.x += x3; v5.y -= y2;break;
						case 2: currentItemSelect = 9;v5.x += x1; v5.y -= y2;break;
						case 3: currentItemSelect = 12;v5.x;v5.y -= y3;break;
						case 4: currentItemSelect = 4;v5.x;v5.y -= y1;break;
					}
				}break;
			case 9:{
					switch(key){
						case 1: currentItemSelect = 8;v5.x; v5.y -= y2;break;
						case 2: currentItemSelect = 10;v5.x += x2; v5.y -= y2;break; 
						case 3: currentItemSelect = 13;v5.x += x1;v5.y -= y3;break;
						case 4: currentItemSelect = 5;v5.x += x1;v5.y -= y1;break;
					}
				}break;
			case 10:{
					switch(key){
						case 1: currentItemSelect = 9;v5.x += x1;v5.y -= y2;break; 
						case 2: currentItemSelect = 11;v5.x += x3;v5.y -= y2;break; 
						case 3: currentItemSelect = 14;v5.x += x2;v5.y -= y3;break;
						case 4: currentItemSelect = 6;v5.x += x2;v5.y -= y1;break;
					}
				}break;
			case 11:{
					switch(key){
						case 1: currentItemSelect = 10;v5.x += x2;v5.y -= y2;break;
						case 2: currentItemSelect = 8;v5.x;v5.y -= y2;break;
						case 3: currentItemSelect = 15;v5.x += x3;v5.y -= y3;break;
						case 4: currentItemSelect = 7;v5.x += x3;v5.y -= y1;break;
					}
				}break;
			case 12:{
					switch(key){
						case 1: currentItemSelect = 15;v5.x += x3;v5.y -= y3;break;
						case 2: currentItemSelect = 13;v5.x += x1;v5.y -= y3;break;
						case 3: currentItemSelect = 16;v5.x; v5.y -= y4;break;//
						case 4: currentItemSelect = 8;v5.x; v5.y -= y2;break;
					}
				}break;
			case 13:{
					switch(key){
						case 1: currentItemSelect = 12;v5.x;v5.y -= y3;break;
						case 2: currentItemSelect = 14;v5.x += x2;v5.y -= y3;break; 
						case 3: currentItemSelect = 17;v5.x += x1; v5.y -= y4;break;
						case 4: currentItemSelect = 9;v5.x += x1; v5.y -= y2;break;
					}
				}break;
			case 14:{
					switch(key){
						case 1: currentItemSelect = 13;v5.x += x1;v5.y -= y3;break;
						case 2: currentItemSelect = 15;v5.x += x3;v5.y -= y3;break;
						case 3: currentItemSelect = 18;v5.x += x2; v5.y -= y4;break;
						case 4: currentItemSelect = 10;v5.x += x2; v5.y -= y2;break;
					}
				}break;
			case 15:{
					switch(key){
						case 1: currentItemSelect = 14;v5.x += x2;v5.y -= y3;break;
						case 2: currentItemSelect = 12;v5.x;v5.y -= y3;break;
						case 3: currentItemSelect = 19;v5.x += x3; v5.y -= y4;break;
						case 4: currentItemSelect = 11;v5.x += x3; v5.y -= y2;break;
					}
				}break;
			case 16:{
					switch(key){
						case 1: currentItemSelect = 19;v5.x += x3;v5.y -= y4;break;
						case 2: currentItemSelect = 17;v5.x += x1;v5.y -= y4;break;
						case 3: currentItemSelect = 0;v5.x; v5.y;break;
						case 4: currentItemSelect = 12;v5.x; v5.y -= y3;break;
					}
				}break;
			case 17:{
					switch(key){
						case 1: currentItemSelect = 16;v5.x;v5.y -= y4;break;//
						case 2: currentItemSelect = 18;v5.x += x2;v5.y -= y4;break;
						case 3: currentItemSelect = 1;v5.x += x1; v5.y;break;
						case 4: currentItemSelect = 13;v5.x += x1; v5.y -= y3;break;
					}
				}break;
			case 18:{
					switch(key){
						case 1: currentItemSelect = 17;v5.x += x1;v5.y -= y4;break;
						case 2: currentItemSelect = 19;v5.x += x3;v5.y -= y4;break;
						case 3: currentItemSelect = 2;v5.x += x2; v5.y;break;
						case 4: currentItemSelect = 14;v5.x += x2; v5.y -= y3;break;
					}
				}break;
			case 19:{
					switch(key){
						case 1: currentItemSelect = 18;v5.x += x2;v5.y -= y4;break;//18
						case 2: currentItemSelect = 16;v5.x;v5.y -= y4;break;//16
						case 3: currentItemSelect = 3;v5.x += x3; v5.y;break;//3
						case 4: currentItemSelect = 15;v5.x += x3; v5.y -= y3;break;//15
					}
				}break;
			}

			m_itemCursorObjects[0] = new CGameObject(ITEMCURSOR_OBJECT, "itemCursor", v5, 0, 0);
}

void CObjectManager::ShowTextBox(){

	D3DXVECTOR3 v2; //location of hero
    v2.x = m_pPlayerObject->m_structLocation.x - 65;
    v2.y = m_pPlayerObject->m_structLocation.y - 200;
    v2.z = 300.0f;

	if(TalkBubble ){
		if(menuON == 0){
			//m_textBoxObjects[0] = new CGameObject(TEXTBOX_OBJECT, "textBox", v2, 0, 0);
			textBoxON = TRUE;
			menuON = 1;
		}	
	}else{
		textBoxON = FALSE;
		menuON = 0;
		TalkBubble = FALSE;
		delete m_textBoxObjects[0];
		m_textBoxObjects[0] = NULL;
	}
}

void CObjectManager::ShowText(){

	D3DXVECTOR3 v2; //location of hero
    v2.x = m_pPlayerObject->m_structLocation.x - 445;
    v2.y = m_pPlayerObject->m_structLocation.y + 280; // - 180
    v2.z = 600.0f; //500

	//add to database of text here

	char* strings[6];
	strings[0] = "CAN IT BE CALLED GRAPE?";
	strings[1] = "HERE IS THE SECOND LINE, PRETTY NEAT HUH?";
	strings[2] = "THIRD LINE IS NOT AS COOL AS THE FIRST TWO.";
	strings[3] = "SOME IMPORTANT INFO";
	strings[4] = "WOOT THE WOOT";
	strings[5] = "THE THIRD LINE AGAIN";

	//add global int and function for changing text. Replace 3 with global int.

	for(int h = 0; h < 3; h++){	
		for(int g = 0; g < 50; g++){
			v2.x += 20;
			m_alphaObjects[0] = new CGameObject(ALPHA_OBJECT, "alpha", v2, 0, 0);
			if(m_alphaObjects[0])
				if(strings[h][g] == NULL){
					break;
				}else if(strings[h][g] == ' '){
					v2.x += 0;
				}else{
					m_alphaObjects[0]->draw(strings[h][g]);
				}
		}
		v2.x = m_pPlayerObject->m_structLocation.x- 445;
		v2.y -= 50;
	}
}

void CObjectManager::createUi()
{
	m_nUiCount = 0;
	m_nMaxUiCount = 5;
	

	//Health
	m_nHealthCount = 0;
	m_nMaxHealthCount = 100;

	m_healthObjects = new CGameObject*[m_nMaxHealthCount];
	
	//Create Empty Objects
	for(int i=0; i<m_nMaxHealthCount; i++)
		m_healthObjects[i] = NULL;

	m_nManaCount = 0;
	m_nMaxManaCount = 100;

	m_manaObjects = new CGameObject*[m_nMaxManaCount];
	
	//Create Empty Objects
	for(int i=0; i<m_nMaxManaCount; i++)
		m_manaObjects[i] = NULL;

	m_healthBoxObjects = new CGameObject*[0];
}

void CObjectManager::healthBar(){

	//drawMiniMap();

	D3DXVECTOR3 v;
		v.x = m_pPlayerObject->m_structLocation.x-434;
		v.y = m_pPlayerObject->m_structLocation.y-259;//+301;
		v.z = 500.0f;

	float health = (float)HP;
	float maxHealth = (float)totalHP;
	float healthPercent = (health / maxHealth) * 100;
	int healthPeices = healthPercent;

	if(healthPeices < 0)
		healthPeices = 0;

	for(int i=0; i<m_nMaxHealthCount; i++){
		delete m_healthObjects[i];
		m_healthObjects[i] = NULL;
	}

	for(int i=0; i<healthPeices; i++){
		m_healthObjects[i] = new CGameObject(HEALTH_OBJECT, "healthPiece", v, 0, 0);
		v.x = v.x + 3;
	}

	for(int i=healthPeices; i<m_nMaxHealthCount; i++)
	{
		m_healthObjects[i] = NULL;
	}
	
}

void CObjectManager::manaBar(){
	D3DXVECTOR3 v2;
		v2.x = m_pPlayerObject->m_structLocation.x-434;
		v2.y = m_pPlayerObject->m_structLocation.y -300; //+ 259;
		v2.z = 500.0f;

	float mana = (float)MP;
	float maxMana = (float)totalMP;
	float manaPercent = (mana / maxMana) * 100;
	int manaPeices = (int)manaPercent;

	if(manaPeices < 0)
		manaPeices = 0;

	for(int i=0; i<m_nMaxManaCount; i++){
		delete m_manaObjects[i];
		m_manaObjects[i] = NULL;
	}

	for(int i=0; i<manaPeices; i++){
		m_manaObjects[i] = new CGameObject(MANA_OBJECT, "manaPiece", v2, 0, 0);
		v2.x = v2.x + 3;
	}

	for(int i=manaPeices; i<m_nMaxManaCount; i++)
	{
		m_manaObjects[i] = NULL;
	}
}

void CObjectManager::healthBox(){
	D3DXVECTOR3 v2;
		v2.x = m_pPlayerObject->m_structLocation.x-247;
		v2.y = m_pPlayerObject->m_structLocation.y -280; //+ 280;
		v2.z = 500.0f;

		m_healthBoxObjects[0] = new CGameObject(HEALTHBOX_OBJECT, "healthBox", v2, 0, 0);

}


void CObjectManager::monsterHealthBarDestroy(int index)
{
	monsterHealthBars[index].show = false;
	monsterHealthBars[index].currentMonster = -1;

	if(monsterHealthBars[index].m_level)
		monsterHealthBars[index].m_level = NULL;

	for(int i=0; i<m_nMaxHealthCountMob; i++){
		monsterHealthBars[index].m_monsterHealthFrameObjects[i] = NULL;
		monsterHealthBars[index].m_monsterHealthObjects[i] = NULL;
	}
}

void CObjectManager::monsterHealthBar(int index)
{
	//Attempt to draw

	int existsIndex = -1;
	for(int d=0; d<monsterAmount; d++)
		if(monsterHealthBars[d].currentMonster == index){
			existsIndex = d;
			d = monsterAmount + 1;
		}

	for(int c=0; c<monsterAmount; c++){

		if(existsIndex > -1)
			c = existsIndex;

		if(monsterHealthBars[c].show == false || monsterHealthBars[c].currentMonster == index){

			monsterHealthBars[c].show = true;
			monsterHealthBars[c].currentMonster = index;
			monsterHealthBars[c].m_level = NULL;
			monsterHealthBars[c].mLV = monsterInfo[index].LV;

			D3DXVECTOR3 v;
				v.x = m_pObjectList[index]->m_structLocation.x + 20.0f;
				v.y = m_pObjectList[index]->m_structLocation.y + 20.0f;
				

			float health = (float) monsterInfo[index].HP;
			float maxHealth = (float) monsterInfo[index].totalHP;
			float healthPercent = (health / maxHealth) * 100;
			float healthParts = healthPercent / 3;
			int healthPeices = (int)healthParts;
			v.z = 500.0f;

			if(healthPeices < 0)
				healthPeices = 0;

			for(int i=0; i<m_nMaxHealthCountMob; i++){
				monsterHealthBars[c].m_monsterHealthFrameObjects[i] = NULL;
				monsterHealthBars[c].m_monsterHealthObjects[i] = NULL;
			}

			for(int i=0; i<healthPeices; i++){
				monsterHealthBars[c].m_monsterHealthFrameObjects[i] = new CGameObject(HEALTH_FRAMEMOB,"healthframemob",v,0,0);
				monsterHealthBars[c].m_monsterHealthObjects[i] = new CGameObject(HEALTH_PIECEMOB, "healthpiecemob", v, 0, 0);
				v.x = v.x + 3;
			}

			for(int i=healthPeices; i<m_nMaxHealthCountMob; i++)
			{
				monsterHealthBars[c].m_monsterHealthFrameObjects[i] = new CGameObject(HEALTH_FRAMEMOB,"healthframemob",v,0,0);
				monsterHealthBars[c].m_monsterHealthObjects[i] = NULL;
				v.x = v.x + 3;
			}

			v.z = 500.0f;
			v.x += 40.0f;
			monsterHealthBars[c].m_level = new CGameObject(ALPHA_OBJECT,"alpha",v,0,0);

			c = monsterAmount + 1;
		}

	}
}

void CObjectManager::drawMiniMap(){
	for(int i=0; i<m_mapObjectsCount; i++){
		m_miniMapObjects[i] = NULL;
	}

	D3DXVECTOR3 playerCoord = m_pPlayerObject->m_structLocation;
	playerCoord.z = 500.0f;

	int zWidth = zoneWidth / 64;
	int zHeight = zoneHeight / 64;

	//Change mini map position relative to player coords
	playerCoord.x = playerCoord.x + 450;
	playerCoord.y = playerCoord.y + 340;

	float originalX = playerCoord.x;
	float originalY = playerCoord.y;
	int mapCount = 0;

	playerCoord.x = originalX - 4*(zWidth-17);
	for(int r=0; r<zHeight-17; r++){
		playerCoord.y -= 4;

		bool madeExit = false;
		for(int i=0; i<m_nMaxExitCount; i++){
				if(exitInfo[i].x == 0)
					if((r-4) == exitInfo[i].x){
						m_miniMapObjects[mapCount++] = new CGameObject(MINIEXIT, "miniexit", playerCoord, 0,0);
						madeExit = true;
					}
		}
		if(!madeExit)
			m_miniMapObjects[mapCount++] = new CGameObject(MINIWALL,"miniwall",playerCoord,0,0);	
	}
	playerCoord.y = originalY;
	playerCoord.x = originalX;
	
	playerCoord.x -= 4;
	for(int r=0; r<zHeight-17; r++) {
		playerCoord.y -= 4;

		bool madeExit = false;
		for(int i=0; i<m_nMaxExitCount; i++){
				if(exitInfo[i].x == (zWidth-18))
					if((r-4) == exitInfo[i].x){
						m_miniMapObjects[mapCount++] = new CGameObject(MINIEXIT, "miniexit", playerCoord, 0,0);
						madeExit = true;
					}
		}
		if(!madeExit)
			m_miniMapObjects[mapCount++] = new CGameObject(MINIWALL,"miniwall",playerCoord,0,0);

	}
	
	playerCoord.x = originalX;
	playerCoord.y = originalY;

	for(int r=0; r<zWidth-17; r++){
		playerCoord.x -= 4;
		bool madeExit = false;
		for(int i=0; i<m_nMaxExitCount; i++){
				if(exitInfo[i].y == 0)
					if((r-4) == exitInfo[i].y){
						m_miniMapObjects[mapCount++] = new CGameObject(MINIEXIT, "miniexit", playerCoord, 0,0);
						madeExit = true;
					}
		}
		if(!madeExit)
			m_miniMapObjects[mapCount++] = new CGameObject(MINIWALL,"miniwall",playerCoord,0,0);
	}

	playerCoord.x = originalX;
	playerCoord.y = originalY - 4*(zHeight-17);

	for(int r=0; r<zWidth-17; r++){
		playerCoord.x -= 4;
		bool madeExit = false;
		for(int i=0; i<m_nMaxExitCount; i++){
				if(exitInfo[i].y == (zHeight-18))
					if((r-4) == exitInfo[i].y){
						m_miniMapObjects[mapCount++] = new CGameObject(MINIEXIT, "miniexit", playerCoord, 0,0);
						madeExit = true;
					}
		}
		if(!madeExit)
			m_miniMapObjects[mapCount++] = new CGameObject(MINIWALL,"miniwall",playerCoord,0,0);
	}

	playerCoord.x = originalX;
	playerCoord.y = originalY;

	//Attempt to draw the character dot

	float top = zoneHeight / 2.0f - 533;
	float left = -zoneWidth / 2.0f + 533;

	float relativeX = originalX - 4*(zWidth-17) + 4; // - length*8; // - 350;
	float relativeY = m_pPlayerObject->m_structLocation.y + 335; // + 100;

	float topDistance = (top - m_pPlayerObject->m_structLocation.y) / 16.5;
	float leftDistance = (m_pPlayerObject->m_structLocation.x - left) / 17.5;

	relativeY -= topDistance;
	relativeX += leftDistance;

	playerCoord.x = relativeX;
	playerCoord.y = relativeY;

	m_miniMapObjects[mapCount++] = new CGameObject(MINIPLAYER,"miniplayer",playerCoord,0,0);
}

//Level Up and Mob Info

void CObjectManager::expInc(int newXP){
	defineNextXP();
	XP += newXP;

	if(XP >= nextXP && LV < 9){
		levelUp(LV);
		XP = 0; //nextXP - newXP;
	}	
}

void CObjectManager::defineNextXP(){
	switch(LV){ //Values have been brought down for testing purposes
		case 1: nextXP = 212; break;
		case 2: nextXP = 318; break;
		case 3: nextXP = 424; break;
		case 4: nextXP = 529; break;
		case 5: nextXP = 635; break;
		case 6: nextXP = 741; break;
		case 7: nextXP = 847; break;
		case 8: nextXP = 953; break;
		case 9: nextXP = 1058; break;
	}
}

void CObjectManager::levelUp(int level){
	totalMP = (LV+2)*79 + 50*LV;
	totalHP = 120*LV + 20*LV;
	HP = totalHP;
	MP = totalMP;
	LV++;
}

bool CObjectManager::loadSavedGame()
{
	const char* g_xmlFileName = "xmlsave.xml";

	TiXmlDocument g_xmlDocument(g_xmlFileName); ///< TinyXML document for settings.
	TiXmlElement* g_xml = NULL; ///< TinyXML element for settings tag..	
	if(g_xmlDocument.LoadFile()){
		g_xml = g_xmlDocument.FirstChildElement("settings");

		TiXmlElement* ist = NULL;
		ist = g_xml->FirstChildElement("charInformation");

		ist->Attribute("totalHP",&totalHP); //Total Hit Points
		ist->Attribute("HP",&HP); //Current HP
		ist->Attribute("totalMP",&totalMP); //Total Mana
		ist->Attribute("MP",&MP); //Current Mana
		ist->Attribute("XP",&XP); //Experience
		ist->Attribute("LV",&LV); //Level
		ist->Attribute("playerGold",&playerGold); //Gold
		defineNextXP();

		ist = g_xml->FirstChildElement("inventory");
		if(ist){
			ist = ist->FirstChildElement("item");
			while(ist){

				int index = 0;
				int iId = 0;
				ObjectType obj = DUMMY_OBJECT;
				int type = 0;
				int maxStack = 0;
				int stack = 0;

				ist->Attribute("slot",&index); //Index
				ist->Attribute("id",&iId);
				obj = anObject(iId); //Object
				ist->Attribute("type",&type);
				ist->Attribute("maxStack",&maxStack);
				ist->Attribute("stack",&stack);

				//ItemType
				const char* temp = ist->Attribute("itemType");
				char *itemType;
				itemType = new char[strlen(temp)];
				strcpy(itemType,temp);
		
				//Title
				const char* temp2 = ist->Attribute("title");
				char *title;
				title = new char[strlen(temp2)];
				strcpy(title,temp2);
		
				//Name
				const char* temp3 = ist->Attribute("name");
				char *name;
				name = new char[strlen(temp3)];
				strcpy(name,temp3);
		
				if(strcmp(itemType,"pot") == 0){
					//Create New Potion
					int amount;
					ist->Attribute("amount",&amount);

					playerLoot[index] = new potion(title,name,obj,type,maxStack,stack,amount);
				}
				else
					playerLoot[index] = new item(title,name,obj,type,maxStack,stack);



				ist = ist->NextSiblingElement("item");
			}
		}
	}

	loadedSaved = true;
	return true;
}

bool CObjectManager::saveGame() {
	TiXmlDocument doc;
	TiXmlDeclaration * dec = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild(dec);

	TiXmlElement * settingsElement = new TiXmlElement("settings");
	doc.LinkEndChild(settingsElement);

	TiXmlElement * element = new TiXmlElement("charInformation");
	element->SetAttribute("totalHP",totalHP); //Total Hit Points
	element->SetAttribute("HP",HP); //Current HP
	element->SetAttribute("totalMP",totalMP); //Total Mana
	element->SetAttribute("MP",MP); //Current Mana
	element->SetAttribute("XP",XP); //Experience
	element->SetAttribute("LV",LV); //Level
	element->SetAttribute("playerGold",playerGold); //Gold
	settingsElement->LinkEndChild(element);

	TiXmlElement * inventory = new TiXmlElement("inventory");
	settingsElement->LinkEndChild(inventory);

	int i = 0;
	while(playerLoot[i]){

		int obId = 0;
		char *obType = "nothing";
		itemType(playerLoot[i]->object,obId,obType);

		TiXmlElement * item = new TiXmlElement("item");
		item->SetAttribute("slot",i);
		item->SetAttribute("id",obId);
		item->SetAttribute("title",playerLoot[i]->title);
		item->SetAttribute("name",playerLoot[i]->name);
		item->SetAttribute("type",playerLoot[i]->type);
		item->SetAttribute("itemType",obType);
		item->SetAttribute("stack",playerLoot[i]->stack);
		item->SetAttribute("maxStack",playerLoot[i]->maxStack);

		//For specific item types
		//Potion
		if(strcmp(obType,"pot") == 0){
			potion* tempItem = (potion*)playerLoot[i];
			item->SetAttribute("amount",tempItem->amount);
		}
		inventory->LinkEndChild(item);
		i++;
	}

	return doc.SaveFile("xmlsave.xml");
	//return true;
}

void CObjectManager::itemType(ObjectType obj,int &id, char* &type){
	switch(obj) {
	case POTION1_OBJECT: id = 1; type = "pot"; break;
	case POTION2_OBJECT: id = 2; type = "pot"; break;
	case POTION3_OBJECT: id = 3; type = "pot"; break;
	default: id = -1; type = "";
	}
}

void CObjectManager::Dead(int wait){
D3DXVECTOR3 v2; //location of hero
	v2.x = m_pPlayerObject->m_structLocation.x; //  g_nScreenWidth/2;
    v2.y = m_pPlayerObject->m_structLocation.y; //g_nScreenHeight/2;
    v2.z = 500.0f;
	m_Dead = new CGameObject(DEADTEXT_OBJECT,"dead", v2, 0,0);
	dead = true;
}
	
void CObjectManager::wait() {
	if(dead == true){
		playerGold = playerGold/2;
		HP = totalHP/2;
		MP = totalMP/2;
		//destroyWorld(); //Turned off these to keep same world
		m_Dead = NULL;
		spawnX = 0;
		spawnY = 0;
		//createZone(1);
	}
}

int CObjectManager::playerDamageDealt(ObjectType obj){
	float minDamage = 0.0f;
	float maxDamage = 0.0f;
	int damage = 0;

	switch (obj) {
	case LINKATK_OBJECT:
			minDamage = 5.0f*((float)LV-1.0f) + (float)LV*2.5f + (float)(LV%2)*2.5f + (float)(LV%3)*0.5f;
			maxDamage = 7.0f*((float)LV-1.0f) + (float)LV*3.5f + (float)(LV%2)*3.5f + (float)(LV%3)*1.5f; break;
	case FIREBALL_OBJECT: 
			minDamage = 6.0f*((float)LV-1.0f) + (float)LV*3.5f + (float)(LV%2)*4.5f + (float)(LV%3)*2.5f;
			maxDamage = 8.0f*((float)LV-1.0f) + (float)LV*5.5f + (float)(LV%2)*6.5f + (float)(LV%3)*4.5f; break;
	case BARRIER_OBJECT:
			minDamage = ((float)LV*11.0f) - ((float)LV/8.0f);
			maxDamage = ((float)LV*18.0f) - ((float)LV/3.0f); break;
	case LIGHTNING_OBJECT:
			minDamage = ((float)LV*LV*2.75f) - ((float)LV*5.5f);
			maxDamage = ((float)LV*LV*3.75f) - ((float)LV*3.5f); break;
	}

	damage = g_cRandom.number((int)minDamage,(int)maxDamage);

	return damage;
}

int CObjectManager::manaCost(int spellId){

	int cost = 0;

	cost = 25;
	/*

	switch (spellId) {
	case 1: cost = 15 + (LV-1)*8; break; //Fireball
	case 2: cost = 70 + (LV-1)*29; break; //Barrier
	case 3: cost = 24 + (LV-1)*24; break; //Lightning
	case 4: cost = (15 + (LV-1)*8)*5; break; //Fan of Fire
	}*/

	return cost;
}