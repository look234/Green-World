/// \file main.cpp 
/// Main file for for Demo 5.
/// Copyright Ian Parberry, 2004.
/// Last updated July 14, 2010.

/// \mainpage Demo 5: Artificial Intelligence
/// Crows have simple rule-based artificial intelligence with some randomness
/// thrown in to make them behave slightly differently.  
/// They try to avoid the plane as much as they can given a limited attention span.
/// You might see some flocking as emergent behaviour.
/// The plane fires a bullet each time you hit the space bar.
/// Bullets have a fixed lifetime.
/// When you hit a crow with a bullet, the crow explodes and turns into
/// a falling corpse, which disappears when it hits the ground. The object manager
/// now has full functionality, in that you can now delete objects.

//system includes

// #include <vld.h>

#include <windows.h>
#include <windowsx.h>
#include <d3dx9.h>
#include <process.h>
#include <vfw.h>

#include "defines.h"
#include "abort.h"
#include "gamerenderer.h"
#include "tinyxml.h"
#include "imagefilenamelist.h"
#include "debug.h"
#include "sprite.h"
#include "object.h"
#include "spriteman.h"
#include "objman.h"
#include "input.h"
#include "sound.h"
#include <iostream>
#include "timer.h"

using namespace std;

extern CSoundManager* g_pSoundManager;

//#include "random.h"

#define length(a) ( sizeof ( a ) / sizeof ( *a ) )

//globals
BOOL g_bActiveApp;  ///< TRUE if this is the active application
HWND g_HwndApp; ///< Application window handle.
HWND g_MCIHand = NULL;
HINSTANCE g_hInstance; ///< Application instance handle.
char g_szGameName[256]; ///< Name of this game
CImageFileNameList g_cImageFileName; ///< List of image file names.
BOOL g_bImagesLoaded = FALSE; ///< TRUE if image loading complete.
BOOL g_bCameraDefaultMode=TRUE; ///< FALSE for eagle-eye camera mode.

//graphics settings
extern int LV = 1;
extern int VFrameCount = 0;
extern BOOL result = FALSE;
extern BOOL MenuUp = FALSE;
extern BOOL SubMenuUp = FALSE;
extern BOOL TalkBubble = FALSE;
extern BOOL textBoxON = FALSE;
extern int menuON = 0;
extern int theCount = 0;
extern int currentItemSelect = 0;

IDirect3D9* g_d3d=NULL; ///< D3D object.
IDirect3DDevice9* g_d3ddevice=NULL; ///< Graphics device.
D3DPRESENT_PARAMETERS g_d3dpp; ///< Device specifications.
LPDIRECT3DTEXTURE9 g_pBackgroundTexture=NULL; ///< Texture for background.
LPDIRECT3DTEXTURE9 g_pFloorTexture=NULL; ///< Texture for floor.
LPDIRECT3DVERTEXBUFFER9 g_pBackgroundVB;  ///< Vertex buffer for background. 
IDirect3DSurface9* g_d3dbackbuffer=NULL; ///< Back buffer for rendering.

//int theKey = 0;
int g_nScreenWidth; ///< Screen width.
int g_nScreenHeight; ///< Screen height.
BOOL g_bWindowed=FALSE; ///< TRUE to run in a window, FALSE for fullscreen.
BOOL g_bWireFrame = FALSE; ///< TRUE for wireframe rendering.

//abort variables
extern char g_errMsg[]; ///< Error message if we had to abort.
extern BOOL g_errorExit; ///< True if we had to abort.

//XML settings
const char* g_xmlFileName="gamesettings.xml"; ///< Settings file name.
TiXmlDocument g_xmlDocument(g_xmlFileName); ///< TinyXML document for settings.
TiXmlElement* g_xmlSettings = NULL; ///< TinyXML element for settings tag..

//Zone Settings (Global)
zoneInformation xmlNextZone[5];
int zoneInformationCounter = 0; //Ghetto

//LOOT Settings
//lootList* loot = new lootList();

//debug variables
#ifdef DEBUG_ON
  CDebugManager g_cDebugManager; //< The debug manager.
#endif //DEBUG_ON

//other game variables
CGameRenderer GameRenderer; ///< The game renderer.
CTimer g_cTimer; ///< The game timer.
CSpriteManager g_cSpriteManager; ///< The sprite manager.
CObjectManager g_cObjectManager; ///< The object manager.
CRandom g_cRandom; ///< The random number generator.
CInputManager* g_pInputManager; ///< The input manager.
CSoundManager* g_pSoundManager; //< The sound manager.


BOOL g_bEndGameState=FALSE; ///< Should we abort current state?
int g_nGameStateTime=0; ///< Time in state.
BOOL g_bDisplayCredits = FALSE; //< TRUE to display intro sequence.
int g_nLogoDisplayTime = 1; //< Number of seconds to display logo screen.
int g_nTitleDisplayTime = 1; //< Number of seconds to display title screen.
int g_nCreditsDisplayTime = 1; //< Number of seconds to display credits screen.

//prototypes for init functions in setup.cpp
HWND CreateDefaultWindow(char* name,HINSTANCE hInstance,int nCmdShow);
BOOL InitD3D(HINSTANCE hInstance,HWND hwnd); //initialize graphics
void ReleaseD3D(); //clean up graphics
void SetD3dStates(); //set D3D render states
BOOL DeviceLost(); //check for lost device
BOOL RestoreDevice(); //restore lost device and all surfaces
void SetWorldMatrix(float); //set world matrix
void SetViewMatrix(float, float, BOOL); //set view matrix

/// Initialize XML settings.
/// Opens an XML file and prepares to read settings from it. Settings
/// tag is loaded to XML element g_xmlSettings for later processing. Aborts if it
/// cannot load the file or cannot find settings tag in loaded file.

void InitXMLSettings(){
  
  //open and load XML file
  if(!g_xmlDocument.LoadFile())
    ABORT("Cannot load settings file %s.", g_xmlFileName);

  //get settings tag
  g_xmlSettings = g_xmlDocument.FirstChildElement("settings"); //settings tag
  if(!g_xmlSettings) //abort if tag not found
    ABORT("Cannot find <settings> tag in %s.", g_xmlFileName);
} //InitXMLSettings

void PlayMovie(char *filename){
  //create movie window
  g_MCIHand = MCIWndCreate(g_HwndApp, g_hInstance,
    MCIWNDF_NOTIFYMODE | MCIWNDF_NOTIFYMEDIA | 
    MCIWNDF_NOMENU | MCIWNDF_NOPLAYBAR, filename);
  //play movie in it
  if(g_MCIHand){
		MCIWndSetZoom(g_MCIHand,160);
		MCIWndPlay(g_MCIHand); // play it 
  }
} //PlayMovie

void KillMovie(){ //stop the movie
if(g_MCIHand) //if valid handle
MCIWndClose(g_MCIHand); //kill it
} //KillMovie

BOOL PageFlip(){ //show back buffer
  return SUCCEEDED(g_d3ddevice->Present(NULL,NULL,NULL,NULL));
} //PageFlip

BOOL CreateOffscreenSurface(IDirect3DSurface9** lplpSurface){
  HRESULT hr = g_d3ddevice->CreateOffscreenPlainSurface(
    g_d3dpp.BackBufferWidth, g_d3dpp.BackBufferHeight,
    g_d3dpp.BackBufferFormat, D3DPOOL_SYSTEMMEM, lplpSurface, NULL);
  return SUCCEEDED(hr);
} //CreateOffscreenSurface

BOOL DisplayScreen(char *filename){ //display image file

  IDirect3DSurface9* pSurface = NULL; //surface for image

  //get surface for image
  CreateOffscreenSurface(&pSurface);

  //load image
  if(FAILED(D3DXLoadSurfaceFromFile(pSurface, NULL, NULL, //copy to surface
    filename, NULL, D3DX_DEFAULT, 0, NULL))) //using default settings
    return FALSE;

  //draw image to back buffer
  g_d3ddevice->BeginScene();
  g_d3ddevice->UpdateSurface(pSurface, NULL, g_d3dbackbuffer, NULL);
  g_d3ddevice->EndScene(); 
  PageFlip(); //flip to front

  //release image surface and return
  if(pSurface)pSurface->Release(); //release
  return TRUE;

} //DisplayScreen

void LoadGameSettings(){

  if(!g_xmlSettings)return; //bail and fail

  //get game name
  TiXmlElement* ist = g_xmlSettings->FirstChildElement("game"); 
  if(ist){
    int len = strlen(ist->Attribute("name")); //length of name string
    strncpy_s(g_szGameName, len+1, ist->Attribute("name"), len); 
  }

  //get renderer settings
  TiXmlElement* renderSettings = 
    g_xmlSettings->FirstChildElement("renderer"); //renderer tag
  if(renderSettings){ //read renderer tag attributes
    renderSettings->Attribute("width", &g_nScreenWidth);
    renderSettings->Attribute("height", &g_nScreenHeight);
    renderSettings->Attribute("windowed", &g_bWindowed);
  }

  //get image file names
  g_cImageFileName.GetImageFileNames(g_xmlSettings);

  //get debug settings
  #ifdef DEBUG_ON
    g_cDebugManager.GetDebugSettings(g_xmlSettings);
  #endif //DEBUG_ON

} //LoadGameSettings

/// Create game objects. 
/// Creates a plane and a collection of crows at random positions.

void CreateObjects(){
	g_pSoundManager->LoadSounds(1); //load game sounds
	g_cObjectManager.createZone(1);
//		 g_pSoundManager->clear(); //clear out old sounds
//      g_pSoundManager->LoadSounds(1); //load game sounds

	
} //CreateObjects

/// Keyboard handler.
/// Handler for keyboard messages from the Windows API. Takes the appropriate
/// action when the user presses a key on the keyboard.
///  \param keystroke virtual key code for the key pressed
///  \return TRUE if the game is to exit


//windows functions: window procedure and winmain

/// Window procedure.
/// Handler for messages from the Windows API. 
///  \param hwnd window handle
///  \param message message code
///  \param wParam parameter for message 
///  \param lParam second parameter for message
///  \return 0 if message is handled

LRESULT CALLBACK WindowProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  
  switch(message){ //handle message

    case WM_ACTIVATEAPP: g_bActiveApp=(BOOL)wParam; break; //iconize

	case WM_KEYDOWN: //keyboard hit
      if(result)DestroyWindow(hwnd);
      break;

	case MCIWNDM_NOTIFYMODE: //from movie player
      if((int)lParam==MCI_MODE_STOP){ //if movie over
        KillMovie();
      }
      break;
   
    case WM_DESTROY: //on exit
		 delete g_pInputManager; //ditto for input manager
//		 		  delete g_pSoundManager; //kill sound manager
      if(!g_bWindowed)ShowCursor(TRUE); //show mouse cursor
      GameRenderer.Release(); //release textures
      //display error message if we aborted
      if(g_errorExit)
        MessageBox(NULL,g_errMsg,"Error",MB_OK|MB_ICONSTOP);
      //finally, quit
      PostQuitMessage(0); //this is the last thing to do on exit
      break;

    default: //default window procedure
      return DefWindowProc(hwnd,message,wParam,lParam);

  } //switch(message)

  return 0L;
} //WindowProc

/// Create a default window.
/// Registers and creates a full screen window.
///  \param name the name of this application
///  \param hInstance handle to the current instance of this application
///  \param nCmdShow specifies how the window is to be shown
///  \return handle to the application window

HWND CreateDefaultWindow(char* name, HINSTANCE hInstance, int nCmdShow){

  WNDCLASS wc; //window registration info
  //fill in registration information wc
  wc.style = CS_HREDRAW|CS_VREDRAW; //style
  wc.lpfnWndProc = WindowProc; //window message handler
  wc.cbClsExtra = wc.cbWndExtra=0;
  wc.hInstance = hInstance; //instance handle
  wc.hIcon = LoadIcon(hInstance,IDI_APPLICATION); //default icon
  wc.hCursor = NULL; //no cursor
  wc.hbrBackground = NULL; //we will draw background
  wc.lpszMenuName = NULL; //no menu
  wc.lpszClassName = name; //app name provided as parameter

  //register window
  RegisterClass(&wc);

  //create and set up window 
  HWND hwnd;
  if(g_bWindowed){   
    hwnd = CreateWindowEx(WS_EX_APPWINDOW|WS_EX_DLGMODALFRAME, name, name,
      WS_POPUP|WS_CLIPCHILDREN|WS_OVERLAPPEDWINDOW, 0, 0,
      g_nScreenWidth, g_nScreenHeight,
      NULL, NULL, hInstance, NULL);
  
	  //center window on screen
	  int x = (GetSystemMetrics(SM_CXSCREEN)-g_nScreenWidth)/2;
	  int y = (GetSystemMetrics(SM_CYSCREEN)-g_nScreenHeight)/2;
	  ::SetWindowPos(hwnd, NULL, x, y ,g_nScreenWidth, g_nScreenHeight,
	    SWP_NOZORDER|SWP_SHOWWINDOW);
  }
  else 
    hwnd = CreateWindowEx(WS_EX_TOPMOST, name, name,
      WS_POPUP, 0, 0,
      GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
      NULL, NULL, hInstance, NULL);  

  if(hwnd){ //if successfully created window
    ShowWindow(hwnd, nCmdShow); UpdateWindow(hwnd); //show and update
    SetFocus(hwnd); //get input from keyboard
  }

  return hwnd; //return window handle
} //CreateDefaultWindow

/// Initialize the graphics using DirectX.
/// Draws a floor and a backdrop.

void InitGraphics(){ //initialize graphics
  if(!GameRenderer.InitD3D(g_hInstance, g_HwndApp))
      ABORT("Unable to select graphics display mode.");

  if(!g_bWindowed)ShowCursor(FALSE); //hide the mouse cursor
  GameRenderer.InitBackgroundVertexBuffer();
} //InitGraphics

/// DrawWelcomeScreen.  
/// Put a white background on the screen initially. This code should
/// be familiar to you - we used it in Demo 0.

void DrawWelcomeScreen(){ //welcome screen
  HDC hdc = GetDC(g_HwndApp); //device context for screen
  RECT rc; //screen rectangle

  //draw black rectangle to screen
  GetClientRect(g_HwndApp, &rc); //get screen rectangle into rc
  FillRect(hdc, &rc,( HBRUSH)GetStockObject(WHITE_BRUSH)); //fill rc black

  //draw white text
  COLORREF cOldTextColor = SetTextColor(hdc, RGB(0, 0, 0)); //white text
  COLORREF cOldBkColor = SetBkColor(hdc, RGB(255, 255, 255)); //text bg color black
  DrawText(hdc, "Loading Data...", -1, &rc, //draw text...
    DT_CENTER | DT_VCENTER | DT_SINGLELINE); //...centered on screen

  //clean up and exit
  SetTextColor(hdc, cOldTextColor); //reset text color
  SetBkColor(hdc, RGB(0, 0, 0)); //reset text background color
  ReleaseDC(g_HwndApp, hdc); //release device context
} //InitGraphics

/// LoadImages.
/// Multithread the loading process so that things don't freeze
/// up when we're loading. This isn't important now, but it will
/// be later if you have lots of images in your game.

void __cdecl LoadImages(void *pParam){
  GameRenderer.LoadTextures(); //load images
  //Sleep(10000); //uncomment this to see what happens in long loads
  g_bImagesLoaded=TRUE; //signal that loading is done
}
  
/// Winmain. 
/// Main entry point for this application. 
///  \param hInstance handle to the current instance of this application
///  \param hPrevInstance unused
///  \param lpCmdLine unused 
///  \param nCmdShow specifies how the window is to be shown
///  \return TRUE if application terminates correctly

void ProcessFrame(){
 GameRenderer.ProcessFrame();

    g_pSoundManager->beginframe();
	if(!MenuUp)
   g_cObjectManager.UpdateMP();

  
	//DEBUGPRINTF("This is a test of the DEBUGPRINTF system.\n");
  //input handlers 
  if(g_pInputManager->ProcessKeyboardInput()){ //process  input
    DestroyWindow(g_HwndApp); 
    return; //bail from ProcessFrame
	
	
  }
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,int nCmdShow){
  MSG msg; //current message
  HWND hwnd; //handle to fullscreen window

  #ifdef DEBUG_ON
    g_cDebugManager.open(); //open debug streams, settings came from XML file
  #endif //DEBUG_ON

  g_hInstance=hInstance; //save instance handle

  g_cTimer.start(); //start game timer

  InitXMLSettings(); //initialize XML settings reader
  LoadGameSettings();

  //create fullscreen window
  hwnd = CreateDefaultWindow(g_szGameName, hInstance, nCmdShow);
  if(!hwnd)return FALSE; //bail if problem creating window
  g_HwndApp = hwnd; //save window handle

      g_pSoundManager = new CSoundManager(g_HwndApp);
  g_pSoundManager->LoadSettings(); //load general settings

//  PlayMovie("Intro.wmv");

  DrawWelcomeScreen();
  InitGraphics(); //initialize graphics
  g_pInputManager = new CInputManager(g_hInstance, g_HwndApp); //create input manager (after graphics)
  CreateObjects(); //create game objects
  

  _beginthread(LoadImages, 0, NULL); 

  //message loop
  while(TRUE)
    if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)){ //if message waiting
      if(!GetMessage(&msg,NULL,0,0))return (int)msg.wParam; //get it
      TranslateMessage(&msg); DispatchMessage(&msg); //translate it
    }
    else     
      if(g_bActiveApp)ProcessFrame(); 
      else WaitMessage(); //process frame
} //WinMain
