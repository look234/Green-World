#pragma once

///< AI state for monsters
enum States5Type{CRUISINGS5_STATE,AVOIDINGS5_STATE, CHASINGS5_STATE, WAITS5_STATE};

#include "ai.h"


class CRoundManObject: public CIntelligentObject{

  private:
	float mHeight;
	float mWidth;
    States5Type m_eState; ///< Current state.

    
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
    void SetState(States5Type state); ///< Change state

  public:
    CRoundManObject(char* name, D3DXVECTOR3 location, float xspeed, float yspeed); //< Constructor.
    virtual void move(); ///< Move depending on time and speed

};

