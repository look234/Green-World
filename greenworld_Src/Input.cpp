#include <objbase.h>
#include <stdio.h>

#include "input.h"
#include "objman.h" //for object manager
#include "gamerenderer.h"
#include "debug.h"
#include "Timer.h"
#include "Sound.h"


UCHAR keystate[256];
extern CSoundManager* g_pSoundManager;
extern CTimer g_cTimer;  //game timer
extern CObjectManager g_cObjectManager; //object manager
extern int g_nScreenWidth; //screen width
extern int g_nScreenHeight; //screen height
extern BOOL g_bWindowed; //windowed or fullscreen
extern CGameRenderer GameRenderer; ///< The game renderer.
extern POINT g_pointWindowPosition; //position of upper left corner of client area in windowed mode
extern int g_nWindowWidth; //actual window width in windowed mode
extern int g_nWindowHeight; //actual window height in windowed mode
extern BOOL g_bWireFrame; //draw in wireframe if TRUE
int time = 0;
extern int theKey = 0;
int keystroke2 = 4;
extern int LV;
extern bool dead;
extern BOOL MenuUp;
extern BOOL SubMenuUp;
extern BOOL result;
extern BOOL TalkBubble;
int testDown = 0; //check to see if a button is pressed
extern int currentItemSelect;

/// Constructor.
/// \param hInstance Instance handle
/// \param hwnd Window handle

CInputManager::CInputManager(HINSTANCE hInstance, HWND hwnd){ //constructor

  //button manager doesn't exist yet

	
  //create DirectInput object
  HRESULT hr=DirectInput8Create(hInstance, DIRECTINPUT_VERSION, 
    IID_IDirectInput8A, (void**)&m_lpDirectInput, NULL); 
  if(FAILED(hr)){
    m_lpDirectInput = NULL; //in case it fails
    return; //bail out
  }

  //device initializations
  InitKeyboardInput(hwnd); //set up keyboard
 

  //etc.

}

CInputManager::~CInputManager(){ //destructor

  if(m_pKeyboard){ //release keyboard
    m_pKeyboard->Unacquire(); //unacquire device
    m_pKeyboard->Release(); //release it
  }


  

  //now release DirectInput
  if(m_lpDirectInput)m_lpDirectInput->Release();

  //get rid of the button manager

}

//DirectInput setup routines

/// Initialize keyboard input.
/// Create the DirectInput keyboard device, set the cooperative level,
///  set for buffered input, and acquire the keyboard.
/// \param hwnd Window handle

BOOL CInputManager::InitKeyboardInput(HWND hwnd){ //set up keyboard input
	lootList* loot = new lootList();
  //create keyboard device
  HRESULT hr=m_lpDirectInput->CreateDevice(GUID_SysKeyboard,&m_pKeyboard,NULL); 
  if(FAILED(hr)){
    m_pKeyboard=NULL; //in case it fails
    return FALSE; //bail out
  }

  //set keyboard data format, c_dfDIKeyboard is a "predefined global"
  //yucko ugly part of DirectInput, thanks a lot Microsoft
  hr=m_pKeyboard->SetDataFormat(&c_dfDIKeyboard); 
  if(FAILED(hr))return FALSE; //fail and bail

  //set the cooperative level 
  hr=m_pKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);  
  if(FAILED(hr))return FALSE; //fail and bail

  //set buffer size for buffered input
  DIPROPDWORD dipdw;
  dipdw.diph.dwSize = sizeof(DIPROPDWORD);
  dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
  dipdw.diph.dwObj = 0;
  dipdw.diph.dwHow = DIPH_DEVICE;
  dipdw.dwData = DI_BUFSIZE; //buffer size
  hr = m_pKeyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);  
  if(FAILED(hr))return FALSE; //fail and bail

  //acquire the keyboard
  hr=m_pKeyboard->Acquire();  
  if(FAILED(hr))return FALSE; //fail and bail
  
		  
		  

  return TRUE;
}

/// Initialize mouse input.
/// Create the DirectInput mouse device, set the cooperative level,
/// set for buffered input, and acquire the mouse. Also creates the
/// mouse event to handle mouse interrupts, and sets the mouse cursor.
/// \param hwnd Window handle



//keyboard processing

/// Process keyboard input.
/// Processes keyboard events that have been buffered since the last
/// time this function was called.
/// \return TRUE if user has hit the ESC key

BOOL CInputManager::ProcessKeyboardInput(){ //process buffered keyboard events
  BOOL result=FALSE; //return result
  if(!m_pKeyboard)return FALSE; //no device so fail and bail
  DWORD dwElements=DI_BUFSIZE; //number of events to process

  //get number of events to process into member buffer m_DIod
  HRESULT hr=m_pKeyboard->GetDeviceData(
    sizeof(DIDEVICEOBJECTDATA) ,m_DIod,&dwElements, 0);

  //if it failed, we may be able to acquire the device
  if(FAILED(hr)){ //lost the device somehow
    hr = m_pKeyboard->Acquire(); //try to reacquire
    while(hr == DIERR_INPUTLOST) hr = m_pKeyboard->Acquire(); //try real hard
    return FALSE; //live to fight another day
  }
  if(FAILED(hr))return FALSE; //fail and bail paranoia
  //process inputs from m_DIod now
  for(int i=0; (i<(int)dwElements) && !result; i++){ //for each input
    if(m_DIod[i].dwData & 0x80) //if key went down
      result = result || KeyboardHandler(m_DIod[i].dwOfs); //handle it
  }
  return result;
}

//keyboard handlers for different game states

/// Main keyboard handler.
/// This function checks the current game state, then calls the
/// keyboard handler for the appropriate state.
/// \param keystroke Keyboard event to process
/// \return TRUE if the player hit ESC

BOOL CInputManager::KeyboardHandler(DWORD keystroke){ //main keyboard handler
	
  BOOL result = FALSE; //TRUE if we are to exit game

  GameKeyboardHandler(keystroke);
  

  return result;
} //KeyboardHandler

/// Intro keyboard handler.
/// Sets g_bEndGameState in response to any keystroke.


/// Main menu keyboard handler.
/// Selects menu items in response to the appropriate keystroke.
/// \param keystroke Keyboard event to process
/// \return TRUE if the player hit ESC



/// Game menu keyboard handler.
/// Handles keystroke during game play.
/// Sets g_bEndGameState if the player hit ESC
/// \param keystroke Keyboard event to process

void CInputManager::GameKeyboardHandler(DWORD keystroke){ //game

	
	for(int i = 0; i < 256; i++)
			keystate[i] = 0;

	int b = 0;

	m_pKeyboard->GetDeviceState(sizeof(keystate), keystate);
  switch(keystroke){
    case DIK_ESCAPE: result = TRUE; break;
	case DIK_SPACE: if(!TalkBubble){
		if(!MenuUp && !dead){GameRenderer.GetKey(theKey+4);
				  g_pSoundManager->play(FIRE_SOUND);
				g_cObjectManager.HeroAttack(theKey);break;}
				g_cObjectManager.SubMenuYes();
				g_cObjectManager.SubMenu();break;
				}g_cObjectManager.ShowTextBox();break;

   /* case DIK_F1: GameRenderer.FlipCameraMode(); break;
    case DIK_F2:
      g_bWireFrame = !g_bWireFrame;
      if(g_bWireFrame)
        GameRenderer.m_d3ddevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
      else
        GameRenderer.m_d3ddevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
      break;*/
	case DIK_UP:theKey = 4;
		if(!SubMenuUp){
			g_cObjectManager.ShowMenu(theKey);
			if(!MenuUp){
				if(!dead){
				while(keystate[DIK_UP] & 0x80){theKey = 4;
					testDown = 1;
					if(g_cTimer.elapsed(time, 30)){
						GameRenderer.GetKey(4);
			
						if(g_cObjectManager.speedOn){
							g_cObjectManager.stepUp(g_cObjectManager.m_pPlayerObject->step);
							g_cObjectManager.speedDuration();
						}else if(b > 6)
							g_cObjectManager.stepUp(15);
						else
							g_cObjectManager.stepUp((float)(3 + b*2));
							b++;
							g_cObjectManager.MoveUp(7);
							GameRenderer.ProcessFrame();
							for(int i = 0; i < 256; i++)
								keystate[i] = 0;
								m_pKeyboard->GetDeviceState(sizeof(keystate), keystate);
					}
					ProcessKeyboardInput();
				}testDown = 0;
				}break;
			}break;
		}g_cObjectManager.ItemSelect(theKey);break;
    case DIK_DOWN: theKey = 3;
		if(!SubMenuUp && !dead){
			g_cObjectManager.ShowMenu(theKey);
			if(!MenuUp){
				if(!dead){
				while(keystate[DIK_DOWN] & 0x80){testDown = 1;
				theKey = 3;
					if(g_cTimer.elapsed(time, 30)){
						GameRenderer.GetKey(3);

						if(g_cObjectManager.speedOn){
							g_cObjectManager.stepUp(g_cObjectManager.m_pPlayerObject->step);
							g_cObjectManager.speedDuration();
						}else if(b > 6)
							g_cObjectManager.stepUp(15);
						else
							g_cObjectManager.stepUp((float)(3 + b*2));
							b++;
							g_cObjectManager.MoveDown(8);
							GameRenderer.ProcessFrame();
							for(int i = 0; i < 256; i++)
								keystate[i] = 0;
								m_pKeyboard->GetDeviceState(sizeof(keystate), keystate);
					}
					ProcessKeyboardInput();
				}testDown=0;
				}break;
			}break;
		}g_cObjectManager.ItemSelect(theKey);break;
    case DIK_LEFT: theKey = 1;
		if(!SubMenuUp){
			g_cObjectManager.ShowMenu(theKey);
			if(!MenuUp){
				if(!dead){
				while(keystate[DIK_LEFT] & 0x80){testDown=1;theKey = 1;
					if(g_cTimer.elapsed(time, 30)){
						GameRenderer.GetKey(1);

						if(g_cObjectManager.speedOn){
							g_cObjectManager.stepUp(g_cObjectManager.m_pPlayerObject->step);
							g_cObjectManager.speedDuration();
						}else if(b > 6)
							g_cObjectManager.stepUp(15);
						else
							g_cObjectManager.stepUp((float)(3 + b*2));
							b++;
							g_cObjectManager.MoveLeft(5);
							GameRenderer.ProcessFrame();
							for(int i = 0; i < 256; i++)
								keystate[i] = 0;
								m_pKeyboard->GetDeviceState(sizeof(keystate), keystate);
					}
					ProcessKeyboardInput();
				}testDown=0;
				}break;
			}break;
		}g_cObjectManager.ItemSelect(theKey);break;
    case DIK_RIGHT:theKey = 2;
		if(!SubMenuUp){

			g_cObjectManager.ShowMenu(theKey);
			if(!MenuUp){//tells the heroattack function to attack up
				if(!dead){
				while(keystate[DIK_RIGHT] & 0x80){testDown=1;theKey = 2;
					if(g_cTimer.elapsed(time, 30)){
						GameRenderer.GetKey(2);
		
						if(g_cObjectManager.speedOn){
							g_cObjectManager.stepUp(g_cObjectManager.m_pPlayerObject->step);
							g_cObjectManager.speedDuration();
						}else if(b > 6)
							g_cObjectManager.stepUp(15);
						else
							g_cObjectManager.stepUp((float)(3 + b*2));
							b++;
							g_cObjectManager.MoveRight(6);
							GameRenderer.ProcessFrame();
							for(int i = 0; i < 256; i++)
								keystate[i] = 0;
							m_pKeyboard->GetDeviceState(sizeof(keystate), keystate);
					}
					ProcessKeyboardInput();
				}testDown=0;
				}break;
			}break;
		}g_cObjectManager.ItemSelect(theKey);break;
    //changed name to heroattack
				   	///For any keyboard key number go here:
	///http://msdn.microsoft.com/en-us/library/dd375731(v=VS.85).aspx
//need to be level 2 to use this spell
case DIK_1 :	if(!MenuUp){if(g_cObjectManager.LV >= 2){  g_pSoundManager->play(DEADCROW10_SOUND);g_cObjectManager.Fireball(theKey);} }break;
case DIK_2 :	if(!MenuUp){if(g_cObjectManager.LV >= 3){   g_pSoundManager->play(DEADCROW12_SOUND);g_cObjectManager.Lightning(theKey);} }break;
case DIK_3 :	if(!MenuUp){if(g_cObjectManager.LV >= 4){g_cObjectManager.Barrier(theKey);} }break;
case DIK_4 :	if(!MenuUp){if(g_cObjectManager.LV >= 5){g_cObjectManager.FanofFire();} }break;
case DIK_M :    g_cObjectManager.MenuYes(); break;
case DIK_V :    g_cObjectManager.useItem(currentItemSelect); break;
case DIK_S:     g_cObjectManager.saveGame(); break;

/*case DIK_F3: g_cObjectManager.useItem(0); break;
case DIK_F4: g_cObjectManager.useItem(1); break;
case DIK_F5: g_cObjectManager.useItem(2); break;
case DIK_F6: g_cObjectManager.useItem(3); break;
case DIK_F7: g_cObjectManager.useItem(4); break;*/
case DIK_RETURN:   g_cObjectManager.wait();dead = false; break;
//  }

    default: break;
  }
} //GameKeyboardHandler



