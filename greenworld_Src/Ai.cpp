/// \file ai.cpp
/// Code for the intelligent object class CIntelligentObject.
/// Copyright Ian Parberry, 2004.
/// Last updated August 20, 2004.

#include "ai.h"

extern int g_nScreenWidth;

CIntelligentObject::CIntelligentObject(ObjectType object, char* name,
  D3DXVECTOR3 location, float xspeed, float yspeed):
CGameObject(object, name, location, xspeed, yspeed){ //constructor
  m_bIntelligent = TRUE;
  m_fDistance = m_fHorizontalDistance = m_fVerticalDistance = 0.0f;
  m_structPlaneLocation.x = m_structPlaneLocation.y = 0.0f;
}

/// Compute relationship with plane. Intelligent objects need to know where
/// the plane is so that they can intelligently avoid it. This function allows
/// an intelligent object to be notified of the plane's whereabouts. It squirrels
/// away information about the plane into private member variables.
/// \param planeloc plane location
/// \param d distance to plane

void CIntelligentObject::plane(D3DXVECTOR3 planeloc, float d){ //compute relationship with plane

  //remember plane location
  m_structPlaneLocation = planeloc;

  //Euclidean and verticaldistances from plane
  m_fDistance = d;
  m_fVerticalDistance = m_structLocation.y-planeloc.y;
  ydistance = m_structLocation.y;
  xdistance = m_structLocation.x;

  //horizontal distance
  m_fHorizontalDistance = m_structLocation.x-planeloc.x;
  xPlanePosition = planeloc.x;
  yPlanePosition = planeloc.y;

  //wrap horizontal distance to half of world width in magnitude
  //const int nWorldWidth = 2 * g_nScreenWidth; //should be at least 2* screen width
  //if(m_fHorizontalDistance > nWorldWidth/2)
  //  m_fHorizontalDistance -= nWorldWidth;
  //if(m_fHorizontalDistance < -nWorldWidth/2)
  //  m_fHorizontalDistance += nWorldWidth;

}

