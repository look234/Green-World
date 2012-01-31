/// \file debug.cpp
/// Code for debug manager class CDebugManager.
/// Copyright Ian Parberry, 2004.
/// Last updated June 15, 2010.

#include <string.h>
#include "debug.h"

#ifdef DEBUG_ON //ensure debug code is not compiled unless needed

CDebugManager::CDebugManager(){ //constructor

  //default settings
  strncpy_s(m_szDebugFileName, "debug.txt", DEBUG_FNAME_SIZE); //file name
  strncpy_s(m_szDebugIPAddress, "127.0.0.1", sizeof(m_szDebugIPAddress)); //localhost
  m_nDebugPort = 1617; //debug port
  m_bOutputToFile = TRUE; //default output to file
  m_bOutputToDebugger = TRUE; //default output to debugger
  m_bOutputToIP = TRUE; //default output to IP
  m_bPrependFileInfo = TRUE; //output file name and line
  m_bHeader = TRUE; //output header
}

CDebugManager::~CDebugManager(){ //destructor
  
  //send out announcement of start of debug
  if(m_bHeader){
    BOOL temp = m_bPrependFileInfo;
    m_bPrependFileInfo = FALSE;
    printf("**** Exiting debugger\n\n");
    m_bPrependFileInfo = temp;
  }

  //close down
  if(m_bOutputToFile)
    fclose(m_fileHandle); //close output file
  delete m_pIPManager;
}

void CDebugManager::open(){
  //open output file
  if(m_bOutputToFile)
     fopen_s( &m_fileHandle, m_szDebugFileName, "wt" ); //open output file

  //init IP manager
  m_pIPManager = new IPManager(m_szDebugIPAddress, m_nDebugPort);

  //send out announcement of start of debug
  if(m_bHeader){
    BOOL temp = m_bPrependFileInfo;
    m_bPrependFileInfo = FALSE;
    printf("**** Starting debugger\n");
    m_bPrependFileInfo = temp;
  }
}

/// \param file code file name that error came from
/// \param line code line number that error came from

void CDebugManager::setsource(char* file,int line){ //set file name and line number
  char* p=strrchr(file,'\\');
  if(p==NULL)strcpy_s(m_szFileName,file); //set file name
  else strcpy_s(m_szFileName,(char*)(p+1));
  m_nLineNumber=line; //set line number
}

/// Printf function.
/// \param format printf style format string

void CDebugManager::printf(const char* format,...){ //debug printf

  //print file and line number to m_szOutBuffer
  if(m_bPrependFileInfo)
    sprintf_s(m_szOutBuffer,"%s(%d): ",m_szFileName,m_nLineNumber);
  else m_szOutBuffer[0] = '\0';

  //append debug message from parameter list
  va_list arglist;
  va_start(arglist,format);
  _vsnprintf_s((char*)(m_szOutBuffer+strlen(m_szOutBuffer)), sizeof(m_szOutBuffer),
    sizeof(m_szOutBuffer)-strlen(m_szOutBuffer), format, arglist);
  va_end(arglist);

  //print m_szOutBuffer to file if needed
  if(m_bOutputToFile){
    fprintf(m_fileHandle,"%s",m_szOutBuffer);
    fflush(m_fileHandle);
  }

  //print m_szOutBuffer to debugger if needed
  if(m_bOutputToDebugger)
    OutputDebugString(m_szOutBuffer);

  //print m_szOutBuffer to IP if needed
  if(m_bOutputToIP)
    m_pIPManager->SendPacket(m_szOutBuffer,strlen(m_szOutBuffer));
}

/// Get debug settings from XML setting document.
/// \param settings TinyXML element containing settings tags

void CDebugManager::GetDebugSettings(TiXmlElement* settings){
  TiXmlElement* ist = settings->FirstChildElement("debug"); 
  if(ist){
    ist->Attribute("linenumber",&m_bPrependFileInfo);
    ist->Attribute("header",&m_bHeader);
    TiXmlElement* tag = ist->FirstChildElement("file");
    if(tag){
      tag->Attribute("select",&m_bOutputToFile);
      strncpy_s(m_szDebugFileName, tag->Attribute("name"), sizeof(m_szDebugFileName));
    }
    tag = ist->FirstChildElement("debugger");
    if(tag){
      tag->Attribute("select",&m_bOutputToDebugger);
    }
    tag = ist->FirstChildElement("ip");
    if(tag){
      tag->Attribute("select",&m_bOutputToIP);
      strncpy_s(m_szDebugIPAddress, tag->Attribute("address"), sizeof(m_szDebugIPAddress));
      tag->Attribute("port",&m_nDebugPort);
    }
  }
}

extern CDebugManager g_cDebugManager;

/// The real debug printf function.
/// \param fmt printf style format string

void realDebugPrintf(const char *fmt, ...){
  static char buffer[1024];
  va_list ap;
  va_start(ap,fmt);
  _vsnprintf_s(buffer,sizeof(buffer),fmt,ap);
  g_cDebugManager.printf("%s",buffer);
  va_end(ap);
}

#endif
