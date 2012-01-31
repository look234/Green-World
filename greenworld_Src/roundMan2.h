#pragma once

///< AI state for monsters
enum States6Type{CRUISINGS6_STATE,AVOIDINGS6_STATE, CHASINGS6_STATE, WAITS6_STATE};

#include "ai.h"


class CRoundMan2Object: public CIntelligentObject{

  private:
	float mHeight;
	float mWidth;
    States6Type m_eState; ///< Current state.
	D3DXVECTOR3 loc;
    
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
    void SetState(States6Type state); ///< Change state

  public:
    CRoundMan2Object(char* name, D3DXVECTOR3 location, float xspeed, float yspeed); //< Constructor.
    virtual void move(); ///< Move depending on time and speed

};
