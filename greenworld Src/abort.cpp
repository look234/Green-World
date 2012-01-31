/// \file abort.cpp
/// Code for aborting when something catastrophic goes wrong.
/// Allows an error message to be displayed in a dialog box (provided things 
/// don't go TOO wrong) last thing on exit by calling a printf style multi-parameter 
/// ABORT macro anyplace in the code.
/// Copyright Ian Parberry, 2004.
/// Last updated August 22, 2006.

#include "abort.h"

#include <stdio.h>
#include <windows.h>

// globals for handling abort error message reporting
char g_errMsg[1024]; ///< Error message if we had to abort.
BOOL g_errorExit=FALSE; ///< True if we had to abort.
extern HWND g_HwndApp; ///< Application window handle.

void reallyAbort(const char *fmt, ...){ //this is where we really abort
  //uses safe versions of string printf functions to avoid buffer overruns
  //format the error message into our buffer
  va_list ap;
  va_start(ap, fmt);
  _vsnprintf_s(g_errMsg, sizeof(g_errMsg), fmt, ap);
  va_end(ap);
  //flag the error so the app exits cleanly
  g_errorExit = TRUE; //flag that there's been an error
  DestroyWindow(g_HwndApp); //shut down the app
  ExitProcess(1); //stop whatever caused this abort
}