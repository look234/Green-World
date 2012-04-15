#pragma once

enum HealType{herCRUISINGS3_STATE,herAVOIDINGS3_STATE, herCHASINGS3_STATE, herWAITS3_STATE};

#include "ai.h"

class CHealGuyObject:public CIntelligentObject{

  private:
	float mHeight;
	float mWidth;
    HealType m_eState; ///< Current state.

    
	int m_nDesiredHeight; ///< Desired altitude.
    int m_nHeightTime; ///< Time between height changes.
    int m_nHeightDelayTime; ///< Time to next height change.

    int m_nSpeedVariationTime; ///< Last time speed varied.
    int m_nSpeedVariationDuration; ///< Time to next speed variation.
    int m_nLastAiTime; ///< Last time AI was used.
    int m_nAiDelayTime; ///< Time until AI next used.

    void ai(); ///< Artificial intelligence.
    void CruisingAi(); ///< Ai for cruising along.
    void AvoidingAi(); ///< Ai for avoiding plane.
	void ChasingAi();
	void WaitingAi();
    void SetState(HealType state); ///< Change state

  public:
    CHealGuyObject(char* name, D3DXVECTOR3 location, float xspeed, float yspeed); //< Constructor.
    virtual void move(); ///< Move depending on time and speed




};