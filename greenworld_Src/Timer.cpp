/// \file timer.cpp
/// Code for timer class CTimer.
/// Copyright Ian Parberry, 2004.
/// Last updated June 16, 2010.

#include "timer.h"

CTimer::CTimer(): m_nStartTime(0){ //constructor
}

void CTimer::start(){ //start the timer
	m_nStartTime = timeGetTime();
}

int CTimer::time(){ //return the time
	return timeGetTime() - m_nStartTime;
}

/// The elapsed function is a useful function for measuring repeating time intervals.
/// Given the start and duration times, this function returns TRUE if the interval
/// is over, and has the side-effect of resetting the start time when that happens,
/// thus setting things up for the next interval.
/// \param start start of time interval
/// \param interval duration of time interval
/// \return TRUE if time interval is over

int CTimer::elapsed(int &start,int interval){
	int current_time=time(); //current time

	if(current_time >= start + interval){ //if interval is over
		start = current_time; return TRUE; //reset the start and succeed
	}else return FALSE; //otherwise, fail
}

void CTimer::wait(){
 for (time_t t = time() + 50; time() < t; ) {}
}
