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
  3.2) Will become the property of the author.

*/

#pragma once

#define LOGF_NONE       0
#define LOGF_ENABLED    (1<<0)
#define LOGF_STRIPCODES (1<<1)

class CLogger :
  public CListener
{
private:
  int m_UpdateFrequency;  // how long to wait in seconds before the log buffer is flushed
  int m_SecondsSinceLastFlush;
  CBufferedFile m_LogFile;
  DWORD m_Flags; // see LOGF_* defines
  int *m_FilterList;

public:
  char *m_LogFormatStr; // UNUSED so far.
  char *m_LogFileName; // just the relative path+filename part. e.g. network\server_log.txt
  char *m_LogFolder; // just the path part, e.g. "c:\documents and settings\me\My Documents\Logs\IRC"  // no trailing '\'

  CLogger(void);
  ~CLogger(void);

  void Start( void );
  void Stop( void );
  void Flush( void );

  void UpdateSettings(DWORD Flags); // starts logging if LOGF_ENABLED was specified in the new flags
  inline int GetSettings( void ) { return m_Flags; }

  // To log: first call IsFiltered(), then call Log() if IsFiltered() returns FALSE
  inline BOOL IsFiltered(int Contents) { return ProcessSimpleFilter(m_FilterList,Contents); }
  BOOL Log( char *Data );

  BOOL NeedsFlushing( void ); // if returns true, call Flush() asap.  use with EV_TICK 
  inline BOOL IsEnabled( void ) { return (m_Flags & LOGF_ENABLED); }
  inline void SetFilterList(int *FilterList) { m_FilterList = FilterList; }
  void SetPaths(char *Folder, char *FileName); // NOTE: a copy of the strings is NOT taken, so make sure you allocate the strings so they can be free()'d
  char *GetFileName( void ); //returns a copy of the filename, dont' forget to free() it

  void OnEvent(int EventID, void *pData);
};
