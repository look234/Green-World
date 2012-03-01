/// \file sound.h
/// Interface for the base sprite class C3DSprite.
/// Copyright Ian Parberry, 2004
/// Last updated September 30, 2004

#pragma once

#include <dsound.h> //direct sound
#include "sndlist.h" //list of sound names

/// The sound manager class. The sound manager allows you to play multiple 
/// overlapping copies of sounds simultaneously.  It reads settings from the
/// XML settings file, including a list of file names to be loaded. It can load
/// WAV format sounds. The code is currently written so that all sound files
/// must have the same bit depth and sample rate. Multiple copies of sounds are
/// made by sharing the sound data, to save memory.

class CSoundManager{ //sound manager

  private:
    int m_nCount; ///< Number of sounds loaded/
    LPDIRECTSOUND8 m_lpDirectSound; ///< DirectSound object.
    LPLPDIRECTSOUNDBUFFER* m_lpBuffer; ///< Sound buffers.
    int* m_nCopyCount; ///< Number of copies of each sound.
    BOOL* m_bStartedThisFrame; ///< TRUE if sound started in this frame.
    BOOL m_bOperational; ///< TRUE if DirectSound initialized correctly.
    int m_nChannelsPerSample; ///< Number of channels per sample.
    int m_nBitsPerSample; ///< Number of bits per sample.
    int m_nSampleRate; ///< Sample rate in Hertz.
    int m_nMaxSounds; ///< Maximum number of sounds allowed.

    BOOL CreateBuffer(int index, int length, int copies); ///< Create a sound buffer.
    BOOL LoadBuffer(int index, BYTE *buffer, int length);///< Load a sound buffer.
    int LoadSound(char *filename, BYTE **sound); ///< Load a sound from file.
    BOOL CopyBuffer(int index, int copies); ///< Make multiple copies of sound

  public:
    CSoundManager(HWND hwnd); ///< Constructor.
    ~CSoundManager(); ///< Destructor.
    void LoadSettings(); ///< Load settings from XML document.
    void LoadSounds(int level); ///< Load sounds from list in XML document.
    void beginframe(void); ///< To be called at start of frame.
    void clear(); ///< Clear all sounds from buffers.
    void load(char *filename, int copies=1); ///< Load sound from file.
    void play(int index, BOOL looping=FALSE); ///< Play sound.
    void stop(int index); ///< Stop playing sound.
    void stop(void); ///< Stop playing all sounds.

};
