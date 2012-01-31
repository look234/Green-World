/// \file ai.h
/// Interface for the intelligent object class CIntelligentObject.
/// Copyright Ian Parberry, 2004
/// Last updated August 18, 2004

#pragma once

#include "object.h"

/// The intelligent object class.
/// This is the base class for our artificial intelligence. It maintains
/// the functionality that all intelligent objects have in common, that is,
/// knowledge of the plane's location.

class CIntelligentObject: public CGameObject{

  protected:
    D3DXVECTOR3 m_structPlaneLocation; ///< Plane location.
    float m_fDistance; ///< Euclidean distance to plane.
    float m_fVerticalDistance; ///< Vertical distance to plane
    float m_fHorizontalDistance; ///< horizontal distance to plane
		float ydistance;
	float xdistance;

	float xPlanePosition;
	float yPlanePosition;

  public:

    CIntelligentObject(ObjectType object, char* name, D3DXVECTOR3 location,
      float xspeed, float yspeed); ///< Constructor.
    void plane(D3DXVECTOR3 planeloc,float d); ///< Relationship to plane.

};

