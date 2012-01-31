// random.cpp
// random number generator

//Copyright Ian Parberry, 1998
//Last updated July 3, 1998

#include <windows.h>

#include "random.h"

CRandom::CRandom(){ //constructor
  sowseed(); //seed random number generator
}

void CRandom::sowseed(){ //seed random number generator
  srand(timeGetTime()); m_nCount=0;
}

int CRandom::number(int i,int j){  
  //return random number in  i..j
  if(++m_nCount>STALE_RAND)sowseed(); //reseed if stale
  int sample; //random sample
  //get a random sample
  if(j<0x7FFF)sample=rand(); //15-bit
  else sample=((int)rand()<<15)|(int)rand(); //30-bit
  //compute result
  return sample%(j-i+1)+i;
}
