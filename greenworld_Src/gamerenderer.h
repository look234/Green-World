/// \file gamerenderer.h 
/// Definition of the renderer class CGameRenderer.
/// Copyright Ian Parberry, 2010.
/// Last updated June 16, 2010.

#pragma once

#include "renderer.h"

/// The game renderer handles all the nasty Direct3D details for the game

class CGameRenderer: public CRenderer{
private: 
  LPDIRECT3DVERTEXBUFFER9 m_pBackgroundVB;  ///< Vertex buffer for background.
  LPDIRECT3DTEXTURE9 m_pBackgroundTexture; ///< Texture for background.
  LPDIRECT3DTEXTURE9 m_pFloorTexture; ///< Texture for floor.
  LPDIRECT3DTEXTURE9 m_pWireframeTexture; ///< Texture for showing wireframe, all black.
  BOOL m_bCameraDefaultMode;
public:
  CGameRenderer::CGameRenderer(); //constructor
  void InitBackgroundVertexBuffer();
  void DrawBackground(float x);
  
  void LoadTextures(); //create textures for image storage
  void Release(); //release offscreen images 

  void ComposeFrame();
  void ProcessFrame();

  void FlipCameraMode();
  void GetKey(int key);
};
