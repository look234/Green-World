/// \file objman.h
/// Interface for the object manager class CObjectManager.
/// Copyright Ian Parberry, 2004
/// Last updated July 14, 2010

#pragma once

#define MAX_OBJECTS 2000 //max number of objects in game
#define MAX_LOOT_OBJECTS 300
//#define MAX_TILE_OBJECTS 5000

#include "lootList.h"
#include "object.h"
#include "Random.h"
#include "timer.h"
#include "item.h"
#include <string>

using namespace std;


/// The object manager. The object manager is responsible for managing
/// the game objects. The plane object is a special object that gets
/// special treatment because it represents the player.

struct exitInformation{
	int x;
	int y;
	int zoneId;
	int spawnX;
	int spawnY;
	int keyId;
};

struct monsterHealthBarsList{
	CGameObject** m_monsterHealthObjects;
	CGameObject** m_monsterHealthFrameObjects;
	CGameObject* m_level;
	bool show;
	int currentMonster;
	int mLV;
};

struct monsterInformation{
	int LV;
	int HP;
	int totalHP;
	int tClass;
	int xp;
	int bossId;
};

struct lootPeiceInformation{
	char* title;
	char* name; //b
	char* objType;
	ObjectType object; //c  //will be used for finding the right sprite
	int type; //d //1 = wear, 2 = use
	int maxStack; //e
	int stack; //f

	//Potion
	int amount;
};

class CObjectManager{

	//So in order to save a bunch of time on redoing methods I've changed data from private to public to make some things external...
	//duct tape and chewing gum boys
  public:
    CGameObject **m_pObjectList; ///< List of game objects.
	CGameObject *m_Dead;
	
	int monsterAmount;

	//Variables for text output, probably will make them global later
	int collisionCount; //= 0;//keeps track of how many times the hero has collided with a monster
	int HP; //= 500; //Hero's Health
	int totalHP; //= 500; //Max Helath
	int MP;
	int totalMP;
	int LV; //Hero's current level
	int XP; //= 0; //Hero's current Experience
	int nextXP;
	int playerGold;

	void expInc(int newXP);
	void defineNextXP();
	void levelUp(int level);

	int zoneHeight;
	int zoneWidth;
	int spawnX;
	int spawnY;
	
    CGameObject *m_pPlayerObject; //< Player object - the plane.
    int m_nCount; ///< How many objects in list.
    int m_nMaxCount; ///< Maximum number of objects.
    int m_nLastGunFireTime; ///< Time gun was last fired.
	int m_nLastFireball;
	int m_nLastLightning;
	int m_nLastBarrier;
	int m_nLastTornado;

	monsterInformation *monsterInfo;

	CRandom g_cRandom;
	CTimer g_cTimer;

	//Zone Settings (Global)
	//Tile
	CGameObject **m_tileObjects;
	int m_nTileCount;
	int m_nMaxTileCount;

	//Impassible Wall
	CGameObject **m_wallObjects;
	int m_nWallCount;
	int m_nMaxWallCount;

	//Player invulnerability
	int timeHit;
	int timeToBeHitAgain;
	
	//Spells
	void createSpells(ObjectType object, char* name, D3DXVECTOR3 location, float xspeed, float yspeed);
	CGameObject **m_spellObjects;
	int *spellTimers;
	int m_nSpellCount;
	int m_nMaxSpellCount;
	
	//Exits
	CGameObject **m_exitObjects;
	int m_nExitCount;
	int m_nMaxExitCount;
	exitInformation *exitInfo;
	bool inAnExit;
	int* zoneIds;

	//Impassible Objects
	CGameObject **m_impasibleObjects;
	int m_nImpassibleCount;
	int m_nMaxImpassibleCount;

	CGameObject **m_impasibleObjects32;
	int m_nImpassibleCount32;
	int m_nMaxImpassibleCount32;

	//Night
	CGameObject *night;

	//menu
	CGameObject **m_menuObjects;
	int m_nMenuCount;
	int m_nMaxMenuCount;

	CGameObject **m_itemCursorObjects;
	int m_nItemCursorCount;
	int m_nMaxItemCursorCount;

	CGameObject **m_NPCObjects;
	int m_nNPCCount;
	int m_nMaxNPCCount;
	char* m_nNPCName;

	CGameObject **m_pointerObjects;
	int m_nPointerCount;
	int m_nMaxPointerCount;
	CGameObject **m_itemsMenuObjects;
	int m_nItemsMenuCount;
	int m_nMaxItemsMenuCount;
	CGameObject **m_spellsMenuObjects;
	int m_nSpellsMenuCount;
	int m_nMaxSpellsMenuCount;
	CGameObject **m_optionsMenuObjects;
	int m_nOptionsMenuCount;
	int m_nMaxOptionsMenuCount;

	CGameObject **m_talkBubbleObjects;
	int m_nTalkBubbleCount;
	int m_nMaxTalkBubbleCount;

	CGameObject **m_textBoxObjects;
	int m_nTextBoxCount;
	int m_nMaxTextBoxCount;
	CGameObject **m_alphaObjects;
	int m_nAlphaCount;
	int m_nMaxAlphaCount;

    //distance functions
    float distance(CGameObject *first, CGameObject *second); ///< Distance between objects.
	float distanceRect(CGameObject *first, CGameObject *second); ///< Distance between objects.
	bool isSafe;

    //collision detection
	void MonsterDetection(); /// for hero/monster damage calculation
	void NPCDetection(); /// for hero/monster damage calculation
    void CollisionDetection(); ///< Process all collisions.
    void CollisionDetection(int index); ///< Process collisions with one object.
	void ObjectCollision(int index);
	void Invincible();

    //managing dead objects
    void cull(); ///< Cull dead objects
    void kill(int index); ///< Remove dead object from list.
	void killSpell(int index);
    void replace(int index); ///< Replace object by next in series.
	void destroyWorld();

	//Loot (For Real-Reals)
	item **playerLoot;
	item **lootOnGround;
	CGameObject **m_groundLootObjects;
	CGameObject **m_invintoryLootObjects;
	int maxGroundLoot;

	item *m_equipmentObjects[3];

	int playerLootWidth;
	int playerLootHeight;
	CGameObject **m_playerLootObjects;
	int m_nMaxPlayerLootObjects; //for drawing objects only
	int m_nPlayerLootCount; //for drawing objects only
	void defineLoot();
	bool addLoot(item* a);
	bool useItem(int index);
	bool equipItem(int index);
	void reduceItem(int index, int amount);

	void pickUpLoot();
	void determineLoot(int tClass, int mLevel, D3DXVECTOR3 mobV);
	void randLoot(int tClass, int mLevel, lootPeiceInformation &inf);
	ObjectType anObject(int id);
	void dropBossLoot(D3DXVECTOR3 mobV, int id);
	
	void speedDuration();
	int timeEnd;
	int getTime();

	//spee
	bool speedOn;

	char* zoneName;
    CObjectManager(); ///< Constructor
    ~CObjectManager(); ///< Destructor
    void create(ObjectType object, char* name, D3DXVECTOR3 location,
     float xspeed, float yspeed); ///< Create new object
	//bool isAttacking();  //USELESS RIGHT NOW
    void move(); ///< Move all objects
    void draw(); ///< Draw all objects
	void draw(int key); ///< Draw all objects
	void draw2(int key); ///< Draw all objects
	void draw(char letter); ///< Draw all objects
    void GetPlayerLocation(float &x, float &y); ///< Get location of plane.
    void Accelerate(int xdelta,int ydelta); ///< Change speed.
	void HeroAttack(int key); ///< Fire the plane's gun.
	void FanofFire();
	void Lightning(int key);
	void Fireball(int key);
	void enemyFireball(int key, D3DXVECTOR3 location,float xspeed, float yspeed);
	void enemyHoming(int key, D3DXVECTOR3 location,float xspeed, float yspeed);
	void Tornado(int key);
	void Barrier(int key);

	void MoveUp(int move);
	void MoveDown(int move);
	void MoveLeft(int move);
	void MoveRight(int move);
	void stepUp(float step);

	void createZone(int zId);

	void createTile(int tileSet, int zoneHeight, int zoneWidth);
	void createWall(int tileSet, int zoneHeight, int zoneWidth);
	void createForest(int tileSet, int zoneHeight, int zoneWidth);
	void inExit();

	bool CanMove(float angle);

	void lotsofloot();

	void DrawValue(char* title, int value, int x, int y); ///Dont worry about this, it just is
	void DrawValue2(char* title, const char* value, int x, int y); //Drawing two strings
	void DrawTextHeader(); ///This is where you go to add text
	int enemies(); /// Get number of enemies left.
	void createHouse(float x, float y, int type);

	bool monsterSmartMoveCheck(CGameObject* tempPlayerObject);
	bool spellSmartMoveCheck(CGameObject* tempPlayerObject);

	void nightMove();

	//menu
	void MenuYes();
	void ShowMenu(int key);
	void ItemTextBox();
	void SpellTextBox();
	void OptionsTextBox();
	void ShowTextBox();
	void ShowText();
	void SubMenu();
	void SubMenuYes();
	void TextBoxYes();
	char AmountConveter(int bah);
	void ItemSelect(int key);

	void UpdateMP();
	void wait();
	void Dead(int wait);

	int m_nMaxHealthCountMob;
	CGameObject **m_healthObjects;
	int m_nHealthCount;
	int m_nMaxHealthCount;
	CGameObject **m_manaObjects;
	int m_nManaCount;
	int m_nMaxManaCount;

	CGameObject **m_healthBoxObjects;
	int m_nHealthBoxCount;
	int m_nMaxHealthBoxCount;

	int m_nUiCount;
	int m_nMaxUiCount;

	void createUi();
	void HELPBox();
	void healthBox();
	void healthBar();
	void manaBar();
	
	void monsterHealthBarDestroy(int index);
	void monsterHealthBar(int index);
	monsterHealthBarsList* monsterHealthBars;

	//Minimap
	CGameObject **m_miniMapObjects;
	void drawMiniMap();
	int m_mapObjectsCount;

	//Save and Load Game Data
	bool loadedSaved;
	bool saveGame();
	void itemType(ObjectType obj,int &id, char* &type);
	bool loadSavedGame();

	D3DXVECTOR3 prevCoords;

	//Monster and Player is hit
	CGameObject *playerHurt;
	CGameObject *mobHurt;
	int mobIndex;
	int playerHurtTime;
	bool playerIsHurt;
	int mobHurtTime;
	bool mobIsHurt;


	//Last minute things
	bool cooliding;
	char convertChar(int num);
	int playerDamageDealt(ObjectType obj);
	int manaCost(int spellId);
	int damageDone;
};