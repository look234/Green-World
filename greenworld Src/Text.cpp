/// \file text.cpp
/// Code for the text manager class CTextManager.
/// Copyright Ian Parberry, 2004
/// Last updated September 23, 2010

#include <string.h>

#include "text.h"
#include "spriteman.h"

CTextManager::CTextManager(){ //constructor
  m_rectText.left = 0; 
  m_rectText.right = 00; 
  m_rectText.top = 0;
  m_rectText.bottom = 100;
} //CTextManager

/// Move text cursor.
/// This class maintains an imaginary text cursor that moves as you print.
/// This function lets you jump the cursor to a new position.
/// \param xloc x coordinate to jump to
/// \param yloc y coordinate to jump to

void CTextManager::MoveTo(int xloc, int yloc){ //jump to here
  m_rectText.left = xloc; 
  m_rectText.right = xloc + 1000; 
  m_rectText.top = yloc;
  m_rectText.bottom = yloc + 500;
} //MoveTo

/// Write string to device context.
/// Write string to the device context, in the rectangle m_rectText.
/// \param string Text string
/// \param hdc Device context

void CTextManager::Write(char* string){ //write string to device context
  DrawText(m_hdc, string, -1, &m_rectText, 0); //draw text.
} //Write


void CTextManager::GetDC(IDirect3DSurface9* buffer){
  m_pSurface = buffer;
  m_pSurface->GetDC(&m_hdc);
  //draw white text
   m_cOldTextColor = SetTextColor(m_hdc, RGB(0, 0, 0)); //white text
  SetBkMode(m_hdc, TRANSPARENT);
} //GetDC

void CTextManager::ReleaseDC(){ 
  SetTextColor(m_hdc, m_cOldTextColor); //reset text col
  m_pSurface->ReleaseDC(m_hdc);
} //ReleaseDC

