/// \file sound.cpp
/// Code for the sound manager class CSoundManager.
/// Copyright Ian Parberry, 2004.
/// Last updated August 22, 2006.

#include <stdio.h>

#include "sound.h"
#include "tinyxml.h"

extern TiXmlElement* g_xmlSettings;

/// CSoundManager constructor.
/// Sets member variables to sensible values and starts DirectSound. Member variable
/// m_bOperational is set to TRUE is it is able to start DirectSound and set the 
/// cooperative level correctly.

CSoundManager::CSoundManager(HWND hwnd){ //constructor

  //default values for sound settings, changed later from XML settings file
  m_nChannelsPerSample = 1;
  m_nBitsPerSample = 8;
  m_nSampleRate = 22050;

  m_nCount = 0; //no sounds yet
  m_lpBuffer = NULL; //no sound buffers yet

  //start DirectSound
  m_bOperational = SUCCEEDED(DirectSoundCreate8(NULL, &m_lpDirectSound, NULL));
  if(!m_bOperational)return; //bail if we failed

  //set the cooperative level
  m_bOperational = SUCCEEDED(m_lpDirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY));
}

/// CSoundManager destructor.
/// Reclaims all dynamic memory and releases DirectSound.

CSoundManager::~CSoundManager(){ //destructor
  if(!m_bOperational)return;
  clear(); //clear all buffers
  if(m_lpDirectSound) //safe release
    (void)m_lpDirectSound->Release(); //release direct sound
}


/// Start of new animation frame.
/// Records the start of a new animation frame by resetting the array m_bStartedThisFrame.
/// If two copies of the same sound start at exactly the same time (meaning
/// in the same frame of animation), then the two sounds reinforce each other.
/// That is, the sound simply plays twice as loud. m_bStartedThisFrame is used to
/// prevent this from happening. If a sound has already been started,
/// it cannot be started again until this function has been called.

void CSoundManager::beginframe(){ //to be called at start of frame
  for(int i=0; i<m_nCount; i++)
    m_bStartedThisFrame[i] = FALSE;
}

/// Clear the sound manager.
/// Returns the sound manager to a pristine state by releasing and deleting all 
/// sound buffers, then deleting all dynamic arrays.

void CSoundManager::clear(){ //clear all sounds

  if(!m_bOperational)return; //bail if not initialized

  stop(); //stop all sounds (paranoia)

  for(int i=0; i<m_nCount; i++){ //for each sound
    for(int j=0; j<m_nCopyCount[i]; j++){ //for each copy
      m_lpBuffer[i][j]->Release(); //release the sound
      m_lpBuffer[i][j] = NULL; //probably not needed
    }

    //reclaim memory
    delete [] m_lpBuffer[i];
  }

  //delete arrays
  delete [] m_lpBuffer;
  delete [] m_nCopyCount;
  delete [] m_bStartedThisFrame;

  m_nCount = 0; //no sounds left (hopefully)
}

/// Load a sound from a file into a sound buffer.
/// Loads a sound from a file into a temporary buffer. Creates a DirectSound
/// sound buffer of the right size and copies it over. Creates a number of copies
/// of the sound so that we can play overlapping copies of the sound.
/// \param filename name of file to be loaded
/// \param copies number of copies of sound that can be played simultaneously

void CSoundManager::load(char *filename, int copies){

  int length; //length of sound
  BYTE *sound=NULL; //temporary buffer to hold sound data

  //bail out if necessary
  if(!m_bOperational)return; //bail if not initialized
  if(m_nCount >= m_nMaxSounds)return; //bail if no space left

  //load sound data from file
  length = LoadSound(filename, &sound); //load sound from file
  CreateBuffer(m_nCount, length, copies); //create buffers
  LoadBuffer(m_nCount, sound, length); //load into buffer
  CopyBuffer(m_nCount, copies); //make copies of contents

  //clean up and exit
  delete [] sound; //delete temporary sound buffer
  m_bStartedThisFrame[m_nCount] = FALSE; //not started yet
  m_nCount++; //increment counter
}

/// Play a sound.
/// If the sound has multiple copies, locates a copy that is not currently playing,
/// and plays it. If there is no copy available, nothing happens.
/// \param index index of sound to be played
/// \param looping TRUE if sound is to be looped

void CSoundManager::play(int index,BOOL looping){ //play sound

  if(!m_bOperational)return; //bail if not initialized
  if(index<0||index>=m_nCount)return; //bail if bad index
  if(m_bStartedThisFrame[index])return; //bail if already playing

  int copy=0; //current copy
  DWORD status; //status of that copy

  //get status of first copy
  if(FAILED(m_lpBuffer[index][copy]->GetStatus(&status)))
    status = DSBSTATUS_PLAYING; //assume playing if failed

  //find next unplayed copy, if any
  while(copy < m_nCopyCount[index]&&
  (status&DSBSTATUS_PLAYING)){ //while current copy in use
    if(++copy < m_nCopyCount[index]) //go to next copy
      if(FAILED(m_lpBuffer[index][copy]->GetStatus(&status)))
        status = DSBSTATUS_PLAYING; //assume playing if failed
  }

  //play copy
  if(copy < m_nCopyCount[index]){ //if unused copy found
    m_lpBuffer[index][copy]->
      Play(0, 0, looping?DSBPLAY_LOOPING:0); //play it
    m_bStartedThisFrame[index] = TRUE; //record fact that we started it
  }
}

/// Stop sound.
/// Stops all copies of a sound from playing, and resets them back to the
/// start of the sound so that they start properly next time they are played.
///\param index index of sound to stop

void CSoundManager::stop(int index){ //stop playing sound

  if(!m_bOperational)return; //bail if not initialized 
  if(index<0||index>=m_nCount)return; //bail if bad index

  for(int i=0; i<m_nCopyCount[index]; i++){ //for each copy
    m_lpBuffer[index][i]->Stop(); //stop playing
    m_lpBuffer[index][i]->SetCurrentPosition(0); //rewind
  }
}

/// Stop all sounds.
/// Stops all sounds from playing. It simply loops through all sounds and calls the
/// other stop function to do all the work.

void CSoundManager::stop(void){ //stop playing sound
  if(!m_bOperational)return; //bail if not initialized 
  for(int index=0; index<m_nCount; index++) //for each sound
    stop(index); //stop that sound
}

/// Copy buffer.
/// Duplicates a sound in a DirectSound buffer so that a sound can be played multiple
/// times, overlapping. The sound data is not duplicated, but shared between the copies
/// to save memory.
/// \param index index of sound to copy
/// \param copies number of copies to make
/// \return TRUE if the duplication succeeds

BOOL CSoundManager::CopyBuffer(int index,int copies){ 

  BOOL result=TRUE; //TRUE if everything went OK

  if(!m_bOperational)return FALSE; //bail if not initialized

  m_nCopyCount[index]=copies; //record number of copies

  for(int i=1; i<copies; i++) //for each copy
    result=result&& SUCCEEDED( //copy the sound
        m_lpDirectSound->DuplicateSoundBuffer(*m_lpBuffer[index],&(m_lpBuffer[index][i]))
    );

  return result;
}

/// Create buffer.
/// Create a single DirectSound buffer for the first copy of a sound. NULL out the
/// buffer pointers for the copies.
/// \param index index in the sound buffer array
/// \param length length of the sound buffer
/// \param copies number of copies of sound
/// \return TRUE if it succeeds

BOOL CSoundManager::CreateBuffer(int index, int length, int copies){

  if(!m_bOperational)return FALSE; //bail if not initialized
  if(length<=0)return FALSE; //bail if length info wrong

  DSBUFFERDESC dsbdesc;
  PCMWAVEFORMAT pcmwf;

  //init pcmwf, wave format descriptor
  memset(&pcmwf,0,sizeof(PCMWAVEFORMAT));
  pcmwf.wf.wFormatTag = WAVE_FORMAT_PCM;
  pcmwf.wf.nChannels = m_nChannelsPerSample;
  pcmwf.wf.nSamplesPerSec = m_nSampleRate;
  pcmwf.wf.nBlockAlign = m_nChannelsPerSample * m_nBitsPerSample / 8;
  pcmwf.wf.nAvgBytesPerSec = pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign;
  pcmwf.wBitsPerSample = m_nBitsPerSample;

  //init dsbdesc, direct sound buffer descriptor
  memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
  dsbdesc.dwSize = sizeof(DSBUFFERDESC);
  dsbdesc.dwFlags = DSBCAPS_STATIC;
  dsbdesc.dwBufferBytes = length;
  dsbdesc.lpwfxFormat = (LPWAVEFORMATEX)&pcmwf;

  //create sound buffers and return success code
  m_lpBuffer[index] = new LPDIRECTSOUNDBUFFER[copies];
  for(int i=0; i<copies; i++)
    m_lpBuffer[index][i]=NULL;
  return SUCCEEDED(m_lpDirectSound->
    CreateSoundBuffer(&dsbdesc, m_lpBuffer[index], NULL));
}

/// Load sound buffer.
/// Copies a sound from a byte buffer into a DirectSound buffer. The hairy part
/// is that when you lock down a DirectSound buffer, you get the buffer in two
/// halves. This function copies the correct two parts of the byte buffer into
/// the DirectSound buffer.
/// \param index index in the sound buffer array
/// \param sound pointer to byte buffer containing sound data
/// \param length length of the byte buffer
/// \return TRUE if it succeeds

BOOL CSoundManager::LoadBuffer(int index, BYTE *sound, int length){

  if(!m_bOperational)return FALSE; //bail if not initialized
  if(length<=0)return FALSE; //bail if length info wrong

  LPVOID w1,w2; //write pointer (use 2 for buffer wraparound)
  DWORD l1,l2; //length of sound to be written to write pointers
  LPDIRECTSOUNDBUFFER buffer=*m_lpBuffer[index];

  //lock down buffer
  if(buffer->Lock(0, length, &w1, &l1, &w2, &l2, 0) == DSERR_BUFFERLOST){ //if buffer lost
    buffer->Restore(); //restore, then try again
    if(FAILED(buffer->Lock(0, length, &w1, &l1, &w2, &l2, 0)))
      return FALSE; //abort if failed the second time
  }

  //load both halves of sound to buffer
  CopyMemory(w1,sound,l1); //load first half 
  if(w2!=NULL)CopyMemory(w2, sound+l1, l2); //load second half

  //unlock the buffer and return
  if(FAILED(buffer->Unlock(w1, l1, w2, l2)))return FALSE;
  return TRUE;

}

/// Load a sound.
/// Creates a byte buffer and loads sound data from a file into that buffer.
/// Cribbed from the SDK sample files.
/// \param filename name of sound file
/// \param buffer pointer to a byte buffer pointer
/// \return the length of the byte buffer created

int CSoundManager::LoadSound(char *filename, BYTE **buffer){

  if(!m_bOperational)return 0; //bail if not initialized

  int size=0;
  HMMIO hmfr;
  MMCKINFO parent,child;
  WAVEFORMATEX wfmtx;

  //reclaim memory from buffer, if already used
  delete [] *buffer;

  //init parent and child MMCKINFOs
  parent.ckid = (FOURCC)0;
  parent.cksize = 0;
  parent.fccType = (FOURCC)0;
  parent.dwDataOffset = 0;
  parent.dwFlags = 0;
  child = parent;

  //open sound file
  hmfr=mmioOpen(filename, NULL, MMIO_READ|MMIO_ALLOCBUF);
  if(hmfr==NULL)return NULL;

  //descend into the RIFF
  parent.fccType=mmioFOURCC('W','A','V','E');
  if(mmioDescend(hmfr, &parent, NULL, MMIO_FINDRIFF)){
    mmioClose(hmfr, 0); return NULL; //not a wave file
  }

  //descend to the WAVEfmt
  child.ckid=mmioFOURCC('f', 'm', 't', ' ');
  if(mmioDescend(hmfr, &child, &parent, 0)){
    mmioClose(hmfr, 0); return NULL; //file has no fmt chunk
  }

  //read the WAVEFMT from the wave file
  if(mmioRead(hmfr, (char*)&wfmtx, sizeof(wfmtx)) != sizeof(wfmtx)){
    mmioClose(hmfr, 0); return NULL; //unable to read fmt chunk
  }

  //check wave format
  if(wfmtx.wFormatTag != WAVE_FORMAT_PCM){
    mmioClose(hmfr, 0); return NULL; //WAVE file is not PCM format
  }

  //ascend back to RIFF level
  if(mmioAscend(hmfr, &child, 0)){
    mmioClose(hmfr, 0); return NULL; //unable to ascend
  }

  //descend to the data chunk
  child.ckid=mmioFOURCC('d', 'a', 't', 'a');
  if(mmioDescend(hmfr, &child, &parent, MMIO_FINDCHUNK)){
    mmioClose(hmfr, 0); return NULL; //WAVE file has no data chunk
  }

  //grab memory to store sound
  size = child.cksize;
  *buffer = new BYTE[size];
  if(*buffer == NULL){
    mmioClose(hmfr, 0); return NULL; //out of memory
  }

  //read the wave data
  if((int)mmioRead(hmfr, (char *)*buffer, size) != size){
    //data read failed
    mmioClose(hmfr, 0); delete [] *buffer; return NULL; 
  }

  //close file and return
  mmioClose(hmfr, 0);
  return size;
}

/// Load sound settings.
/// Load the basic sound settings from g_xmlSettings. Looks for a <sound>
/// tag with attributes cps, bps, and rate.

void CSoundManager::LoadSettings(){

  if(g_xmlSettings){ //got "settings" tag

    //get "sound" tag
    TiXmlElement* sndSettings = 
      g_xmlSettings->FirstChildElement("sound"); //sound tag
    
    //get settings
    sndSettings->Attribute("cps", &m_nChannelsPerSample);
    sndSettings->Attribute("bps", &m_nBitsPerSample);
    sndSettings->Attribute("rate", &m_nSampleRate);
  }
}

/// Load sounds.
/// Loads the sound files for a given level from the file list in g_xmlSettings.
/// Searches for a <sounds> tag with a "level" attribute equal to the parameter.
/// Processes sound file names in <sound> tags within the <sounds></sounds> pair.
/// Starts by counting the number of sound files needed, and creating arrays of
/// the right size.
/// \param level sounds are to be loaded for this level

void CSoundManager::LoadSounds(int level){

  if(g_xmlSettings){ //got "settings" tag

    //get "sound" tag
    TiXmlElement* sndSettings = 
      g_xmlSettings->FirstChildElement("sound"); //sound tag

    //find <sounds> tag with correct "level" attribute
    BOOL found = FALSE; //TRUE when tag found
    TiXmlElement* snd = 
      g_xmlSettings->FirstChildElement("sounds"); //first <sounds> tag
    while(snd && !found){ //while not finished and not found
      int lev; //for level attribute from tag
      snd->Attribute("level", &lev); //get level attribute
      found = lev==level; //is it the one we're looking for?
      if(!found)snd = snd->NextSiblingElement("sounds"); //keep going if not
    }

    //count number of sounds in list, and create arrays of the right size
    m_nMaxSounds = 0; //counter
    TiXmlElement* s; //<sound> tag element
    if(found){ //<sounds> tag was found, so go ahead and count
      for(s=snd->FirstChildElement("sound"); s; s=s->NextSiblingElement("sound"))
        ++m_nMaxSounds;
      //create arrays and initialize
      m_lpBuffer = new LPLPDIRECTSOUNDBUFFER[m_nMaxSounds];
      m_nCopyCount = new int[m_nMaxSounds];
      m_bStartedThisFrame = new BOOL[m_nMaxSounds];
      for(int i=0; i<m_nMaxSounds; i++){
        m_lpBuffer[i] = NULL;
        m_nCopyCount[i] = 0;
        m_bStartedThisFrame[i] = FALSE;
      }
    }

    //load sounds from sound list
    if(found){
      int copies=0; //number of copies to be made of sound
      const int bufsize = 64;
      char buffer[bufsize]; //buffer for file name
      for(s = snd->FirstChildElement("sound"); s; s = s->NextSiblingElement("sound")){
        strncpy_s(buffer, bufsize, s->Attribute("file"), sizeof(buffer)); //get file name
        s->Attribute("copies", &copies); //get number of copies
        load(buffer, copies); //load the file //next <sound> tag
      }
    }
  }
}

