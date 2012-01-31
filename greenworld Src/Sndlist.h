/// \file sndlist.h
/// Enumerated types for sounds.
/// Copyright Ian Parberry, 2004
/// Last updated September 30, 2004

#pragma once

/// Game sound enumerated type. 
/// These are the sounds used in actual gameplay. Sounds must be listed here in
/// the same order that they are in the sound settings XML file.

enum GameSoundType{ //sounds used in game engine

  CAW_SOUND, //sound a crow makes
  GUN_SOUND, //sound of gun firing
  BOOM_SOUND, //sound of explosion
  HIT_SOUND,
  FIRE_SOUND,
  DEADCROW_SOUND,
  DEADCROW2_SOUND,
  DEADCROW3_SOUND,
   DEADCROW4_SOUND,
    DEADCROW5_SOUND,
	 DEADCROW6_SOUND,
	  DEADCROW7_SOUND,
	   DEADCROW8_SOUND,
	     DEADCROW9_SOUND,
	 DEADCROW10_SOUND,
	  DEADCROW11_SOUND,
	   DEADCROW12_SOUND,
	     DEADCROW13_SOUND,
	 DEADCROW14_SOUND,
  THUMP_SOUND, //sound of object hitting the ground

  //engine sounds must be consecutive and in this order
  PUTT0_SOUND,
  PUTT1_SOUND,
  PUTT2_SOUND,
  PUTT3_SOUND,
  PUTT4_SOUND,
  PUTT5_SOUND,
  PUTT6_SOUND,
  PUTT7_SOUND,
  PUTT8_SOUND,
  PUTT9_SOUND,
};

/// Intro sound enumerated type. 
/// These are the sounds used in the intro sequence and menus. Sounds must 
/// be listed here in  the same order that they are in the sound settings XML file.

enum IntroSoundType{ //sounds used during the intro
  TITLE_SOUND, //sound used during title screen
  LOGO_SOUND, //signature chord
  CREDITS_SOUND, //credits sound
};
