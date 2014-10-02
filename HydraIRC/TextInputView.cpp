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

// TextInputView.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

BOOL CTextInputView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT CTextInputView::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  bHandled = FALSE;
  // Filter out some keyboard input
  switch(wParam)
  {
    case VK_TAB:
      bHandled = TRUE;
      break;
  }
  return 0;
}

enum VirtKeyFlags
{
  VKF_Control = (1<<0),
  VKF_Alt     = (1<<1),
  VKF_Shift   = (1<<2),
};

LRESULT CTextInputView::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  bHandled = FALSE;  

 int virtkeys = (GetKeyState(VK_CONTROL) & 0x10000 ? VKF_Control : 0) +
                (GetKeyState(VK_MENU) & 0x10000 ? VKF_Alt : 0) +
                (GetKeyState(VK_SHIFT) & 0x10000 ? VKF_Shift : 0);

#ifdef VERBOSE_DEBUG
  sys_Printf(BIC_GUI,"key down message: '%c' 0x%02x 0x%08x\n",wParam,wParam,virtkeys);
#endif

  // handle keys regardless of modifers
  switch(wParam)
  {
    case VK_PRIOR:
    case VK_NEXT:
      {
        bHandled = TRUE;
        CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(m_MessageWindow));
        if (pChildWnd->m_WindowType == CWTYPE_SERVER ||
            pChildWnd->m_WindowType == CWTYPE_CHANNEL ||
            pChildWnd->m_WindowType == CWTYPE_DCCCHAT ||
            pChildWnd->m_WindowType == CWTYPE_QUERY)
        {
          // referencing pChildWnd->m_MsgView would be bad if
          // the m_MessageWindow didn't point to one of the above types of windows.
          WPARAM MessageParam;
          if (wParam == VK_PRIOR)
            MessageParam = (virtkeys == VKF_Control) ? SB_TOP : SB_PAGEUP;
          else
            MessageParam = (virtkeys == VKF_Control) ? SB_BOTTOM : SB_PAGEDOWN;

          pChildWnd->m_MsgView.SendMessage(WM_VSCROLL, MessageParam, NULL);
        }
        bHandled = TRUE;
      }
      break;
  }

  // handle keys when control is pressed
  if (virtkeys == VKF_Control)
  {
    // add a control character depending on what keys were pressed.
    bHandled = TRUE;
    switch(wParam)
    {
      case 'B': // Bold
        ReplaceSel("\002");
        break;
      case 'I': // Ctrl+I or Ctrl+N for italics, but stop the tab appearing..
      //case 'N': // Italics // depreciated, ctrl+i only now..
        ReplaceSel("\035");
        break;
      case 'U': // Underline
        ReplaceSel("\037");
        break;
      case 'K': // Color
        ReplaceSel("\003");
        break;
      case 'M': // Hi-Color
        ReplaceSel("\004");
        break;
      case 'R': // Reverse
        ReplaceSel("\026");
        break;
      case 'O': // Normal
        ReplaceSel("\017");
        break;
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        {
          int Bookmark = (wParam - '0');
          IRCChannel *pChannel = FindIRCChannelByName(STRINGPREF( PREF_sChannelBookmark1 + (Bookmark - 1) ));
          if( pChannel && pChannel->m_pChildWnd)
          {
            pChannel->m_pChildWnd->ActivateWindow();
            //g_pMainWnd->MDIActivate(pChannel->m_pChildWnd->m_hWnd);
          }
        }
        break;

      case VK_RETURN: // send text AS-IS.
        {
          int len = GetWindowTextLength();
          char *buffer = (char *)malloc(len + 1);
          GetWindowText(buffer,len+1);
          SetWindowText(NULL);
          m_History.Add(buffer);
          m_History.ResetHistoryItemPointer();
          ::SendMessage(m_MessageWindow,WM_TEXTINPUT,(WPARAM)OTIF_None,(LPARAM)buffer);
          bHandled = TRUE;  
        }
        break;
      default:
        bHandled = FALSE;
    }
  }
  else // handle other keys
  {
    switch(wParam)
    {
      case VK_RETURN:
        {
          int len = GetWindowTextLength();
          char *buffer = (char *)malloc(len + 1);
          GetWindowText(buffer,len+1);
          SetWindowText(NULL);
          m_History.Add(buffer);
          m_History.ResetHistoryItemPointer();
          ::SendMessage(m_MessageWindow,WM_TEXTINPUT,(WPARAM)OTIF_StripWhitespace | OTIF_Parse,(LPARAM)buffer); // Note the TRUE for WPARAM
          bHandled = TRUE;  
        }
        break;
      case VK_UP: // next history item
        {
          bHandled = TRUE;  

          BOOL WasFirst = m_History.IsFirst();
          char *tempbuffer = NULL;
          int len = GetWindowTextLength();

          // don't store an empty buffer.
          // this means we can handle no text, then up, down, some text, up, down
          if (len>0) 
          {
            tempbuffer = (char *)malloc(len + 1);
            if (tempbuffer)
            {
              GetWindowText(tempbuffer,len+1);
            }
          }

          char *currenthistory = m_History.GetCurrentHistoryItem();

          char *buffer = m_History.GetNextHistoryItem();
          if (buffer)
          {
            SetWindowText(buffer);
          }
          else
          {
            // don't erase the buffer, leave it at the first one
            g_pNotificationManager->PlaySoundID(SID_DEFAULT);
          }
          SetSel(-1,-1);


          if (tempbuffer)
          {
            // add to history if different
            if (WasFirst || (currenthistory && stricmp(currenthistory,tempbuffer) != 0))
            {
              m_History.Add(tempbuffer);
              //m_History.Next();
            }
            free(tempbuffer);
          }

        }
        break;
      case VK_DOWN: // previous history item
        {
          bHandled = TRUE;

          BOOL WasLast = m_History.IsLast();
          BOOL WasFirst = m_History.IsFirst();

          char *tempbuffer = NULL;
          int len = GetWindowTextLength();

          // don't store an empty buffer.
          // this means we can handle no text, then up, down, some text, up, down
          if (len>0) 
          {
            tempbuffer = (char *)malloc(len + 1);
            if (tempbuffer)
            {
              GetWindowText(tempbuffer,len+1);
            }
          }

          char *currenthistory = m_History.GetCurrentHistoryItem();

          char *buffer = m_History.GetPreviousHistoryItem();
          if (buffer)
          {
            SetWindowText(buffer);
          }
          else
          {
            SetWindowText(NULL);
            g_pNotificationManager->PlaySoundID(SID_DEFAULT);
          }
          SetSel(-1,-1);


          if (tempbuffer)
          {
            // add to history if different
            if (!currenthistory || (currenthistory && stricmp(currenthistory,tempbuffer) != 0))
            {
              m_History.Add(tempbuffer);
              if (WasFirst)
                m_History.ResetHistoryItemPointer();
            }
            free(tempbuffer);
          }
        }
        break;
      case VK_TAB: // Nick completion
        {
          IRCServer *pServer;

          // are we in a channel ?
          IRCChannel *pChannel = FindIRCChannel(m_MessageWindow);
          // or DCC Chat ?
          pServer = FindDCCChat(m_MessageWindow);
          // or Query ?
          IRCQuery *pQuery = FindIRCQuery(m_MessageWindow);
          // or Server ?
          if (!pServer) // don't overwrite if we've already found a dcc server!
            pServer = FindIRCServer(m_MessageWindow,FALSE,FALSE);

          if (pChannel || pServer || pQuery)
          {
            bHandled = TRUE;
            CHARRANGE cr;
            char nick[1024]; // TODO: convert to char *
            int len = GetWindowTextLength();
            GetSel(cr);
            *nick = 0; // null terminate it.

            int startpos = cr.cpMax;
            int endpos = cr.cpMax;
            int curpos;
            char *inputbuffer = NULL;

            // don't store an empty buffer.
            // this means we can handle no text, then up, down, some text, up, down
            if (len>0) 
            {
              inputbuffer = (char *)malloc(len + 1);
              ATLASSERT(inputbuffer);

              GetWindowText(inputbuffer,len+1);

              // find the beginning of the word
              while (startpos > 0 && inputbuffer[startpos-1] != ' ')
                startpos--;

              // find the end of the word
              while (endpos < len && inputbuffer[endpos] != ' ')
                endpos++;

              // get the word
              int chars = min(endpos - startpos,sizeof(nick)-1);
              strncpy(nick,inputbuffer+startpos,chars);
              nick[chars] = 0;
            }

            char *newnick = NULL; // shut the compiler up

            // complete the nick, or get the next one if the nick is valid.
            if (pChannel)
              pChannel->CompleteNick(nick,&newnick,GetKeyState(VK_SHIFT) & 0x10000 ? FALSE : TRUE,TRUE);
            else if (pServer && pServer->m_IsDirect)
              newnick = strdup(pServer->m_OtherNick); // if we're in a dcc chat, then just get the other nick.
            else if (pServer) // implies "&& !pServer->m_IsDirect"
              newnick = strdup(pServer->GetNick()); // if we're in a server, then just get *our* nick.
            else if (pQuery)
              newnick = strdup(pQuery->m_OtherNick); // if we're in a query window, then just get the other nick.

            if (newnick == NULL)
            {
              g_pNotificationManager->PlaySoundID(SID_DEFAULT);
            }
            else
            {
              
              CChildFrame *pChild = CHILDFRAMEPTR(FindChildWindow(m_MessageWindow));
              
              if (pChild && pChild->IsUserListVisible())
              {
                // select nick!
                sys_Printf(BIC_INFO,"Selecting nick %s\n", newnick);
                IRCUser *pUser = NULL;

                if (pChild->m_WindowType == CWTYPE_CHANNEL)
                  pUser = pChannel->m_Users.FindUser(newnick);

                if (pUser)
                  pChild->m_UserListView.Select(pUser);
              }

              char *nickbuffer = newnick;
              if( startpos == 0 )
              {
                nickbuffer = (char *) malloc( sizeof( char ) *
                  ( strlen( newnick )
                    + strlen( STRINGPREF( PREF_sNickCompletionSuffix ) )
                    + 1
                  )
                );
                if (nickbuffer)
                {
                  strcpy( nickbuffer, newnick );
                  strcat( nickbuffer, STRINGPREF( PREF_sNickCompletionSuffix ) );
                  // free the old buffer and swap the pointers over
                  free(newnick);
                  newnick = nickbuffer;
                }
              }

              if (inputbuffer)
              {
                // allocate and build a new buffer
                int newbufferlen = startpos + strlen(newnick) + (len - endpos) + 1;
                char *newbuffer = (char *)malloc(newbufferlen);

                strncpy(newbuffer,inputbuffer,startpos); // strncpy doesn't null terminate
                strcpy(newbuffer+startpos,newnick);      // so write to the end of the string and terminate
                strcat(newbuffer,inputbuffer+endpos);    // then add the rest of the input buffer
                newbuffer[newbufferlen-1]=0;             // null terminate, to make sure...

#ifdef DEBUG
                // check that new buffer size is not more than allocated buffer size
                int checklen = strlen(newbuffer)+1; // add space for null terminator.
                ATLASSERT(checklen == newbufferlen); // This was the cause of the " | " crash in < v0.308, code above is now fixed.
#endif
                // replace the buffer with the new one
                SetWindowText(newbuffer);
                
                // set the cursor to point to the character after the end of the nick
                curpos = startpos + strlen(newnick);
                SetSel(curpos,curpos);
                free(newbuffer);
              }
              else
              {
                curpos = strlen(newnick);
                SetWindowText(newnick);
                SetSel(curpos,curpos);
              }
              free(newnick);
            }
            if (inputbuffer) free(inputbuffer);
          }
        }
        break;
    }
  }
  return 0;
}

LRESULT CTextInputView::OnFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
#ifdef VERBOSE_DEBUG
  sys_Printf(BIC_GUI,"INPUT: Got Focus\n");
#endif


#ifndef MICROSOFTACKNOWLEDGESTHATWINDOWSISFUCKINGSHITSOMETIMES
  // Doing this here is so fucking shit it's un-true, see this::OnCreate()
  UpdateSettings();
#endif

  bHandled = FALSE;
  return 1;//TODO: Check
}

void CTextInputView::SetMessageWindow(HWND hWnd)
{
  m_MessageWindow = hWnd;
}

LRESULT CTextInputView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{

#ifdef MICROSOFTACKNOWLEDGESTHATWINDOWSISFUCKINGSHITSOMETIMES
  // aaaaaaaaaaaaaaaaaaaaaaaaarrrrrrrrrrrrrrrrrrrrrrrggggggggggggggggghhhhhhhhhhhhhhhhh
  // why the *FUCK* doesn't this set the text colors ?  It sets the background colors
  // but not the text colors!  Windows is SOOOO ghey sometimes.
  // aaaaaaaaaaaaaaaaaaaaaaaaarrrrrrrrrrrrrrrrrrrrrrrggggggggggggggggghhhhhhhhhhhhhhhhh
  // NOTE: problematic code moved to UpdateSettings()
#endif
  UpdateSettings();
  SetEventMask(ENM_MOUSEEVENTS);
  LimitText(512);

  bHandled = FALSE;
  return 0;
}

LRESULT CTextInputView::OnPaste(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  // TODO: if selection is not at the end then we need to cut the selection
  // and then get the text to the right of the selection and add it onto the
  // last line of the multi-line paste.

  // access clipboard and check for clipboard contents with more that one line of text..
  if (OpenClipboard())
  {
    bHandled = TRUE;
    // Retrieve the Clipboard data (specifying that 
    // we want ANSI text (via the CF_TEXT value).
    HANDLE hClipboardData = GetClipboardData(CF_TEXT);
    if (hClipboardData)
    {
      // Call GlobalLock so that to retrieve a pointer
      // to the data associated with the handle returned
      // from GetClipboardData.
      char *pData = (char*)GlobalLock(hClipboardData);

      if (pData)
      {
        char *pLineStart = pData;
        int LineCount = 0;
        do
        {
          if (!*pData || *pData == '\r' || *pData == '\n' || pData - pLineStart > 400) // limit to 450 chars
          {
            int BufferLen = pData - pLineStart;

            int LineLen = 0;

            if (LineCount == 0)
              LineLen = GetWindowTextLength();

            char *buffer = (char *)malloc(LineLen + 1 + BufferLen);
  
            if (LineCount == 0)
              GetWindowText(buffer,LineLen+1);

            strncpy(buffer + LineLen,pLineStart,BufferLen);
            buffer[LineLen + BufferLen] = 0;

            // skip the trailing \n and \r, also skip blank lines.
            while (*pData && (*pData == '\r' || *pData == '\n'))
              pData++;
            pLineStart = pData;
            LineCount++;

            if (*pData)
            {
              SetWindowText(NULL);
              m_History.Add(buffer); // TODO: make this a pref?
              m_History.ResetHistoryItemPointer();
              ::SendMessage(m_MessageWindow,WM_TEXTINPUT,(WPARAM)OTIF_None,(LPARAM)buffer); // Note the TRUE for the WPARAM
            }
            else
            {
              // no more data!

              if (LineCount > 1)
              {
                // if multi-line, send the last line too
                SetWindowText(NULL);
                m_History.Add(buffer); // TODO: make this a pref?
                m_History.ResetHistoryItemPointer();
                ::SendMessage(m_MessageWindow,WM_TEXTINPUT,(WPARAM)OTIF_None,(LPARAM)buffer); // Note the TRUE for the WPARAM
              }
              else
              {
                // if not multi-line then insert the buffer
                ReplaceSel(buffer+LineLen); // ignoring what we got from the buffer already..
                free(buffer); // we must rember to free() in this instance.
                // NOTE: NOT adding to history buffer...
              }
              break;
            }
          }
          else
          {
            pData++;
          }
        } while (LineCount < INTPREF(PREF_nMaxClipboardPasteLines));
      }
      // Unlock the global memory.
      GlobalUnlock(hClipboardData);
    }
    CloseClipboard();
  }

  return 0;
}

void CTextInputView::UpdateSettings( void )
{
	CHARFORMAT2 fmt;
  GetDefaultCharFormat(fmt);
  fmt.dwMask = CFM_COLOR;
  fmt.dwEffects = 0;
  fmt.crTextColor = m_TextColor;
  fmt.crBackColor = m_BackColor;
  fmt.cbSize = sizeof(CHARFORMAT2);
  SetDefaultCharFormat(fmt);
  SetBackgroundColor(m_BackColor);
  m_History.SetMaxLines(INTPREF(PREF_nMaxInputHistoryLines));

}

void CTextInputView::SetColors( COLORREF *pColors )
{
  // extract just the colors we need from the array of colors.
  m_TextColor = pColors[item_textinputnormaltext - PREF_COLOR_FIRST];
  m_BackColor = pColors[item_textinputbackground - PREF_COLOR_FIRST];
}

short CTextInputView::GetRequiredHeight( int Windowtype )
{
  int InputHeight;
  HFONT hFont;

  switch (Windowtype)
  {
    case CWTYPE_SERVER:
      hFont = GetAppFont(PREF_fServerInputFont);
      break;
    case CWTYPE_DCCCHAT:
      hFont = GetAppFont(PREF_fDCCChatInputFont);
      break;
    case CWTYPE_QUERY:
      hFont = GetAppFont(PREF_fQueryInputFont);
      break;
    case CWTYPE_CHANNEL:
      hFont = GetAppFont(PREF_fChannelInputFont);
      break;
  }
  InputHeight = 6 + GetFontMaxHeight(hFont); // value (6) is dependant on window styles.

  return InputHeight;
}