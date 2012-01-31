/// \file text.h
/// Interface for the text manager class CTextManager.
/// Copyright Ian Parberry, 2004
/// Last updated September 22, 2010

#pragma once

#include <d3d9.h>

/// Text manager class.
/// Provides an interface for drawing text to a surface.

class CTextManager{ //fixed width font text sprite manager

  private:
    RECT m_rectText; ///< Text rectangle.
    HDC m_hdc; ///< Device context.
    IDirect3DSurface9* m_pSurface; ///< Surface to write text to.
    COLORREF m_cOldTextColor; ///< Saved old text color, use to reset.

  public:
    CTextManager(); ///< Constructor.
    void GetDC(IDirect3DSurface9* buffer); ///< Get device context.
    void ReleaseDC(); ///< Release device context.
    void MoveTo(int xloc,int yloc); ///< Move cursor.
    void Write(char* string); ///< Write string at cursor position.
};

