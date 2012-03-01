/// \file debug.h
/// Interface for the debug manager class CDebugManager.
/// Copyright Ian Parberry, 2004
/// Last updated August 11, 2004

#pragma once

//#define DEBUG_ON ///< Define this for debugging, comment out to turn off.

#ifdef DEBUG_ON //ensure debug code is not compiled unless needed

#include <windows.h>
#include <stdio.h>

#include "IPMgr.h"
#include "tinyxml.h"

#include "dxerr.h"

#define DEBUG_OUTBUF_SIZE 1024 ///< Size of debug output buffer.
#define DEBUG_FNAME_SIZE 256 ///< Size of debug file name.

/// The debug manager class. The debug manager allows you to send a debug string 
/// to a file, to the Visual Studio debugger, and/or to IP to be displayed by a client 
/// console app. It is designed to be accessed using the macro DEBUGPRINTF.

class CDebugManager{ //the debug manager

  private:
    FILE *m_fileHandle; ///< File handle for debug output file.
    char m_szOutBuffer[DEBUG_OUTBUF_SIZE]; ///< Output buffer.
    char m_szFileName[DEBUG_FNAME_SIZE]; ///< File that debug message comes from.
    int m_nLineNumber; ///< Line that debug message comes from.
    BOOL m_bOutputToFile; ///< TRUE for debug output to file.
    BOOL m_bOutputToDebugger; ///< TRUE for debug output to debugger.
    BOOL m_bOutputToIP; ///< TRUE for debug output to IP.
    BOOL m_bPrependFileInfo; ///< TRUE to output file name and line in debug messages
    BOOL m_bHeader; ///< TRUE to output header in debug messages
    char m_szDebugFileName[DEBUG_FNAME_SIZE]; ///< Output file for debug messages.
    char m_szDebugIPAddress[16]; ///< IP address to send debug messages to.
    int m_nDebugPort; ///< Debug port number for IP.
    IPManager* m_pIPManager; ///< IP manager.

  public:
    CDebugManager(); ///< Constructor.
    ~CDebugManager(); ///< Destructor.
    void open(); ///< Open output methods.
    void printf(const char* format,...); ///< Debug printf.
    void setsource(char* file,int line); ///< Set file and line number.
    void GetDebugSettings(TiXmlElement* xmlSettings); ///< Get names from XML element.

};
 
extern CDebugManager g_cDebugManager;

/// Real debug printf function.

/// Normally we will call this function using the DEBUGPRINTF macro.

void realDebugPrintf(const char *fmt, ...);

#endif

//debug printf
//this nasty little macro allows DEBUGPRINTF to take a variable number of arguments
  
#ifdef DEBUG_ON 
  #define DEBUGPRINTF (g_cDebugManager.setsource(__FILE__,__LINE__),realDebugPrintf)
#else
  #define DEBUGPRINTF //nothing
#endif //DEBUG_ON

#define DEBUGPRINTFD3D(x) DEBUGPRINTF("D3D Hresult %s: %s\n", DXGetErrorString(x), DXGetErrorDescription(x))
