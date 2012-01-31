#include "defines.h"

struct stats{
	//We can use this to increase stats when an item is equipped
	//be sure to remove these stats when the item is unequipped - make sure health and mana stay atleast 1 when doing so
	int hitPoints;
	int manaPoints;

};

class item
{
public:
	char* title; //a //What the player will see
	char* name; //b
	ObjectType object; //c  //will be used for finding the right sprite
	int type; //d //1 = wear, 2 = use
	int maxStack; //e
	int stack; //f


	item(char* a, char* b, ObjectType c, int d, int e, int f);
	~item();

	int changeStack(int a);
	int stackAmount();

	stats itemStats;
};

class potion: public item
{
public:
	int amount; //g
	int timeStart;
	int timeEnd;

	potion(char* a, char* b, ObjectType c, int d, int e, int f, int g);
	~potion();

	void consume();
	void consumeMana();
	void speed();
};

class key: public item
{
public:
	int exitDoor;

	key(char* a, char* b, ObjectType c, int d, int e, int f, int g);
	~key();
};


class weapon: public item {
public:
	bool unique;
	int minDam;
	int maxDam;

	weapon(char* a, char* b, ObjectType c, int d, int e, int f, bool g, int h, int i);
	~weapon();

	int damage(int level);
};