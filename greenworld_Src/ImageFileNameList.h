/// \file imagefilenamelist.h 
/// Definition of the image file name class CImageFileNameList.
/// Copyright Ian Parberry, 2004.
/// Last updated August 11, 2004.

#pragma once

#include "tinyxml.h"

/// The image file name list class. Stores a list of image file names, with the
/// capability to import them from an XML document. Also provides a safe index 
/// operation. If an attempt is made to access a file name at an invalid index, 
/// the string NotAValidFileName.bmp is returned instead.

class CImageFileNameList{

private:
  char** m_lplpImageFileName; ///< Array of file name strings.
  int m_nImageFileCount; ///< Number of image file names stored.

public:
  CImageFileNameList(void); ///< Constructor.
  ~CImageFileNameList(void); ///< Destructor.
  void GetImageFileNames(TiXmlElement* xmlSettings); ///< Get names from XML element.
  char* operator[](const int); ///< Safe index into name list.

};
