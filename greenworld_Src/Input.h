/// \file input.h
/// Interface for the input manager class CInputManager.
/// Copyright Ian Parberry, 2004
/// Last updated September 2, 2010

#pragma once

#define DIRECTINPUT_VERSION 0x0800 //put before #include dinput.h
#include <windows.h>
#include <dinput.h> //DirectInput

#include "defines.h" //essential defines


#define DI_BUFSIZE 16 ///< Buffer size for DirectInput events from devices

/// Input manager class.
/// Manages keyboard and mouse input using DirectInput.


class CInputManager{

  private:
    LPDIRECTINPUT8 m_lpDirectInput; ///< DirectInput object.
    LPDIRECTINPUTDEVICE8 m_pKeyboard; ///< Keyboard device.
    DIDEVICEOBJECTDATA m_DIod[DI_BUFSIZE]; ///< Input device buffer.
  unsigned char keystate[256];

	
    //private helper functions
    BOOL KeyboardHandler(DWORD keystroke); ///< Keyboard handler.
    void GameKeyboardHandler(DWORD keystroke); ///< Keyboard handler for game.
	

    //setup functions
    BOOL InitKeyboardInput(HWND hwnd); ///< Set up keyboard input.

  public:

    CInputManager(HINSTANCE hInstance, HWND hwnd); ///< Constructor.
    ~CInputManager(); ///< Destructor.
    BOOL ProcessKeyboardInput(); ///< Process buffered keyboard events.


};