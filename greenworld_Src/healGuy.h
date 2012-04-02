#pragma once

enum HealType{herCRUISINGS3_STATE,herAVOIDINGS3_STATE, herCHASINGS3_STATE, herWAITS3_STATE};

#include "ai.h"

class CHealGuyObject:public CIntelligentObject{

private:
	float height;
	float width;

	
	HealType m_eState;

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
	void SetState(HealType state);

public:
	
	CHealGuyObject(char* name, D3DXVECTOR3 location, float xspeed, float yspeed);
	virtual void move();



};