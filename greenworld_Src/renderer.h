/// \file renderer.h 
/// Definition of the renderer class CRenderer.
/// Copyright Ian Parberry, 2010.
/// Last updated June 16, 2010.

#pragma once

#include "Text.h"

/// The renderer handles all the nasty Direct3D details

class CRenderer{
protected:
  D3DPRESENT_PARAMETERS m_d3dpp; ///< Device specifications.
  IDirect3D9* m_d3d; ///< D3D object.
    IDirect3DSurface9* m_d3dbackbuffer; ///< Back buffer for rendering.
  CTextManager m_cTextManager; ///< Text manager.

  HRESULT CreateD3DDevice(HWND hwnd,D3DDISPLAYMODE mode);
  BOOL ChooseGraphicsMode(D3DDISPLAYMODE& chosenmode);
  void SetWorldMatrix(float dx=0.0f);
  void SetViewMatrix(float x, float y, float z);
  void SetProjectionMatrix();
  void SetAmbientLighting();
  void SetTextureStates();
  void SetD3dStates();
  BOOL RestoreDevice();
  BOOL DeviceLost();

public:
  IDirect3DDevice9* m_d3ddevice; ///< Graphics device.
    IDirect3DSurface9* m_pCursorBitmap; ///<surface for mouse cursor.

  CRenderer();
  BOOL InitD3D(HINSTANCE hInstance,HWND hwnd);
  void ReleaseD3D();
  BOOL PageFlip();
  void LoadTexture(LPDIRECT3DTEXTURE9& texture, char* filename);
    void GetTextDC();
  void ReleaseTextDC();
  void TextWrite(char *string);
  void TextMoveTo(int x, int y);
};
