#pragma once

///< AI state for monsters
enum States4Type{CRUISINGS4_STATE,AVOIDINGS4_STATE, CHASINGS4_STATE, WAITS4_STATE};

#include "ai.h"


class CFlameGuy2Object: public CIntelligentObject{

  private:
	float mHeight;
	float mWidth;
    States4Type m_eState; ///< Current state.

    
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
    void SetState(States4Type state); ///< Change state

  public:
    CFlameGuy2Object(char* name, D3DXVECTOR3 location, float xspeed, float yspeed); //< Constructor.
    virtual void move(); ///< Move depending on time and speed

};

