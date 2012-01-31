/// \file abort.h
/// Code for the ABORT macro.
/// Copyright Ian Parberry, 2004
/// Last updated August 11, 2004

#pragma once

/// Terminate the program with a printf-like formatted error message. 

/// Normally we will call this function using the ABORT macro.

void reallyAbort(const char *fmt, ...);

/// ABORT macro.

/// This nasty looking macro uses a handy little trick to allow it to
/// appear to to take a variable number of arguments

#define ABORT (reallyAbort)
