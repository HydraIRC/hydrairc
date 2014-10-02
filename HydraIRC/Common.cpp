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

// Common.cpp : comment here
//
// These routines are common to both IRCClient and IRCServer
// they mainly deal with text buffering
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

// forward declare
CChildCommon *FindChildWindow(HWND hWnd);

void IRCCommon::ProcessOutputBuffer( void )
{
#ifdef VERBOSE_DEBUG
  sys_printf(BIC_FUNCTION,"IRCCommon::ProcessOutputBuffer()\n");
#endif
  // process the buffer if we have text in it
  // but keep the buffer for later if the window's not open yet.

  if (m_pChildWnd)
  {
    CChildFrame *pChildFrame = CHILDFRAMEPTR(m_pChildWnd);
    
    switch (m_ObjectType)
    {
      case OTYPE_QUERY:
      case OTYPE_CHANNEL:
      case OTYPE_SERVER:
      case OTYPE_DCCCHAT:

        // there are several ways of handling this:
        // 1) process 1 line for 1 message.
        //    NOTE: this kinda works, but because we can add lines to
        //    the buffer before the window exists we can end up with
        //    lines in the buffer that don't get processed immediately.
        // 2) process entire buffer when we get a message
        //    (we'll get several messages that we don't do anything with though..)
        //    NOTE: This works fine, except that user input is delayed while we process
        //    the buffer, which can take a while if we have alot to process.
        // 3) process some of the buffer when we get a message
        //    and post another message if there's some left.
        // 4) process 1 line for 1 message
        //    post another message if there's some left.

        //
        // Tried method 2, see notes, tried 1, see notes, going with 4..

        // Method 4
        // Process, a single line of the buffer
        // Then send another message if any remaining buffer

        // sometimes we have already processed the buffer
        // by the time we process the message.
        // as ProcessBuffer() is also called just after creation.
        if (!(m_OutputBuffer.GetSize() > 0))
          break;

        BufferItem *pBI = m_OutputBuffer[0];

        pChildFrame->m_MsgView.Put(pBI->m_Contents, pBI->m_Buffer,&pBI->m_Time);    

        // Slap it over to the channel monitor too..
        if (m_ObjectType == CWTYPE_CHANNEL)
        {
          if (g_pMainWnd->m_ChannelMonitorView.IsMonitoring(pChildFrame->m_pChannel))
          {
            g_pMainWnd->m_ChannelMonitorView.Put(pChildFrame->m_pChannel,pBI);
          }
        }

        // Slap it over to the server monitor too..
        if (m_ObjectType == CWTYPE_SERVER)
        {
          if (g_pMainWnd->m_ServerMonitorView.IsMonitoring(pChildFrame->m_pServer))
          {
            g_pMainWnd->m_ServerMonitorView.Put(pChildFrame->m_pServer,pBI);
          }
        }

        m_OutputBuffer.RemoveAt(0);
        delete pBI;

        // more waiting ?
        if (m_OutputBuffer.GetSize() > 0)
        {
          m_pChildWnd->PostMessage(WM_BUFFERWAITING,0,0);
        } 

        // Method 2
        /*
        while (m_OutputBuffer.GetSize() > 0)
        {
          // Process, display and then free each buffer
          BufferItem *pBI = m_OutputBuffer[0];

          pChildFrame->m_MsgView.Put(pBI->m_Contents, pBI->m_Buffer);    

          // Slap it over to the channel monitor too..
          if (m_ObjectType == CWTYPE_CHANNEL)
          {
            if (g_pMainWnd->m_ChannelMonitorView.IsMonitoring(pChildFrame->m_pChannel))
            {
              g_pMainWnd->m_ChannelMonitorView.Put(pChildFrame->m_pChannel,pBI);
            }
          }

          m_OutputBuffer.RemoveAt(0);
          delete pBI;
        }
        */
        break;
    }
  }
#ifdef VERBOSE_DEBUG
  sys_printf(BIC_FUNCTION,"Exiting IRCCommon::ProcessOutputBuffer()\n");
#endif
}

// note, DO NOT FREE Buffer after calling,  Buffer is freed after it is processed.
void IRCCommon::AddToOutputBuffer(int Contents, char *Buffer)
{  
  time(&m_LastActivityTime);

  if (m_pChildWnd && !::IsWindow(m_pChildWnd->m_hWnd))
    return;
  // Set the dirty flag is the window is NOT active, (or the window is active, but hidden)

  if (!ProcessSimpleFilter(m_DirtyFilterList,Contents))
  {
    time(&m_DirtyTime); // update the time of the last thing that made the window dirty
    if (!(m_Dirty & DIRTY_RECENT))
    {      
      m_Dirty |= DIRTY_RECENT;// if we didn't filter it, then set the DIRTY_RECENT flag.

      if (!(m_Dirty & DIRTY_TRUE)) // speed things up a bit!
      {

        HWND hWnd = g_pMainWnd->MDIGetActive();
        if (m_pChildWnd && 
            ((m_pChildWnd->m_hWnd != hWnd) || 
            (m_pChildWnd->m_hWnd == hWnd && !m_pChildWnd->IsWindowVisible() )) )
        {
          m_Dirty |= DIRTY_TRUE; // if we didn't filter it, then set the DIRTY_TRUE flag.
        }
      }
    }
  }

  m_TicksSinceLastBufferAdd = 0;
  m_OutputBuffer.Add(new BufferItem(Contents,Buffer));
  if (m_pChildWnd)
    m_pChildWnd->PostMessage(WM_BUFFERWAITING,0,0);
}

#define IRCCOMMON_PRINTF_BUFSIZE 1024
/** Output some text to the output view
 *
 *  NOTE: you must only send one line of text with each Put()
 *  the function handles it ok, but we need to count the lines
 *  of text in the buffer somehow...
 *  ad we need to know what each line contains, so it can be filtered.
 */
void IRCCommon::Printf(const int Contents, const char *format, ...)
{
  if (!format)
    return;

	va_list args;

	char *buf;

  buf = (char *)malloc(IRCCOMMON_PRINTF_BUFSIZE);
  if (buf)
  {
	  ZeroMemory(buf, IRCCOMMON_PRINTF_BUFSIZE);
	  va_start(args, format);
	  _vsnprintf(buf, IRCCOMMON_PRINTF_BUFSIZE, format, args);
	  va_end(args);
    buf[IRCCOMMON_PRINTF_BUFSIZE-1] = 0;

    AddToOutputBuffer(Contents, buf);
    // Note: buf is freed automatically later
  }
}

void IRCCommon::ClearOutputBuffer( void )
{
  // free any items remaining the the output buffer
  while (m_OutputBuffer.GetSize() > 0)
  {
    delete m_OutputBuffer[0];
    m_OutputBuffer.RemoveAt(0);
  }
  m_TicksSinceLastBufferAdd = 0;
}

void IRCCommon::BufferTick( void )
{
  m_TicksSinceLastBufferAdd++;
}

// returns TRUE if theme was changed.
BOOL IRCCommon::SetTheme(char *ThemeName)
{
  ThemeHandle *pNewTheme = g_ThemeManager.GetThemeHandle(ThemeName);

  if (!pNewTheme) // make sure there was no problem getting the new theme...
    return FALSE;

  BOOL Changed = (m_pTheme != pNewTheme);

  g_ThemeManager.ReleaseThemeHandle(m_pTheme);
  m_pTheme = pNewTheme;

  return Changed;
}


void IRCCommon::ClearUserList( void )
{
  m_Users.DeleteAll();
  if (m_pChildWnd)
  {
    CHILDFRAMEPTR(m_pChildWnd)->m_UserListView.RemoveAll();
  }

  // if the global user list is pointing at this window, then clear it.
  // because we just free()'d the users!
  if (g_pMainWnd->m_UserListView.m_pChannel == this)
  {
    g_pMainWnd->ClearGlobalUserList();
  }
}
