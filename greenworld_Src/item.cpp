//#include "item.h"
#include "objman.h"
#include "Timer.h"
#include "Random.h"
//

extern CObjectManager g_cObjectManager;
extern CTimer g_cTimer; 
extern CRandom g_cRandom;

//item
/*	char* title; //a //What the player will see
	char* name; //b
	ObjectType object; //c  //will be used for finding the right sprite
	int type; //d //1 = wear, 2 = use
	int maxStack; //e
	int stack; //f
*/
	item::item(char* a, char* b, ObjectType c, int d, int e, int f){
		title = a;
		name = b;
		object = c;
		type = d;
		maxStack = e;
		stack = f;
	}

	item::~item(){
		delete title;
		delete name;
	}

	int item::changeStack(int a){
		int stackPrev = stack;
		stack = stack + a;
		if(stack > maxStack){
			stackPrev = stack - maxStack;
			stack = maxStack;
			return stackPrev;
		}
		else 
			return 0;
	}

	int item::stackAmount(){
		return stack;
	}

//Potion 
	potion::potion(char* a, char* b, ObjectType c, int d, int e, int f, int g) : item(a, b, c, d, e, f) {
		//Potion
		amount = g;
	}

	potion::~potion(){ item::~item(); }

	void potion::consume() {
		int maxHP = g_cObjectManager.totalHP;
		int currentHP = g_cObjectManager.HP;

		currentHP += amount;

		if(currentHP > maxHP)
			currentHP = maxHP;

		g_cObjectManager.HP = currentHP;
	}

	void potion::speed() {
		if(g_cObjectManager.speedOn == false)
		{
			timeStart = g_cObjectManager.getTime();
			g_cObjectManager.timeEnd = timeStart + 1500;
			g_cObjectManager.speedOn = true;
			g_cObjectManager.m_pPlayerObject->step = (float)amount;

		}
		else
			g_cObjectManager.speedOn = false;	
	}

	void potion::consumeMana() {
		int maxMP = g_cObjectManager.totalMP;
		int currentMP = g_cObjectManager.MP;

		currentMP += amount;

		if(currentMP > maxMP)
			currentMP = maxMP;

		g_cObjectManager.MP = currentMP;
	}

	//Key
	key::key(char* a, char* b, ObjectType c, int d, int e, int f, int g) : item(a, b, c, d, e, f){
		exitDoor = g;	
	}
	key::~key(){ item::~item(); }

	weapon::weapon(char* a, char* b, ObjectType c, int d, int e, int f, bool g, int h, int i): item(a, b, c, d, e, f){
		unique = g;
		minDam = h;
		maxDam = i;
	}
	weapon::~weapon(){ item::~item(); }

	int weapon::damage(int level){
		float dam1 = (float)(3.5*level*g_cRandom.number(minDam,maxDam) / (g_cRandom.number(2,5)));
		float dam2 = (float)(3.5*level*g_cRandom.number(minDam,maxDam) / (g_cRandom.number(2,5)));

		return g_cRandom.number((int)dam1,(int)dam2);
	}