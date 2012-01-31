/// \file renderer.cpp
/// Direct3D rendering.
/// DirectX stuff that won't change much is hidden away in this file
/// so you won't have to keep looking at it.
/// Copyright Ian Parberry, 2010.
/// Last updated June 15, 2010.

//system includes
#include <d3d9.h>

#include "renderer.h"
#include "defines.h" //essential defines
#include "abort.h"
#include "debug.h"
#include "imagefilenamelist.h"

extern BOOL g_bWindowed;
extern int g_nScreenWidth;
extern int g_nScreenHeight;
extern CImageFileNameList g_cImageFileName;

const DWORD BILLBOARDVERTEX::FVF = D3DFVF_XYZ | D3DFVF_TEX1; //flexible vertex format

/// Constructor
CRenderer::CRenderer():m_d3d(NULL), m_d3ddevice(NULL), m_d3dbackbuffer(NULL){
}

/// Choose which graphics mode to use.
/// Queries D3D to see which graphics modes are compatible with a screen of
/// size g_nScreenWidth by g_nScreenHeight.
///  \param chosenmode returns which D3D graphics mode has been chosen
///  \return TRUE if a compatible mode was found

BOOL CRenderer::ChooseGraphicsMode(D3DDISPLAYMODE& chosenmode){

  D3DFORMAT format=D3DFMT_X8R8G8B8; //graphics format
  int nNumGraphicsModes = m_d3d->GetAdapterModeCount(D3DADAPTER_DEFAULT,format);
  if(nNumGraphicsModes <= 0)return FALSE; //no available graphics modes
  BOOL bChosenMode=FALSE; //TRUE when mode chosen
  D3DDISPLAYMODE curmode; //current mode under consideration

  for(int i=0; i<nNumGraphicsModes && !bChosenMode; i++){ //for each mode
    m_d3d->EnumAdapterModes(D3DADAPTER_DEFAULT, format, i, &curmode); //get specs in curmode
    if( curmode.Width==g_nScreenWidth && curmode.Height==g_nScreenHeight ){ //right size
        chosenmode = curmode; 
        bChosenMode = TRUE;
      }
  }

  return bChosenMode;
} //ChooseGraphicsMode

///Initialize D3D.
/// Creates a D3D object, sets the graphics mode, creates a vertex buffer 
/// for the background.
///  \param hInstance instance handle
///  \param hwnd window handle
///  \return TRUE if it succeeded

BOOL CRenderer::InitD3D(HINSTANCE hInstance, HWND hwnd){ 

  HRESULT hr; //result from D3D function calls

  //create d3d object
  m_d3d=Direct3DCreate9(D3D_SDK_VERSION); //create D3D object
  if(m_d3d == NULL)return FALSE; //bail if failed

  //choose graphics format
  D3DDISPLAYMODE displaymode; //display mode
  if(!ChooseGraphicsMode(displaymode))return FALSE; //bail and fail if no mode acceptable

  //create d3d device for mode
  hr=CreateD3DDevice(hwnd, displaymode);
  if(FAILED(hr)){ //bail if failed
    m_d3ddevice = NULL; return FALSE;
  }

  SetD3dStates();

      //get interface to back buffer for rendering
  hr = m_d3ddevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_d3dbackbuffer);
  if(FAILED(hr)){ //bail if failed
    m_d3dbackbuffer = NULL; return FALSE;
  }

  return TRUE; //success exit
} //InitD3D

//graphics init functions

/// Create the D3D device.
///  \param hwnd window handle
///  \param mode display mode (width, height, color depth, refresh rate)
///  \return the error code returned by D3D, if any

HRESULT CRenderer::CreateD3DDevice(HWND hwnd, D3DDISPLAYMODE mode){ //create d3d device

  //set up device specs data structure
  ZeroMemory(&m_d3dpp, sizeof(D3DPRESENT_PARAMETERS)); //null it out
  m_d3dpp.BackBufferWidth = mode.Width; //width
  m_d3dpp.BackBufferHeight = mode.Height; //height
  m_d3dpp.BackBufferFormat = mode.Format; //color mode
  m_d3dpp.BackBufferCount = 1; //one back buffer
  m_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE; //unused
  m_d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP; //flip pages
  m_d3dpp.hDeviceWindow = NULL; //full screen
  m_d3dpp.Windowed = 0; //full screen
  m_d3dpp.EnableAutoDepthStencil = FALSE; //unused
  m_d3dpp.AutoDepthStencilFormat = (D3DFORMAT)0; //unused
  m_d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER; //can lock buffer
  m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
  m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

  //check for windowed mode
  if(g_bWindowed){ //running in windowed mode, change some settings
    m_d3dpp.Windowed = 1; //windowed
    m_d3dpp.BackBufferCount = 1; //1 back buffer
    m_d3dpp.SwapEffect = D3DSWAPEFFECT_COPY; //blit instead of swapping
    m_d3dpp.FullScreen_RefreshRateInHz = 0; //unused
  }

  //now we can create the device
  return m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
    D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_d3dpp, &m_d3ddevice);
} //CreateD3DDevice


/// Set the world matrix.
/// Sets the world matrix to a translation matrix a given distance along the X-axis.
///  \param dx distance along the X-axis

void CRenderer::SetWorldMatrix(float dx){
  D3DXMATRIXA16 matWorld;   
  D3DXMatrixTranslation(&matWorld, dx, 0.0, 0.0);
  m_d3ddevice->SetTransform(D3DTS_WORLD, &matWorld );
} //SetWorldMatrix

/// Set the view matrix.
/// Sets the view matrix to a given position, in a hard-coded orientation, and direction.
/// \param x x-coordinate of camera location
/// \param y y-coordinate of camera location
/// \param z z-coordinate of camera location

void CRenderer::SetViewMatrix(float x, float y, float z){ //set view matrix
  D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f); //up vector
  D3DXVECTOR3 vEyePt; //camera position
  D3DXVECTOR3 vLookatPt; //camera view vector
  D3DXMATRIXA16 matView; //view matrix

  vEyePt = D3DXVECTOR3(x, y, z);
  vLookatPt = D3DXVECTOR3(x, y, 1000.0f);
  D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec); //compute view matrix
  m_d3ddevice->SetTransform(D3DTS_VIEW, &matView); //set it
}

/// Set the projection matrix.
/// Sets the projection matrix to the correct aspect ratio, and a 90 degree field of view.

void CRenderer::SetProjectionMatrix(){ //set projection matrix
  D3DXMATRIXA16 matProj;
  D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/4.0f,
    (float)g_nScreenWidth/g_nScreenHeight, 1.0f, 10000.0f);
  m_d3ddevice->SetTransform(D3DTS_PROJECTION, &matProj);
} //SetProjectionMatrix

/// Set the ambient lighting.
/// Sets the ambient lighting and the material to something sensible.

void CRenderer::SetAmbientLighting(){ //set ambient lighting
  //scene ambient light
  m_d3ddevice->SetRenderState(D3DRS_AMBIENT, 0xFFFFFFFF);
  //material ambient color
  D3DMATERIAL9 mtrl;
  ZeroMemory(&mtrl, sizeof(mtrl));
  mtrl.Ambient.r = 1.0f;
  mtrl.Ambient.g = 1.0f;
  mtrl.Ambient.b = 1.0f;
  mtrl.Ambient.a = 0.0f;
  m_d3ddevice->SetMaterial(&mtrl);
} //SetLighting

/// Set the texture states.
/// Sets the D3D texture states to useful defaults.

void CRenderer::SetTextureStates(){ //set texture states to default
  m_d3ddevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  m_d3ddevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  m_d3ddevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  m_d3ddevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  m_d3ddevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  m_d3ddevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
  m_d3ddevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  m_d3ddevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
  m_d3ddevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
  m_d3ddevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
} //SetTextureStates



/// Set the D3D render states.
/// Sets the D3D render states (texture states, world matrix, view matrix,
/// projection matrix, background vertex buffer) to useful values for the game.

void CRenderer::SetD3dStates(){
  SetTextureStates(); //set default texture states
  SetWorldMatrix(); //set world matrix
  SetViewMatrix(1024.0f, 384.0f, -350.0f); //set view matrix
  SetProjectionMatrix(); //set projection matrix
  SetAmbientLighting(); //set ambient lighting
} //SetD3dStates

//graphics release functions

///Release D3D.
///Releases the background vertex buffer, D3D device, and D3D object.

void CRenderer::ReleaseD3D(){ 
	  if(m_d3dbackbuffer)m_d3dbackbuffer->Release(); //release back buffer
  if(m_d3ddevice)m_d3ddevice->Release(); //release device
  if(m_d3d)m_d3d->Release(); //release d3d object
} //ReleaseD3D

/// Show the back buffer.
/// If we are fullscreen, flip back buffer to front to show next frame of animation.
/// If we are windowed, blit the back buffer to the front instead. D3D takes care
/// of the details.
///  \return TRUE if it succeeded

BOOL CRenderer::PageFlip(){ //show back buffer
  return SUCCEEDED(m_d3ddevice->Present(NULL, NULL, NULL, NULL));
} //PageFlip

/// Load a texture from a file.
/// Loads an image from a file into a D3D texture. Aborts if the file doesn't exist.
///  \param texture pointer to D3D texture to receive the image
///  \param filename name of the fle containing the texture

void CRenderer::LoadTexture(LPDIRECT3DTEXTURE9& texture, char* filename){
  D3DXIMAGE_INFO structImageInfo; //image information
  HRESULT hres=D3DXCreateTextureFromFileEx(m_d3ddevice, filename,
    0, 0, 1,0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE,
    D3DX_DEFAULT, 0, &structImageInfo, NULL, &texture);
  if(FAILED(hres)) //fail and bail
    ABORT("Texture file %s not found", filename);
} //LoadTexture


//graphics restore functions

/// Restore the D3D device. The D3D device can be lost, for example, by the use alt+tabbing out
/// of the app. This function restores the device, resets the default states that we initially set,
/// and tests the result.
/// \return TRUE if it succeeds

BOOL CRenderer::RestoreDevice(){ //restore lost device and all surfaces
  //release back buffer
  m_d3dbackbuffer->Release(); m_d3dbackbuffer = NULL;
  //reset device
  if(FAILED(m_d3ddevice->Reset(&m_d3dpp)))
    return FALSE;
  //get new back buffer
  if(FAILED(m_d3ddevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_d3dbackbuffer)) )
    return FALSE;
  SetD3dStates(); //set D3D default states
  return SUCCEEDED(m_d3ddevice->TestCooperativeLevel()); //just to be sure
} //RestoreDevice

/// Check for lost D3D device.
/// \return TRUE if the D3D device is lost

BOOL CRenderer::DeviceLost(){ //check for lost device
  return FAILED(m_d3ddevice->TestCooperativeLevel());
} //DeviceLost


void CRenderer::GetTextDC(){
  m_cTextManager.GetDC(m_d3dbackbuffer);
}

void CRenderer::ReleaseTextDC(){
  m_cTextManager.ReleaseDC();
}

void CRenderer::TextWrite(char *string){
  m_cTextManager.Write(string);
}

void CRenderer::TextMoveTo(int x, int y){
  m_cTextManager.MoveTo(x, y);
}