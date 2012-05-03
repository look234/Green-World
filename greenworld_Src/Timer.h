/// \file timer.h
/// Interface for the timer class CTimer.
/// Copyright Ian Parberry, 2004
/// Last updated August 12, 2004

#pragma once

#include <windows.h> //needed for BOOL

/// The timer class. The timer allows you to manage game events by duration, rather than
/// on a frame-by-frame basis. This simple version of the timer is based on
/// the Windows API function timeGetTime, which is notoriously inaccurate
/// but perfectly adequate for a simple game demo.

class CTimer{ //game timer class

	protected:
		int m_nStartTime; ///< Time that timer was started.

	public:
		CTimer(); ///< Constructor.
		virtual void start(); ///< Start the timer.
		virtual int time(); ///< Return the time in ms.
		virtual void wait();
		BOOL elapsed(int &start,int interval); ///< Has interval ms elapsed since start?
};

