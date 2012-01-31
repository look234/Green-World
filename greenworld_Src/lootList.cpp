#include "lootList.h"
#include <iostream>
using namespace std;

#ifndef NULL
#define NULL 0
#endif


	lootList::lootList() {
		start = NULL; 
		lootAmount = 0;
		maxLootAmount = 64;
	}

	lootList::~lootList() { 
		lootListStruct *temp;
		if(start == NULL)
			return;

		while(start != NULL)
		{
			temp = start->next;
			delete start;
			start = temp;
		}
	}

	void lootList::add(char *name){
		

		if(start == NULL){
			start = new lootListStruct;
			start->name = name;
			start->next = NULL;
		}
		else
		{
			lootListStruct *temp;
			lootListStruct *temp2;

			temp = start;

			while(temp->next != NULL)
				temp = temp->next;

			temp2 = new lootListStruct;
			temp2->name = name;
			temp2->next = NULL;
			temp->next = temp2;
		}

		lootAmount++;
	};

	//void lootList::next(){ }
	//void lootList::remove(lootListStruct){ }
	//char lootList::returnList(){ }

	void lootList::print(){
		lootListStruct *temp;
		temp = start;

		while(temp->next != NULL){
			cout << temp->name << endl;
			temp = temp->next;
		}
	}
