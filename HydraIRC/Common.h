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

// Common.h : comment here
// 
// These routines are common to both IRCClient and IRCServer
// they mainly deal with display output and window control
// These will also be common to DCCChat and Private Message windows too.
/////////////////////////////////////////////////////////////////////////////

class IRCCommon
{
public:
  IRCUserGroup m_Users;
  time_t m_LastActivityTime;
  time_t m_CreateTime;

  int m_ObjectType; // see OTYPE_* defines
  // pointer to the child window instance
  // the IRCServer class is created BEFORE the window
  // the child window has a pointer that refers back to "this".
  CChildCommon *m_pChildWnd; 
  CSimpleArray<BufferItem *> m_OutputBuffer;

  // Logger!
  CLogger m_Logger;

  // Theme handle
  ThemeHandle *m_pTheme;


  // this is reset each time AddToOutputBuffer is called and incremented
  // each EV_TICK
  int m_TicksSinceLastBufferAdd; 
                                 
  void AddToOutputBuffer(int Contents, char *Buffer);
  void Printf(const int Contents, const char *format, ...);
  void ProcessOutputBuffer( void );
  void ClearOutputBuffer( void );
  void BufferTick( void );

  int GetDirtyStatus( void )
  {
    if (m_Dirty & DIRTY_RECENT)
    {
      time_t Now;
      time(&Now);

      int ResetSeconds = -1;
      switch (m_ObjectType)
      {
        case OTYPE_DCCCHAT:
          ResetSeconds = INTPREF(PREF_nServerListDCCChatRecentSeconds);
          break;
        case OTYPE_SERVER:
          ResetSeconds = INTPREF(PREF_nServerListServerRecentSeconds);
          break;
        case OTYPE_CHANNEL:
          ResetSeconds = INTPREF(PREF_nServerListChannelRecentSeconds);
          break;
        case OTYPE_QUERY:
          ResetSeconds = INTPREF(PREF_nServerListQueryRecentSeconds);
          break;
      }
      if ((ResetSeconds <= 0) || ((ResetSeconds > 0) && (Now - m_DirtyTime > ResetSeconds)) )
        m_Dirty &= ~DIRTY_RECENT; // remove the flag.
    }
    return m_Dirty; 
  }

  void SetDirtyStatus( int Status = DIRTY_TRUE ) { m_Dirty = Status; }
  void ClearDirtyFlag( void )  { m_Dirty = DIRTY_FALSE; }
  time_t GetDirtyTime( void ) { return m_DirtyTime; }
  
  void SetDirtyFilterList( int *FilterList ) { m_DirtyFilterList = FilterList; }  
  BOOL SetTheme(char *ThemeName);
  void ClearUserList( void );

  IRCCommon( void )
  {
    m_TicksSinceLastBufferAdd = 0;
    m_pChildWnd = NULL;
    m_Dirty = DIRTY_FALSE;
    m_DirtyFilterList = NULL; // don't filter..
    m_pTheme = g_ThemeManager.GetThemeHandle(STRINGPREF(PREF_sDefaultTheme));
    m_ObjectType = -1; // -1 == don't know yet
    time(&m_LastActivityTime);
    time(&m_CreateTime);
  }

  ~IRCCommon( void )
  {
    if (m_pTheme)
      g_ThemeManager.ReleaseThemeHandle(m_pTheme);
  }

private:
  int m_Dirty; //TODO: rename to m_DirtyStatus
  time_t m_DirtyTime; // record the that the buffer was made dirty
  int *m_DirtyFilterList; // )NULL) - OR - (FILTER_EXCLUDE | FILTER_INCLUDE, filternum, filternum,...,FILTER_END) - see BIC_* defines.
};
