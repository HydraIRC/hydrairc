/*

  HydraIRC
  Copyright (C) 2002-2006 Dominic Clifton aka Hydra

  HydraIRC limited-use source license

  1) You can:
  1.1) Use the source to create improvements and bug-fixes to send to the
       author to be incorporated in the main program.
  1.2) Use it for review/educational purposes.

  2) You can NOT:
  2.1) Use the source to create derivative works. (That is, you can't release
       your own version of HydraIRC with your changes in it)
  2.2) Compile your own version and sell it.
  2.3) Distribute unmodified, modified source or compiled versions of HydraIRC
       without first obtaining permission from the author. (I want one place
       for people to come to get HydraIRC from)
  2.4) Use any of the code or other part of HydraIRC in anything other than 
       HydraIRC.
       
  3) All code submitted to the project:
  3.1) Must not be covered by any license that conflicts with this license 
       (e.g. GPL code)
  3.2) Will become the property of HydraIRC's author.

*/

#define VERSIONNUM "v0.3.169"
#define VERSIONDATE "(9/October/2023)"

#ifdef RELEASE_VERSION
#define VERSIONSTRING_EXTRA_1 ""
#else
#define VERSIONSTRING_EXTRA_1 "-Test"
#endif

#ifdef USE_XPCOMMANDBAR
#define VERSIONSTRING_EXTRA_2 "-XP"
#else
#define VERSIONSTRING_EXTRA_2 ""
#endif

#ifdef DEBUG
#define VERSIONSTRING_EXTRA_3 "-Debug"
#else
#define VERSIONSTRING_EXTRA_3 ""
#endif

#define VERSIONSTRING VERSIONNUM VERSIONSTRING_EXTRA_1 VERSIONSTRING_EXTRA_2 VERSIONSTRING_EXTRA_3 " " VERSIONDATE

/*
#ifndef RELEASE_VERSION
#ifndef USE_XPCOMMANDBAR
#define VERSIONSTRING VERSIONNUM "-Test " VERSIONDATE
#else
#define VERSIONSTRING VERSIONNUM "-Test-XP " VERSIONDATE
#endif
#else
#ifndef USE_XPCOMMANDBAR
#define VERSIONSTRING VERSIONNUM " " VERSIONDATE
#else
#define VERSIONSTRING VERSIONNUM "-XP " VERSIONDATE
#endif
#endif
*/

#define AUTHORSTRING "Dominic Clifton aka Hydra"
