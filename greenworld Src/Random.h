// random.h
// header for random number generator

//Copyright Ian Parberry, 1999
//Last updated September 29, 1999

#ifndef __RANDOM__
#define __RANDOM__

#define STALE_RAND 1000 //stale after this many calls

class CRandom{
  private:
    int m_nCount; //count of number of times used
  public:
    CRandom(); //constructor
    int number(int i,int j); //return random number in i..j
    void sowseed(); //seed the random number generator
};

#endif
