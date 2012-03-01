/// \file sprite.h
/// Interface for the base sprite class C3DSprite.
/// Copyright Ian Parberry, 2004
/// Last updated July 14, 2010

#pragma once

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "tinyxml.h"

/// The base sprite class. The base sprite contains basic information for drawing a
/// billboard sprite in our 3D world.

class C3DSprite{ //3D sprite

  friend class CSpriteManager;

  private:
    LPDIRECT3DTEXTURE9* m_lpTexture; ///< Pointer to the texture containing the sprite image.
    D3DXIMAGE_INFO m_structImageInfo; ///< Image information.
    LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;  ///< Vertex buffer.
    D3DXMATRIXA16 m_matWorld; ///< Translation matrix.
    int m_nFrameCount; ///< Number of sprite frames.

  public:
    C3DSprite(int framecount); ///< Constructor.
    C3DSprite::~C3DSprite(); ///< Destructor.
    BOOL Load(char* filename,int frame); ///< Load texture image from file.
    void Draw(D3DXVECTOR3 p, float angle, int frame); ///< Draw sprite at point p in 3D space.
    void Release(); ///< Release sprite.

};