/*

  HydraIRC
  Copyright (C) 2002-2026 Dominic Clifton aka Hydra

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
