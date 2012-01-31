#pragma once

///< AI state for monsters
enum ATTACK_STATE;

#include "ai.h"


class CTornadoObject: public CIntelligentObject{

  private:
	float mHeight;
	float mWidth; 
	int m_nDesiredHeight; ///< Desired altitude.
    int m_nHeightTime; ///< Time between height changes.
    int m_nHeightDelayTime; ///< Time to next height change.
	int direction;
    int m_nSpeedVariationTime; ///< Last time speed varied.
    int m_nSpeedVariationDuration; ///< Time to next speed variation.
    int m_nLastAiTime; ///< Last time AI was used.
    int m_nAiDelayTime; ///< Time until AI next used.

    void ai(); ///< Artificial intelligence.
    void AttackingAi(int direct);

  public:
    CTornadoObject(char* name, D3DXVECTOR3 location, float xspeed, float yspeed); //< Constructor.
    virtual void move(); ///< Move depending on time and speed

};

