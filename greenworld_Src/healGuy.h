#pragma once

enum States3Type{CRUISINGS3_STATE,AVOIDINGS3_STATE, CHASINGS3_STATE, WAITS3_STATE};

#include "ai.h"

class CHealGuyObject:public CIntelligentObject{

private:
	float height;
	float width;

	States3Type m_eState;

	int maxHeight;//max height
	int timeHeight;//time between witch
	int DelayTime;//time to next switch

	int speedVarTime;//last time speed varried
	int speedVarDur;//time to next speed varry
	int lastAi;//last time Ai was used
	int AiDelay;// time until ai used

	void ai();
	void KeepUp();
	void AvoidMonster();
	void Stay();
	void Healing();

public:
	CHealGuyObject(char* name, D3DXVECTOR3 location, float xspeed, float yspeed);
	virtual void move();



};