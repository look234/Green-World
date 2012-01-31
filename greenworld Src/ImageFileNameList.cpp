/// \file imagefilenamelist.cpp 
/// Image file name class CImageFileNameList.
/// The image file name list reads the image file names
/// from an XML document and holds the names in an array.
/// Copyright Ian Parberry, 2004.
/// Last updated August 22, 2006.

#include "imagefilenamelist.h"

CImageFileNameList::CImageFileNameList(void){ //constructor
  m_lplpImageFileName = NULL;
  m_nImageFileCount = 0;
}

CImageFileNameList::~CImageFileNameList(void){ //destructor
  for(int i = 0; i < m_nImageFileCount; i++) //for each string
    delete [] m_lplpImageFileName[i]; //delete the string
  delete [] m_lplpImageFileName; //delete the array
}

/// \param ix index into name list
/// \return pointer to name string if valid index, 
///   pointer to "NotAValidFileName.bmp" otherwise

char* CImageFileNameList::operator[](const int ix){ //safe index into name list
  if(ix >= 0 && ix < m_nImageFileCount) //if it's safe
    return m_lplpImageFileName[ix]; //return indexed string
  else return "NotAValidFileName.bmp"; //else return a default string
}

/// Get image file names.
/// Loads image file names from tags in a TinyXML element.
/// \param settings TinyXML element containing settings tags

void CImageFileNameList::GetImageFileNames(TiXmlElement* settings){
  //image file names
  TiXmlElement* ist = settings->FirstChildElement("images"); 
  if(ist){
    TiXmlElement* img; //image element
    //count number of images
    m_nImageFileCount=0;
    for(img = ist->FirstChildElement("image"); img; img = img->NextSiblingElement("image"))
      ++m_nImageFileCount;
    //create file name array
    m_lplpImageFileName = new char*[m_nImageFileCount];
    //get image file names
    int i=0; //counter for number of names
    for(img = ist->FirstChildElement("image"); img; img = img->NextSiblingElement("image")){
      int len = strlen(img->Attribute("src")); //length of name string
      m_lplpImageFileName[i] = new char[len+1]; //create array space (+1 for NULL)
      strncpy_s(m_lplpImageFileName[i], len+1, img->Attribute("src"), len); //copy file name string
      m_lplpImageFileName[i][len]='\0'; //NULL at end of string
      i++; //next filename
    }
  }
}
