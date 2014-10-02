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

// ChildFrm.cpp : implementation of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

LRESULT CChildFrame::OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	LPMSG pMsg = (LPMSG)lParam;

	if(baseClass::PreTranslateMessage(pMsg))
		return TRUE;

  return m_MsgView.PreTranslateMessage(pMsg);
}

void CChildFrame::OnFinalMessage(HWND hWnd)
{
  g_pMainWnd->PostMessage(WM_REQUESTAUTOTILE);
  g_pMainWnd->PostMessage(WM_CHILDFRAMECLOSED, NULL, (LPARAM) this);
  //delete this;
}

LRESULT CChildFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;

  UpdateStatus();

  // Splitter Bars
  const DWORD dwSplitStyle = WS_CHILD | WS_VISIBLE
                             | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
              dwSplitExStyle = 0;

  // Panes
  const DWORD dwPaneStyle = WS_CHILD | WS_VISIBLE
                         | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
              dwPaneExStyle = 0;

  // Header View
  const DWORD dwHeaderStyle = WS_CHILD | WS_VISIBLE
                         | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
              dwHeaderExStyle = 0;

  // Message View
  const DWORD dwMsgViewStyle = WS_CHILD | WS_VISIBLE
                               | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
                               | WS_VSCROLL | ES_MULTILINE
                               | ES_READONLY | ES_DISABLENOSCROLL,
              dwMsgViewExStyle = 0;

  // UserList View
  const DWORD dwUserListViewStyle = WS_CHILD | WS_VISIBLE
                         | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
              dwUserListViewExStyle = 0;


  // Info View
  const DWORD dwInfoViewStyle = WS_CHILD | WS_VISIBLE
                         | ES_READONLY
                         | ES_CENTER
                         | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
                         | WS_VSCROLL | ES_MULTILINE | ES_DISABLENOSCROLL,
              dwInfoViewExStyle = 0;

  // Input View
  const DWORD dwInputViewStyle = WS_CHILD | WS_VISIBLE
                               | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
                               | WS_VSCROLL | ES_MULTILINE
                               | ES_AUTOHSCROLL,
              dwInputViewExStyle = 0;


  // Create the splitters
  m_HorizSplitter1.Create( *this, rcDefault, NULL,
                        dwSplitStyle, dwSplitExStyle );

  m_HorizSplitter2.Create( m_HorizSplitter1, rcDefault, NULL,
                        dwSplitStyle, dwSplitExStyle );

  m_VertSplitter1.Create( m_HorizSplitter2, rcDefault, NULL,
                        dwSplitStyle, dwSplitExStyle );

  m_HorizSplitter3.Create( m_VertSplitter1, rcDefault, NULL,
                        dwSplitStyle, dwSplitExStyle );


  m_hWndClient = m_HorizSplitter1;

  // Set the splitter styles and sizes
  m_VertSplitter1.SetSplitterExtendedStyle(SPLIT_RIGHTALIGNED, 0);
  m_HorizSplitter1.SetSplitterExtendedStyle(SPLIT_NONINTERACTIVE, 0);
  m_HorizSplitter2.SetSplitterExtendedStyle(SPLIT_NONINTERACTIVE | SPLIT_BOTTOMALIGNED, 0);
  m_HorizSplitter3.SetSplitterExtendedStyle(SPLIT_BOTTOMALIGNED, 0);

  // 2 for interactive, 1 for static
  m_VertSplitter1.m_cxySplitBar = 4;
  m_HorizSplitter1.m_cxySplitBar = 3;
  m_HorizSplitter2.m_cxySplitBar = 3;
  m_HorizSplitter3.m_cxySplitBar = 4;

  // Create the views and pane containers

  m_HeaderView.Create(m_HorizSplitter1, rcDefault, NULL, dwHeaderStyle, dwHeaderExStyle );
  m_HeaderView.SetMessageWindow(m_hWnd);

  // Output
  m_MsgView.Create(m_VertSplitter1, rcDefault, NULL,dwMsgViewStyle,dwMsgViewExStyle);

  // User List
  m_UserListPane.Create(m_HorizSplitter3, _T("User List"),dwPaneStyle,dwPaneExStyle);
  m_UserListPane.SetGradientFill(BOOLPREF(PREF_bUseGradientFilledPanes));
  m_UserListView.Create(m_UserListPane, rcDefault, NULL, dwUserListViewStyle, dwUserListViewExStyle );

  m_UserListPane.SetClient ( m_UserListView );
  m_UserListPane.m_cxyHeader -=1;

  m_UserListView.SetColors(m_ColorPrefs);
  m_UserListView.SetShowBuddyGroups(BOOLPREF(PREF_bShowBuddyGroupsInUserList));
  m_UserListView.UpdateSettings();
  m_UserListView.m_ObjectType = OTYPE_CHANNELWINDOWUSERLIST;

  // Info View
  m_InfoPane.Create(m_HorizSplitter3, _T("Info"),dwPaneStyle,dwPaneExStyle);
  m_InfoPane.SetGradientFill(BOOLPREF(PREF_bUseGradientFilledPanes));
  m_InfoView.Create(m_InfoPane, rcDefault, NULL, dwInfoViewStyle, dwInfoViewExStyle );

  m_InfoPane.SetClient ( m_InfoView );
  m_InfoPane.m_cxyHeader -=1;

  m_UserListView.SetColors(m_ColorPrefs);
  m_UserListView.UpdateSettings();

  //m_HorizSplitter1.SetSplitterPanes( m_VertSplitter1, m_TextInputView );
  //m_VertSplitter1.SetSplitterPane( SPLIT_PANE_LEFT, m_MsgView );
  //m_VertSplitter1.SetSinglePaneMode( SPLIT_PANE_LEFT );

  // Input
  m_TextInputView.Create(m_HorizSplitter2, rcDefault, NULL,dwInputViewStyle, dwInputViewExStyle);
  m_TextInputView.SetMessageWindow(m_hWnd);

  // Assign the views to the Splitter Panes
  m_VertSplitter1.SetSplitterPanes( m_MsgView, m_HorizSplitter3);
  m_HorizSplitter1.SetSplitterPanes( m_HeaderView, m_HorizSplitter2);
  m_HorizSplitter2.SetSplitterPanes( m_VertSplitter1, m_TextInputView);
  m_HorizSplitter3.SetSplitterPanes( m_UserListPane, m_InfoPane);

  // set the basic layout (only show input and output views by default)
  // we turn the other panes on later according to user prefs.
  m_HorizSplitter1.SetSinglePaneMode( SPLIT_PANE_BOTTOM );  // hide the header view
  m_HorizSplitter2.SetSinglePaneMode( SPLIT_PANE_NONE );    // show vert splitter and text input view
  m_HorizSplitter3.SetSinglePaneMode( SPLIT_PANE_NONE );    // show the info userlist views
  m_VertSplitter1.SetSinglePaneMode( SPLIT_PANE_LEFT );     // show the msgview (hide the info and userlist)

  // show the userlist pane by default?
  if ((m_WindowType == CWTYPE_CHANNEL && BOOLPREF(PREF_bShowChannelWindowUserListPaneByDefault)) ||
      (m_WindowType == CWTYPE_QUERY && BOOLPREF(PREF_bShowQueryWindowUserListPaneByDefault)) ||
      (m_WindowType == CWTYPE_SERVER && BOOLPREF(PREF_bShowServerWindowUserListPaneByDefault)) ||
      (m_WindowType == CWTYPE_DCCCHAT && BOOLPREF(PREF_bShowDCCChatWindowUserListPaneByDefault)) )
  {
    ShowUserList();
  }

  // show the info pane by default?
  if ((m_WindowType == CWTYPE_CHANNEL && BOOLPREF(PREF_bShowChannelWindowInfoPaneByDefault)) ||
      (m_WindowType == CWTYPE_QUERY && BOOLPREF(PREF_bShowQueryWindowInfoPaneByDefault)) ||
      (m_WindowType == CWTYPE_SERVER && BOOLPREF(PREF_bShowServerWindowInfoPaneByDefault)) ||
      (m_WindowType == CWTYPE_DCCCHAT && BOOLPREF(PREF_bShowDCCChatWindowInfoPaneByDefault)) )
  {
    ShowInfo();
  }

  // show the header pane by default?
  if ((m_WindowType == CWTYPE_CHANNEL && BOOLPREF(PREF_bShowChannelWindowHeaderPaneByDefault)) ||
      (m_WindowType == CWTYPE_QUERY && BOOLPREF(PREF_bShowQueryWindowHeaderPaneByDefault)) ||
      (m_WindowType == CWTYPE_SERVER && BOOLPREF(PREF_bShowServerWindowHeaderPaneByDefault)) ||
      (m_WindowType == CWTYPE_DCCCHAT && BOOLPREF(PREF_bShowDCCChatWindowHeaderPaneByDefault)) )
  {
    ShowHeader();
  }

  UpdateLayout();

  InitialiseWindows();

  ProcessBuffer();

#ifdef USE_TABBEDMDI
  PostMessage(WM_UPDATETAB, (WPARAM)0, (LPARAM)NULL);
#endif

  /*
  If an application processes this message, it should return zero to
  continue creation of the window. If the application returns –1,
  the window is destroyed and the CreateWindowEx or CreateWindow
  function returns a NULL handle.
  */
	return 0;
}

LRESULT CChildFrame::OnMDIActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  // wParam contains the address of the window being Deactivated
  // lParam contains the address of the window being Activated

#ifdef DEBUG
  sys_Printf(BIC_INFO,"CChildFrame::OnMDIActivate - WParam = 0x%8x, lParam = 0x%8x\n",wParam, lParam);
#endif

  if ((HWND)wParam == m_hWnd)
  {
#ifdef DEBUG
    sys_Printf(BIC_INFO,"CChildFrame::OnMDIActivate - Window type %d Deactivated\n",m_WindowType);
#endif

// <gliptic> Clear dirty flags when window looses focus if the pref is set
                                                                           
    if (BOOLPREF(PREF_bClearDirtyFlagOnDeactivate))                        
    {                                                                      
      switch (m_WindowType)                                                
      {                                                                    
      case CWTYPE_QUERY:                                                   
        m_pQuery->ClearDirtyFlag();                                        
        break;                                                             
      case CWTYPE_CHANNEL:                                                 
        m_pChannel->ClearDirtyFlag();                                      
        break;                                                             
      case CWTYPE_DCCCHAT:                                                 
      case CWTYPE_SERVER:                                                  
        m_pServer->ClearDirtyFlag();                                       
        break;                                                             
      }                                                                    
    }                                                                      
// </gliptic>                                                              

  }

  if ((HWND)lParam == m_hWnd)
  {
#ifdef DEBUG
    sys_Printf(BIC_INFO,"CChildFrame::OnMDIActivate - Window type %d Activated\n",m_WindowType);
#endif

    // Note m_hWnd is *NOT* visible at this point

    switch (m_WindowType)
    {
      case CWTYPE_QUERY:
        m_pQuery->ClearDirtyFlag();
        g_pMainWnd->m_ServerListView.Select(m_pQuery);
        break;
      case CWTYPE_CHANNEL:
        m_pChannel->ClearDirtyFlag();
        g_pMainWnd->m_ServerListView.Select(m_pChannel);
        break;
      case CWTYPE_DCCCHAT:
        m_pServer->ClearDirtyFlag();
        g_pMainWnd->m_ServerListView.Select(m_pServer);
        break;
      case CWTYPE_SERVER:
        m_pServer->ClearDirtyFlag();
        g_pMainWnd->m_ServerListView.Select(m_pServer);
        break;
    }

    // set the active pane and the focus
    ActivateTextInputPane();

    // Update the Status and ServerList controls
    UpdateStatus();
  }
  bHandled = FALSE;
  return bHandled ? 0 : 1; // WM_MDIACTIVATE - An MDI child window should return zero if it processes this message.
}

LRESULT CChildFrame::OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  // Note: WM_SETFOCUS messages are received *before* WM_MDIACTIVATE messages
#ifdef DEBUG
    sys_Printf(BIC_INFO,"CChildFrame::OnSetFocus\n");
#endif

  // Clear the dirty status flags.

  if (IsWindowVisible())
  {
    switch (m_WindowType)
    {
      case CWTYPE_QUERY:
        m_pQuery->ClearDirtyFlag();
        break;
      case CWTYPE_CHANNEL:
        m_pChannel->ClearDirtyFlag();
        break;
      case CWTYPE_DCCCHAT:
        m_pServer->ClearDirtyFlag();
        break;
      case CWTYPE_SERVER:
        m_pServer->ClearDirtyFlag();
        break;
    }

    // First focus message?
    if (m_FirstFocus)
    {
      m_FirstFocus = FALSE;
      g_pMainWnd->PostMessage(WM_REQUESTAUTOTILE);
    }

  }

	bHandled = FALSE;
  return bHandled ? 0 : 1;// WM_SETFOCUS - An application should return zero if it processes this message
}

LRESULT CChildFrame::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
/*
  bHandled = FALSE;
  if (m_WindowType == CWTYPE_SERVER)
  {
    // prompt if server is still connected.
    if (BOOLPREF(PREF_bPromptOnServerClose) && m_pServer->GetStatus() == SVR_STATE_CONNECTED)
    {
      if (IDNO == MessageBox(_T("Server is still connected! Disconnect ?"),_T(m_pServer->m_pDetails->m_Name), MB_YESNO | MB_ICONQUESTION))
        bHandled = TRUE;
    }
  }
  return 0;
*/
  bHandled = TRUE; // TRUE: changed 3:47
  switch (m_WindowType)
  {
    case CWTYPE_SERVER:
      {
        BOOL KillServer = TRUE;
        // prompt if server is still connected.
        if (BOOLPREF(PREF_bPromptOnServerClose) && m_pServer->GetStatus() == SVR_STATE_CONNECTED)
        {
          if (IDNO == MessageBox(_T("Server is still connected! Disconnect ?"),_T(m_pServer->m_pDetails->m_Name), MB_YESNO | MB_ICONQUESTION))
            KillServer = FALSE;
        }
        if (KillServer)
          delete m_pServer;
      }
      break;
    case CWTYPE_CHANNEL:
      {
        delete m_pChannel;
      }
      break;
    case CWTYPE_QUERY:
      {
        delete m_pQuery;
      }
      break;
    case CWTYPE_DCCCHAT:
      {
        delete m_pServer;
      }
      break;
  }
	return 1; // WM_CLOSE - If an application processes this message, it should return zero
}

void CChildFrame::UpdateStatus( void )
{
  if (!::IsWindow(m_hWnd))
    return;
  char *newstr,*descriptionstr;
  short status;
  int len;
  switch (m_WindowType)
  {
    case CWTYPE_QUERY:
      descriptionstr = m_pQuery->GetDescriptionString();
      len = strlen(descriptionstr) + 1;
      newstr = (char *) malloc( len );
      sprintf(newstr,"%s",descriptionstr);
      SetWindowText(newstr);
      break;
    case CWTYPE_DCCCHAT: // TODO: define a more custom message (add remote user's IP?)
      descriptionstr = m_pServer->GetDescriptionString();
      status = m_pServer->GetStatus();
      len = strlen(descriptionstr) + 3 +
        strlen(g_ServerStatusNames[status]) + 1;
      newstr = (char *) malloc( len );
      sprintf(newstr,"%s - %s",descriptionstr,g_ServerStatusNames[status]);
      SetWindowText(newstr);
      break;
    case CWTYPE_SERVER:
      descriptionstr = m_pServer->GetDescriptionString();
      status = m_pServer->GetStatus();
      len = strlen(descriptionstr) + 3 +
            strlen(g_ServerStatusNames[status]) + 1;
      newstr = (char *) malloc( len );
      sprintf(newstr,"%s - %s",descriptionstr,g_ServerStatusNames[status]);
      SetWindowText(newstr);
      break;
    case CWTYPE_CHANNEL:
      descriptionstr = m_pChannel->GetDescriptionString();
      len = strlen(descriptionstr) + 1;
      newstr = (char *) malloc( len );
      sprintf(newstr,"%s",descriptionstr);
      SetWindowText(newstr);
      break;

  }

  if (newstr) // Swap the status strings over
  {
    if (m_StatusStr) free(m_StatusStr);
    m_StatusStr = newstr;
  }

  g_pMainWnd->SetStatusBar(STATUS_ICON_DONTCHANGE,NULL); // show the nick in the status bar.
  UpdateInfo();
#ifdef USE_TABBEDMDI
  UpdateTab();
#endif

}

void CChildFrame::CMD_CTCPRequest(char *Args)
{
  if (Args && *Args)
  {
    char *to = strtok(Args," ");
    char *msg = strtok(NULL,"");

    if (to && *to && msg && *msg)
    {
      char percentescapedbuf[512*2 + 1];
      EscapePercentCharacters(percentescapedbuf,msg,sizeof(percentescapedbuf));

      BOOL Sent = m_pServer->CMD_CTCPRequest(to,percentescapedbuf);
      // if a ctcp request was sent, echo it in the window channel/query
      if (Sent && (m_WindowType == CWTYPE_CHANNEL || m_WindowType == CWTYPE_QUERY))
        Put(BIC_CTCP,strdup(m_pServer->m_Variables[VID_SERVEROUTPUT]));
      return;
    }
    // fall through to error message
  }
  Printf(BIC_ERROR,"ERROR: <to> <args>");
}

void CChildFrame::CMD_Notice(char *Args)
{
  if (Args && *Args)
  {

    char *to = strtok(Args," ");
    char *msg = strtok(NULL,"");

    if (to && *to && msg && *msg)
    {
      BOOL Sent = m_pServer->CMD_Notice(to,msg);
      return;
    }
    // fall through to error message
  }
  Printf(BIC_ERROR,"ERROR: <to> <message>");
}

void CChildFrame::CMD_PrivMsg(char *Args)
{
  if (Args && *Args)
  {

    char *to = strtok(Args," ");
    char *msg = strtok(NULL,"");

    if (to && *to && msg && *msg)
    {
      BOOL Sent = m_pServer->CMD_PrivMsg(to,msg);
      return;
    }
    // fall through to error message
  }
  Printf(BIC_ERROR,"ERROR: <to> <message>");
}

void CChildFrame::CMD_PrivMsg(const char *To, const char *Msg)
{
  m_pServer->CMD_PrivMsg(To,Msg);
}

void CChildFrame::CMD_Help(const char *Args)
{
  // Args are ignored... toto: for now.. ;)
  Printf(BIC_INFO,"HydraIRC is beta software. Complete end-user documentation has not been written yet. "
                  "If you need help or information then read \"readme.txt\", then \"docs.htm\" and "
                  "if you still can't find what you need then load up CHANGES.TXT into a text "
                  "editor (e.g. notepad) and search it.  e.g.  if you want to know how to "
                  "change fonts, then search for FONTS and you'll find the docs on how to do it "
                  "(you might have to repeat the search)  Also, there's a wiki, check it out at "
                  "www.hydrairc.com/wiki/ and there are forums, see www.hydrairc.com/forums/");
}

void CChildFrame::SetVerboseLevel(int VerboseLevel)
{
  switch(m_WindowType)
  {
    case CWTYPE_CHANNEL:
      {
        switch (VerboseLevel)
        {
          case 0:
            m_MsgView.SetFilterList(NULL);
            break;
          case 1:
            m_MsgView.SetFilterList(g_DefaultFilterList_VerboseLevel1);
            break;
          case 2:
            m_MsgView.SetFilterList(g_DefaultFilterList_VerboseLevel2);
            break;
          case 3:
            m_MsgView.SetFilterList(g_DefaultFilterList_VerboseLevel3);
            break;
          default:
            Printf(BIC_ERROR,"only values 0 - 3 are valid.");
        }
      }
      break;
    default:
      Printf(BIC_ERROR,"/SET VERBOSELEVEL is only valid for channel windows");
  }
}

LRESULT CChildFrame::OnSetVerboseLevel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  SetVerboseLevel(wID - IDC_CHILDFRAME_VERBOSELEVEL_LEVEL0);
  return 0;
}

void CChildFrame::CMD_Ignore(const char *Args)
{
  IgnoreInfo *pIgnoreInfo;
  int i;
  char *arg;
  int RemoveCount = 0;

  if (Args && *Args)
  {
    char *ArgsCopy = strdup(Args);
    if(!ArgsCopy)
      return;

    char *nick = UseEmptyString(strtok(ArgsCopy," "));

    if (*nick == '+')
    {
      nick++; // skip the +
      if (!*nick)
      {
        Printf(BIC_WARNING,"No nick given, check usage");
      }
      else
      {

        // check to see if we're already ignoring this user
        BOOL Found = FALSE;
        for (i = 0; i < g_pPrefs->m_IgnoreList.GetSize() && !Found; i++)
        {
          pIgnoreInfo = g_pPrefs->m_IgnoreList[i];
          if (stricmp(pIgnoreInfo->m_Nick,nick) == 0)
          {
            Found = TRUE;
          }
        }

        if (Found)
          Printf(BIC_WARNING,"Already ignoring \"%s\", check /IGNORE usage",nick);
        else
        {
          pIgnoreInfo = new IgnoreInfo(nick,NULL,NULL,NETWORK_ALL,0);

          if (pIgnoreInfo)
          {
            while (arg = strtok(NULL," "))
            {
              if (ischannelstartchar(*arg))
              {
                pIgnoreInfo->AddChannel(arg);
              }
              else
              {
                if (stricmp(arg,"disabled") == 0)
                {
                  pIgnoreInfo->m_Flags |= IIF_DISABLED;
                }
                else if (stricmp(arg,"thisnetwork") == 0)
                {
                  pIgnoreInfo->m_NetworkID = GetServer()->m_pDetails->m_NetworkID;
                }
                else if (strnicmp(arg,"MASK=",5) == 0)
                {
                  char *Mask = arg+5;
                  if (Mask)
                    SetString(pIgnoreInfo->m_HostMask,Mask);
                }
                else if (strnicmp(arg,"NOTES=",6) == 0)
                {
                  // TODO: only takes single parameter, need to support stuff like
                  // /IGNORE +MikeD NOTES=This guy is a spammer
                  char *Notes = arg+6;
                  if (Notes)
                    SetString(pIgnoreInfo->m_Notes,Notes);
                }
                // TODO: add "BYHOST" as an option so we can ignore this user by their hostname
                // and not via their nick, we need to search all channels on the current
                // network to see if we know the user's hostmask first though...
                else
                {
                  Printf(BIC_WARNING,"Unknown IGNORE parameter \"%s\"",arg);
                }
              }
            }
            Printf(BIC_INFO,"Ignoring \"%s\"",nick);
            pIgnoreInfo->Validate();
            g_pPrefs->m_IgnoreList.Add(pIgnoreInfo);
          }
        }
      }

    }
    else if (*nick == '-')
    {
      nick++; // skip the -
      if (!*nick)
      {
        Printf(BIC_WARNING,"No nick given, check usage");
      }
      else
      {
        for (i = 0; i < g_pPrefs->m_IgnoreList.GetSize(); i++)
        {
          pIgnoreInfo = g_pPrefs->m_IgnoreList[i];
          if (stricmp(pIgnoreInfo->m_Nick,nick) == 0)
          {
            Printf(BIC_INFO,"No longer ignoring \"%s\"",nick);
            RemoveCount++;
            delete pIgnoreInfo;
            g_pPrefs->m_IgnoreList.RemoveAt(i);
            i--; // skip back one and continue...
          }
        }
        if (RemoveCount == 0)
        {
          Printf(BIC_INFO,"Wasn't ignoring \"%s\"",nick);
        }
      }
    }
    else
    {
      arg = strtok(NULL," ");
      if (!arg || !*arg)
      {
        Printf(BIC_ERROR,"No options specified!");
      }
      else
      {
        BOOL Found = FALSE;
        for (i = 0; i < g_pPrefs->m_IgnoreList.GetSize() && !Found; i++)
        {
          pIgnoreInfo = g_pPrefs->m_IgnoreList[i];
          if (stricmp(pIgnoreInfo->m_Nick,nick) == 0)
          {
            Found = TRUE;
            do
            {
              if (*arg == '-')
              {
                arg++; // skip the -
                if (pIgnoreInfo->RemoveChannel(arg))
                {
                  RemoveCount++;
                  Printf(BIC_WARNING,"No longer ignoring \"%s\" in \"%s\"",nick,arg);
                }
                else
                  Printf(BIC_WARNING,"Wasn't ignoring \"%s\" in \"%s\"",nick,arg);
              }
              else if (*arg == '+')
              {
                arg++; // skip the +
                if (pIgnoreInfo->AddChannel(arg))
                {
                  Printf(BIC_WARNING,"ignoring \"%s\" in \"%s\"",nick,arg);
                  if (pIgnoreInfo->m_Flags & IIF_IGNORECHANNELLIST)
                  {
                    pIgnoreInfo->m_Flags &= ~IIF_IGNORECHANNELLIST;
                    pIgnoreInfo->Validate();
                  }

                }
              }
              else if (stricmp(arg,"disable") == 0)
              {
                if (pIgnoreInfo->m_Flags & IIF_DISABLED)
                  Printf(BIC_WARNING,"Already disabled for \"%s\"",nick);
                else
                {
                  Printf(BIC_WARNING,"Disabling ignoring of \"%s\"",nick);
                  pIgnoreInfo->m_Flags |= IIF_DISABLED;
                }
              }
              else if (stricmp(arg,"enable") == 0)
              {
                if (!(pIgnoreInfo->m_Flags & IIF_DISABLED))
                  Printf(BIC_WARNING,"Already enabled for \"%s\"",nick);
                else
                {
                  Printf(BIC_WARNING,"enabling ignoring of \"%s\"",nick);
                  pIgnoreInfo->m_Flags &= ~IIF_DISABLED;
                }
              }
              else if (stricmp(arg,"ignorechannellist") == 0)
              {
                if (pIgnoreInfo->m_Flags & IIF_IGNORECHANNELLIST)
                  Printf(BIC_WARNING,"Already ignoring channellist for \"%s\"",nick);
                else
                {
                  Printf(BIC_WARNING,"ignoring channellist for \"%s\"",nick);
                  pIgnoreInfo->m_Flags |= IIF_IGNORECHANNELLIST;
                }
              }
              else if (stricmp(arg,"usechannellist") == 0)
              {
                if (!(pIgnoreInfo->m_Flags & IIF_IGNORECHANNELLIST))
                  Printf(BIC_WARNING,"Already using channel list for \"%s\"",nick);
                else
                {
                  pIgnoreInfo->m_Flags &= ~IIF_IGNORECHANNELLIST;
                  pIgnoreInfo->Validate();

                  if (pIgnoreInfo->m_Flags & IIF_IGNORECHANNELLIST)
                    Printf(BIC_WARNING,"channel list for \"%s\" is empty",nick);
                  else
                    Printf(BIC_WARNING,"using channel list for \"%s\"",nick);
                }
              }
              else
              {
                Printf(BIC_WARNING,"Unknown IGNORE parameter \"%s\"",arg);
              }

              arg = strtok(NULL," ");
            } while (arg);
            if (pIgnoreInfo->m_Flags & IIF_IGNORECHANNELLIST && RemoveCount > 0)
            {
              Printf(BIC_WARNING,"Removed all channels from channel list, ignoring \"%s\" everywhere! ",nick);
            }
          }
        }
        if (!Found)
          Printf(BIC_ERROR,"Not ignoring user \"%s\"",nick);
      }
    }

    free(ArgsCopy);
  }
  else
  {
    // dump ignore list.
    char ChannelList[1024];

    for (i = 0; i < g_pPrefs->m_IgnoreList.GetSize(); i++)
    {
      pIgnoreInfo = g_pPrefs->m_IgnoreList[i];

      int j;
      char *p = ChannelList;
      *ChannelList = 0;

      for (j = 0; j < pIgnoreInfo->m_ChannelNameList.GetSize(); j++)
      {
        int bytes = _snprintf(p,sizeof(ChannelList) - (p - ChannelList),"%s%s",j > 0 ? "," : "",pIgnoreInfo->m_ChannelNameList[j]);
        if (bytes > 0)
          p+=bytes;
      }
      Printf(BIC_INFO,"%d) %s (%s) Network: %s Channels: \"%s\"%s%s%s%s%s",
        i+1, // 0 based..
        pIgnoreInfo->m_Nick,
        pIgnoreInfo->m_HostMask,
        pIgnoreInfo->m_NetworkID == NETWORK_ALL ? "All Networks" : GetNetworkName(pIgnoreInfo->m_NetworkID),
        ChannelList,
        pIgnoreInfo->m_Flags != IIF_NONE ? " Options: " : "",
        pIgnoreInfo->m_Flags & IIF_DISABLED ? "DISABLED " : "",
        pIgnoreInfo->m_Flags & IIF_IGNORECHANNELLIST ? "IGNORINGCHANNELLIST " : "",
        pIgnoreInfo->m_Notes ? " Notes: " : "",
        pIgnoreInfo->m_Notes ? pIgnoreInfo->m_Notes : "");
    }
  }
}

void CChildFrame::CMD_Set(char *Args)
{
  if (Args && *Args)
  {
    char *pref = strtok(Args,"=");
    char *value = strtok(NULL,"");

    if (pref && *pref && value && *value)
    {
      if (stricmp(pref,"THEME") == 0)
      {
        BOOL Changed = FALSE;
        switch(m_WindowType)
        {
          case CWTYPE_SERVER:
          case CWTYPE_DCCCHAT:
            Changed = m_pServer->SetTheme(value);
            break;
          case CWTYPE_CHANNEL:
            Changed = m_pChannel->SetTheme(value);
            break;
          case CWTYPE_QUERY:
            Changed = m_pQuery->SetTheme(value);
            break;
        }
        if (Changed)
          Printf(BIC_INFO,"Theme for this window changed to \"%s\"",value);
        else
          Printf(BIC_INFO,"Theme for this window was not changed",value);
      }
      else if (stricmp(pref,"VERBOSELEVEL") == 0)
      {
        SetVerboseLevel(atoi(value));
      }
      else if (stricmp(pref,"WINDOWCOLORSCHEME") == 0)
      {
        ColorPrefItem_t *pColorScheme = g_pPrefs->LoadColorSchemeByName(value);
        if (pColorScheme)
        {
          g_pPrefs->ConvertColorSchemeToPrefs(pColorScheme,m_ColorPrefs);
          free(pColorScheme);
          UpdateSettings(TRUE);
        }
      }
      else if (stricmp(pref,"COLORSCHEME") == 0)
      {
        ColorPrefItem_t *pColorScheme = g_pPrefs->LoadColorSchemeByName(value);
        if (pColorScheme)
        {
          g_pPrefs->ConvertColorSchemeToPrefs(pColorScheme,g_pPrefs->m_ColorPrefs);
          free(pColorScheme);

          g_pPrefs->UpdatePref(pref);
        }
      }
      else if (stricmp(pref,"GUILAYOUT") == 0)
      {
        g_pMainWnd->ChangeGUILayoutStyle(atoi(value));
      }
      else if (stricmp(pref,"WINDOWPREFS") == 0)
      {
        BOOL Remember = TRUE;
        int NetworkID = NETWORK_UNKNOWN;
        char *WindowName = NULL;

        switch(m_WindowType)
        {
          case CWTYPE_CHANNEL:
            NetworkID = m_pServer->m_pDetails->m_NetworkID;
            WindowName = m_pChannel->m_pProperties->m_Name;
            break;
          case CWTYPE_QUERY:
            NetworkID = m_pServer->m_pDetails->m_NetworkID;
            WindowName = m_pQuery->m_OtherNick;
            break;
          case CWTYPE_SERVER:
            NetworkID = m_pServer->m_pDetails->m_NetworkID;
            break;
          default:
            Printf(BIC_INFO,"Command not available for this window");
            return;
        }

        g_pPrefs->SetBooleanPref(value,&Remember);

        if (Remember)
        {
          if (!m_pWindowPrefs)
          {
            WindowPrefs *pWindowPrefs = new WindowPrefs(g_pPrefs);

            if (pWindowPrefs)
            {
              // initialise the windowprefs class
              pWindowPrefs->m_WindowType = m_WindowType;
              pWindowPrefs->m_NetworkID = NetworkID;
              pWindowPrefs->m_WindowName = strdup(WindowName);

              // TODO: remember these...
              //pWindowPrefs->m_WindowPositions.Set(10,10,300,200);
              //pWindowPrefs->m_Flags |= WPF_HAVEPOSITIONS;

              CopyMemory(pWindowPrefs->m_ColorPrefs,m_ColorPrefs,sizeof(m_ColorPrefs));
              pWindowPrefs->m_Flags |= WPF_HAVECOLORS;

              // tell prefs about it
              g_pPrefs->m_WindowPrefsList.Add(pWindowPrefs);

              // remember it for this CChildFrame instance.
              m_pWindowPrefs=pWindowPrefs;

              // tell the user
              Printf(BIC_INFO,"Remembering prefs for this window");
            }
          }
          else
            Printf(BIC_INFO,"Already remembering prefs for this window");
        }
        else
        {
          if (g_pPrefs->DeleteWindowPrefs(m_WindowType,NetworkID,WindowName))
          {
            Printf(BIC_INFO,"No longer remembering prefs for this window");
            m_pWindowPrefs = NULL;
          }
          else
            Printf(BIC_INFO,"Was not remembering prefs for this window");
        }
      }
      else if (g_pPrefs->SetPref(pref,value))
      {
        Printf(BIC_INFO,"Pref \"%s\" set to \"%s\"",pref,value);
        g_pPrefs->UpdatePref(pref);
      }
      else
        Printf(BIC_ERROR,"Bad pref or value");
      return;
    }
    // fall through to error message
  }
  Printf(BIC_ERROR,"ERROR: <pref>=<value>");
}

void CChildFrame::CMD_RefreshPlugins(const char *Args)
{
  g_pPluginManager->RefreshPlugins();
}

void CChildFrame::CMD_ListPlugins(const char *Args)
{
  g_pPluginManager->ListPlugins();
}

void CChildFrame::CMD_UnloadPlugin(char *Args)
{
  if (Args && *Args)
  {
    int Result;
    char *name = strtok(Args," ");
    char *params = strtok(NULL,"");
    char *param;

    BOOL IgnoreInUse = FALSE;
    BOOL Force = FALSE;

    if (params)
    {
      param = strtok(params," ");
      while (param)
      {

        if (param && stricmp(param,"FORCE") == 0)
          Force = TRUE;
        else if (param && stricmp(param,"IGNOREINUSE") == 0)
          IgnoreInUse = TRUE;

        param = strtok(NULL," ");
      }
    }

    Result = g_pPluginManager->UnloadByName(name, Force,IgnoreInUse);
    Printf(BIC_INFO,"Unload plugin \"%s\" : %s",name, g_PluginManagerErrorNames[Result]);
    return;
  }
  Printf(BIC_ERROR,"ERROR: <plugin name>");
}

void CChildFrame::CMD_GetNick(char *Args)
{
  if (!Args || !*Args)
  {
    Printf(BIC_ERROR,"ERROR: <nick>");
    return;
  }

  char *Nick = strtok(Args," ");
  if (Nick)
  {
    SetString(m_pServer->m_ReclaimNick,Nick);
    g_pMainWnd->SetStatusBar(STATUS_ICON_DONTCHANGE,NULL); // update the nick the status bar
    m_pServer->CMD_Nick(Nick);
  }
}

void CChildFrame::CMD_Nick(char *Args)
{
  if (!Args || !*Args)
  {
    Printf(BIC_ERROR,"ERROR: <nick>");
    return;
  }
  char *Nick = strtok(Args," ");
  if (Nick)
  {
    SetString(m_pServer->m_ReclaimNick,NULL);
    m_pServer->CMD_Nick(Args);
  }
}

void CChildFrame::CMD_Window(const char *Args)
{
  /*
   * support:
   *
   * SHOW
   * HIDE
   * MAXIMIZE
   * POSITION
   * USERLIST [TOGGLE|SHOW|HIDE]
   * HEADER [TOGGLE|SHOW|HIDE]
   * INFO [TOGGLE|SHOW|HIDE]
   * CLEAR [ALL]
   */

  char *ArgsCopy = strdup(Args);
  char *arg = strtok(ArgsCopy," ");
  char *rest = strtok(NULL,"");

  bool OK=FALSE;
  if (arg && *arg)
  {
    if (stricmp(arg,"SHOW") == 0)
    {
      ActivateWindow();
      OK = TRUE;
    }
    else if (stricmp(arg,"HIDE") == 0)
    {
      HideWindow();
      OK = TRUE;
    }
    else if (stricmp(arg,"MAXIMIZE") == 0)
    {
      ShowWindow(SW_MAXIMIZE);
      OK = TRUE;
    }
    else if (stricmp(arg,"POSITION") == 0)
    {
      if (rest && *rest)
      {
        int x,y,w,h;
        if (sscanf(rest,"%d %d %d %d",&x,&y,&w,&h) == 4)
        {
          // do a basic check
          if (x >= 0 && y >= 0 && w >= 50 && h >= 50 &&
              x < 65535 && y < 65535 && w < 65535 && h < 65535) // todo: replace 65535 with a better value
          {
            SetWindowPos(HWND_TOP,x,y,w,h,SWP_NOZORDER|SWP_NOACTIVATE);
            OK = TRUE;
          }
        }
        // fall through to error message
      }
      if (!OK)
      {
        Printf(BIC_ERROR,"ERROR: POSITION <x> <y> <w> <h>");
        OK = TRUE;
      }
    }
    else if (stricmp(arg,"USERLIST") == 0)
    {
      if (rest && *rest)
      {
        if (stricmp(rest,"TOGGLE") == 0)
        {
          ToggleUserList();
          OK = TRUE;
        }
        else if (stricmp(rest,"SHOW") == 0)
        {
          ShowUserList();
          OK = TRUE;
        }
        else if (stricmp(rest,"HIDE") == 0)
        {
          HideUserList();
          OK = TRUE;
        }
      }
      if (!OK)
      {
        Printf(BIC_ERROR,"ERROR: USERLIST TOGGLE|SHOW|HIDE");
        OK = TRUE;
      }
    }
    else if (stricmp(arg,"HEADER") == 0)
    {
      if (rest && *rest)
      {
        if (stricmp(rest,"TOGGLE") == 0)
        {
          ToggleHeader();
          OK = TRUE;
        }
        else if (stricmp(rest,"SHOW") == 0)
        {
          ShowHeader();
          OK = TRUE;
        }
        else if (stricmp(rest,"HIDE") == 0)
        {
          HideHeader();
          OK = TRUE;
        }
      }
      if (!OK)
      {
        Printf(BIC_ERROR,"ERROR: HEADER TOGGLE|SHOW|HIDE");
        OK = TRUE;
      }
    }
    else if (stricmp(arg,"INFO") == 0)
    {
      if (rest && *rest)
      {
        if (stricmp(rest,"TOGGLE") == 0)
        {
          ToggleInfo();
          OK = TRUE;
        }
        else if (stricmp(rest,"SHOW") == 0)
        {
          ShowInfo();
          OK = TRUE;
        }
        else if (stricmp(rest,"HIDE") == 0)
        {
          HideInfo();
          OK = TRUE;
        }
      }
      if (!OK)
      {
        Printf(BIC_ERROR,"ERROR: INFO TOGGLE|SHOW|HIDE");
        OK = TRUE;
      }
    }
    else if (stricmp(arg,"CLEAR") == 0)
    {
      if (rest)
      {
        int i;
        CChildFrame *pChild;
        for (i = 0; i < g_ChildWndList.GetSize(); i++)
        {
          pChild = CHILDFRAMEPTR(g_ChildWndList[i]);

	        if (((stricmp(rest,"CHANNELS") == 0)
            || (stricmp(rest,"-c") == 0))
            && (pChild->m_WindowType == CWTYPE_CHANNEL))
		      { 
          pChild->m_MsgView.ClearBuffer();
        }
          else if (((stricmp(rest,"SERVERS") == 0)
              ||   (stricmp(rest,"-s") == 0))
              &&   (pChild->m_WindowType == CWTYPE_SERVER))
          {
            pChild->m_MsgView.ClearBuffer();
          }
          else if (((stricmp(rest,"DCCCHATS") == 0)
              ||   (stricmp(rest,"-d") == 0))
              &&   (pChild->m_WindowType == CWTYPE_DCCCHAT))
          {
            pChild->m_MsgView.ClearBuffer();
          }
          else if (((stricmp(rest,"QUERIES")  == 0) || (stricmp(rest,"-q") == 0)
              ||   (stricmp(rest,"MESSAGES") == 0) || (stricmp(rest,"-m") == 0))
              && (pChild->m_WindowType == CWTYPE_QUERY))
          {
            pChild->m_MsgView.ClearBuffer();
          }
          else if (stricmp(rest,"ALL") == 0) { pChild->m_MsgView.ClearBuffer(); }
        }
      }
      else
      {
        m_MsgView.ClearBuffer();
      }

      ActivateTextInputPane();

      OK=TRUE;
    }
  }
  if (!OK)
  {
    Printf(BIC_ERROR,"ERROR: SHOW|HIDE|MAXIMIZE|POSITION|USERLIST [TOGGLE|SHOW|HIDE]|HEADER [TOGGLE|SHOW|HIDE]|INFO [TOGGLE|SHOW|HIDE]|CLEAR [ALL]");
  }

  if (ArgsCopy) free(ArgsCopy);
}

void CChildFrame::CMD_Timestamps(char *Args)
{
  char *arg = strtok(Args,"=");
  char *value = strtok(NULL,"");
  if (!arg)
  {
    Printf(BIC_UNKNOWN,"TIMESTAMPS are %s", m_MsgView.GetTimeStamps() ? "ON" : "OFF");
  }
  else
  {
    if (stricmp(arg,"TOGGLE") == 0)
    {
      m_MsgView.SetTimeStamps(!m_MsgView.GetTimeStamps());
    }
    else if (stricmp(arg,"ON") == 0)
    {
      m_MsgView.SetTimeStamps(TRUE);
    }
    else if (stricmp(arg,"OFF") == 0)
    {
      m_MsgView.SetTimeStamps(FALSE);
    }
    else if (stricmp(arg,"SET") == 0 && value && *value)
    {
      m_MsgView.SetTimeStampFormat(value);
    }
    else if (stricmp(arg,"USEDEFAULTFORMAT") == 0)
    {
      m_MsgView.SetTimeStampFormat(NULL);
    }
    else
    {
      Printf(BIC_ERROR,"ERROR: [\"TOGGLE|ON|OFF|USEDEFAULTFORMAT|SET=<timestamp>\"]");
    }
  }
}

void CChildFrame::CMD_Action(const char *Msg)
{
  if (!Msg || !*Msg)
  {
    Printf(BIC_ERROR,"ERROR: <message>");
    return;
  }

  if (m_pServer->m_IsDirect)
  {
    m_pServer->CMD_Action(NULL,Msg);
  }
  else if (m_WindowType == CWTYPE_CHANNEL)
  {
    m_pServer->CMD_Action(m_pChannel->m_pProperties->m_Name, Msg);
  }
  else if (m_WindowType == CWTYPE_QUERY)
  {
    m_pServer->CMD_Action(m_pQuery->m_OtherNick, Msg);
  }
}

void CChildFrame::CMD_Monitor( const char *Args )
{
  if (Args && *Args)
  {
    if (stricmp(Args,"STOP") == 0)
    {
      if (m_WindowType == CWTYPE_CHANNEL)
        g_pMainWnd->m_ChannelMonitorView.RemoveChannel(m_pChannel);
      else if (m_WindowType == CWTYPE_SERVER)
        g_pMainWnd->m_ServerMonitorView.RemoveServer(m_pServer);
      return;
    }
    else if (stricmp(Args,"START") == 0)
    {
      // do nothing, follow through below to start monitoring
    }
    else if (stricmp(Args,"STATUS") == 0)
    {
      BOOL Monitoring = FALSE;
      if (m_WindowType == CWTYPE_CHANNEL)
        Monitoring = g_pMainWnd->m_ChannelMonitorView.IsMonitoring(m_pChannel);
      else if (m_WindowType == CWTYPE_SERVER)
        Monitoring = g_pMainWnd->m_ServerMonitorView.IsMonitoring(m_pServer);

      Printf(BIC_UNKNOWN,"MONITORING IS %s", Monitoring ? "ON" : "OFF");
    }
    else
    {
      Printf(BIC_ERROR,"ERROR: [STOP|START|STATUS]");
      return;
    }
  }

  // if no args or if /start is specified...

  if (m_WindowType == CWTYPE_CHANNEL)
    g_pMainWnd->m_ChannelMonitorView.AddChannel(m_pChannel);
  else if (m_WindowType == CWTYPE_SERVER)
    g_pMainWnd->m_ServerMonitorView.AddServer(m_pServer);
}

void CChildFrame::CMD_Logging( const char *Args )
{
  BOOL Logging = FALSE;
  switch (m_WindowType)
  {
    case CWTYPE_CHANNEL:
      Logging = m_pChannel->m_Logger.IsEnabled();
      break;
    case CWTYPE_QUERY:
      Logging = m_pQuery->m_Logger.IsEnabled();
      break;
    case CWTYPE_DCCCHAT:
      Logging = m_pServer->m_Logger.IsEnabled();
      break;
    case CWTYPE_SERVER:
      Logging = m_pServer->m_Logger.IsEnabled();
      break;
  }

  if (Args && *Args)
  {
    if (stricmp(Args,"STOP") == 0)
    {
      if (Logging)
      {
        switch (m_WindowType)
        {
          case CWTYPE_CHANNEL:
            m_pChannel->m_Logger.Stop();
            break;
          case CWTYPE_QUERY:
            m_pQuery->m_Logger.Stop();
            break;
          case CWTYPE_DCCCHAT:
            m_pServer->m_Logger.Stop();
            break;
          case CWTYPE_SERVER:
            m_pServer->m_Logger.Stop();
            break;
        }
      }
      return;
    }
    else if (stricmp(Args,"START") == 0)
    {
      if (!Logging)
      {
        switch (m_WindowType)
        {
          case CWTYPE_CHANNEL:
            m_pChannel->m_Logger.Start();
            break;
          case CWTYPE_QUERY:
            m_pQuery->m_Logger.Start();
            break;
          case CWTYPE_DCCCHAT:
            m_pServer->m_Logger.Start();
            break;
          case CWTYPE_SERVER:
            m_pServer->m_Logger.Start();
            break;
        }
      }
      return;
    }
    else if (stricmp(Args,"STATUS") == 0)
    {
      Printf(BIC_UNKNOWN,"LOGGING IS %s", Logging ? "ON" : "OFF");
      return;
    }
    else
    {
      Printf(BIC_ERROR,"ERROR: [STOP|START|STATUS]");
      return;
    }
  }
}

void CChildFrame::CMD_Cycle( void )
{
  if (m_WindowType != CWTYPE_CHANNEL)
    return;

  m_pChannel->Cycle();
}

void CChildFrame::CMD_Part(char *Args)
{
  // if in a query, just close the query window
  if (m_WindowType == CWTYPE_QUERY)
  {
    PostMessage(WM_CLOSE);
    return;
  }

  // check this is a channel window...
  if (m_WindowType != CWTYPE_CHANNEL)
    return;

  if (!m_pServer->IsLoggedIn())
    return;

  if (!Args || !*Args) // no args, that's ok, part the current channel.
  {
    m_pServer->CMD_Part(m_pChannel->m_pProperties->m_Name,NULL);
  }
  else
  {
    char *channelname = NULL;
    char *message;
    char *rest = Args;
    do
    {
      if (ischannelstartchar(*rest))
      {
        if (strchr(rest,' '))
        {
          channelname = strtok(rest," ");
          rest = strtok(NULL,",");
          message = rest;
        }
        else
        {
          channelname = strtok(rest,",");
          message = NULL;
        }
      }
      else
      {
        message = rest;
        if (strchr(message,','))
          if (channelname)
            message = strtok(NULL,",");
          else
            message = strtok(rest,",");
        channelname = m_pChannel->m_pProperties->m_Name;

      }

      if (channelname)
      {
        IRCChannel *pChannel = m_pServer->FindChannel(channelname);
        if (pChannel)
        {
          m_pServer->CMD_Part(pChannel->m_pProperties->m_Name,message);
        }
        rest = strtok(NULL,"");
      }
    }
    while (rest);
  }
}

/** Create a new server window, and connect to a server
 *
 * /NewServer [irc://]<hostname>[:<port>][,<password>]
 * /Server [irc://]<hostname>[:<port>][,<password>]
 * For new server windows:
 *   the network is defaulted to Unknown
 *   (but corrected by the new IRCServer instance if the server is in the network lists)
 *
 * For existing server windows:
 *   the network and description are taken from the current server
 *   (but corrected by the new IRCServer instance if the server is in the network lists)
 */
void CChildFrame::CMD_Server(char *Args, BOOL NewWindow )
{
  BOOL ShowError = FALSE;
  if (!Args || !*Args)
  {
    ShowError = TRUE;
  }
  else
  {
    // TODO: allow user to provide identityname via command line.

    IRCServerDetails *pServerDetails;
    
    // create a new Details instance taking the defaults as appropriate.
    if (NewWindow)
      pServerDetails = new IRCServerDetails(g_DefaultServerDetails);
    else
      pServerDetails = new IRCServerDetails(m_pServer->m_pDetails);

    if (!pServerDetails)
      return;

    // parse the args.
    if (!pServerDetails->ParseURI(Args))
    {
      ShowError = TRUE;
    }
    else
    {
      // parsed args, pServerDetails is valid.
      if (NewWindow)
      {
        IRCServer *pNewServer = new IRCServer(pServerDetails,(UserIdentity *)NULL); // let the server find an identity to use
      }
      else
      {
        // disconnect, update details, identity and nick, reconnect and update status.
        m_pServer->Disconnect(FALSE);

        m_pServer->m_pDetails->Copy(pServerDetails);

        m_pServer->UpdateUserIdentity();
        m_pServer->ResetNick();

        m_pServer->Reconnect();
        UpdateSettings(FALSE);
      }
    }

    delete pServerDetails;
  }
  if (ShowError)
  {
    Printf(BIC_ERROR,"ERROR: <hostname>[:<port>][,<password>]");
  }
}

void CChildFrame::CMD_Quit( char *Args )
{
  // call even if we're not logged in (we might want to cancel the login...)
  m_pServer->CMD_Quit(Args);
}

void CChildFrame::CMD_Exit( char *Args )
{
  // TODO: store the rest of the /exit args somewhere, and use as the quit message.
  if (Args && stricmp(Args,"NOW") == 0)
  {
    g_pMainWnd->m_OkToQuit = 1; // don't prompt!
  }
  g_pMainWnd->PostMessage(WM_CLOSE);
}

void CChildFrame::CMD_AudioMute( const char *Args )
{
  BOOL OK=FALSE;

  if (Args && *Args)
  {
    if (stricmp(Args,"ON") == 0)
    {
      BOOLPREF(PREF_bAudioMute) = TRUE;
      OK = TRUE;
    }
    else if (stricmp(Args,"OFF") == 0)
    {
      BOOLPREF(PREF_bAudioMute) = FALSE;
      OK = TRUE;
    }
    else if (stricmp(Args,"TOGGLE") == 0)
    {
      BOOLPREF(PREF_bAudioMute) = !BOOLPREF(PREF_bAudioMute); // toggle it
      OK = TRUE;
    }
    else if (stricmp(Args,"STATUS") == 0)
    {
      // do nothing except print the status, below
      OK = TRUE;
    }
  }

  if (OK)
  {
    sys_Printf(BIC_INFO,"Audio Mute is now %s\n",BOOLPREF(PREF_bAudioMute) ? "Enabled" : "Disabled");
  }
  else
  {
    Printf(BIC_ERROR,"ERROR: [ON|OFF|TOGGLE]");
  }

}

void CChildFrame::CMD_Raw( char *Args )
{
  m_pServer->CMD_Raw(Args);
}

CTimerCommand *CChildFrame::GetTimerCommand(char *CommandProfileName)
{
  for (int i=0; i < m_TimerCommands.GetSize(); i++) 
  {
    CTimerCommand *pTimerCommand = (CTimerCommand *)m_TimerCommands[i];
    if (stricmp(pTimerCommand->m_CommandProfileName,CommandProfileName) == 0)
    {
      return pTimerCommand;
    }
  }
  return NULL;
}

void CChildFrame::AddTimerCommand(int Delay, char *CommandProfileName)
{
  CCommandProfile *pCommandProfile = g_pPrefs->FindCommandProfile(CommandProfileName);

  if (!pCommandProfile)
  {
    Printf(BIC_ERROR,"ERROR: Could not find a profile with that name");
    return;
  }


  CTimerCommand *pTimerCommand = GetTimerCommand(CommandProfileName);
  if (pTimerCommand) {
    Printf(BIC_ERROR, "A timer named '%s' has already been started for this window", CommandProfileName);
    return;
  }

  Printf(BIC_INFO, "Starting timer, using command profile %s every %d seconds", CommandProfileName, Delay);

  pTimerCommand = new CTimerCommand(Delay, CommandProfileName, this);
  m_TimerCommands.Add(pTimerCommand);

  g_EventManager.QueueTimedEvent((CListener *)this,FALSE,EV_CW_TIMER,Delay,TRUE,FALSE,pTimerCommand);
}

void CChildFrame::CMD_Timer( char *Args )
{
  int Delay;
  char *DelayString = strtok(Args," ");
  char *CommandProfileName = strtok(NULL,"");

  if (!DelayString || !CommandProfileName || !Args || !*Args || sscanf(DelayString, "%d", &Delay) != 1 )
  {
    Printf(BIC_ERROR,"ERROR: [Delay] [CommandProfileName]");
    return;
  }

  AddTimerCommand(Delay, CommandProfileName);  
}

void CChildFrame::CMD_StopTimer( char *Args )
{
  if (!Args || !*Args)
  {
    Printf(BIC_ERROR,"ERROR: [CommandProfileName]");
    return;
  }

  CTimerCommand *pTimerCommand = GetTimerCommand(Args);
  if (!pTimerCommand)
  {
    Printf(BIC_ERROR,"ERROR: A timer with the name of %s has not been started", Args);
    return;
  }

  m_TimerCommands.Remove(pTimerCommand);
  BOOL Deleted = g_EventManager.DeleteEvents(this, EV_CW_TIMER, pTimerCommand);

  if (Deleted) 
  {
    Printf(BIC_INFO,"A timer with the name of %s was stopped!", Args);
  }
  else
  {
    Printf(BIC_ERROR,"ERROR: A timer with the name of %s could not be stopped!", Args);
  }

}

void CChildFrame::CMD_Exec( char *Args )
{
  if (!Args || !*Args)
  {
    Printf(BIC_ERROR,"ERROR: [CommandProfileName]");
    return;
  }

  CCommandProfile *pCommandProfile = g_pPrefs->FindCommandProfile(Args);

  if (!pCommandProfile)
  {
    Printf(BIC_ERROR,"ERROR: Could not find a profile with that name");
    return;
  }

  char *Command = pCommandProfile->GetFirstCommand();
  while (Command)
  {
    ProcessTextInput(Command,TRUE);
    Command = pCommandProfile->GetNextCommand();
  }
}

/** Join one or more channels
 *
 * Join [#channel [key][,...]
 */
void CChildFrame::CMD_Join(char *Args,int Flags /* = CCF_NONE */ )
{
  if (!Args || !*Args || !m_pServer->IsLoggedIn())
    return;

  char *channelinfo = strtok(Args,",");
  while (channelinfo)
  {
    IRCChannel *pChannel = m_pServer->CreateChannel(channelinfo,Flags);
    if (pChannel)
    {
      pChannel->Join();
    }
    channelinfo = strtok(NULL,",");
  }
}

/* BEGIN_CEL_PATCH */
void CChildFrame::CMD_CLEAR(void)
{
  m_MsgView.ClearBuffer();
}
/* END_CEL_PATCH */

void CChildFrame::CMD_DCC(char *Args)
{

  if (!Args || !*Args || !m_pServer->IsLoggedIn())
    return;

  char *option = strtok(Args," ");
  char *rest = strtok(NULL,"");

  if (!rest)
    Printf(BIC_ERROR,"need more parameters");
  else
  {
    if (stricmp(option,"CHAT") == 0)
      CMD_DCCChat(rest);
    else if (stricmp(option,"SEND") == 0)
      CMD_DCCSend(rest);
    else
      Printf(BIC_ERROR,"Unknown DCC option, use SEND or CHAT");
  }
}


void CChildFrame::CMD_DCCChat(char *Args)
{
  if (!Args || !*Args || !m_pServer->IsLoggedIn())
    return;

  char *nick = strtok(Args," ");

  IRCServerDetails ServerDetails;

  ServerDetails.m_NetworkID = m_pServer->m_pDetails->m_NetworkID;
  ServerDetails.m_Description = HydraIRC_BuildString(128,"%s - DCC Chat Server",nick);
  ServerDetails.m_Name = strdup(nick);
  ServerDetails.m_Password = NULL;
  ServerDetails.m_Port = 0;

  IRCServer *pNewServer = new IRCServer(&ServerDetails,m_pServer,nick,TRUE,TRUE);
}

char *GetPathPart(char *FullPathAndFileName);

/** Send files to a user via DCC
 *
 * DCCSEND [nick] [file][,...]
 */
void CChildFrame::CMD_DCCSend(char *Args)
{
  if (!Args || !*Args || !m_pServer->IsLoggedIn())
    return;

  char *nick = strtok(Args," ");
  char *file = strtok(NULL,",");
  if (!nick)
    return;

  CFileDialog *pFileDialog;

  if (!file)
  {
    // Build the local file name

    char *FolderName;
    char *LastFolderName = PATHACTIVESETTING(AS_pLastDCCSend);
    if (LastFolderName)
      FolderName = HydraIRC_BuildString(MAX_PATH,"%s\\*.*",LastFolderName);
    else
      FolderName = HydraIRC_BuildString(MAX_PATH,"%s\\*.*",PATHPREF(PREF_pDCCUploadsFolder));

    if (!FolderName)
      return;

    ReplaceString(&FolderName,"<HOMEDIR>",g_HomeDir);

    pFileDialog = new CFileDialog(TRUE,"*.*",FolderName);
    if (pFileDialog)
    {
      int result = pFileDialog->DoModal();
      if (result == IDOK)
      {
        // remember the last path used.
        file = pFileDialog->m_szFileName;

        LastFolderName = GetPathPart(file);
        SetString(PATHACTIVESETTING(AS_pLastDCCSend),LastFolderName);
        SetString(LastFolderName,NULL);

        g_pTransfersManager->QueueFileSend(m_pServer,nick, file);
      }
      delete pFileDialog;
    }
    free(FolderName);
  }
  else
  {
    while (file)
    {
      g_pTransfersManager->QueueFileSend(m_pServer,nick, file);

      file = strtok(NULL,",");
    }
  }
}

/** Create one or more query windows, and send a message to the users
 *
 * Query [nick][,...] [message]
 */
void CChildFrame::CMD_Query(char *Args)
{
  // Note: we don't *need* to be logged in to create a query window
  if (!Args || !*Args)
    return;

  char *OtherNick = strtok(Args," "); // get the list of , seperated users
  char *Message = strtok(NULL,""); // get the message

  OtherNick = strtok(OtherNick,","); // get the first user
  while (OtherNick)
  {
    m_pServer->CMD_Query(OtherNick,Message);
    OtherNick = strtok(NULL,","); // any more usernames?
  }
}

void CChildFrame::CMD_Clear(const char *Args )
{
  if (Args && *Args)
  {
    int i;
    CChildFrame *pChild;
    for (i = 0; i < g_ChildWndList.GetSize(); i++)
    {
      pChild = CHILDFRAMEPTR(g_ChildWndList[i]);

	    if (((stricmp(Args,"CHANNELS") == 0)
        || (stricmp(Args,"-c") == 0))
        && (pChild->m_WindowType == CWTYPE_CHANNEL))
		  { 
			  pChild->m_MsgView.ClearBuffer();
		  }
      else if (((stricmp(Args,"SERVERS") == 0)
           ||   (stricmp(Args,"-s") == 0))
           &&   (pChild->m_WindowType == CWTYPE_SERVER))
      {
        pChild->m_MsgView.ClearBuffer();
      }
      else if (((stricmp(Args,"DCCCHATS") == 0)
           ||   (stricmp(Args,"-d") == 0))
           &&   (pChild->m_WindowType == CWTYPE_DCCCHAT))
      {
        pChild->m_MsgView.ClearBuffer();
      }
      else if (((stricmp(Args,"QUERIES")  == 0) || (stricmp(Args,"-q") == 0)
           ||   (stricmp(Args,"MESSAGES") == 0) || (stricmp(Args,"-m") == 0))
           && (pChild->m_WindowType == CWTYPE_QUERY))
      {
        pChild->m_MsgView.ClearBuffer();
      }
      else if (stricmp(Args,"ALL") == 0) { pChild->m_MsgView.ClearBuffer(); }
    }
  }
  else
    m_MsgView.ClearBuffer();

  ActivateTextInputPane();
}

void CChildFrame::CMD_SaveTheme(char *Args)
{
  // Note: we don't *need* to be logged in to create a query window
  if (!Args || !*Args)
    return;

  char *ThemeName = strtok(Args," "); // get the list of , seperated users
  char *FileName = strtok(NULL," "); // get the message

  BOOL Saved = g_pPrefs->SaveTheme(ThemeName, FileName);

  if (!Saved)
    Printf(BIC_ERROR,"Saving theme failed, see output view for details");
}

/*
// TODO: we can probably ditch this
BOOL CChildFrame::ProcessServerCommand(char *Command, char *Args)
{
  if (!Command || !*Command)
    return FALSE;

#if 0 // TODO: add more commands.
  BOOL Handled = TRUE;

  if (stricmp(Command,"<SomeCommandHere>") == 0)
  {
    CMD_<SomeCommandHere>(Args);
  }
  else
  {
    // didn't match a command
    Handled = FALSE;
  }
#else
  BOOL Handled = FALSE;
#endif

  return Handled;
}
*/

/*
// TODO: probably merge this into ProcessCommand() -- MERGED!
BOOL CChildFrame::ProcessChannelCommand(char *Command, char *Args)
{
  if (!Command || !*Command)
    return FALSE;

  BOOL Handled = TRUE;

  // TODO: add commands (topic, etc..)

  if (stricmp(Command,"USERLIST") == 0)
  {
    CMD_ToggleUserList();
  }
  else
  {
    // didn't match a command
    Handled = FALSE;
  }
  return Handled;
}
*/

/** Process a command..
 *
 *  A plugin can callback this function.
 */
BOOL CChildFrame::ProcessCommand(char *Command, char *Args)
{
  // Args may be NULL
  if (!Command || !*Command)
    return FALSE;

  BOOL Handled = TRUE;

  // Global Commands,  can be issued in any text input box.
  if (stricmp(Command,"NEWSERVER") == 0)// || stricmp(Command,"NSERVER") == 0)
  {
    CMD_Server(Args, TRUE);
  }
  else if (stricmp(Command,"EXIT") == 0)
  {
    CMD_Exit(Args);
  }
  else if (stricmp(Command,"UNLOADPLUGIN") == 0)
  {
    CMD_UnloadPlugin(Args);
  }
  else if (stricmp(Command,"REFRESHPLUGINS") == 0)
  {
    CMD_RefreshPlugins(Args);
  }
  else if (stricmp(Command,"LISTPLUGINS") == 0)
  {
    CMD_ListPlugins(Args);
  }
  else if (stricmp(Command,"AUDIOMUTE") == 0)
  {
    CMD_AudioMute(Args);
  }
  else if (stricmp(Command,"TIMESTAMPS") == 0)
  {
    CMD_Timestamps(Args);
  }
  else if (stricmp(Command,"SET") == 0)
  {
    CMD_Set(Args);
  }
  else if (stricmp(Command,"IGNORE") == 0)
  {
    CMD_Ignore(Args);
  }
  else if (stricmp(Command,"HELP") == 0)
  {
    CMD_Help(Args);
  }
  else if (stricmp(Command,"RAW") == 0)// || stricmp(Command,"QUOTE") == 0)
  {
    CMD_Raw(Args);
  }
  else if (stricmp(Command,"EXEC") == 0)
  {
    CMD_Exec(Args);
  }
  else if (stricmp(Command,"TIMER") == 0)
  {
    CMD_Timer(Args);
  }
  else if (stricmp(Command,"STOPTIMER") == 0)
  {
    CMD_StopTimer(Args);
  }
  else if (stricmp(Command,"QUERY") == 0)
  {
    CMD_Query(Args);
  }
  else if (stricmp(Command,"DCC") == 0)
  {
    CMD_DCC(Args);
  }
  else if (stricmp(Command,"DCCSEND") == 0)
  {
    CMD_DCCSend(Args);
  }
  else if (stricmp(Command,"DCCCHAT") == 0)
  {
    CMD_DCCChat(Args);
  }
  else if (stricmp(Command,"CLEAR") == 0)
  {
    CMD_Clear(Args);
  }
  else if (stricmp(Command,"SAVETHEME") == 0)
  {
    CMD_SaveTheme(Args);
  }
  else if (stricmp(Command,"WINDOW") == 0)
  {
    CMD_Window(Args);
  }
  else if (stricmp(Command,"LOGGING") == 0)
  {
    CMD_Logging(Args);
  }
/* BEGIN_CEL_PATCH */
  else if (stricmp(Command,"CLEAR") == 0)
  {
    CMD_CLEAR();
  }
/* END_CEL_PATCH */

  else
  {
    Handled = FALSE;
  }

  // Common commands relative to Channel/DCCChat/Message windows on the same server (not Server)
  if (!Handled &&
      (m_WindowType == CWTYPE_CHANNEL ||
      m_WindowType == CWTYPE_QUERY ||
      m_WindowType == CWTYPE_DCCCHAT))
  {
    Handled = TRUE;
    if (stricmp(Command,"ME") == 0)
    {
      CMD_Action(Args);
    }
    else
    {
      Handled = FALSE;
    }
  }

  // Common commands relative to Channel/Query windows
  if (!Handled &&
      (m_WindowType == CWTYPE_CHANNEL ||
       m_WindowType == CWTYPE_QUERY))
  {
    Handled = TRUE;
    if (stricmp(Command,"PART") == 0)// || stricmp(Command,"LEAVE") == 0)
    {
      CMD_Part(Args);
    }
    else
    {
      Handled = FALSE;
    }
  }

  // Common commands relative to Channel/Server windows
  if (!Handled &&
      (m_WindowType == CWTYPE_CHANNEL ||
       m_WindowType == CWTYPE_SERVER))
  {
    Handled = TRUE;
    if (stricmp(Command,"MONITOR") == 0)
    {
      CMD_Monitor(Args);
    }
    else
    {
      Handled = FALSE;
    }
  }

  // Common commands relative to Channel windows
  if (!Handled && m_WindowType == CWTYPE_CHANNEL)
  {
    Handled = TRUE;
    if (stricmp(Command,"CYCLE") == 0)
    {
      CMD_Cycle();
    }
    else
    {
      Handled = FALSE;
    }
  }

  // Common commands relative to Channel/Server/Message windows on the same server (not DCC)
  if (!Handled &&
      (m_WindowType == CWTYPE_CHANNEL ||
       m_WindowType == CWTYPE_QUERY ||
       m_WindowType == CWTYPE_SERVER))
  {
    Handled = TRUE;

    if (stricmp(Command,"SERVER") == 0)
    {
      CMD_Server(Args);
    }
    else if (stricmp(Command,"JOIN") == 0)// || stricmp(Command,"J") == 0)
    {
      CMD_Join(Args);
    }
    else if (stricmp(Command,"QUIETJOIN") == 0)// || stricmp(Command,"QJ") == 0)
    {
      CMD_Join(Args,CCF_DONTSHOWIFEXISTS);
    }
    else if (stricmp(Command,"NICK") == 0)
    {
      CMD_Nick(Args);
    }
    else if (stricmp(Command,"GETNICK") == 0)
    {
      CMD_GetNick(Args);
    }
    else if (stricmp(Command,"MSG") == 0)
    {
      CMD_PrivMsg(Args);
    }
    else if (stricmp(Command,"NOTICE") == 0)
    {
      CMD_Notice(Args);
    }
    else if (stricmp(Command,"CTCP") == 0)
    {
      CMD_CTCPRequest(Args);
    }
    else if (stricmp(Command,"QUIT") == 0)// || stricmp(Command,"DISCONNECT") == 0)
    {
      CMD_Quit(Args);
    }
    else if (stricmp(Command,"RECONNECT") == 0)
    {
      m_pServer->Disconnect(FALSE);
      m_pServer->Reconnect();
    }
    else
    {
      Handled = FALSE;
    }
  }
  return Handled;
}

/* BEGIN_CEL_PATCH */
void CChildFrame::CheckPluginProcessing(char *Buffer, DWORD Mode)
{
  char *BufferCopy = strdup(Buffer);
  if (!BufferCopy)
    return;
  BOOL Handled = FALSE;
  HydraIRCPlugin *pPlugin = NULL;
  CNode *pNode = NULL;
  char *BufferPtr = BufferCopy;
  char *cmd_ptr = strtok(BufferPtr, " ");
  char *args_ptr = strtok(NULL, "");
  char *Command = strdup(cmd_ptr);
  char *Args = strdup(args_ptr);

  switch (Mode) {
    case CWTYPE_CHANNEL:
      {
        PFN_PROCESSCHANNELCOMMAND pfn_Func;
        pPlugin = NULL;
        pNode = NULL;
        do {
          pfn_Func = (PFN_PROCESSCHANNELCOMMAND)g_pPluginManager->GetFunction(PLUGIN_API_CMDPROC,PLUGIN_CMDPROC_PROCESSCHANNELCOMMAND,&pPlugin,&pNode);
          if (pfn_Func != NULL) {
            Handled = pfn_Func(m_ServerID,m_ChannelID,&Command,&Args);
          }
        } while (!Handled && (HydraIRCPlugin*)(pPlugin)->m_Next);
      }
      break;
    case CWTYPE_SERVER:
      {
        PFN_PROCESSSERVERCOMMAND pfn_Func;
        pPlugin = NULL;
        pNode = NULL;
        do {
          pfn_Func = (PFN_PROCESSSERVERCOMMAND)g_pPluginManager->GetFunction(PLUGIN_API_CMDPROC,PLUGIN_CMDPROC_PROCESSSERVERCOMMAND,&pPlugin,&pNode);
          if (pfn_Func != NULL) {
            Handled = pfn_Func(m_ServerID,&Command,&Args);
          }
        } while (!Handled && (HydraIRCPlugin*)(pPlugin)->m_Next);
      }
      break;
    case CWTYPE_DCCCHAT:
      {
        PFN_PROCESSDCCINPUT pfn_Func;
        pPlugin = NULL;
        pNode = NULL;
        do {
          pfn_Func = (PFN_PROCESSDCCINPUT)g_pPluginManager->GetFunction(PLUGIN_API_CMDPROC,PLUGIN_CMDPROC_PROCESSDCCINPUT,&pPlugin,&pNode);
          if (pfn_Func != NULL) {
            Handled = pfn_Func(m_ServerID,&Command,&Args);
          }
        } while (!Handled && (HydraIRCPlugin*)(pPlugin)->m_Next);
      }
      break;
    case CWTYPE_QUERY:
      {
        PFN_PROCESSQUERYINPUT pfn_Func;
        pPlugin = NULL;
        pNode = NULL;
        do {
          pfn_Func = (PFN_PROCESSQUERYINPUT)g_pPluginManager->GetFunction(PLUGIN_API_CMDPROC,PLUGIN_CMDPROC_PROCESSQUERYINPUT,&pPlugin,&pNode);
          if (pfn_Func != NULL) {
            Handled = pfn_Func(m_ServerID,&Command,&Args);
          }
        } while (!Handled && (HydraIRCPlugin*)(pPlugin)->m_Next);
      }
      break;
  }
  if (Command) free (Command);
  if (Args) free (Args);
  if (BufferCopy) free(BufferCopy);
}
/* END_CEL_PATCH */

void CChildFrame::ProcessTextInput(char *Buffer,BOOL Parse)
{
  char *BufferCopy = strdup(Buffer); // sometimes we pass static strings as arguments to this function...
  if (!BufferCopy)
    return;

  // plugins have 4 points of entry:
  // ProcessInput(char *input)
  // ProcessCommand(char *actualuserinput)
  // ProcessChannelCommand(char *command, char *args)
  // ProcessServerCommand(char *command, char *args)
  // a given plugin can implement any (or all) of them.
  // so you can have an alias plugin, as well as one that
  // can modify just Channel commands (or whatever).

  // TODO: pass input through to plugin here
  // the idea is that the plugins do what they want, and callback to
  // the following functions:
  //   CChildFrame::ProcessServerCommand
  //   CChildFrame::ProcessChannelCommand
  //   CChildFrame::ProcessCommand
  // and then the plugin tells us if we still need to process
  // the command.
  // By doing this here, the plugins can make aliases for actual commands, etc..
  // so internally we have a set of command (/JOIN etc..)
  // but the plugin could accept "/J" as a command, then call
  // ProcessCommand("/JOIN",Args); which would then trigger the actual
  // join command to be issued by the core.

  BOOL Handled = FALSE;
  // Handled = Plugins->ProcessInput(command);


  // Heh,  this is totally sweet...
  // We keep calling CPluginManager::GetFunction() until it
  // returns NULL (which means, no more plugins that provide the function
  // we want)
  // If it does return a pointer to a function then we call it and we
  // set the Handled flag to it's return value.
  HydraIRCPlugin *pPlugin = NULL;
  CNode *pNode = NULL;

  if (Parse)
  {
    PFN_PROCESSINPUT pfn_Func; // Keep variable name this local
    do
    {
      pfn_Func = (PFN_PROCESSINPUT)g_pPluginManager->GetFunction(PLUGIN_API_CMDPROC,PLUGIN_CMDPROC_PROCESSINPUT,&pPlugin,&pNode);

      if (pfn_Func != NULL)
      {
        Handled = pfn_Func(m_ServerID,m_ChannelID,&BufferCopy);
      }
    } while (!Handled && pfn_Func != NULL);
  }

  char *BufferPtr = BufferCopy;

  if (!Handled) // the command wasn't handled by a plugin
  {
    if (Parse && *BufferPtr == '/')
    {
      BufferPtr++;

      if (*BufferPtr) // just a "/", if so ignore.
      {
        char *cmd_ptr = strtok(BufferPtr, " ");
        char *args_ptr = strtok(NULL, "");

        // take a copy of the command and args so that the plugins can return
        // a modified copy that can be of a different length
        char *Command = strdup(cmd_ptr);
        char *Args = strdup(args_ptr);

        // start from the first plugin and first api again.
        pPlugin = NULL;
        pNode = NULL;

        switch (m_WindowType)
        {
          case CWTYPE_CHANNEL:
            {
              // Process channel specific commands

              PFN_PROCESSCHANNELCOMMAND pfn_Func;
              do
              {
                pfn_Func = (PFN_PROCESSCHANNELCOMMAND)g_pPluginManager->GetFunction(PLUGIN_API_CMDPROC,PLUGIN_CMDPROC_PROCESSCHANNELCOMMAND,&pPlugin,&pNode);

                if (pfn_Func != NULL)
                {
                  Handled = pfn_Func(m_ServerID,m_ChannelID,&Command,&Args);
                }
              } while (!Handled && pfn_Func != NULL);
            }
            break;
          case CWTYPE_SERVER:
            {
              // Process channel specific commands

              PFN_PROCESSSERVERCOMMAND pfn_Func;
              do
              {
                pfn_Func = (PFN_PROCESSSERVERCOMMAND)g_pPluginManager->GetFunction(PLUGIN_API_CMDPROC,PLUGIN_CMDPROC_PROCESSSERVERCOMMAND,&pPlugin,&pNode);

                if (pfn_Func != NULL)
                {
                  Handled = pfn_Func(m_ServerID,&Command,&Args);
                }
              } while (!Handled && pfn_Func != NULL);
            }
            break;
          case CWTYPE_DCCCHAT:
            {
              // TODO!
            }
            break;
          case CWTYPE_QUERY:
            {
              // TODO!
            }
            break;

        } // Switch

        if (!Handled)
        {
          // Process commands that can be typed in anywhere
          // start from the first plugin and first api again.
          pPlugin = NULL;
          pNode = NULL;

          PFN_PROCESSCOMMAND pfn_Func;
          do
          {
            pfn_Func = (PFN_PROCESSCOMMAND)g_pPluginManager->GetFunction(PLUGIN_API_CMDPROC,PLUGIN_CMDPROC_PROCESSCOMMAND,&pPlugin,&pNode);

            if (pfn_Func != NULL)
            {
              Handled = pfn_Func(m_ServerID,m_ChannelID,&Command,&Args);
            }
          } while (!Handled && pfn_Func != NULL);

          Handled = ProcessCommand(Command,Args);
        }

        // If we've still not Handled the /command, tell the user!
        if (!Handled)
        {
          char *RawCommand = HydraIRC_BuildString(512+1,"%s %s",Command,UseEmptyString(Args));
          if (RawCommand)
          {
            stripwhitespace(RawCommand); // if no args, strip the whitespace from the end of the string, just in case the ircd doesn't like it.
            CMD_Raw(RawCommand);
            free(RawCommand);
          }
          //Printf(BIC_ERROR,"Command not implemented or bad/not enough paramaters\"%s\"",Command);
        }

        if (Command) free (Command);
        if (Args) free (Args);

      }
    }
    else
    {
      // ok, so it's not a command, so depending on what kind of window
      // "this" is we need to handle the text accordingly
      switch (m_WindowType)
      {
        case CWTYPE_QUERY:
          // Send to channel
          // TODO: check for "+m" (moderated channel) here.
          m_pServer->CMD_PrivMsg(m_pQuery->m_OtherNick, BufferPtr);
          break;
        case CWTYPE_DCCCHAT:
          // Send to other user
          m_pServer->CMD_PrivMsg(NULL, BufferPtr);
          break;
        case CWTYPE_CHANNEL:
          // Send to channel
          // TODO: check for "+m" (moderated channel) here.
          m_pServer->CMD_PrivMsg(m_pChannel->m_pProperties->m_Name, BufferPtr);
          break;

        case CWTYPE_SERVER:
          // I dunno what the user was expecting that to do! :)
          Printf(BIC_ERROR,"ERROR: You need to prefix commands with a / character.");
          break;
      }
    }
  }
  free(BufferCopy);
}

LRESULT CChildFrame::OnTextInput(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
  // wParam = PARSE flag
  // lParam = Buffer Pointer
  char *buffer = (char *)lParam;

//  bHandled = TRUE;
  if (!lParam)
    return 0;

#ifdef DEBUG
  //sys_Printf(BIC_INFO,buffer);
#endif

  // remove leading/trailing spaces and tabs - only when wParam is non-zero
  if(wParam & OTIF_StripWhitespace)
  {
    while (*buffer == ' ' || *buffer == '\t')
      buffer++;
    stripwhitespace(buffer);
  }

  // still got something ?
  if (*buffer)
  {
    ProcessTextInput(buffer,wParam & OTIF_Parse ? TRUE : FALSE);
  }
  free ((char *)lParam); // finished with the data..

  return 0;
}

/** Sets up the output window
 *
 *  Here we setup the output windows styles, as configured by the user's
 *  preferences.
 *  Only called at window creation time unlike UpdateChildFrameLayout which is
 *  called after prefs changes.
 */
void CChildFrame::InitialiseWindows( void )
{
  switch (m_WindowType)
  {
    case CWTYPE_SERVER:
      m_MsgView.SetTimeStamps(BOOLPREF(PREF_bServerWindowTimestamps));
      break;
    case CWTYPE_QUERY:   // TODO: it's using channel prefs for now.
      m_MsgView.SetTimeStamps(BOOLPREF(PREF_bQueryWindowTimestamps));
      SetIcon(AtlLoadIcon(MAKEINTRESOURCE(IDI_QUERY)));
      break;
    case CWTYPE_DCCCHAT: // TODO: it's using channel prefs for now.
      m_MsgView.SetTimeStamps(BOOLPREF(PREF_bDCCChatWindowTimestamps));
      break;
    case CWTYPE_CHANNEL:
      m_MsgView.SetTimeStamps(BOOLPREF(PREF_bChannelWindowTimestamps));
      SetIcon(AtlLoadIcon(MAKEINTRESOURCE(IDI_CHANNEL)));
      break;
  }

  RECT r;
  GetClientRect(&r);
  m_HorizSplitter1.SetSplitterPos(14);
  m_VertSplitter1.SetSplitterPos(r.right - INTPREF(PREF_nDefaultUserListWidth));

  m_VertSplitter1.GetClientRect(&r);
  m_HorizSplitter3.SetSplitterPos(r.bottom - INTPREF(PREF_nDefaultInfoHeight));

  // must be done after initial setting of the m_HorizSplitter3 splitter bar position
  UpdateSettings(FALSE);


  // set the active pane and the focus
  ActivateTextInputPane();

  // move window into position
  if (m_pWindowPrefs && m_pWindowPrefs->m_Flags & WPF_HAVEPOSITIONS)
  {
    BOOL Maximized;
    HWND hWnd = g_pMainWnd->MDIGetActive(&Maximized);

    if (!Maximized)
    {
      SetWindowPos(HWND_TOP,
        m_pWindowPrefs->m_WindowPositions.x,
        m_pWindowPrefs->m_WindowPositions.y,
        m_pWindowPrefs->m_WindowPositions.w,
        m_pWindowPrefs->m_WindowPositions.h,
        SWP_NOZORDER);
    }
  }

}

void CChildFrame::UpdateSettings( BOOL IgnoreWindowPrefs )
{
  if (!IgnoreWindowPrefs)
    GetWindowPrefs();
  m_MsgView.SetMaxBufferLines(INTPREF(PREF_nMaxScrollBufferLines));

  switch (m_WindowType)
  {
    case CWTYPE_SERVER:
      m_MsgView.SetFont(FONTPREF(PREF_fServerOutputFont));
      m_TextInputView.SetFont(GetAppFont(PREF_fServerInputFont));
      m_MsgView.SetControlCodeMode(BOOLPREF(PREF_bStripCodesFromServerWindows) ? CTRLCODE_MODE_STRIP : CTRLCODE_MODE_INTERPRET);
      break;
    case CWTYPE_QUERY:
      m_MsgView.SetFont(FONTPREF(PREF_fQueryOutputFont));
      m_TextInputView.SetFont(GetAppFont(PREF_fQueryInputFont));
      m_MsgView.SetControlCodeMode(BOOLPREF(PREF_bStripCodesFromQueryWindows) ? CTRLCODE_MODE_STRIP : CTRLCODE_MODE_INTERPRET);
      break;
    case CWTYPE_DCCCHAT:
      m_MsgView.SetFont(FONTPREF(PREF_fDCCChatOutputFont));
      m_TextInputView.SetFont(GetAppFont(PREF_fDCCChatInputFont));
      m_MsgView.SetControlCodeMode(BOOLPREF(PREF_bStripCodesFromDCCChatWindows) ? CTRLCODE_MODE_STRIP : CTRLCODE_MODE_INTERPRET);
      break;
    case CWTYPE_CHANNEL:
      m_MsgView.SetFont(FONTPREF(PREF_fChannelOutputFont));
      m_TextInputView.SetFont(GetAppFont(PREF_fChannelInputFont));
      m_MsgView.SetControlCodeMode(BOOLPREF(PREF_bStripCodesFromChannelWindows) ? CTRLCODE_MODE_STRIP : CTRLCODE_MODE_INTERPRET);
      break;
  }

  m_HeaderView.SetColors(m_ColorPrefs);
  m_MsgView.SetColors(m_ColorPrefs);
  m_UserListView.SetColors(m_ColorPrefs);
  m_InfoView.SetColors(m_ColorPrefs);
  m_TextInputView.SetColors(m_ColorPrefs);

  m_HeaderView.UpdateSettings();
  // m_MsgView.UpdateSettings(); // we don't have this function, colors are not updated on the fly yet.
  m_UserListView.UpdateSettings();
  m_InfoView.UpdateSettings();
  m_TextInputView.UpdateSettings();

  m_UserListPane.SetGradientFill(BOOLPREF(PREF_bUseGradientFilledPanes));
  m_InfoPane.SetGradientFill(BOOLPREF(PREF_bUseGradientFilledPanes));

	m_UserListPane.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
	m_InfoPane.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);

  RECT r;
  m_HorizSplitter2.GetClientRect(&r);
  m_HorizSplitter2.SetSplitterPos(r.bottom - m_TextInputView.GetRequiredHeight(m_WindowType));

}

/** Handle network/socket events
 *
 */
LRESULT CChildFrame::OnNetEvent(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
  //bHandled = TRUE;

  int Event = WSAGETSELECTEVENT(lParam);
  int Err = WSAGETSELECTERROR(lParam);

  if (m_pServer->m_pServerSocket && m_pServer->m_pServerSocket->GetSocket() == (SOCKET)wParam)
    m_pServer->OnServerSocket(Event, Err);
  else
    m_pServer->OnNetEvent(Event, Err);

  return 0;
}

/** Handle network receive queue
 *
 */
LRESULT CChildFrame::OnProcessQueue(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
  //bHandled = TRUE;
  m_pServer->OnProcessQueue();
  return 0;
}


void CChildFrame::ProcessBuffer( void )
{
#ifdef VERBOSE_DEBUG
  sys_printf(BIC_FUNCTION,"CChildFrame::ProcessBuffer()\n");
#endif
  switch (m_WindowType)
  {
    case CWTYPE_SERVER:
    case CWTYPE_DCCCHAT:
      m_pServer->ProcessOutputBuffer();
      break;
    case CWTYPE_CHANNEL:
      m_pChannel->ProcessOutputBuffer();
      break;
    case CWTYPE_QUERY:
      m_pQuery->ProcessOutputBuffer();
      break;
  }
#ifdef VERBOSE_DEBUG
  sys_printf(BIC_FUNCTION,"Exiting CChildFrame::ProcessBuffer()\n");
#endif
}

// buffer is freed automatically later...
void CChildFrame::Put(const int Contents, char *buffer)
{
  if (!buffer)
    return;

  switch (m_WindowType)
  {
    case CWTYPE_SERVER:
    case CWTYPE_DCCCHAT:
      m_pServer->AddToOutputBuffer(Contents, buffer);
      break;
    case CWTYPE_CHANNEL:
      m_pChannel->AddToOutputBuffer(Contents, buffer);
      break;
    case CWTYPE_QUERY:
      m_pQuery->AddToOutputBuffer(Contents, buffer);
      break;
    default: // wtf? how did we get here ? :)
      free(buffer);
  }
}

#define CCHILDFRAME_PRINTF_BUFSIZE 4096

void CChildFrame::Printf(const int Contents, const char *format, ...)
{
  if (!format)
    return;

	va_list args;
	char *CChildFramePrintfBuf;

  CChildFramePrintfBuf = (char *)malloc(CCHILDFRAME_PRINTF_BUFSIZE);
  if (CChildFramePrintfBuf)
  {
	  va_start(args, format);
	  _vsnprintf(CChildFramePrintfBuf, CCHILDFRAME_PRINTF_BUFSIZE, format, args);
	  va_end(args);
    CChildFramePrintfBuf[CCHILDFRAME_PRINTF_BUFSIZE-1] = 0;

    Put(Contents,CChildFramePrintfBuf);
  }
}


LRESULT CChildFrame::OnBufferWaiting(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  bHandled = TRUE;
  ProcessBuffer();
  return 0;
}

/*
LRESULT CChildFrame::OnEnSelChange(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
  bHandled = FALSE;
  SELCHANGE *selection = (SELCHANGE *)pnmh;

  if (pnmh->hwndFrom == m_MsgView.m_hWnd)
  {
    if (selection->seltyp == SEL_EMPTY)
    {
      m_TextInputView.SetFocus();
    }
  }
  return 0;
}
*/

LRESULT CChildFrame::OnCycleChannel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  switch (m_WindowType)
  {
    case CWTYPE_CHANNEL:
      m_pChannel->Cycle();
      break;
  }
  return 0;
}

LRESULT CChildFrame::OnViewLogFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CLogger *pLogger = NULL;

  switch (m_WindowType)
  {
    case CWTYPE_QUERY:
      pLogger = &m_pQuery->m_Logger;
      break;
    case CWTYPE_CHANNEL:
      pLogger = &m_pChannel->m_Logger;
      break;
    case CWTYPE_DCCCHAT:
    case CWTYPE_SERVER:
      pLogger = &m_pServer->m_Logger;
      break;
  }


  char *LogFileName = pLogger ? pLogger->GetFileName() : NULL;

  if (!LogFileName)
  {
    sys_Printf(BIC_ERROR,"No logfile for this window\n");
    return 0;
  }
  else
  {
    pLogger->Flush();
    char *Command = HydraIRC_BuildString(MAX_PATH * 2,"\"%s\" %s", STRINGPREF(PREF_sLogFileViewer), LogFileName);
    if (Command)
    {
      WinExec(Command,SW_NORMAL);
      free(Command);
    }
    free(LogFileName);
  }

  return 0;
}

LRESULT CChildFrame::OnDCCSend(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  switch (m_WindowType)
  {
    case CWTYPE_QUERY:
      char *Command = HydraIRC_BuildString(512,"/dcc send %s",m_pQuery->m_OtherNick);
      ProcessTextInput(Command,TRUE);
      free(Command);
      break;
  }
  return 0;
}

LRESULT CChildFrame::OnDCCChat(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  switch (m_WindowType)
  {
    case CWTYPE_QUERY:
      char *Command = HydraIRC_BuildString(512,"/dcc chat %s",m_pQuery->m_OtherNick);
      ProcessTextInput(Command,TRUE);
      free(Command);
      break;
  }
  return 0;
}

LRESULT CChildFrame::OnWhoIs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  switch (m_WindowType)
  {
    case CWTYPE_QUERY:
      char *Command = HydraIRC_BuildString(512,"/whois %s",m_pQuery->m_OtherNick);
      ProcessTextInput(Command,TRUE);
      free(Command);
      break;
  }
  return 0;
}

void CChildFrame::DoContextMenu( void )
{
  UINT ContextMenuID = 0;
  switch(this->m_WindowType)
  {
    case CWTYPE_SERVER:
      ContextMenuID = IDR_SERVER_CONTEXT;
      break;
    case CWTYPE_CHANNEL:
      ContextMenuID = IDR_CHANNEL_CONTEXT;
      break;
    case CWTYPE_QUERY:
      ContextMenuID = IDR_QUERY_CONTEXT;
      break;
    case CWTYPE_DCCCHAT:
      ContextMenuID = IDR_DCCCHAT_CONTEXT;
      break;
  }

  if (!ContextMenuID)
    return;

  // Display the menu at the current mouse location.
	CMenuHandle menuContext;
	menuContext.LoadMenu(ContextMenuID);
	CMenuHandle menuPopup(menuContext.GetSubMenu(0));


  UINT Check;
  Check = m_MsgView.GetTimeStamps() ? MF_CHECKED : MF_UNCHECKED;
  menuPopup.CheckMenuItem(ID_OPTIONS_TIMESTAMPS,Check);

  // get the verbose submenu.
	CMenuHandle menuPopupSub(menuPopup.GetSubMenu(11));

  Check = (m_MsgView.GetFilterList() == NULL) ? MF_CHECKED : MF_UNCHECKED;
  menuPopupSub.CheckMenuItem(IDC_CHILDFRAME_VERBOSELEVEL_LEVEL0,Check);

  Check = (m_MsgView.GetFilterList() == g_DefaultFilterList_VerboseLevel1) ? MF_CHECKED : MF_UNCHECKED;
  menuPopupSub.CheckMenuItem(IDC_CHILDFRAME_VERBOSELEVEL_LEVEL1,Check);

  Check = (m_MsgView.GetFilterList() == g_DefaultFilterList_VerboseLevel2) ? MF_CHECKED : MF_UNCHECKED;
  menuPopupSub.CheckMenuItem(IDC_CHILDFRAME_VERBOSELEVEL_LEVEL2,Check);

  Check = (m_MsgView.GetFilterList() == g_DefaultFilterList_VerboseLevel3) ? MF_CHECKED : MF_UNCHECKED;
  menuPopupSub.CheckMenuItem(IDC_CHILDFRAME_VERBOSELEVEL_LEVEL3,Check);

  switch(this->m_WindowType)
  {
    case CWTYPE_QUERY:
      // check checkboxes as appropriate
      Check = (m_pQuery->m_Logger.IsEnabled()) ? MF_CHECKED : MF_UNCHECKED;
      menuPopup.CheckMenuItem(ID_OPTIONS_LOGGING,Check);
      break;
    case CWTYPE_DCCCHAT:
      // check checkboxes as appropriate
      Check = (m_pServer->m_Logger.IsEnabled()) ? MF_CHECKED : MF_UNCHECKED;
      menuPopup.CheckMenuItem(ID_OPTIONS_LOGGING,Check);
      break;
    case CWTYPE_SERVER:
      // check checkboxes as appropriate
      Check = (m_pServer->m_Logger.IsEnabled()) ? MF_CHECKED : MF_UNCHECKED;
      menuPopup.CheckMenuItem(ID_OPTIONS_LOGGING,Check);
      Check = (g_pMainWnd->m_ServerMonitorView.IsMonitoring(m_pServer)) ? MF_CHECKED : MF_UNCHECKED;
      menuPopup.CheckMenuItem(ID_SERVER_MONITOR,Check);
      break;
    case CWTYPE_CHANNEL:
      // check checkboxes as appropriate
      Check = (g_pMainWnd->m_ChannelMonitorView.IsMonitoring(m_pChannel)) ? MF_CHECKED : MF_UNCHECKED;
      menuPopup.CheckMenuItem(ID_CHANNEL_MONITOR,Check);
      Check = (IsUserListVisible()) ? MF_CHECKED : MF_UNCHECKED;
      menuPopup.CheckMenuItem(ID_OPTIONS_TOGGLEUSERLIST,Check);
      Check = (m_pChannel->m_Logger.IsEnabled()) ? MF_CHECKED : MF_UNCHECKED;
      menuPopup.CheckMenuItem(ID_OPTIONS_LOGGING,Check);

      int *CurrentFilterList = m_MsgView.GetFilterList();
      Check = (CurrentFilterList == NULL) ? MF_CHECKED : MF_UNCHECKED;
      menuPopup.CheckMenuItem(IDC_CHILDFRAME_VERBOSELEVEL_LEVEL0,Check);
      Check = (CurrentFilterList == g_DefaultFilterList_VerboseLevel1) ? MF_CHECKED : MF_UNCHECKED;
      menuPopup.CheckMenuItem(IDC_CHILDFRAME_VERBOSELEVEL_LEVEL1,Check);
      Check = (CurrentFilterList == g_DefaultFilterList_VerboseLevel2) ? MF_CHECKED : MF_UNCHECKED;
      menuPopup.CheckMenuItem(IDC_CHILDFRAME_VERBOSELEVEL_LEVEL2,Check);
      Check = (CurrentFilterList == g_DefaultFilterList_VerboseLevel3) ? MF_CHECKED : MF_UNCHECKED;
      menuPopup.CheckMenuItem(IDC_CHILDFRAME_VERBOSELEVEL_LEVEL3,Check);

      break;
  }

  POINT pt;
  ::GetCursorPos(&pt);
  g_pMainWnd->m_CmdBar.TrackPopupMenu(menuPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y);
  //::TrackPopupMenu(menuPopup, 0, pt.x, pt.y, 0, m_hWnd, NULL);
}

// sent by the main frame when user presses ctrl+c or via the menu.
LRESULT CChildFrame::OnEditCopy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  // forward the WM_COPY message based on the active frame window

  CWindow DestinationWnd = m_TextInputView;

  if (m_HorizSplitter1.GetActivePane() == SPLIT_PANE_TOP)
    DestinationWnd = m_HeaderView;
  else
    if (m_VertSplitter1.GetActivePane() == SPLIT_PANE_RIGHT)
      if (m_HorizSplitter3.GetActivePane() == SPLIT_PANE_BOTTOM)
        DestinationWnd = m_InfoView;


  DestinationWnd.SendMessage(WM_COPY,0,0);

  return 0; // WM_COPY - no return value
}

// When the user drags a selection, and then releases the mouse
// we copy the select to the clipboard and then set the focus back to the
// input view
LRESULT CChildFrame::OnEnMsgFilter(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
  bHandled = FALSE;
#ifdef DEBUG
  //sys_Printf(BIC_GUI,"child message: %d - %d\n",idCtrl,pnmh->code);
  //sys_Printf(BIC_GUI,"msg: %d - %x\n",msgfilter->msg, msgfilter->msg);
/*
  WM_KEYUP
  WM_LBUTTONUP
  WM_MOUSEMOVE
*/
#endif
  MSGFILTER *msgfilter = (MSGFILTER *)pnmh;
  //m_MsgView.HideCaret();
  if (pnmh->hwndFrom == this->m_TextInputView.m_hWnd)
  {
    switch(msgfilter->msg)
    {
      case WM_MOUSEWHEEL:
      {
        m_MsgView.SendMessage(WM_MOUSEWHEEL,msgfilter->wParam,msgfilter->lParam);
        bHandled = TRUE;
      }
      break;
    }
  }
  else if (pnmh->hwndFrom == m_MsgView.m_hWnd)
  {
    switch(msgfilter->msg)
    {
      case WM_RBUTTONUP:
        {
          bHandled = TRUE;
          DoContextMenu();
        }
        break;
      case WM_LBUTTONUP:
        {
          CHARRANGE cr;
          m_MsgView.GetSel(cr);
          if (cr.cpMin != cr.cpMax)
          {
            // if we click, drag and let go, we copy to the clipboard.
            m_MsgView.SendMessage(WM_COPY,0,0);
            m_MsgView.SetSel(cr.cpMax,cr.cpMax);
            ActivateTextInputPane();
          }
          else
          {
            // if we just click we want to focus the input box instead
            // *BUT* we also need to allow link double clicking
            // and we don't get a doubleclick message
            // if we focus on the text input window after each single click.

            if (!m_MsgView.IsLink(cr))
              ActivateTextInputPane();
          }

          // unlock window
          m_MsgView.SetScrollLock(MBSL_UNLOCKED);

        }
        break;
      default:
        {
          // for all other messages, call the original function.
          // TODO: this crappy behaviour probably needs to replaced with some
          // better altmessage maps or dynamic message maps or something.

          BOOL TempHandled = TRUE;
          m_MsgView.OnMsgFilter(idCtrl, pnmh, TempHandled);
        }

    }

  }
  /*
    EN_MSGFILTER
    If the control should process the event, the message returns a zero value.
    If the control should ignore the event, the message returns a nonzero value.
  */
  return bHandled ? 0 : 1;
}

//+ <gliptic> Drag and drop files                                                       
BOOL CChildFrame::IsReadyForDrop(void)                                                  
{                                                                                       
  if(m_WindowType == CWTYPE_QUERY)                                                      
    return true;                                                                        
  else                                                                                  
    return false;                                                                       
}                                                                                       
                                                                                        
BOOL CChildFrame::HandleDroppedFile(LPCTSTR szBuff)                                     
{                                                                                       
  // Implement this.                                                                    
  // Return TRUE unless you're done handling files (e.g., if you want                   
  // to handle only the first relevant file, and you have already found it).            
  ATLTRACE("%s\n", szBuff);                                                             
                                                                                        
  char buffer[MAX_PATH + 97];                                                           
  sprintf(buffer, "%s %s", m_pQuery->m_OtherNick, szBuff); //TODO: Cluster DCC transfers
  CMD_DCCSend(buffer);                                                                  
                                                                                        
  return TRUE;                                                                          
}                                                                                       

void CChildFrame::EndDropFiles(void)                                                    
{                                                                                       
  return; //Do nothing for now                                                          
}                                                                                       
// </gliptic>                                                                           
                                                                                        


void CChildFrame::GetWindowPrefs( void )
{

  switch(m_WindowType)
  {
    case CWTYPE_CHANNEL:
      m_pWindowPrefs = g_pPrefs->GetWindowPrefs(m_WindowType,m_pServer->m_pDetails->m_NetworkID,m_pChannel->m_pProperties->m_Name);
      break;
    case CWTYPE_SERVER:
      m_pWindowPrefs = g_pPrefs->GetWindowPrefs(m_WindowType,m_pServer->m_pDetails->m_NetworkID,NULL);
      break;
    case CWTYPE_QUERY:
      m_pWindowPrefs = g_pPrefs->GetWindowPrefs(m_WindowType,m_pServer->m_pDetails->m_NetworkID,m_pQuery->m_OtherNick);
      break;
  }

  if (m_pWindowPrefs && m_pWindowPrefs->m_Flags & WPF_HAVECOLORS)
    CopyMemory(m_ColorPrefs,m_pWindowPrefs->m_ColorPrefs,sizeof(m_ColorPrefs));
  else
    CopyMemory(m_ColorPrefs,g_pPrefs->m_ColorPrefs,sizeof(m_ColorPrefs));
}
void CChildFrame::Initialise( void )
{
  m_StatusStr = NULL;
  m_ServerID = m_pServer->m_ServerID;
  m_ScrollTickCount = 0;
  m_LastDirtyStatus = FALSE;
  m_FirstFocus = TRUE;
  m_pWindowPrefs = NULL;
  m_LastSize = 0;
  m_InfoUpdateTicks = 0;
  // setup the colors
  //CopyMemory(m_ColorPrefs,g_pPrefs->m_ColorPrefs,sizeof(m_ColorPrefs));
}

CChildFrame::CChildFrame(CEventManager *pEventManager, IRCServer *pServer)
  : CListener(pEventManager),
    m_UserListView(NULL,IDI_USERLIST,pEventManager)
{
  m_pServer = pServer;
  m_pChannel = NULL;
  m_pQuery = NULL;

  if (pServer->m_IsDirect)
    m_WindowType = CWTYPE_DCCCHAT;
  else
    m_WindowType = CWTYPE_SERVER;

  m_ChannelID = 0; // no channel ID, we're a server...
  Initialise();
}

CChildFrame::CChildFrame(CEventManager *pEventManager, IRCServer *pServer, IRCChannel *pChannel)
  : CListener(pEventManager),
    m_UserListView(NULL,IDI_USERLIST,pEventManager)
{
  m_pServer = pServer;
  m_pChannel = pChannel;
  m_pQuery = NULL;
  m_WindowType = CWTYPE_CHANNEL;
  m_ChannelID = m_pChannel->m_ChannelID;
  Initialise();
}

CChildFrame::CChildFrame(CEventManager *pEventManager, IRCServer *pServer, IRCQuery *pQuery)
  : CListener(pEventManager),
    m_UserListView(NULL,IDI_USERLIST,pEventManager)
{
  m_pServer = pServer;
  m_pChannel = NULL;
  m_pQuery = pQuery;
  m_WindowType = CWTYPE_QUERY;
  m_QueryID = m_pQuery->m_QueryID;
  Initialise();
}

CChildFrame::~CChildFrame()
{
#ifdef DEBUG
  sys_Printf(BIC_FUNCTION,"Deleting Class Instance: CChildFrame\n");
  ATLASSERT(!IsWindow()); // window still exists
#endif
  
  while (m_TimerCommands.GetSize() > 0)
  {
    CTimerCommand *pTimerCommand = m_TimerCommands[0];
    g_EventManager.DeleteEvents(this, EV_CW_TIMER);
    m_TimerCommands.RemoveAt(0);
    delete pTimerCommand;
  }

  g_ChildWndList.Remove(this);
  if (m_StatusStr) free(m_StatusStr);
}

void CChildFrame::OnEvent(int EventID, void *pData)
{
  switch (EventID)
  {
    case EV_CW_TIMER:
      {
        CTimerCommand *pTimerCommand = (CTimerCommand *)pData;
        CMD_Exec(pTimerCommand->m_CommandProfileName);
      }
      break;
    case EV_TICK:
      {
        m_InfoUpdateTicks++;
        if (m_InfoUpdateTicks >= 5)
        {
          m_InfoUpdateTicks = 0;
          UpdateInfo();
        }

        // If the message window has text in it that has not
        // been shown to the user (IsDirty() == TRUE), then
        // increment a tick count.  if the tick count reaches a
        // particular value, then scroll it to be bottom
        // (and reset the dirty flag too..)
        // Increment the tick counter since the last output buffer addition
        if (m_MsgView.IsDirty())
        {
          m_ScrollTickCount++;
          if (m_ScrollTickCount == INTPREF(PREF_nInactivityScrollDelay))
          {
            g_pMainWnd->PostMessage(WM_REQUESTSCROLLTOEND,0,(LPARAM)m_MsgView.m_hWnd); // and then it gets round to it...
            m_ScrollTickCount = 0;
          }
        }

        // Update the item in the server window if we're dirty, but make sure
        // we don't keep on updating the list, so remember the last dirty status.
        int DirtyStatus;
        switch(m_WindowType)
        {
          case CWTYPE_QUERY:
            DirtyStatus = m_pQuery->GetDirtyStatus();
            if (DirtyStatus != m_LastDirtyStatus)
            {
              g_pMainWnd->m_ServerListView.Update(m_pQuery);
              m_LastDirtyStatus = DirtyStatus;
            }
            m_pQuery->BufferTick();
            break;

          case CWTYPE_CHANNEL:
            DirtyStatus = m_pChannel->GetDirtyStatus();
            if (DirtyStatus != m_LastDirtyStatus)
            {
              g_pMainWnd->m_ServerListView.Update(m_pChannel);
              m_LastDirtyStatus = DirtyStatus;
            }
            m_pChannel->BufferTick();
            break;

          case CWTYPE_DCCCHAT:
          case CWTYPE_SERVER:
            DirtyStatus = m_pServer->GetDirtyStatus();
            if (DirtyStatus != m_LastDirtyStatus)
            {
              g_pMainWnd->m_ServerListView.Update(m_pServer);
              m_LastDirtyStatus = DirtyStatus;
            }
            m_pServer->BufferTick();
            break;
        }

      }
      break;
  }
}

LRESULT CChildFrame::OnDNSEvent(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  DNSResolveItem *pDNSRI = (DNSResolveItem *)lParam;
  ATLASSERT(pDNSRI);

  switch (m_WindowType)
  {
    case CWTYPE_SERVER:
      // we might have submitted multiple DNS resolution attempts in quick
      // sucession, check to see if this is the one we're interested in.
      if (stricmp(m_pServer->m_pDetails->m_Name,pDNSRI->m_fqdn) == 0)
      {
        m_pServer->ActualConnect(pDNSRI->m_address); // Do the connect!
      }
      break;
  }
  delete pDNSRI;

  return FALSE;
}

LRESULT CChildFrame::OnUserListContext(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  char *Command = NULL; // freed by ProcessTextInput()
  BOOL ReFocus = TRUE;

  if (m_SelectedUserListUser)
  {
    IRCUser *pUser = m_SelectedUserListUser;
    if (!(m_pChannel->m_Users.m_UserList.Find(pUser) >= 0))
      return 0; // user not in channel any more

    sys_Printf(BIC_INFO,"wID = %d, Username: %s, HostMask: %s\n",wID,pUser->m_Nick,pUser->m_Address);


    switch (wID)
    {
      case ID_USERLIST_QUERY:
        Command = HydraIRC_BuildString(512,"/query %s",pUser->m_Nick);
        ReFocus = FALSE;
        break;
      case ID_USERLIST_DCCSENDFILE:
        Command = HydraIRC_BuildString(512,"/dcc send %s",pUser->m_Nick);
        break;
      case ID_USERLIST_DCCCHAT:
        Command = HydraIRC_BuildString(512,"/dcc chat %s",pUser->m_Nick);
        break;
      case ID_USERLIST_OP:
        Command = HydraIRC_BuildString(512,"/mode +o %s",pUser->m_Nick);
        break;
      case ID_USERLIST_DEOP:
        Command = HydraIRC_BuildString(512,"/mode -o %s",pUser->m_Nick);
        break;
      case ID_USERLIST_VOICE:
        Command = HydraIRC_BuildString(512,"/mode +v %s",pUser->m_Nick);
        break;
      case ID_USERLIST_DEVOICE:
        Command = HydraIRC_BuildString(512,"/mode -v %s",pUser->m_Nick);
        break;
      case ID_USERLIST_WHOIS:
        Command = HydraIRC_BuildString(512,"/whois %s",pUser->m_Nick);
        break;
      case ID_USERLIST_SLAP:
        Command = HydraIRC_BuildString(512,"/slap %s",pUser->m_Nick);
        break;
      case ID_USERLIST_KICK:
        Command = HydraIRC_BuildString(512,"/kick %s",pUser->m_Nick);
        break;
	  case ID_USERLIST_CTCP_VERSION:
		Command = HydraIRC_BuildString(512,"/ctcp %s version",pUser->m_Nick);
		break;
	  case ID_USERLIST_CTCP_USER:
		Command = HydraIRC_BuildString(512,"/ctcp %s userinfo",pUser->m_Nick);
		break;
	  case ID_USERLIST_CTCP_CLIENT:
		Command = HydraIRC_BuildString(512,"/ctcp %s clientinfo",pUser->m_Nick);
		break;
	  case ID_USERLIST_CTCP_PING:
		Command = HydraIRC_BuildString(512,"/ctcp %s ping",pUser->m_Nick);
		break;
	  case ID_USERLIST_CTCP_TIME:
		Command = HydraIRC_BuildString(512,"/ctcp %s time",pUser->m_Nick);
		break;
	  case ID_USERLIST_ADMIN:
		Command = HydraIRC_BuildString(512,"/mode +a %s",pUser->m_Nick);
		break;
	  case ID_USERLIST_DEADMIN:
		Command = HydraIRC_BuildString(512,"/mode -a %s",pUser->m_Nick);
		break;
	  case ID_USERLIST_HALFOP:
		Command = HydraIRC_BuildString(512,"/mode +h %s",pUser->m_Nick);
		break;
	  case ID_USERLIST_DEHALFOP:
		Command = HydraIRC_BuildString(512,"/mode -h %s",pUser->m_Nick);
		break;




      case ID_USERLIST_KICKANDBAN:
      case ID_USERLIST_BAN:
        // we need a valid hostmask before we can ban the user.
        if (pUser->m_Address)
        {
          Command = HydraIRC_BuildString(512,"/mode +b %s",pUser->m_Address);
        }
        else
        {
          Command = HydraIRC_BuildString(512,"/whois %s",pUser->m_Nick);
          sys_Printf(BIC_WARNING,
            "Can't ban %s from %s, unknown hostmask, wait for whois responce and try again\n",
            pUser->m_Nick,m_pChannel->m_pProperties->m_Name);
        }

        if (Command && wID == ID_USERLIST_KICKANDBAN)
        {
          ProcessTextInput(Command,TRUE);
          free(Command);
          Command = HydraIRC_BuildString(512,"/kick %s",pUser->m_Nick);
        }
        break;
      case ID_USERLIST_CB_COPYUSERNAME:
        Clipboard_Copy(CF_TEXT, pUser->m_Nick, strlen(pUser->m_Nick) + 1); // +1 for null term!
        break;
      case ID_USERLIST_CB_COPYHOSTMASK:
        if (pUser->m_Address)
          Clipboard_Copy(CF_TEXT, pUser->m_Address, strlen(pUser->m_Address) + 1); // +1 for null term!
        break;
    }
  }

  if (Command)
  {
    ProcessTextInput(Command,TRUE);
    free(Command);
  }

  if (ReFocus)
    ActivateTextInputPane();

  bHandled = TRUE;
  return 0;
}

void CChildFrame::DoUserListContextMenu(TreeItemInfo *pTII, CUserListView *pUserListView)
{
  BOOL DoDefaultMenu = TRUE;
  m_SelectedUserListUser = NULL;
  g_pMainWnd->SetUserListMenuWindow(pUserListView->m_hWnd);
  g_pMainWnd->SetUserListMenuInstance(pUserListView); // do we need this, don't think so.

  CMenuHandle menuContext;
  POINT pt;
  ::GetCursorPos(&pt);

  if (pTII)
  {
    switch (pTII->m_Type)
    {
      case TII_USER:
        {
          DoDefaultMenu = FALSE;
          IRCUser *pUser = (IRCUser *)pTII->m_Data;
          m_SelectedUserListUser = pUser; // must be checked for validity in function dealing with the WM_COMMAND message
          // Display the menu at the current mouse location.
				  menuContext.LoadMenu(IDR_USERLIST_CONTEXT);
				  CMenuHandle menuPopup(menuContext.GetSubMenu(0));
          menuPopup.SetMenuDefaultItem(0,TRUE);

          g_pMainWnd->m_CmdBar.TrackPopupMenu(menuPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y);
          //::TrackPopupMenu(menuPopup, 0, pt.x, pt.y, 0,m_pChannel->m_pChildWnd->m_hWnd, NULL); // send messages to the childframe window
        }
        break;
    /* // TODO: display group context menu. :)
      case TII_USERGROUP:
        {
          IRCUserGroup *pGroup = (IRCUserGroup *)pTII->m_Data;
        }
        break;
      */
    }
  }

  if (DoDefaultMenu)
  {
		menuContext.LoadMenu(IDR_USERLIST_CONTEXT);
		CMenuHandle menuPopup(menuContext.GetSubMenu(1));

    UINT Check;
    Check = pUserListView->GetShowBuddyGroups() ? MF_CHECKED : MF_UNCHECKED;
    menuPopup.CheckMenuItem(ID_USERLIST_SHOWBUDDYGROUPS,Check);

    g_pMainWnd->m_CmdBar.TrackPopupMenu(menuPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y);
    //::TrackPopupMenu(menuPopup, 0, pt.x, pt.y, 0,m_pChannel->m_pChildWnd->m_hWnd, NULL); // send messages to the childframe window
  }
}

void CChildFrame::DoUserListDoubleClick(TreeItemInfo *pTII)
{
  switch (pTII->m_Type)
  {
    case TII_USER:
      {
        IRCUser *pUser = (IRCUser *)pTII->m_Data;

        IRCQuery *pQuery = m_pServer->CreateQuery(pUser->m_Nick, pUser->m_Address);
        if (pQuery)
          pQuery->ShowQueryWindow();
      }
      break;
  }
}

LRESULT CChildFrame::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND hWndActive = this->MDIGetActive();
	if(m_hWnd != hWndActive)
	{
		this->MDIActivate(m_hWnd);
	}

	return MA_ACTIVATE;
}

IRCServer *CChildFrame::GetServer( void )
{
  switch (m_WindowType)
  {
    case CWTYPE_QUERY:
    case CWTYPE_CHANNEL:
    case CWTYPE_SERVER:
      return m_pServer;
    case CWTYPE_DCCCHAT:
      return m_pServer->m_pOtherServer;
  }
  return NULL;
}

LRESULT CChildFrame::OnPaneClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
  if (hWndCtl == m_UserListPane.m_hWnd)
  {
    HideUserList();
  }
  if (hWndCtl == m_HeaderView.m_hWnd)
  {
    if (m_HorizSplitter1.GetSinglePaneMode() == SPLIT_PANE_NONE)
      m_HorizSplitter1.SetSinglePaneMode( SPLIT_PANE_BOTTOM );
  }
  if (hWndCtl == m_InfoPane.m_hWnd)
  {
    if (m_HorizSplitter3.GetSinglePaneMode() == SPLIT_PANE_NONE)
      m_HorizSplitter3.SetSinglePaneMode( SPLIT_PANE_TOP );
    else
      m_VertSplitter1.SetSinglePaneMode( SPLIT_PANE_LEFT );
  }
  return 0;
}

void CChildFrame::ResizeHeader( void )
{
  HFONT hFont = GetAppFont(PREF_fHeaderFont);
  int InputHeight = GetFontMaxHeight(hFont); // value (4) is dependant on window styles.

  m_HorizSplitter1.SetSplitterPos(2 + InputHeight * max(1,min(INTPREF(PREF_nMaxHeaderLines), m_HeaderView.GetLineCount())));
}

LRESULT CChildFrame::OnHeaderChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  ResizeHeader();
  return 0;
}

BOOL CChildFrame::IsHeaderVisible( void )
{
  if (m_HorizSplitter1.GetSinglePaneMode() == SPLIT_PANE_BOTTOM)
    return FALSE;
  return TRUE;
}

void CChildFrame::ToggleHeader( void )
{
  if (IsHeaderVisible())
    HideHeader();
  else
    ShowHeader();
}

void CChildFrame::ActivateTextInputPane( void )
{
  // call this after hiding/showing a pane

  // set the active pane and the focus
  m_HorizSplitter1.SetActivePane(SPLIT_PANE_BOTTOM);
  m_HorizSplitter2.SetActivePane(SPLIT_PANE_BOTTOM);
  m_TextInputView.SetFocus();
}

void CChildFrame::HideHeader( void )
{
  if (!IsHeaderVisible())
  {
#ifdef DEBUG
    sys_Printf(BIC_INFO,"Header Already Hidden!\n");
#endif
    return;
  }

  m_HorizSplitter1.SetSinglePaneMode( SPLIT_PANE_BOTTOM );
  m_MsgView.ScrollToEnd();
  ActivateTextInputPane();
#ifdef DEBUG
    sys_Printf(BIC_INFO,"Hiding Header\n");
#endif
}

void CChildFrame::ShowHeader( void )
{
  if (IsHeaderVisible())
  {
#ifdef DEBUG
    sys_Printf(BIC_INFO,"Header Already Visible!\n");
#endif
    return;
  }

  m_HorizSplitter1.SetSinglePaneMode( SPLIT_PANE_NONE );
  m_MsgView.ScrollToEnd();
  ActivateTextInputPane();

#ifdef DEBUG
    sys_Printf(BIC_INFO,"Showing Header\n");
#endif
}

void CChildFrame::ToggleUserList( void )
{
  if (IsUserListVisible())
    HideUserList();
  else
    ShowUserList();
}

BOOL CChildFrame::IsUserListVisible( void )
{
  if (m_VertSplitter1.GetSinglePaneMode() == SPLIT_PANE_NONE)
    if (m_HorizSplitter3.GetSinglePaneMode() == SPLIT_PANE_TOP ||
        m_HorizSplitter3.GetSinglePaneMode() == SPLIT_PANE_NONE)
      return TRUE;

  return FALSE;
}

void CChildFrame::HideUserList( void )
{
  if (!IsUserListVisible())
  {
#ifdef DEBUG
    sys_Printf(BIC_INFO,"User List Already Hidden!\n");
#endif
    return;
  }

  if (m_HorizSplitter3.GetSinglePaneMode() == SPLIT_PANE_TOP)
    m_VertSplitter1.SetSinglePaneMode( SPLIT_PANE_LEFT );
  else
    m_HorizSplitter3.SetSinglePaneMode( SPLIT_PANE_BOTTOM );

  m_MsgView.ScrollToEnd();
  ActivateTextInputPane();

#ifdef DEBUG
    sys_Printf(BIC_INFO,"Hiding User List\n");
#endif
}

void CChildFrame::ShowUserList( void )
{
  if (IsUserListVisible())
  {
#ifdef DEBUG
    sys_Printf(BIC_INFO,"User List Already Visible!\n");
#endif
    return;
  }

  if (m_VertSplitter1.GetSinglePaneMode() == SPLIT_PANE_LEFT) // off
  {
    m_VertSplitter1.SetSinglePaneMode( SPLIT_PANE_NONE ); // show both left and right panes
    m_HorizSplitter3.SetSinglePaneMode( SPLIT_PANE_TOP ); // show top pane only
  }
  if (m_HorizSplitter3.GetSinglePaneMode() == SPLIT_PANE_BOTTOM) // bottom on
  {
    m_HorizSplitter3.SetSinglePaneMode( SPLIT_PANE_NONE ); // show both
    // reset the height of the info pane
    RECT r;
    m_VertSplitter1.GetClientRect(&r);
    m_HorizSplitter3.SetSplitterPos(r.bottom - INTPREF(PREF_nDefaultInfoHeight));
  }
  else
    m_HorizSplitter3.SetSinglePaneMode( SPLIT_PANE_TOP ); // show top only

  m_MsgView.ScrollToEnd();
  ActivateTextInputPane();

#ifdef DEBUG
    sys_Printf(BIC_INFO,"Showing User List\n");
#endif
}

void CChildFrame::ToggleInfo( void )
{
  if (IsInfoVisible())
    HideInfo();
  else
    ShowInfo();
}

BOOL CChildFrame::IsInfoVisible( void )
{
  if (m_VertSplitter1.GetSinglePaneMode() == SPLIT_PANE_NONE)
    if (m_HorizSplitter3.GetSinglePaneMode() == SPLIT_PANE_BOTTOM ||
        m_HorizSplitter3.GetSinglePaneMode() == SPLIT_PANE_NONE)
      return TRUE;

  return FALSE;
}

void CChildFrame::HideInfo( void )
{
  if (!IsInfoVisible())
  {
#ifdef DEBUG
    sys_Printf(BIC_INFO,"User List Already Hidden!\n");
#endif
    return;
  }

  if (m_HorizSplitter3.GetSinglePaneMode() == SPLIT_PANE_BOTTOM)
    m_VertSplitter1.SetSinglePaneMode( SPLIT_PANE_LEFT );
  else
    m_HorizSplitter3.SetSinglePaneMode( SPLIT_PANE_TOP );

  m_MsgView.ScrollToEnd();
  ActivateTextInputPane();
#ifdef DEBUG
    sys_Printf(BIC_INFO,"Hiding Info\n");
#endif
}

void CChildFrame::ShowInfo( void )
{
  if (IsInfoVisible())
  {
#ifdef DEBUG
    sys_Printf(BIC_INFO,"Info Already Visible!\n");
#endif
    return;
  }

  if (m_VertSplitter1.GetSinglePaneMode() == SPLIT_PANE_LEFT) // off
  {
    m_VertSplitter1.SetSinglePaneMode( SPLIT_PANE_NONE ); // show both panes
    m_HorizSplitter3.SetSinglePaneMode( SPLIT_PANE_BOTTOM ); // show bottom only
  }
  if (m_HorizSplitter3.GetSinglePaneMode() == SPLIT_PANE_TOP) // top on
  {
    m_HorizSplitter3.SetSinglePaneMode( SPLIT_PANE_NONE ); // show both

    // reset the height of the info pane
    RECT r;
    m_VertSplitter1.GetClientRect(&r);
    m_HorizSplitter3.SetSplitterPos(r.bottom - INTPREF(PREF_nDefaultInfoHeight));
  }
  else
    m_HorizSplitter3.SetSinglePaneMode( SPLIT_PANE_BOTTOM ); // show bottom only

  m_MsgView.ScrollToEnd();
  ActivateTextInputPane();

#ifdef DEBUG
    sys_Printf(BIC_INFO,"Showing Info\n");
#endif
}

LRESULT CChildFrame::OnSysCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
  bHandled = FALSE;
  if (wParam == SC_MINIMIZE)
  {
    if (BOOLPREF(PREF_bHideNotMinimize))
    {
      bHandled = TRUE;
      HideWindow();
    }
  }
  return bHandled ? 0 : 1; // WM_SYSCOMMAND - If an application processes this message, it should return zero.
}

LRESULT CChildFrame::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
#ifdef DEBUG
    sys_Printf(BIC_WARNING,"CChildFrame::OnSze() called\n");
#endif

  bHandled = FALSE;
  ResizeHeader();
  // m_MsgView.ScrollToEnd(); // calling this directly doesn't work as the richedit control window hasn't updated yet
  // so we do this instead:
  // however, this causes a problem when switching between maximized windows which are first restored
  // then the new one is made front most, then that is maximized.  thus the old active window
  // and the new active window both receive an OnSized message that causes the display
  // to scroll to the bottom,  however..  without this call (or with this call but only for
  // non-maximized windows) richedit still receives the on-size call and resizes itself
  // causing the scroll position to be changed anyway.  maybe not letting richedit get
  // the onsize message might help?
  /*
  BOOL Maximized;
  g_pMainWnd->MDIGetActive(&Maximized);
  if (!Maximized)
  */
    g_pMainWnd->PostMessage(WM_REQUESTSCROLLTOEND,0,(LPARAM)m_MsgView.m_hWnd); // and then it gets round to it...
  return bHandled ? 0 : 1; // WM_SIZE - If an application processes this message, it should return zero.
}

void CChildFrame::UpdateInfo( void )
{
  switch (m_WindowType)
  {
    case CWTYPE_SERVER:
      m_pServer->UpdateInfo();
      break;
    case CWTYPE_DCCCHAT:
      m_pServer->UpdateInfo();
      break;
    case CWTYPE_QUERY:
      m_pQuery->UpdateInfo();
      break;
    case CWTYPE_CHANNEL:
      m_pChannel->UpdateInfo();
      break;
  }
}

LRESULT CChildFrame::OnUpdateInfoPane(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  bHandled = TRUE;
  UpdateInfo();
  return 0;
}

#ifdef USE_TABBEDMDI

void CChildFrame::UpdateTab( void )
{
  switch (m_WindowType)
  {
    case CWTYPE_SERVER:
      SetTabText(m_pServer->m_pDetails->m_Name);
      break;
    case CWTYPE_DCCCHAT:
      SetTabText(m_pServer->m_OtherNick);
      break;
    case CWTYPE_QUERY:
      SetTabText(m_pQuery->m_OtherNick);
      break;
    case CWTYPE_CHANNEL:
      SetTabText(m_pChannel->m_pProperties->m_Name);
      break;
  }
}

LRESULT CChildFrame::OnUpdateTab(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  bHandled = TRUE;
  UpdateTab();
  return 0;
}
#endif
