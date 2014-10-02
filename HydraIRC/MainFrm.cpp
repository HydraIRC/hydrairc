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

// MainFrm.cpp : implmentation of the CMainFrame class
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

CMainFrame::~CMainFrame( void )
{
  if (m_ServerListView.IsWindow())
    m_ServerListView.DestroyWindow();

  if (m_UserListView.IsWindow())
    m_UserListView.DestroyWindow();

  if (m_ChannelMonitorView.IsWindow())
    m_ChannelMonitorView.DestroyWindow();

  if (m_ServerMonitorView.IsWindow())
    m_ServerMonitorView.DestroyWindow();

  if (m_OutputView.IsWindow())
    m_OutputView.DestroyWindow();

  if (g_pTransfersManager->m_pTransfersView->IsWindow())
    g_pTransfersManager->m_pTransfersView->DestroyWindow();

  if (g_pNoticeQueueManager->m_pTextQueueView->IsWindow())
    g_pNoticeQueueManager->m_pTextQueueView->DestroyWindow();

  if (g_pPrivMsgQueueManager->m_pTextQueueView->IsWindow())
    g_pPrivMsgQueueManager->m_pTextQueueView->DestroyWindow();

  if (g_pUrlCatcherManager->m_pTextQueueView->IsWindow())
    g_pUrlCatcherManager->m_pTextQueueView->DestroyWindow();

  if (g_pEventLogManager->m_pEventLogView->IsWindow())
    g_pEventLogManager->m_pEventLogView->DestroyWindow();
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(baseClass::PreTranslateMessage(pMsg))
		return TRUE;

	HWND hWndFocus = ::GetFocus();
	HWND hWndMDIActive = this->MDIGetActive();

	// NOTE: IsChild checks if the window is a direct child or a descendant
	if((hWndMDIActive == hWndFocus) || (::IsChild(hWndMDIActive, hWndFocus)))
	{
		if(hWndMDIActive != NULL)
		{
			return (BOOL)::SendMessage(hWndMDIActive, WM_FORWARDMSG, 0, (LPARAM)pMsg);
		}
	}

  //if (pMsg->hwnd == m_ChannelMonitorView.m_hWnd)
  //  return (BOOL)::SendMessage(m_ChannelMonitorView.m_hWnd, WM_FORWARDMSG, 0, (LPARAM)pMsg);


	return FALSE;
}

// note: these On*Commands() functions are hacks for testxp, because the
// command bar xp passes the messages back to the mainframe and not the window that
// the context menu originated in.
LRESULT CMainFrame::OnEventLogCommands(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
{
  ::SendMessage(g_pEventLogManager->m_pEventLogView->m_hWnd, WM_COMMAND, MAKELONG(wID,wNotifyCode), (LPARAM)hWndCtl);
  return 0;
}

LRESULT CMainFrame::OnTransfersCommands(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
{
  ::SendMessage(g_pTransfersManager->m_pTransfersView->m_hWnd, WM_COMMAND, MAKELONG(wID,wNotifyCode), (LPARAM)hWndCtl);
  return 0;
}

LRESULT CMainFrame::OnServerListCommands(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
{
  ::SendMessage(m_ServerListView.m_hWnd, WM_COMMAND, MAKELONG(wID,wNotifyCode), (LPARAM)hWndCtl);
  return 0;
}

// this one is needed for both normal command bar implementation and commandbar xp
// as it's needed to pass the messages back to the right window.
LRESULT CMainFrame::OnUserListCommands(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
{
  if (::IsWindow(m_LastUserListMenuWindow))  
    ::SendMessage(m_LastUserListMenuWindow, WM_COMMAND, MAKELONG(wID,wNotifyCode), (LPARAM)hWndCtl);
  return 0;
}


bool CMainFrame::IsToolbarVisible(int ID) const
{
	CReBarCtrl rebar(m_hWndToolBar);
	int i = rebar.IdToIndex(ID);
	REBARBANDINFO rbi;
	ZeroMemory(&rbi,sizeof(REBARBANDINFO));
	rbi.cbSize = sizeof(REBARBANDINFO);
	rbi.fMask = RBBIM_STYLE;
	rebar.GetBandInfo(i, &rbi);
	return !(rbi.fStyle&RBBS_HIDDEN);
}

BOOL CMainFrame::OnIdle()
{
  UISetCheck(ID_VIEW_TOOLBAR,IsToolbarVisible(ID_TOOLBAR));
  UISetCheck(ID_VIEW_STATUS_BAR,::IsWindowVisible(m_hWndStatusBar));
  UISetCheck(ID_VIEW_IRCSERVERLIST,  m_ServerListView.IsWindow()     
    && m_ServerListView.IsWindowVisible());
  UISetCheck(ID_VIEW_GLOBALUSERLIST, m_UserListView.IsWindow()     
    && m_UserListView.IsWindowVisible());
  UISetCheck(ID_VIEW_TRANSFERS,      g_pTransfersManager->m_pTransfersView->IsWindow()     
    && g_pTransfersManager->m_pTransfersView->IsWindowVisible());
  UISetCheck(ID_OPTIONS_AUDIOMUTE,   BOOLPREF(PREF_bAudioMute));
  UISetCheck(ID_VIEW_CHANNELMONITOR, m_ChannelMonitorView.IsWindow() 
    && m_ChannelMonitorView.IsWindowVisible());
  UISetCheck(ID_VIEW_SERVERMONITOR,  m_ServerMonitorView.IsWindow() 
    && m_ServerMonitorView.IsWindowVisible());
  UISetCheck(ID_VIEW_EVENTLOG,       g_pEventLogManager->m_pEventLogView->IsWindow() 
    && g_pEventLogManager->m_pEventLogView->IsWindowVisible());  
  UISetCheck(ID_VIEW_PRIVMSGQUEUE,   g_pPrivMsgQueueManager->m_pTextQueueView->IsWindow()     
    && g_pPrivMsgQueueManager->m_pTextQueueView->IsWindowVisible());
  UISetCheck(ID_VIEW_NOTICEQUEUE,    g_pNoticeQueueManager->m_pTextQueueView->IsWindow()     
    && g_pNoticeQueueManager->m_pTextQueueView->IsWindowVisible());  
  UISetCheck(ID_VIEW_URLCATCHER,     g_pUrlCatcherManager->m_pTextQueueView->IsWindow()
    && g_pUrlCatcherManager->m_pTextQueueView->IsWindowVisible());
  UISetCheck(ID_VIEW_OUTPUT,         m_OutputView.IsWindow()         
    && m_OutputView.IsWindowVisible());
  UISetCheck(ID_VIEW_EVENTLOG,       g_pEventLogManager->m_pEventLogView->IsWindow()     
    && g_pEventLogManager->m_pEventLogView->IsWindowVisible());

  
  CChildCommon *pChildWnd = FindChildWindow(MDIGetActive());
  CChildFrame *pChildFrame = CHILDFRAMEPTR(pChildWnd);
  if (pChildWnd)
  {
    UIEnable(ID_FILE_CLOSE,TRUE);

    UIEnable(ID_EDIT_CUT,TRUE);
    UIEnable(ID_EDIT_COPY,TRUE);
    UIEnable(ID_EDIT_PASTE,TRUE);
    UIEnable(ID_EDIT_COPYBUFFER,TRUE);
    UIEnable(ID_EDIT_CLEARBUFFER,TRUE);
    UIEnable(ID_OPTIONS_LOGGING, TRUE);
    UIEnable(ID_OPTIONS_TIMESTAMPS,TRUE);
    UISetCheck(ID_OPTIONS_TIMESTAMPS, pChildFrame->m_MsgView.GetTimeStamps());

    switch(pChildWnd->m_WindowType)
    {
      case CWTYPE_QUERY:
      case CWTYPE_CHANNEL:
      case CWTYPE_DCCCHAT:
      case CWTYPE_SERVER:
        UIEnable(ID_OPTIONS_TOGGLEINFO,TRUE);
        UISetCheck(ID_OPTIONS_TOGGLEINFO, pChildFrame->IsInfoVisible());
        UIEnable(ID_OPTIONS_TOGGLEHEADER,TRUE);
        UISetCheck(ID_OPTIONS_TOGGLEHEADER, pChildFrame->IsHeaderVisible());
        UIEnable(ID_OPTIONS_TOGGLEUSERLIST,TRUE);
        UISetCheck(ID_OPTIONS_TOGGLEUSERLIST, pChildFrame->IsUserListVisible());
    }
    switch(pChildWnd->m_WindowType)
    {      
      case CWTYPE_CHANNEL:
        UISetCheck(ID_OPTIONS_LOGGING, pChildFrame->m_pChannel->m_Logger.IsEnabled());
        UIEnable(ID_CHANNEL_MONITOR,TRUE);
        UISetCheck(ID_CHANNEL_MONITOR, m_ChannelMonitorView.IsMonitoring(pChildFrame->m_pChannel));
        UIEnable(ID_CHANNEL_SHOWSERVER,TRUE);
        UIEnable(ID_CHANNEL_PROPERTIES,TRUE);
        UIEnable(ID_FILE_CONNECT,TRUE);  
        UIEnable(ID_FILE_DISCONNECT,TRUE);  
        UIEnable(ID_CHANNEL_CHANNELLIST,TRUE);
        break;
      case CWTYPE_QUERY:
        UIEnable(ID_CHANNEL_MONITOR,FALSE);  
        UISetCheck(ID_OPTIONS_LOGGING, pChildFrame->m_pQuery->m_Logger.IsEnabled());
        UISetCheck(ID_CHANNEL_MONITOR, FALSE);
        UIEnable(ID_CHANNEL_SHOWSERVER,TRUE); // TODO: need to move this menu item!
        UIEnable(ID_CHANNEL_PROPERTIES,FALSE);
        UIEnable(ID_FILE_CONNECT,TRUE);  
        UIEnable(ID_FILE_DISCONNECT,TRUE);  
        UIEnable(ID_CHANNEL_CHANNELLIST,TRUE);
        break;
      case CWTYPE_DCCCHAT:
        UIEnable(ID_CHANNEL_MONITOR,FALSE);  
        UISetCheck(ID_OPTIONS_LOGGING, pChildFrame->m_pServer->m_Logger.IsEnabled());
        UISetCheck(ID_CHANNEL_MONITOR, FALSE);
        UIEnable(ID_CHANNEL_SHOWSERVER,TRUE); // TODO: need to move this menu item!
        UIEnable(ID_CHANNEL_PROPERTIES,FALSE);
        UIEnable(ID_FILE_CONNECT,FALSE);      // TODO: can't reconnect DCC chat's (yet)
        UIEnable(ID_FILE_DISCONNECT,TRUE);  
        break;
      case CWTYPE_SERVER:
        UIEnable(ID_CHANNEL_MONITOR,FALSE);
        UISetCheck(ID_OPTIONS_LOGGING, pChildFrame->m_pServer->m_Logger.IsEnabled());
        UISetCheck(ID_CHANNEL_MONITOR, FALSE);
        UIEnable(ID_CHANNEL_SHOWSERVER,FALSE);
        UIEnable(ID_CHANNEL_PROPERTIES,FALSE);
        UIEnable(ID_FILE_CONNECT,TRUE);  
        UIEnable(ID_FILE_DISCONNECT,TRUE);  
        UIEnable(ID_CHANNEL_CHANNELLIST,TRUE);
    }
  } // End: a child window is active
  else
  {
    // No child window active
    UIEnable(ID_FILE_CLOSE,FALSE);
    UIEnable(ID_FILE_CONNECT,FALSE);  
    UIEnable(ID_FILE_DISCONNECT,FALSE); 
    UIEnable(ID_EDIT_CUT,FALSE);
    UIEnable(ID_EDIT_COPY,FALSE);
    UIEnable(ID_EDIT_PASTE,FALSE);
    UIEnable(ID_EDIT_COPYBUFFER,FALSE);
    UIEnable(ID_EDIT_CLEARBUFFER,FALSE);
    UIEnable(ID_CHANNEL_MONITOR,FALSE);
    UIEnable(ID_CHANNEL_SHOWSERVER,FALSE);
    UIEnable(ID_CHANNEL_PROPERTIES,TRUE);
    UIEnable(ID_CHANNEL_CHANNELLIST,TRUE);
    UIEnable(ID_OPTIONS_LOGGING, FALSE);
    UIEnable(ID_OPTIONS_TIMESTAMPS,FALSE);
    UIEnable(ID_OPTIONS_TOGGLEINFO,FALSE);  
    UIEnable(ID_OPTIONS_TOGGLEHEADER,FALSE);  
    UIEnable(ID_OPTIONS_TOGGLEUSERLIST,FALSE);  

    UISetCheck(ID_CHANNEL_MONITOR, FALSE);

    UISetCheck(ID_OPTIONS_LOGGING, FALSE);
    UISetCheck(ID_OPTIONS_TIMESTAMPS,FALSE);
    UISetCheck(ID_OPTIONS_TOGGLEINFO,FALSE);  
    UISetCheck(ID_OPTIONS_TOGGLEHEADER,FALSE);  
    UISetCheck(ID_OPTIONS_TOGGLEUSERLIST,FALSE);  
  }
	UIUpdateToolBar();

	return FALSE;
}

LRESULT CMainFrame::OnToggleInfo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(g_pMainWnd->MDIGetActive()));
  if (pChildWnd && pChildWnd->CanHaveInfo())
  {
    pChildWnd->ToggleInfo();
  }
  return 0;
}

LRESULT CMainFrame::OnToggleHeader(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(g_pMainWnd->MDIGetActive()));
  if (pChildWnd && pChildWnd->CanHaveHeader())
  {
    pChildWnd->ToggleHeader();
  }
  return 0;
}

LRESULT CMainFrame::OnToggleUserList(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(g_pMainWnd->MDIGetActive()));
  if (pChildWnd && pChildWnd->CanHaveUserList())
  {
    pChildWnd->ToggleUserList();
  }
  return 0;
}

void CMainFrame::ToggleMonitoring(CChildFrame *pChildWnd)
{
  if (!pChildWnd)
    return;

  switch (pChildWnd->m_WindowType)
  {
    case CWTYPE_SERVER:
      {
        IRCServer *pServer = pChildWnd->m_pServer;
        if (m_ServerMonitorView.IsMonitoring(pServer))
          m_ServerMonitorView.RemoveServer(pServer);
        else
          m_ServerMonitorView.AddServer(pServer);        
      }
      break;
    case CWTYPE_CHANNEL:
      {
        IRCChannel *pChannel = pChildWnd->m_pChannel;
        if (m_ChannelMonitorView.IsMonitoring(pChannel))
          m_ChannelMonitorView.RemoveChannel(pChannel);
        else
          m_ChannelMonitorView.AddChannel(pChannel);        
      }
      break;
  }
}

void CMainFrame::ToggleTimeStamps(CChildFrame *pChild)
{
  if (!pChild)
    return;

  pChild->m_MsgView.SetTimeStamps(!pChild->m_MsgView.GetTimeStamps());
}

void CMainFrame::ToggleLogging(CChildFrame *pChild)
{
  DWORD Flags;

  if (!pChild)
    return;

  switch (pChild->m_WindowType)
  {
    case CWTYPE_SERVER:
    case CWTYPE_DCCCHAT:
      Flags = pChild->m_pServer->m_Logger.GetSettings();
      Flags ^= LOGF_ENABLED;
      pChild->m_pServer->m_Logger.UpdateSettings(Flags);
      // starting logging can fail, so make sure UI is consistant
      UISetCheck(ID_OPTIONS_LOGGING, pChild->m_pServer->m_Logger.IsEnabled(),TRUE);
      break;      
    case CWTYPE_CHANNEL:
      Flags = pChild->m_pChannel->m_Logger.GetSettings();
      Flags ^= LOGF_ENABLED;
      pChild->m_pChannel->m_Logger.UpdateSettings(Flags);
      // starting logging can fail, so make sure UI is consistant
      UISetCheck(ID_OPTIONS_LOGGING, pChild->m_pChannel->m_Logger.IsEnabled(),TRUE);
      break;
    case CWTYPE_QUERY:
      Flags = pChild->m_pQuery->m_Logger.GetSettings();
      Flags ^= LOGF_ENABLED;
      pChild->m_pQuery->m_Logger.UpdateSettings(Flags);
      // starting logging can fail, so make sure UI is consistant
      UISetCheck(ID_OPTIONS_LOGGING, pChild->m_pQuery->m_Logger.IsEnabled(),TRUE);
      break;
  }
  UIUpdateToolBar();
}

LRESULT CMainFrame::OnToggleChannelMonitor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(g_pMainWnd->MDIGetActive()));
  ToggleMonitoring(pChildWnd);
  return 0;
}

LRESULT CMainFrame::OnToggleServerMonitor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(g_pMainWnd->MDIGetActive()));
  ToggleMonitoring(pChildWnd);
  return 0;
}

LRESULT CMainFrame::OnToggleOptionTimeStamps(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CChildFrame *pChild = CHILDFRAMEPTR(FindChildWindow(g_pMainWnd->MDIGetActive()));
  ToggleTimeStamps(pChild);
  return 0;
}


LRESULT CMainFrame::OnToggleOptionLogging(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CChildFrame *pChild = CHILDFRAMEPTR(FindChildWindow(g_pMainWnd->MDIGetActive()));
  ToggleLogging(pChild);
  return 0;
}

#define TOOLBAR_BUTTON_COUNT 22
#define ID_SEPARATOR_1 999
#define ID_SEPARATOR_2 998
#define ID_SEPARATOR_3 997
#define ID_SEPARATOR_4 996

HWND CMainFrame::createToolbar()
{	
	CToolBarCtrl ctrl;
	ctrl.Create(m_hWnd, NULL, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS, 0, ATL_IDW_TOOLBAR);
  ctrl.SendMessage(CCM_SETVERSION, (WPARAM) 5, 0);  // enable multiple image lists
  ctrl.SetButtonStructSize();
	ctrl.SetImageList(m_images_toolbar,0);
	
	TBBUTTON tb[TOOLBAR_BUTTON_COUNT];
	memset(tb, 0, sizeof(tb));
	int n = 0, image = 0;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_FILE_NEW;
	tb[n].fsState = TBSTATE_ENABLED;
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;

	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_FILE_CONNECT;
	tb[n].fsState = TBSTATE_ENABLED;
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;
	
	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_FILE_DISCONNECT;
	tb[n].fsState = TBSTATE_ENABLED;
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;

	n++;
	tb[n].iBitmap = 0;
  tb[n].idCommand = ID_SEPARATOR_1;
	tb[n].fsState = TBSTATE_ENABLED;
	tb[n].fsStyle = TBSTYLE_SEP;

	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_WINDOW_CASCADE;
	tb[n].fsState = TBSTATE_ENABLED;
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;

	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_WINDOW_TILE_VERT;
	tb[n].fsState = TBSTATE_ENABLED;
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;

	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_WINDOW_TILE_HORZ;
	tb[n].fsState = TBSTATE_ENABLED;
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;
	
	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_WINDOW_MINIMIZEALLWINDOWS;
	tb[n].fsState = TBSTATE_ENABLED;
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;

	n++;
	tb[n].iBitmap = 0;
  tb[n].idCommand = ID_SEPARATOR_2;
	tb[n].fsState = TBSTATE_ENABLED;
	tb[n].fsStyle = TBSTYLE_SEP;

	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_OPTIONS_TOGGLEUSERLIST;
	tb[n].fsState = TBSTATE_ENABLED; // disabled
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECK | TBSTYLE_AUTOSIZE;
	
	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_CHANNEL_MONITOR;
	tb[n].fsState = TBSTATE_ENABLED; // disabled
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECK | TBSTYLE_AUTOSIZE;

	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_OPTIONS_LOGGING;
	tb[n].fsState = TBSTATE_ENABLED; // disabled
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECK | TBSTYLE_AUTOSIZE;

	n++;
	tb[n].iBitmap = 0;
  tb[n].idCommand = ID_SEPARATOR_3;
	tb[n].fsState = TBSTATE_ENABLED;
	tb[n].fsStyle = TBSTYLE_SEP;

	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_OPTIONS_AUDIOMUTE;
	tb[n].fsState = TBSTATE_ENABLED; // disabled
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECK | TBSTYLE_AUTOSIZE;

	n++;
	tb[n].iBitmap = 0;
  tb[n].idCommand = ID_SEPARATOR_4;
	tb[n].fsState = TBSTATE_ENABLED;
	tb[n].fsStyle = TBSTYLE_SEP;

	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_VIEW_EVENTLOG;
	tb[n].fsState = TBSTATE_ENABLED; // disabled
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECK | TBSTYLE_AUTOSIZE;

  /*
	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_VIEW_PRIVMSGQUEUE;
	tb[n].fsState = TBSTATE_ENABLED; // disabled
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECK | TBSTYLE_AUTOSIZE;

	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_VIEW_NOTICEQUEUE;
	tb[n].fsState = TBSTATE_ENABLED; // disabled
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECK | TBSTYLE_AUTOSIZE;
  */

	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_VIEW_OUTPUT;
	tb[n].fsState = TBSTATE_ENABLED; // disabled
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECK | TBSTYLE_AUTOSIZE;

	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_VIEW_CHANNELMONITOR;
	tb[n].fsState = TBSTATE_ENABLED; // disabled
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECK | TBSTYLE_AUTOSIZE;

	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_VIEW_SERVERMONITOR;
	tb[n].fsState = TBSTATE_ENABLED; // disabled
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECK | TBSTYLE_AUTOSIZE;

  n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_VIEW_TRANSFERS;
	tb[n].fsState = TBSTATE_ENABLED; // disabled
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECK | TBSTYLE_AUTOSIZE;

	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_VIEW_GLOBALUSERLIST;
	tb[n].fsState = TBSTATE_ENABLED; // disabled
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECK | TBSTYLE_AUTOSIZE;

	n++;
	tb[n].iBitmap = MAKELONG(image++,0);
	tb[n].idCommand = ID_VIEW_IRCSERVERLIST;
	tb[n].fsState = TBSTATE_ENABLED; // disabled
	tb[n].fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECK | TBSTYLE_AUTOSIZE;

	ctrl.AddButtons(TOOLBAR_BUTTON_COUNT, tb);
	
	ctrl.AutoSize();

	return ctrl.m_hWnd;
}

// this workaround solves a bug in CMultiPaneStatusBarCtrl
// (in SetPanes() method) that limits the size of all panes
// after the default pane to a combined total of 100 pixels  
void CMainFrame::SetStatusPaneWidths(int* arrWidths, int nPanes)
{
	// find the size of the borders
	int arrBorders[3];
	m_Status.GetBorders(arrBorders);

	// calculate right edge of default pane
	arrWidths[0] += arrBorders[2];
	for (int i = 1; i < nPanes; i++)
		arrWidths[0] += arrWidths[i];

	// calculate right edge of remaining panes
	for (int j = 1; j < nPanes; j++)
		arrWidths[j] += arrBorders[2] + arrWidths[j - 1];

	// set the pane widths
	m_Status.SetParts(m_Status.m_nPanes, arrWidths);
}


void AddMenuItemsFromXML(CMenuHandle &MenuHandle, IDManager *pIDManager, xmlNodePtr pRootNode, int *RootItems)
{
  xmlNodePtr pNode = pRootNode;
  while (pNode)
  {
    if (pNode->type == XML_ELEMENT_NODE)
    {
      char *name = (char *)xmlGetProp(pNode,(xmlChar *)"name");

      char numstr[10];
      int FavoriteID = pIDManager->ObtainID();
      _snprintf(numstr,sizeof(numstr),"%d",FavoriteID);
      numstr[sizeof(numstr)-1] = 0;

      xmlSetProp(pNode,(xmlChar *)"id",(xmlChar *)numstr);

      // TODO: use icons in the menu!
      int iconindex = 0;

      if (stricmp((char*)pNode->name,"group") == 0)
      {
        iconindex = ICON_CHANNELS;

        if (pNode->children)
        {
          CMenuHandle SubMenu;
          SubMenu.CreatePopupMenu();
          AddMenuItemsFromXML(SubMenu, pIDManager, pNode->children, NULL);
          if (RootItems && *RootItems == 0)
          {
            if (MenuHandle.AppendMenu(MF_SEPARATOR))
              (*RootItems)++;
          }

          if (MenuHandle.AppendMenu(MF_POPUP,(UINT)(HMENU)SubMenu,_T(name)))
          {
            if (RootItems)
              (*RootItems)++;
          }
        }

      }
      else if (stricmp((char*)pNode->name,"favorite") == 0)
      {
        char *type = (char *)xmlGetProp(pNode,(xmlChar *)"type");
        if (name && type)
        {
          switch (toupper(*type))
          {
            case 'C':
              iconindex = ICON_CHANNEL;
              break;
            case 'S':
              iconindex = ICON_SERVER;
              break;
            case 'Q':
              iconindex = ICON_QUERY;
              break;
          }
        }

        if (type) free(type);

        if (FavoriteID != -1)
        {
          if (RootItems && *RootItems == 0)
          {
            if (MenuHandle.AppendMenu(MF_SEPARATOR))
            {
              if (RootItems)
                (*RootItems)++;
            }
          }

          if (MenuHandle.AppendMenu(MF_STRING,FavoriteID,_T(name)))
          {
            if (RootItems)
              (*RootItems)++;
          }
        }

      }

      if (name) free(name);
    }

    pNode = pNode->next;
  }
}


void CMainFrame::ChangeGUILayoutStyle(int LayoutStyle)
{
	CRect rcClientArea;
	::GetClientRect(m_hWnd, &rcClientArea);

  // make sure all autohiding stuff is not anymore
  m_ServerListView.Hide();
  m_UserListView.Hide();
  m_OutputView.Hide();
  m_ChannelMonitorView.Hide();
  m_ServerMonitorView.Hide();
  g_pEventLogManager->m_pEventLogView->Hide();
  g_pNoticeQueueManager->m_pTextQueueView->Hide();
  g_pTransfersManager->m_pTransfersView->Hide();
  g_pPrivMsgQueueManager->m_pTextQueueView->Hide();
  g_pUrlCatcherManager->m_pTextQueueView->Hide();

  // then make sure it's all *really* hidden.. (gay gay gay...)
  m_ServerListView.Hide();
  m_UserListView.Hide();
  m_OutputView.Hide();
  m_ChannelMonitorView.Hide();
  m_ServerMonitorView.Hide();
  g_pEventLogManager->m_pEventLogView->Hide();
  g_pNoticeQueueManager->m_pTextQueueView->Hide();
  g_pTransfersManager->m_pTransfersView->Hide();
  g_pPrivMsgQueueManager->m_pTextQueueView->Hide();
  g_pUrlCatcherManager->m_pTextQueueView->Hide();

  switch (LayoutStyle)
  {
    default:
    case 0:
      // leave everything hidden
      break;
    case 1:
      DockWindow(m_ServerListView,dockwins::CDockingSide(dockwins::CDockingSide::sLeft),
						    0/*nBar*/,float(0.6)/*fPctPos*/,150/*nWidth*/,250/* nHeight*/);
      DockWindow(m_UserListView,dockwins::CDockingSide(dockwins::CDockingSide::sLeft),
						    0/*nBar*/,float(0.4)/*fPctPos*/,150/*nWidth*/,250/* nHeight*/);
	    DockWindow(m_OutputView,dockwins::CDockingSide(dockwins::CDockingSide::sBottom),
		    0/*nBar*/,float(0.5)/*fPctPos*/,100/*nWidth*/,rcClientArea.Width() / 2/* nHeight*/);
      g_pTransfersManager->m_pTransfersView->DockTo(m_OutputView);
      m_ServerMonitorView.DockTo(m_OutputView);
      m_ChannelMonitorView.DockTo(m_OutputView);
      DockWindow(*g_pNoticeQueueManager->m_pTextQueueView,dockwins::CDockingSide(dockwins::CDockingSide::sBottom),
		    0/*nBar*/,float(0.5)/*fPctPos*/,100/*nWidth*/,rcClientArea.Width() / 2/* nHeight*/);
      g_pPrivMsgQueueManager->m_pTextQueueView->DockTo(g_pNoticeQueueManager->m_pTextQueueView->m_hWnd);      
      g_pUrlCatcherManager->m_pTextQueueView->DockTo(g_pNoticeQueueManager->m_pTextQueueView->m_hWnd);
      g_pEventLogManager->m_pEventLogView->DockTo(g_pNoticeQueueManager->m_pTextQueueView->m_hWnd);      
      break;
    case 2:
      DockWindow(m_ServerListView,dockwins::CDockingSide(dockwins::CDockingSide::sLeft),
						    0/*nBar*/,float(0.6)/*fPctPos*/,150/*nWidth*/,250/* nHeight*/);
      m_UserListView.DockTo(m_ServerListView);
	    DockWindow(m_OutputView,dockwins::CDockingSide(dockwins::CDockingSide::sBottom),
		    0/*nBar*/,float(0.5)/*fPctPos*/,100/*nWidth*/,rcClientArea.Width() / 2/* nHeight*/);
      g_pTransfersManager->m_pTransfersView->DockTo(m_OutputView);
      m_ServerMonitorView.DockTo(m_OutputView);
      m_ChannelMonitorView.DockTo(m_OutputView);
      g_pNoticeQueueManager->m_pTextQueueView->DockTo(m_OutputView);
      g_pPrivMsgQueueManager->m_pTextQueueView->DockTo(m_OutputView);      
      g_pUrlCatcherManager->m_pTextQueueView->DockTo(m_OutputView);
      g_pEventLogManager->m_pEventLogView->DockTo(m_OutputView);
      break;
    case 3:
      DockWindow(m_ServerListView,dockwins::CDockingSide(dockwins::CDockingSide::sLeft),
						    0/*nBar*/,float(0.6)/*fPctPos*/,150/*nWidth*/,250/* nHeight*/);
      DockWindow(m_UserListView,dockwins::CDockingSide(dockwins::CDockingSide::sRight),
						    0/*nBar*/,float(0.4)/*fPctPos*/,150/*nWidth*/,250/* nHeight*/);
	    DockWindow(m_OutputView,dockwins::CDockingSide(dockwins::CDockingSide::sBottom),
		    0/*nBar*/,float(0.5)/*fPctPos*/,100/*nWidth*/,rcClientArea.Width() / 2/* nHeight*/);
      g_pTransfersManager->m_pTransfersView->DockTo(m_OutputView);
      m_ServerMonitorView.DockTo(m_OutputView);
      m_ChannelMonitorView.DockTo(m_OutputView);
      DockWindow(*g_pNoticeQueueManager->m_pTextQueueView,dockwins::CDockingSide(dockwins::CDockingSide::sBottom),
		    0/*nBar*/,float(0.5)/*fPctPos*/,100/*nWidth*/,rcClientArea.Width() / 2/* nHeight*/);
      g_pPrivMsgQueueManager->m_pTextQueueView->DockTo(g_pNoticeQueueManager->m_pTextQueueView->m_hWnd);
      g_pUrlCatcherManager->m_pTextQueueView->DockTo(g_pNoticeQueueManager->m_pTextQueueView->m_hWnd);
      g_pEventLogManager->m_pEventLogView->DockTo(g_pNoticeQueueManager->m_pTextQueueView->m_hWnd);
      break;
    case 4:
      DockWindow(m_ServerListView,dockwins::CDockingSide(dockwins::CDockingSide::sLeft),
						    0/*nBar*/,float(0.2)/*fPctPos*/,150/*nWidth*/,250/* nHeight*/);
      m_UserListView.DockTo(m_ServerListView);
      m_ServerListView.PinUp(dockwins::CDockingSide::sLeft);
      m_UserListView.PinUp(dockwins::CDockingSide::sLeft);
      break;
    case 5:
      DockWindow(m_ServerListView,dockwins::CDockingSide(dockwins::CDockingSide::sLeft),
						    0/*nBar*/,float(0.2)/*fPctPos*/,150/*nWidth*/,250/* nHeight*/);
      DockWindow(m_UserListView,dockwins::CDockingSide(dockwins::CDockingSide::sLeft),
						    0/*nBar*/,float(0.2)/*fPctPos*/,150/*nWidth*/,250/* nHeight*/);
      m_ServerListView.PinUp(dockwins::CDockingSide::sLeft);
      m_UserListView.PinUp(dockwins::CDockingSide::sLeft);
      g_pTransfersManager->m_pTransfersView->PinUp(dockwins::CDockingSide::sBottom);
      m_ServerMonitorView.PinUp(dockwins::CDockingSide::sBottom);
      m_ChannelMonitorView.PinUp(dockwins::CDockingSide::sBottom);
      m_OutputView.PinUp(dockwins::CDockingSide::sBottom);
      g_pEventLogManager->m_pEventLogView->PinUp(dockwins::CDockingSide::sBottom);
      g_pPrivMsgQueueManager->m_pTextQueueView->PinUp(dockwins::CDockingSide::sBottom);
      g_pUrlCatcherManager->m_pTextQueueView->PinUp(dockwins::CDockingSide::sBottom);
      g_pNoticeQueueManager->m_pTextQueueView->PinUp(dockwins::CDockingSide::sBottom);
      break;

  }
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  bHandled = FALSE;
  return bHandled ? 0 : 1;// WM_DESTROY - If an application processes this message, it should return zero.
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);

	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// remove old menu
	SetMenu(NULL);

  /*
  CMenuHandle MainMenu;
  CMenuHandle HelpMenu;
  MainMenu.Attach(m_hMenu);
  HelpMenu = MainMenu.GetSubMenu(7); // 7th menu from left (file, edit,...)
  HelpMenu.HiliteMenuItem(m_hWnd,ID_HELP_DONATE,MF_BYCOMMAND | MF_HILITE);

  // bleeh, hilite is actually "open the window and select the item"
  // and not make it bold like i wanted it...
  // need to owner draw that item in a bold font
  */

  // load server list images
	m_images_serverlist.Create(16, 16, ILC_COLOR24 | ILC_MASK, 6, 0);
	ATLASSERT(m_images_serverlist.m_hImageList);
	CBitmap bmp;
	bmp.LoadBitmap(IDB_SERVERLIST);
	m_images_serverlist.Add( bmp, RGB(255,0,255));

	// load command bar images
  // RGB(255,0,255)
	m_images_toolbar.CreateFromImage(IDB_TOOLBAR, 16, 17, CLR_DEFAULT, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_SHARED);
	m_CmdBar.m_hImageList = m_images_toolbar;

	m_CmdBar.m_arrCommand.Add(ID_FILE_NEW);
	m_CmdBar.m_arrCommand.Add(ID_FILE_CONNECT);
	m_CmdBar.m_arrCommand.Add(ID_FILE_DISCONNECT);
  // separator
	m_CmdBar.m_arrCommand.Add(ID_WINDOW_CASCADE);
  m_CmdBar.m_arrCommand.Add(ID_WINDOW_TILE_VERT);
  m_CmdBar.m_arrCommand.Add(ID_WINDOW_TILE_HORZ);
  m_CmdBar.m_arrCommand.Add(ID_WINDOW_MINIMIZEALLWINDOWS);
  // separator
  m_CmdBar.m_arrCommand.Add(ID_OPTIONS_TOGGLEUSERLIST);
  m_CmdBar.m_arrCommand.Add(ID_CHANNEL_MONITOR);
  m_CmdBar.m_arrCommand.Add(ID_OPTIONS_LOGGING);
  // separator
  m_CmdBar.m_arrCommand.Add(ID_OPTIONS_AUDIOMUTE);
  m_CmdBar.m_arrCommand.Add(ID_VIEW_EVENTLOG);
  /*
  m_CmdBar.m_arrCommand.Add(ID_VIEW_PRIVMSGQUEUE);
  m_CmdBar.m_arrCommand.Add(ID_VIEW_NOTICEQUEUE);
  */
  m_CmdBar.m_arrCommand.Add(ID_VIEW_OUTPUT);
  m_CmdBar.m_arrCommand.Add(ID_VIEW_CHANNELMONITOR);
  m_CmdBar.m_arrCommand.Add(ID_VIEW_SERVERMONITOR);
  m_CmdBar.m_arrCommand.Add(ID_VIEW_TRANSFERS);
  m_CmdBar.m_arrCommand.Add(ID_VIEW_GLOBALUSERLIST);
  m_CmdBar.m_arrCommand.Add(ID_VIEW_IRCSERVERLIST);

  HWND hWndToolBar = createToolbar();

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

  CreateSimpleStatusBar();

	// subclass the status bar as multipane
	m_Status.SubclassWindow(m_hWndStatusBar);

	// set status bar panes
	int arrPanes[] = { ID_DEFAULT_PANE, IDI_USERLIST, IDI_TRANSFERS, IDI_SERVER };
	m_Status.SetPanes(arrPanes, sizeof(arrPanes) / sizeof(int), false);

	// set status bar pane widths using local workaround
  //m_StatusBarWidths = { 0, 90, 144, 64 };
  m_StatusBarWidths[0] = 0;
  m_StatusBarWidths[1] = 90;
  m_StatusBarWidths[2] = 144;
  m_StatusBarWidths[3] = 64;
	SetStatusPaneWidths(m_StatusBarWidths, sizeof(m_StatusBarWidths) / sizeof(int));

	// set the status bar pane icons
	//m_Status.SetPaneIcon(ID_DEFAULT_PANE, AtlLoadIconImage(IDR_DEFAULT, LR_DEFAULTCOLOR));
	m_Status.SetPaneIcon(IDI_USERLIST, GetAppIcon(ICONID_USERS));
	m_Status.SetPaneIcon(IDI_TRANSFERS, GetAppIcon(ICONID_TRANSFERS));
	m_Status.SetPaneIcon(IDI_SERVER, GetAppIcon(ICONID_SERVER_CONNECTED)); // TODO: use disconnected icon and adjust UpdateStatusBar() to use conencted or disconnected icons when connected server count is >0 or 0 respectivly

#ifdef USE_XPCOMMANDBAR
  m_CmdBar.Prepare();
#endif

  // Create MDI Client
  HMENU hWindowMenu = GetStandardWindowMenu(m_hMenu);
  CreateMDIClient(hWindowMenu);

#ifdef USE_TABBEDMDI
	// If you want to use the MDI child's document icon, uncomment the following:
	//m_tabbedClient.UseMDIChildIcon(TRUE);

	// If you want to only show MDI tabs when the MDI children
	// are maximized, uncomment the following:
	//m_tabbedClient.HideMDITabsWhenMDIChildNotMaximized(TRUE);

	m_tabbedClient.SetTabOwnerParent(m_hWnd);
	BOOL bSubclass = m_tabbedClient.SubclassWindow(m_hWndMDIClient);
	m_CmdBar.UseMaxChildDocIconAndFrameCaptionButtons(true);
	//m_CmdBar.UseMaxChildDocIconAndFrameCaptionButtons(false);
#endif

  m_CmdBar.SetMDIClient(m_hWndMDIClient);

	UIAddToolBar(hWndToolBar);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);  

  InitializeDockingFrame();

  // Initialise the managers and their viewss!
  g_pEventLogManager = new EventLogManager;
  g_pNoticeQueueManager = new NoticeQueueManager;
  g_pPrivMsgQueueManager = new PrivMsgQueueManager;
  g_pUrlCatcherManager = new UrlCatcherManager;
  g_pTransfersManager = new TransfersManager(&g_EventManager);

  // Create the windows, and dock them
	CRect rcDef(0,0,400,150);
  DWORD dwStyle=WS_OVERLAPPEDWINDOW | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

  // Server List
  m_ServerListView.Create(m_hWnd, rcDef, _T("Server List"), 
    dwStyle, WS_EX_TOOLWINDOW);
  DockWindow(m_ServerListView,dockwins::CDockingSide(dockwins::CDockingSide::sLeft),
						0/*nBar*/,float(0.6)/*fPctPos*/,150/*nWidth*/,250/* nHeight*/);

  // Global User List
  m_UserListView.Create(m_hWnd, rcDef, _T("User List"), 
    dwStyle, WS_EX_TOOLWINDOW);
  DockWindow(m_UserListView,dockwins::CDockingSide(dockwins::CDockingSide::sLeft),
						0/*nBar*/,float(0.4)/*fPctPos*/,150/*nWidth*/,250/* nHeight*/);

	CRect rcClientArea;
	::GetClientRect(m_hWnd, &rcClientArea);

  //dwStyle=WS_OVERLAPPEDWINDOW | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

  m_OutputView.Create(m_hWnd, rcDefault, _T("Output"), 
    dwStyle, WS_EX_TOOLWINDOW);
	DockWindow(m_OutputView,dockwins::CDockingSide(dockwins::CDockingSide::sBottom),
		0/*nBar*/,float(0.5)/*fPctPos*/,100/*nWidth*/,rcClientArea.Width() / 2/* nHeight*/);

  //dwStyle=WS_OVERLAPPEDWINDOW | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

  g_pTransfersManager->m_pTransfersView->Create(m_hWnd, rcDefault, _T("Transfers"), 
    dwStyle, WS_EX_TOOLWINDOW);
  g_pTransfersManager->m_pTransfersView->DockTo(m_OutputView);

	m_ServerMonitorView.Create(m_hWnd, rcDefault, _T("Server Monitor"), 
    dwStyle, WS_EX_TOOLWINDOW);
  m_ServerMonitorView.DockTo(m_OutputView);

	m_ChannelMonitorView.Create(m_hWnd, rcDefault, _T("Channel Monitor"), 
    dwStyle, WS_EX_TOOLWINDOW);// | WS_EX_TOPMOST);
  m_ChannelMonitorView.DockTo(m_OutputView);

  g_pNoticeQueueManager->m_pTextQueueView->Create(m_hWnd, rcDefault, _T("Notice Messages"), 
    dwStyle, WS_EX_TOOLWINDOW);
  DockWindow(*g_pNoticeQueueManager->m_pTextQueueView,dockwins::CDockingSide(dockwins::CDockingSide::sBottom),
		0/*nBar*/,float(0.5)/*fPctPos*/,100/*nWidth*/,rcClientArea.Width() / 2/* nHeight*/);
  
	g_pPrivMsgQueueManager->m_pTextQueueView->Create(m_hWnd, rcDefault, _T("Private Messages"), 
    dwStyle, WS_EX_TOOLWINDOW);
  g_pPrivMsgQueueManager->m_pTextQueueView->DockTo(g_pNoticeQueueManager->m_pTextQueueView->m_hWnd);

  g_pUrlCatcherManager->m_pTextQueueView->Create(m_hWnd, rcDefault, _T("URL Catcher"),
    dwStyle, WS_EX_TOOLWINDOW);
  g_pUrlCatcherManager->m_pTextQueueView->DockTo(g_pNoticeQueueManager->m_pTextQueueView->m_hWnd);

	g_pEventLogManager->m_pEventLogView->Create(m_hWnd, rcDefault, _T("Event Log"), 
    dwStyle, WS_EX_TOOLWINDOW);
  g_pEventLogManager->m_pEventLogView->DockTo(g_pNoticeQueueManager->m_pTextQueueView->m_hWnd);

  ClearGlobalUserList();

  PostMessage(CWM_INITIALIZE);

	return 0;
}

LRESULT CMainFrame::OnInitialize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	sstate::CDockWndMgrEx mgrDockWnds(m_hWnd);	

	mgrDockWnds.Add(sstate::CDockingWindowStateAdapterEx<CServerListView>(m_ServerListView));
  mgrDockWnds.Add(sstate::CDockingWindowStateAdapterEx<CUserListView>(m_UserListView));
	mgrDockWnds.Add(sstate::CDockingWindowStateAdapterEx<COutputView>(m_OutputView));
	mgrDockWnds.Add(sstate::CDockingWindowStateAdapterEx<CChannelMonitorView>(m_ChannelMonitorView));
	mgrDockWnds.Add(sstate::CDockingWindowStateAdapterEx<CServerMonitorView>(m_ServerMonitorView));
	mgrDockWnds.Add(sstate::CDockingWindowStateAdapterEx<CEventLogView>(*g_pEventLogManager->m_pEventLogView));
	mgrDockWnds.Add(sstate::CDockingWindowStateAdapterEx<CTextQueueView>(*g_pPrivMsgQueueManager->m_pTextQueueView));
	mgrDockWnds.Add(sstate::CDockingWindowStateAdapterEx<CTextQueueView>(*g_pNoticeQueueManager->m_pTextQueueView));
	mgrDockWnds.Add(sstate::CDockingWindowStateAdapterEx<CTextQueueView>(*g_pUrlCatcherManager->m_pTextQueueView));
	mgrDockWnds.Add(sstate::CDockingWindowStateAdapterEx<CTransfersView>(*g_pTransfersManager->m_pTransfersView));

	m_stateMgr.Initialize(_T("SOFTWARE\\HydraProductions\\HydraIRC\\WindowStates"),m_hWnd,SW_SHOWMAXIMIZED);
	m_stateMgr.Add(sstate::CRebarStateAdapter(m_hWndToolBar));
	m_stateMgr.Add(sstate::CToggleWindowAdapter(m_hWndStatusBar));
	m_stateMgr.Add(mgrDockWnds);
  if (!m_stateMgr.Restore())
  {
    // if no state was restored, then do some defaults.
    //ShowWindow(SW_SHOWMAXIMIZED); // maximize it by default


    m_ServerListView.ShowWindow(SW_SHOW);
    m_UserListView.ShowWindow(SW_SHOW);
    m_OutputView.ShowWindow(SW_HIDE); // don't show by default.
    m_ChannelMonitorView.ShowWindow(SW_SHOW);
    m_ServerMonitorView.ShowWindow(SW_SHOW);
    g_pEventLogManager->m_pEventLogView->ShowWindow(SW_SHOW);
    g_pTransfersManager->m_pTransfersView->ShowWindow(SW_SHOW);
    g_pPrivMsgQueueManager->m_pTextQueueView->ShowWindow(SW_HIDE);
    g_pNoticeQueueManager->m_pTextQueueView->ShowWindow(SW_HIDE);
    g_pUrlCatcherManager->m_pTextQueueView->ShowWindow(SW_SHOW);
  }

  // when these were created, we didn't have the prefs loaded
  // so we need to update them now so that the fonts and colors are set
  m_OutputView.UpdateSettings(); 
  m_ChannelMonitorView.UpdateSettings(); 
  m_ServerMonitorView.UpdateSettings(); 
  m_ServerListView.UpdateSettings();
  //m_UserListView.SetColors(g_pPrefs->m_ColorPrefs); // done by UpdateSettings()
  m_UserListView.UpdateSettings();
  g_pEventLogManager->m_pEventLogView->UpdateSettings();
  g_pTransfersManager->m_pTransfersView->UpdateSettings();
  g_pPrivMsgQueueManager->m_pTextQueueView->UpdateSettings();
  g_pNoticeQueueManager->m_pTextQueueView->UpdateSettings();
  g_pUrlCatcherManager->m_pTextQueueView->UpdateSettings();

  // Show the window!
	UpdateLayout();

  // create the timer, and set the EV_TICK event in motion.
  m_Timer = SetTimer(TID_MAINFRAME,1000,(TIMERPROC)NULL);
  g_EventManager.QueueTimedEvent(NULL,TRUE,EV_TICK,1,TRUE,TRUE);

  // load the plugins!
  g_pPluginManager->RefreshPlugins();

  // TODO: add option to enable/disable identd
  g_pIdentd = new Identd(m_hWnd);

  // create the DNS Resolver
  g_pDNSResolver = new CDNSResolver;

  sys_Printf(BIC_INFO,"HydraIRC " VERSIONSTRING " by Dominic Clifton (c) Copyright 2002-2006\n");
  sys_Printf(BIC_INFO,"Ready.\n"); // C64's rool!

  SetStatusBar(STATUS_ICON_NOTE,g_DefaultStrings[DEFSTR_Status_Welcome]);
  
// dump colors
#if 0
  m_OutputView.m_MsgView.SetControlCodeMode(CTRLCODE_MODE_INTERPRET);
  sys_Printf(BIC_NORMAL,"Dumping Colors to output window!\n");
  sys_Printf(BIC_UNKNOWN,"BIC_UNKNOWN\n");
  sys_Printf(BIC_NOTICE,"BIC_NOTICE\n");
  sys_Printf(BIC_TOPIC,"BIC_TOPIC\n");
  sys_Printf(BIC_JOIN,"BIC_JOIN\n");
  sys_Printf(BIC_KICK,"BIC_KICK\n");
  sys_Printf(BIC_MODE,"BIC_MODE\n");
  sys_Printf(BIC_NICK,"BIC_NICK\n");
  sys_Printf(BIC_PART,"BIC_PART\n");
  sys_Printf(BIC_ACTION,"BIC_ACTION\n");
  sys_Printf(BIC_QUIT,"BIC_QUIT\n");
  sys_Printf(BIC_CONNECTING,"BIC_CONNECTING\n");
  sys_Printf(BIC_CHANNELTEXT,"BIC_CHANNELTEXT\n");
  sys_Printf(BIC_OWNTEXT,"BIC_OWNTEXT\n");
  sys_Printf(BIC_QUERYTEXT,"BIC_QUERYTEXT\n");
  sys_Printf(BIC_SERVERMSG,"BIC_SERVERMSG\n");
  sys_Printf(BIC_CTCP,"BIC_CTCP\n");
  sys_Printf(BIC_FUNCTION,"BIC_FUNCTION\n");
  sys_Printf(BIC_SOCKET,"BIC_SOCKET\n");
  sys_Printf(BIC_WARNING,"BIC_WARNING\n");
  sys_Printf(BIC_ERROR,"BIC_ERROR\n");
  sys_Printf(BIC_GUI,"BIC_GUI\n");
  sys_Printf(BIC_INFO,"BIC_INFO\n");
  sys_Printf(BIC_XML,"BIC_XML\n");
  sys_Printf(BIC_CHANNELTEXT,"Colors: \0030White  \0031black \0032dark blue \0033dark green \0034red  \0035brown \0036purple \0037orange\n");
  sys_Printf(BIC_CHANNELTEXT,"Colors: \0038yellow \0039green \00310dark cyan \00311cyan       \00312blue \00313pink  \00314gray   \00315light gray\n");
  m_OutputView.m_MsgView.SetControlCodeMode(CTRLCODE_MODE_DISPLAY);
#endif

  if (g_pPrefs->m_UserIdentitiesList.GetSize() == 0)
  {
    if (IDYES == MessageBox("You have no configured identities, would you like to create one now?","No Identities!", MB_YESNO | MB_ICONQUESTION))
    {
      DoPrefs();
    }
  }

  if (g_pPrefs->m_UserIdentitiesList.GetSize() > 0)
  {
    if(GetAsyncKeyState(VK_SHIFT) & 0x8000)
    {
      sys_Printf(BIC_WARNING,"SHIFT key pressed - Not running 'OnStartup' command profile");
    }
    else
    {
      // connect to servers on startup here

      // create a blank server window and run "OnStartup" command profile

      IRCServer *pNewServer = new IRCServer(NULL,g_pPrefs->m_UserIdentitiesList[0],FALSE);
      if (pNewServer)
      {
        pNewServer->Printf(BIC_INFO,"\002Welcome to HydraIRC! " VERSIONSTRING "\002");
        pNewServer->Printf(BIC_INFO,"Use the \035\"Connect\"\035 toolbar button or press \035\"Ctrl+S\"\035 to get started!");

        if (pNewServer->m_pChildWnd && g_pPrefs->FindCommandProfile("OnStartup"))
        {
          CHILDFRAMEPTR(pNewServer->m_pChildWnd)->ProcessTextInput("/exec OnStartup",TRUE);
        }
      }
    }

    // process the command line
    if (g_CommandLine && *g_CommandLine)
      HydraIRC_NewServer(g_CommandLine);
  }


  m_FavoriteRootItems = 0;

  BuildFavoritesMenu();

  return 0;
}

void CMainFrame::BuildFavoritesMenu( void )
{
  CMenuHandle MainMenu;
  CMenuHandle FavoritesMenu;
  MainMenu.Attach(m_hMenu);
  FavoritesMenu = MainMenu.GetSubMenu(4); // 5th menu from left (file, edit,...)

  // delete all after 2nd item
  while (m_FavoriteRootItems)
  {
    FavoritesMenu.DeleteMenu(2,MF_BYPOSITION);
    m_FavoriteRootItems --;
  }

  // Relinquish all the ID's we allocated
  m_FavoriteIDs.RelinquishAll();

  AddMenuItemsFromXML(FavoritesMenu, &m_FavoriteIDs, g_pPrefs->m_Favorites, &m_FavoriteRootItems);

  DrawMenuBar();

  return;
}

LRESULT CMainFrame::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  bHandled = FALSE;
  sys_Printf(BIC_INFO,"Trying to quit...\n");

  if (m_OkToQuit == 0) // see CChildFrame::CMD_Exit()
  {    
    int ActiveTransfers = g_pTransfersManager->GetActiveTransferCount(DCC_ANY);
    int ConnectedServers = GetConnectedServerCount();

    BOOL AllowShutdown = g_pPluginManager->RequestShutdown();

    if (ConnectedServers > 0 || ActiveTransfers > 0 || !AllowShutdown)
    {
      char msgstr[200];
      char len = 0; 

      if (ConnectedServers > 0)
        len = _snprintf(msgstr,sizeof(msgstr),"There are %d session(s) still active!\n",ConnectedServers);
      
      if (ActiveTransfers > 0)
        len += _snprintf(msgstr+len,sizeof(msgstr)-len,"There are %d file transfers(s) still active!\n",ActiveTransfers);

      if (!AllowShutdown)
        len += _snprintf(msgstr+len,sizeof(msgstr)-len,"There are one or more busy plugins\n");

      _snprintf(msgstr+len,sizeof(msgstr)-len,"\nQuit HydraIRC?");
      if (IDNO == MessageBox(msgstr,"Exit?", MB_YESNO | MB_ICONQUESTION))
      {
        // Don't close
        bHandled = TRUE;
        return bHandled ? 1 : 0; // WM_CLOSE - If an application processes this message, it should return zero
      }
    }
  }

  // tell everything to die!
  g_EventManager.DoEvent(NULL,TRUE,EV_DIEDIEDIE);

  while (g_ServerList.GetSize() > 0)
    delete g_ServerList[0];

  if (g_ChildWndList.GetSize() != 0)
  {
    // Don't close yet, wait a bit for child windows to finish up
    m_OkToQuit = 1; // don't prompt again..
    PostMessage(WM_CLOSE); // retry in a moment..
    bHandled = TRUE;
    return bHandled ? 1 : 0; // WM_CLOSE - If an application processes this message, it should return zero
  }

  if (m_Timer)
    KillTimer(TID_MAINFRAME);

  g_EventManager.DeleteEvents(NULL,EV_TICK);

  g_pPluginManager->Shutdown();
    
  #ifdef DEBUG  
    sys_Printf(BIC_INFO,"Main window closing\n");
  #endif   

  m_stateMgr.Store();

#ifdef USE_TABBEDMDI
  m_tabbedClient.UnsubclassWindow();
#endif

  return bHandled ? 1 : 0; // WM_CLOSE - If an application processes this message, it should return zero
}

void CMainFrame::DoPrefs( int PageNum )
{
  // display sever connection dialog box
  g_pNewPrefs = new Prefs(g_pPrefs);
	CPrefsDlg dlg(PageNum);

	int result = dlg.DoModal();
  if (result == 1)
  {
    g_pNewPrefs->SavePrefs();
    m_stateMgr.Store();

    // Apply new prefs. (just swap the pointers and delete the old prefs).
    Prefs *pOldPrefs = g_pPrefs;
    g_pPrefs = g_pNewPrefs;
    delete pOldPrefs;
    g_pNewPrefs = NULL;
    UpdateActivePrefs();

    g_EventManager.DoEvent(NULL,TRUE,EV_PREFSCHANGED);
    RefreshAllChildWindows();
    BuildFavoritesMenu();
  }
  else
  {
    // user clicked cancel, discard the new prefs.
    delete g_pNewPrefs;
  }
}

// find a favorite XML node in an xml tree with a matching ID.
xmlNodePtr FindXMLNodeByID(xmlNodePtr pRootNode, int ID)
{
  xmlNodePtr pNode = pRootNode;
  while (pNode)
  {
    if (pNode->type == XML_ELEMENT_NODE)
    {
      char *idstr = (char *)xmlGetProp(pNode,(xmlChar *)"id");
      if (idstr)
      {
        int NodeID;
        if (sscanf(idstr,"%d",&NodeID) == 1)
        {
          if (NodeID == ID)
          {
            free(idstr);
            return pNode;
          }
        }
        free(idstr);
      }
      if (pNode->children)
      {
        xmlNodePtr pFoundNode = FindXMLNodeByID(pNode->children,ID);
        if (pFoundNode)
          return (pFoundNode);
      }
    }
    pNode = pNode->next;
  }
  return NULL;
}

IRCServerDetails *GetNextServer(int NetworkID, IRCServerDetails *pCurrent = NULL)
{
  // only search for a server on the same network if the network is known
  if (NetworkID <= 0)
    return NULL;

  IRCServerDetails *pDetails;
  for (int s = 0 ; s < g_ServerDetailsList.GetSize() ; s++ )
  {
    pDetails = g_ServerDetailsList[s];
    // but only if they're on the same network.
    if (pDetails->m_NetworkID == NetworkID)
    {
      return pDetails;
    }
  }
  return NULL;
}

LRESULT CMainFrame::OnFavoritesItemSelected(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  char *servername = NULL, *name = NULL, *type = NULL, *data = NULL, *networkidstr = NULL, *portstr = NULL, *password = NULL, *key = NULL;
  int networkid = -1, port = 6667;
  xmlNodePtr pNode = FindXMLNodeByID(g_pPrefs->m_Favorites,wID);
  if (pNode)
  {
    name = (char *)xmlGetProp(pNode,(xmlChar *)"name");
    servername = (char *)xmlGetProp(pNode,(xmlChar *)"servername");
    key = (char *)xmlGetProp(pNode,(xmlChar *)"key");
    type = (char *)xmlGetProp(pNode,(xmlChar *)"type");
    data = (char *)xmlGetProp(pNode,(xmlChar *)"data");
    password = (char *)xmlGetProp(pNode,(xmlChar *)"password");
    portstr = (char *)xmlGetProp(pNode,(xmlChar *)"port");
    networkidstr = (char *)xmlGetProp(pNode,(xmlChar *)"networkid");

    if (networkidstr)
      sscanf(networkidstr,"%d",&networkid);
    if (portstr)
      sscanf(portstr,"%d",&port);

    if (type && name && (data || servername))
    {
      switch(toupper(*type))
      {
        case 'S':
          if (servername)
          {
            IRCServer *pServer = FindServerByName(servername,port);
            if (pServer && pServer->m_pChildWnd)
            {
              pServer->m_pChildWnd->ActivateWindow();
              //g_pMainWnd->MDIActivate(pServer->m_pChildWnd->m_hWnd);
            }
            if (!pServer) // not else!
            {
              // TODO: Move this code to a function
              // we need an identity!
              if (g_pPrefs->m_UserIdentitiesList.GetSize() > 0)
              {
                IRCServerDetails *pDetails = new IRCServerDetails();
                if (pDetails)
                {
                  SetString(pDetails->m_Name,servername);
                  SetString(pDetails->m_Password,password);
                  SetString(pDetails->m_Description,name); // use the displayname
                  pDetails->m_Port = port;
                  pDetails->m_NetworkID = networkid;
                }

                if (pDetails->IsValid())
                  pServer = new IRCServer(pDetails);

                if (pDetails)
                  delete pDetails;
              }
            }
          }
          break;
        case 'Q':
          if (data)
          {
            IRCServer *pServer = FindServerOnNetwork(networkid);
            if (!pServer)
              pServer = FindServerByName(servername,port);

            if (pServer)
            {
              IRCQuery *pQuery = pServer->FindQuery(data);
              if (!pQuery)
                pQuery = pServer->CreateQuery(data,NULL);

              if (pQuery && pQuery->m_pChildWnd )
                pQuery->m_pChildWnd->ActivateWindow();
                //g_pMainWnd->MDIActivate(CHILDFRAMEPTR(pQuery->m_pChildWnd)->m_hWnd);
            }
            else
            {
              // TODO: Move this code to a function
              // we need an identity!
              if (g_pPrefs->m_UserIdentitiesList.GetSize() > 0)
              {
                IRCServerDetails *pDetails;

                if (networkid != NETWORK_UNKNOWN)
                {
                  pDetails = GetNextServer(networkid);
                }
                else
                {
                  pDetails = new IRCServerDetails();
                  if (pDetails)
                  {
                    SetString(pDetails->m_Name,servername);
                    SetString(pDetails->m_Password,password);
                    SetString(pDetails->m_Description,servername); // use the servername
                    pDetails->m_Port = port;
                    pDetails->m_NetworkID = networkid;
                  }
                }

                if (pDetails && pDetails->IsValid()) // call IsValid, because we might not have "servername" set...
                {
                  pServer = new IRCServer(pDetails);
                  char *commandstr = HydraIRC_BuildString(512,"/hidewindow\n/query %s",data,data);
                  if (commandstr)
                  {
                    CCommandProfile *pCommands = new CCommandProfile("Favorite Clicked",commandstr);
                    if (pCommands)
                      pServer->ExecCommandProfile(pCommands,TRUE);
                  }
                }

                if (networkid == NETWORK_UNKNOWN)
                  delete pDetails;

              }
            }
          }
          break;
        case 'C':
          if (data)
          {
            IRCServer *pServer = FindServerOnNetwork(networkid);
            if (!pServer)
              pServer = FindServerByName(servername,port);

            if (pServer)
            {
              IRCChannel *pChannel = pServer->FindChannel(data);
              if (!pChannel)
              {
                pChannel = pServer->CreateChannel(data);
                if (pChannel)
                  pChannel->Join();
              }

              if (pChannel && pChannel->m_pChildWnd )
                pChannel->m_pChildWnd->ActivateWindow();
                //g_pMainWnd->MDIActivate(CHILDFRAMEPTR(pChannel->m_pChildWnd)->m_hWnd);                
            }
            else
            {
              // we need an identity!
              if (g_pPrefs->m_UserIdentitiesList.GetSize() > 0)
              {

                IRCServerDetails *pDetails;

                if (networkid != NETWORK_UNKNOWN)
                {
                  pDetails = GetNextServer(networkid);
                }
                else
                {
                  pDetails = new IRCServerDetails();
                  if (pDetails)
                  {
                    SetString(pDetails->m_Name,servername);
                    SetString(pDetails->m_Password,password);
                    SetString(pDetails->m_Description,servername); // use the servername
                    pDetails->m_Port = port;
                    pDetails->m_NetworkID = networkid;
                  }
                }

                if (pDetails && pDetails->IsValid()) // call IsValid, because we might not have "servername" set...
                {
                  pServer = new IRCServer(pDetails);
                  char *commandstr = HydraIRC_BuildString(512,"/hidewindow\n/join %s%s%s",data,key ? " " : "", key ? key : "");
                  if (commandstr)
                  {
                    CCommandProfile *pCommands = new CCommandProfile("Favorite Clicked",commandstr);
                    if (pCommands)
                      pServer->ExecCommandProfile(pCommands,TRUE);
                  }
                }

                if (networkid == NETWORK_UNKNOWN)
                  delete pDetails;
              }
            }
          }
          break;
      }
    }
  }

  sys_Printf(BIC_INFO,"Favorite with id of %d selected! - %s",wID,name ? name : "Unknown");  

  if (networkidstr) free(networkidstr);
  if (name) free(name);
  if (data) free(data);
  if (type) free(type);
  if (key) free(key);
  if (password) free(password);
  if (portstr) free(portstr);
  return 0;
}

LRESULT CMainFrame::OnFavoritesAddToFavorites(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{	

  CChildCommon *pChildWnd = FindChildWindow(MDIGetActive());
  CChildFrame *pChildFrame = CHILDFRAMEPTR(pChildWnd);
  if (pChildWnd)
  {
    char *data = NULL;
    char *servername = NULL;
    int networkid = -1;
    int port = -1;
    char *password = NULL;
    char *key = NULL;
    char typestr[2] = "?";

    switch(pChildFrame->m_WindowType)
    {
      case CWTYPE_SERVER:
        *typestr = 'S';
        servername = pChildFrame->m_pServer->m_pDetails->m_Name;
        networkid = pChildFrame->m_pServer->m_pDetails->m_NetworkID;
        port = pChildFrame->m_pServer->m_pDetails->m_Port;
        password = pChildFrame->m_pServer->m_pDetails->m_Password;
        break;

      case CWTYPE_CHANNEL:
        *typestr = 'C';
        data = pChildFrame->m_pChannel->m_pProperties->m_Name;
        networkid = pChildFrame->m_pChannel->m_pServer->m_pDetails->m_NetworkID;        
        key = pChildFrame->m_pChannel->m_pProperties->m_Key;

        // remember the server details too
        servername = pChildFrame->m_pChannel->m_pServer->m_pDetails->m_Name;
        port = pChildFrame->m_pChannel->m_pServer->m_pDetails->m_Port;
        password = pChildFrame->m_pChannel->m_pServer->m_pDetails->m_Password;
        break;

      case CWTYPE_QUERY:
        *typestr = 'Q';
        data = pChildFrame->m_pQuery->m_OtherNick;
        networkid = pChildFrame->m_pQuery->m_pServer->m_pDetails->m_NetworkID;

        // rememeber the server details too
        servername = pChildFrame->m_pQuery->m_pServer->m_pDetails->m_Name;
        port = pChildFrame->m_pQuery->m_pServer->m_pDetails->m_Port;
        password = pChildFrame->m_pQuery->m_pServer->m_pDetails->m_Password;
        break;
    }

    if (data || servername)
    {
      xmlNodePtr pNewNode = xmlNewNode(NULL,(xmlChar *)"favorite");
      if (pNewNode)
      {
        char numstr[10];

        char *displayname = NULL;

        if (*typestr == 'S')
        {
          displayname = HydraIRC_BuildString(256,"%s for network %s (%s)",servername,GetNetworkName(networkid), GetTimestamp(NULL, "%d/%m/%y"));
        }
        else
        {
          displayname = HydraIRC_BuildString(256,"%s on %s (%s)",data,networkid > 0 ? GetNetworkName(networkid) : servername, GetTimestamp(NULL, "%d/%m/%y"));
          xmlSetProp(pNewNode,(xmlChar *)"data",(xmlChar *)data);
        }

        xmlSetProp(pNewNode,(xmlChar *)"name",(xmlChar *)displayname); // name is just a display name
        xmlSetProp(pNewNode,(xmlChar *)"servername",(xmlChar *)servername);
        xmlSetProp(pNewNode,(xmlChar *)"type",(xmlChar *)typestr);

        _snprintf(numstr,sizeof(numstr),"%d",networkid);
        xmlSetProp(pNewNode,(xmlChar *)"networkid",(xmlChar *)numstr);

        _snprintf(numstr,sizeof(numstr),"%d",port);
        if (port != -1)
          xmlSetProp(pNewNode,(xmlChar *)"port",(xmlChar *)numstr);

        if (password && *password)
          xmlSetProp(pNewNode,(xmlChar *)"password",(xmlChar *)password);
        if (key && *key)
          xmlSetProp(pNewNode,(xmlChar *)"key",(xmlChar *)key);


        if (displayname)
        {
          CFavoritesAddDlg dlg(pNewNode);
      	  int result = dlg.DoModal();
          if (result == 0) // canceled!
          {
            xmlFreeNode(pNewNode);
          }
          else
          {
            BuildFavoritesMenu();
          }
          free(displayname);
        }
        else
        {
          xmlFreeNode(pNewNode);
        }
      }
    }
  }
  return 0;
}

LRESULT CMainFrame::OnFavoritesOrganizeFavorites(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CFavoritesDlg dlg;

	int result = dlg.DoModal();
  BuildFavoritesMenu();
  return 0;
}

LRESULT CMainFrame::OnToolbarUserItemSelected(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
{
  // We keep calling CPluginManager::GetFunction() until it
  // returns NULL (which means, no more plugins that provide the function
  // we want)
  // If it does return a pointer to a function then we call it and we
  // set the Handled flag to it's return value.
  HydraIRCPlugin *pPlugin = NULL;
  CNode *pNode = NULL;

  bHandled = FALSE;

  PFN_ONCOMMANDMESSAGE pfn_Func; // Keep variable name this local
  do 
  {
    pfn_Func = (PFN_ONCOMMANDMESSAGE)g_pPluginManager->GetFunction(PLUGIN_API_GUI,PLUGIN_GUI_ONCOMMANDMESSAGE,&pPlugin,&pNode);

    if (pfn_Func != NULL)
    {
      bHandled = pfn_Func(wID);
    }
  } while (!bHandled && pfn_Func != NULL);
  return 0;
}

LRESULT CMainFrame::OnOptionsPrefs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  DoPrefs();
	return 0;
}

LRESULT CMainFrame::OnToggleOptionsAudioMute(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  BOOLPREF(PREF_bAudioMute) = !BOOLPREF(PREF_bAudioMute); // toggle it
	return 0;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  HydraIRC_NewServer(NULL);

	return 0;
}



LRESULT CMainFrame::OnFileCloseWindow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{  
  HWND hWnd = MDIGetActive();
  if( hWnd ) ::PostMessage(hWnd, WM_CLOSE, 0, 0);

  return 0;
}

LRESULT CMainFrame::OnFileDisconnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{  
  //m_stateMgr.Store(); // why the fuck is this here ?

  // send a message back to the main window telling it to disconnect from
  // the irc server associated with the window.

  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(g_pMainWnd->MDIGetActive()));
  if (pChildWnd)
  {
    IRCServer *pServer = pChildWnd->m_pServer;
    // don't prompt for DCC Chat windows or if we're already disconnected/connecting
    if (!pServer->m_IsDirect && BOOLPREF(PREF_bPromptOnDisconnect) && pServer->GetStatus() == SVR_STATE_CONNECTED)
    {
      if (IDNO == MessageBox(_T("Server is still connected! Disconnect ?"),_T(pServer->m_pDetails->m_Name), MB_YESNO | MB_ICONQUESTION))
        return 0;       
    }
    pServer->Disconnect(FALSE);
  }
  return 0;
}

LRESULT CMainFrame::OnFileConnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{  
  // send a message back to the main window telling it to re-connect to
  // the irc server associated with the window.
  HWND hWnd = MDIGetActive();  
  if( hWnd )
  {
    IRCServer *pServer = FindIRCServer(hWnd);
    if (pServer)
    {
      if (pServer->GetStatus() == SVR_STATE_CONNECTED) // use server's actual status, not child window's (possibly out of date) copy of the status
      {
        if (IDNO == MessageBox(_T("Server already connected, re-connect?"),_T(pServer->m_pDetails->m_Name), MB_YESNO | MB_ICONQUESTION))
          return 0;

        pServer->Disconnect(FALSE);
        pServer->Reconnect();        
      }
      else
      {
        // display sever connection dialog box
        IRCServerDetails *pDetails = new IRCServerDetails(pServer->m_pDetails);
        UserIdentity *pIdentity = new UserIdentity(pServer->m_pIdentity);
        
        if (pIdentity && pDetails)
        {
	        CConnectDlg dlg = CConnectDlg(pDetails, pIdentity); 
	        int result = dlg.DoModal();

          if (result == 1)
          {
            // user clicked OK, so copy the modified user details and identity back to
            // the server instance and then reconnect.
            pServer->m_pDetails->Clear();
            pServer->m_pDetails->Copy(pDetails);
            pServer->m_pIdentity->Clear();
            pServer->m_pIdentity->Copy(pIdentity);
            pServer->SetNick(strdup(pServer->m_pIdentity->GetNick(0)));
            pServer->Reconnect();
          }
        }
        if (pIdentity) delete pIdentity;
        if (pDetails) delete pDetails;

      }        
    }
  }

  return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = IsToolbarVisible(ID_TOOLBAR);
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ID_TOOLBAR);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	// UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewGuiLayout(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  switch(wID)
  {
    case ID_GUILAYOUT_HIDEALLDOCKERS:
      ChangeGUILayoutStyle(0);
      break;
    case ID_GUILAYOUT_LAYOUT1:
      ChangeGUILayoutStyle(1);
      break;
    case ID_GUILAYOUT_LAYOUT2:
      ChangeGUILayoutStyle(2);
      break;
    case ID_GUILAYOUT_LAYOUT3:
      ChangeGUILayoutStyle(3);
      break;
    case ID_GUILAYOUT_LAYOUT4:
      ChangeGUILayoutStyle(4);
      break;
    case ID_GUILAYOUT_LAYOUT5:
      ChangeGUILayoutStyle(5);
      break;
  }
	return 0;
}

LRESULT CMainFrame::OnHelpAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainFrame::OnHelpHydraIRCOnTheWeb(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  ShellExecute(NULL,NULL,"http://www.hydrairc.com",NULL,NULL,SW_SHOWNORMAL);
  return 0;
}

LRESULT CMainFrame::OnHelpDonate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  ShellExecute(NULL,NULL,"https://www.paypal.com/xclick/business=donations%40hydrairc.com&item_name=HydraIRC&item_number=1&no_note=1&tax=0&currency_code=GBP",NULL,NULL,SW_SHOWNORMAL);
  return 0;
}

LRESULT CMainFrame::OnHelpDocumentation(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  char *Docs = HydraIRC_BuildString(1024,"%s\\docs.htm",g_HomeDir);
  ShellExecute(NULL,NULL,Docs,NULL,NULL,SW_SHOWNORMAL);
  return 0;
}

LRESULT CMainFrame::OnHelpRecentChanges(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  char *Changes = HydraIRC_BuildString(1024,"%s\\CHANGES.TXT",g_HomeDir);
  ShellExecute(NULL,NULL,Changes,NULL,NULL,SW_SHOWNORMAL);
  return 0;
}

LRESULT CMainFrame::OnHelpOnlineDocumentation(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  ShellExecute(NULL,NULL,"http://www.hydrairc.com/wiki/",NULL,NULL,SW_SHOWNORMAL);
  return 0;
}

LRESULT CMainFrame::OnHelpReportABug(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  ShellExecute(NULL,NULL,"http://www.hydrairc.com/forums/index.php?act=SF&f=4",NULL,NULL,SW_SHOWNORMAL);
  return 0;
}

LRESULT CMainFrame::OnHelpForums(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  ShellExecute(NULL,NULL,"http://www.hydrairc.com/forums/",NULL,NULL,SW_SHOWNORMAL);
  return 0;
}

LRESULT CMainFrame::OnWindowCascade(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MDICascade();
	return 0;
}

LRESULT CMainFrame::OnWindowTileHorz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MDITile(MDITILE_HORIZONTAL);
	return 0;
}

LRESULT CMainFrame::OnWindowTileVert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MDITile(MDITILE_VERTICAL);
	return 0;
}

LRESULT CMainFrame::OnWindowMinimizeAllWindows(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  for (int i = 0 ; i < g_ChildWndList.GetSize(); i++)
  {
    CChildCommon *pChildWnd = g_ChildWndList[i];

    //pChildWnd->ShowWindow(SW_MINIMIZE);
    pChildWnd->HideWindow();
  }
  return 0;
}

LRESULT CMainFrame::OnWindowMinimizeActiveWindow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CChildCommon *pChildWnd = CHILDFRAMEPTR(FindChildWindow(MDIGetActive()));

  if (pChildWnd)
  {
    pChildWnd->HideWindow();
    //pChildWnd->ShowWindow(SW_MINIMIZE);
  }

  return 0;
}

LRESULT CMainFrame::OnChannelShowServer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(MDIGetActive()));

  if (pChildWnd)
  {
    switch (pChildWnd->m_WindowType)
    {
      case CWTYPE_QUERY:
      case CWTYPE_CHANNEL:
        if (pChildWnd->m_pServer->m_pChildWnd)
          pChildWnd->m_pServer->m_pChildWnd->ActivateWindow();
          //g_pMainWnd->MDIActivate(pChildWnd->m_pServer->m_pChildWnd->m_hWnd);
        break;
      case CWTYPE_DCCCHAT:
        // chuckle! :)  circular references are great.
        if (pChildWnd->m_pServer->m_pOtherServer->m_pChildWnd)
          pChildWnd->m_pServer->m_pOtherServer->m_pChildWnd->ActivateWindow();
          //g_pMainWnd->MDIActivate(pChildWnd->m_pServer->m_pOtherServer->m_pChildWnd->m_hWnd);
        break;
    }
  }

  return 0;
}

LRESULT CMainFrame::OnChannelChannelList(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(MDIGetActive()));

  if (pChildWnd)
  {
    switch (pChildWnd->m_WindowType)
    {
      case CWTYPE_SERVER:
      case CWTYPE_QUERY:
      case CWTYPE_CHANNEL:
        pChildWnd->m_pServer->ShowChannelListDialog();
        break;
    }
  }
  return 0;
}

LRESULT CMainFrame::OnChannelProperties(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(MDIGetActive()));

  if (pChildWnd)
  {
    if (pChildWnd->m_WindowType == CWTYPE_CHANNEL)
    {
      pChildWnd->m_pChannel->DoPropertiesDlg();
    }
  }

	return 0;
}


// done when we setup, and 
void CMainFrame::ClearGlobalUserList( void )
{
#ifdef DEBUG
  sys_Printf(BIC_INFO,"Clearing Global User List\n");
#endif
  if (m_UserListView.m_pChannel)
  {
    m_UserListView.RemoveAll();
  }
  m_UserListView.m_pChannel = NULL;
  m_UserListView.SetWindowText("No Channel");
}

// if the optional paramater Clear is set to TRUE then
// we check to see if we're looking at pChannel, and if
// so we clear the list.
// this is only done when destroying a channel.
// or when a server disconnects.
void CMainFrame::UpdateGlobalUserList(IRCChannel *pChannel, BOOL Clear, BOOL Refresh)
{
#ifdef DEBUG
  sys_Printf(BIC_INFO,"UpdateGlobalUserList CurrentChannelName: %s NewChannelName: %s Clear:%s\n",m_UserListView.m_pChannel ? m_UserListView.m_pChannel->m_pProperties->m_Name : "NULL" , pChannel ? pChannel->m_pProperties->m_Name : "NULL" , Clear ? "YES" : "NO");
#endif

  if (Refresh || m_UserListView.m_pChannel != pChannel)
  {
    // if a channel class is being destroyed, but we've
    // already set the user list to point to something else, then
    // we skip the update.
    if (pChannel != NULL && Clear)
      return;

    //m_pUserListView->SetRedraw(FALSE); // see below
    
    m_UserListView.m_tree.SetRedraw(FALSE);

    // clean it up.
    if (m_UserListView.m_pChannel)
    {
      m_UserListView.RemoveAll();
    }

    m_UserListView.m_pChannel = pChannel;

    if (pChannel && !Clear)
    {
      for (int i = 0 ; i < pChannel->m_Users.m_UserList.GetSize(); i++)
      {
        m_UserListView.Add(pChannel->m_Users.m_UserList[i]);
      }
    }
    //m_pUserListView->SetRedraw(TRUE);      // this is what i want to do, but i can't seem to force
                                             // a redraw of the window's scrollbars.. :(    
    m_UserListView.m_tree.SetRedraw(TRUE); // so i have to do this instead. :(
    m_UserListView.RedrawWindow();
    m_UserListView.UpdateWindow();
/*
    // none of these attempts work :(

    m_pUserListView->SendMessage(WM_SIZE, SIZE_RESTORED, NULL);
    SendMessage(m_pUserListView->m_hWnd,WM_SIZE, SIZE_RESTORED, NULL);
    m_pUserListView->Invalidate();
    m_pUserListView->m_tree.RedrawWindow();
    RECT r;
    m_pUserListView->GetClientRect(&r);
    m_pUserListView->InvalidateRect(&r);
    CWindow CWnd = m_pUserListView->GetParent();
    CWnd.Invalidate();
    CWnd.RedrawWindow();
*/      
    if (pChannel && !Clear)
    {
      m_UserListView.SetWindowText(pChannel->m_pProperties->m_Name);
      // TODO - REMOVE: Temporary workaround for docked window
      /*
      if(m_pUserListView->IsDocking()
              && dockwins::CDockingBox::IsWindowBox(m_pUserListView->GetOwnerDockingBar()))
      {
        m_pUserListView->Hide();
        m_pUserListView->SetWindowText(pChannel->m_Name);
        m_pUserListView->Show();
      }
      */
    }
  }
  else
  {
    if (Clear)
    {
      ClearGlobalUserList();
    }
  }
  m_UserListView.m_tree.EnsureVisible(m_UserListView.m_tree.GetRootItem());
}

LRESULT CMainFrame::OnEditClearBuffer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  // add to a range handler
#ifdef DEBUG
  //sys_Printf(BIC_INFO,"OnEditClearBuffer\n");
#endif
  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(MDIGetActive()));

  if (!pChildWnd)
    return 0;

  switch (pChildWnd->m_WindowType)
  {
    case CWTYPE_QUERY:
    case CWTYPE_SERVER:
    case CWTYPE_DCCCHAT:
    case CWTYPE_CHANNEL:
      if (pChildWnd && pChildWnd->IsWindow())
      {
        pChildWnd->m_MsgView.ClearBuffer();
      }
      break;
  }
  return 0;
}

LRESULT CMainFrame::OnEditCopyBuffer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
  // add to a range handler
#ifdef DEBUG
  //sys_Printf(BIC_INFO,"OnEditCopy\n");
#endif
  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(MDIGetActive()));

  if (!pChildWnd)
    return 0;

  switch (pChildWnd->m_WindowType)
  {
    case CWTYPE_QUERY:
    case CWTYPE_SERVER:
    case CWTYPE_DCCCHAT:
    case CWTYPE_CHANNEL:

      if (pChildWnd && pChildWnd->IsWindowVisible())    
      {
        pChildWnd->m_MsgView.SetSel(0,-1);
        pChildWnd->m_MsgView.Copy();
        pChildWnd->m_MsgView.SetSel(-1,-1);
      }
      break;
  }
  return 0;
}


LRESULT CMainFrame::OnEditPaste(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  // add to a range handler
#ifdef DEBUG
  //sys_Printf(BIC_INFO,"OnEditPaste\n");
#endif
  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(MDIGetActive()));

  if (!pChildWnd)
    return 0;

  switch (pChildWnd->m_WindowType)
  {
    case CWTYPE_QUERY:
    case CWTYPE_SERVER:
    case CWTYPE_DCCCHAT:
    case CWTYPE_CHANNEL:
      if (pChildWnd && pChildWnd->IsWindowVisible())
      {
        //pChildWnd->m_TextInputView.SendMessage(EM_PASTESPECIAL,CF_TEXT,0);
        pChildWnd->m_TextInputView.SendMessage(WM_PASTE,0,0);
        pChildWnd->m_TextInputView.SetFocus();
      }
      break;
  }
  return 0;
}

LRESULT CMainFrame::OnEditCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
  // add to a range handler
#ifdef DEBUG
  //sys_Printf(BIC_INFO,"OnEditCopy\n");
#endif
  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(MDIGetActive()));

  if (!pChildWnd)
    return 0;

  switch (pChildWnd->m_WindowType)
  {
    case CWTYPE_QUERY:
    case CWTYPE_SERVER:
    case CWTYPE_DCCCHAT:
    case CWTYPE_CHANNEL:

      if (pChildWnd && pChildWnd->IsWindowVisible())    
      {
        pChildWnd->SendMessage(WM_COPY,0,0);
        //pChildWnd->m_TextInputView.SendMessage(WM_COPY,0,0);
      }
      break;
  }
  return 0;
}

LRESULT CMainFrame::OnEditCut(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  // add to a range handler
#ifdef DEBUG
  //sys_Printf(BIC_INFO,"OnEditCut\n");
#endif
  CChildFrame *pChildWnd = CHILDFRAMEPTR(FindChildWindow(MDIGetActive()));

  if (!pChildWnd)
    return 0;

  switch (pChildWnd->m_WindowType)
  {
    case CWTYPE_QUERY:
    case CWTYPE_SERVER:
    case CWTYPE_DCCCHAT:
    case CWTYPE_CHANNEL:
    
      if (pChildWnd && pChildWnd->IsWindowVisible())
      {
        pChildWnd->m_TextInputView.SendMessage(WM_CUT,0,0);
      }
      break;
  }
  return 0;
}


LRESULT CMainFrame::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
#ifdef DEBUG
  if (g_DebugFlags & DBGFLG_TIMER)
    sys_Printf(BIC_INFO,"TIMER: %ld\n",(INT_PTR)wParam);
#endif
  switch((INT_PTR)wParam)
  {
    case TID_MAINFRAME:
      g_EventManager.ProcessEvents();
      UpdateStatusBar();
      break;
  }
  return bHandled ? 0 : 1; // WM_TIMER - An application should return zero if it processes this message.
}

void CMainFrame::SetStatusBar( const int IconID, const char *message )
{
  CChildCommon *pChildWnd = FindChildWindow(MDIGetActive());
  CChildFrame *pChildFrame = CHILDFRAMEPTR(pChildWnd);
  if (pChildFrame)
  {
    if (pChildFrame->m_WindowType == CWTYPE_SERVER ||
        pChildFrame->m_WindowType == CWTYPE_CHANNEL ||
        pChildFrame->m_WindowType == CWTYPE_DCCCHAT ||
        pChildFrame->m_WindowType == CWTYPE_QUERY)
    {
      char *Nick = pChildFrame->m_pServer->GetNick();
      if (!pChildFrame->m_pServer->m_ReclaimNick || stricmp(Nick,pChildFrame->m_pServer->m_ReclaimNick) == 0)
      {
        strncpy(m_StatusBarNick,Nick,sizeof(m_StatusBarNick));
      }
      else
      {
        char *Tmpstr = HydraIRC_BuildString(64,"%s (%s)",Nick,pChildFrame->m_pServer->m_ReclaimNick);
        if (Tmpstr)
        {
          strncpy(m_StatusBarNick,Tmpstr,sizeof(m_StatusBarNick));
          free(Tmpstr);
        }
      }
    }
    m_StatusBarNickIconID = ICONID_USERS; // use this by default.
    switch (pChildFrame->m_WindowType)
    {
      case CWTYPE_CHANNEL:
        IRCUser *pUser = pChildFrame->m_pChannel->m_Users.FindUser(m_StatusBarNick);
        if (pUser)
        {
          if (pUser->m_UserModes & IRCMODE_OP) m_StatusBarNickIconID = ICONID_USEROP;
          else
            if (pUser->m_UserModes & IRCMODE_HALFOP) m_StatusBarNickIconID = ICONID_USERHALFOP;
            else
              if (pUser->m_UserModes & IRCMODE_VOICE) m_StatusBarNickIconID = ICONID_USERVOICE;
              else
                m_StatusBarNickIconID = ICONID_USERNORMAL;
        }
        break;
    }
  }
  if (message)
  {
    strncpy(m_StatusBarText, message, sizeof(m_StatusBarText));
    m_StatusBarText[sizeof(m_StatusBarText) - 1] = 0;

    // TODO: rebuild the string, replacing "%" with "%%"
    // for now we just convert %'s to /'s
    char *p = m_StatusBarText;
    while (*p)
    {
      if (*p == '%')
        *p = '/';
      p++;
    }

  }

  if (IconID != STATUS_ICON_DONTCHANGE)
    m_StatusBarIconID = IconID;
  m_StatusBarTime = time(NULL); // remember when we set the status.
  m_StatusBarDirty = TRUE;
  UpdateStatusBar();
}

void CMainFrame::UpdateStatusBar( void )
{
  HICON hIcon;
  BOOL ChangeIcon;
  char StatusStr[1024];
  time_t CurrentTime = time(NULL);

  // reset the status text after a while.
  if (m_StatusBarTime + 30 < CurrentTime) // 30 seconds.
  {
    SetStatusBar(STATUS_ICON_NONE, g_DefaultStrings[DEFSTR_DefaultStatusBarMessage]);
    return;
  }

  // only update when needed.
  if (!m_StatusBarDirty)
    return;

  m_StatusBarDirty = FALSE;

  m_Status.SetPaneText(ID_DEFAULT_PANE, m_StatusBarText);

  // select an icon for the status text
  ChangeIcon = TRUE;
  switch(m_StatusBarIconID)
  {
    case STATUS_ICON_NOTE:
      hIcon = GetAppIcon(ICONID_NOTE);
      break;
    case STATUS_ICON_INFORMATION:
      hIcon = GetAppIcon(ICONID_INFORMATION);
      break;
    case STATUS_ICON_WARNING:
      hIcon = GetAppIcon(ICONID_WARNING);
      break;
    case STATUS_ICON_DONTCHANGE:
      ChangeIcon = FALSE;
      break;
    case STATUS_ICON_NONE:
    default:
      hIcon = NULL;
      //hIcon = AtlLoadSysIcon(IDI_QUESTION);       // REMOVE just testing..
      break;
  }
  if (ChangeIcon)
    m_Status.SetPaneIcon(ID_DEFAULT_PANE, hIcon);

  HDC hDC = m_Status.GetWindowDC();
  ATLASSERT(hDC);
   ::SelectObject(hDC,m_Status.GetFont());
  SIZE Size;

  // <Nick in use on current server>
  if (g_ServerList.GetSize() > 0)
  {
    m_Status.SetPaneText(IDI_USERLIST,m_StatusBarNick);

    GetTextExtentPoint32(hDC,m_StatusBarNick,strlen(m_StatusBarNick),&Size);
    m_Status.SetPaneWidth(IDI_USERLIST,Size.cx + 16+14);
  }
  else
    m_Status.SetPaneText(IDI_USERLIST,"<N/A>");

  hIcon = GetAppIcon(m_StatusBarNickIconID);
  m_Status.SetPaneIcon(IDI_USERLIST,hIcon);

  if (g_pTransfersManager)
  {
    // "Sends: <Sends>  Receives: <Receives>"
    _snprintf(StatusStr,sizeof(StatusStr),"Sends: %d Receives: %d",g_pTransfersManager->GetActiveTransferCount(DCC_SEND),g_pTransfersManager->GetActiveTransferCount(DCC_RECEIVE));
    m_Status.SetPaneText(IDI_TRANSFERS, StatusStr);

    GetTextExtentPoint32(hDC,StatusStr,strlen(StatusStr),&Size);
    m_Status.SetPaneWidth(IDI_TRANSFERS,Size.cx + 16+14);

  }

  // "<Connected Servers> / <Servers>"
  _snprintf(StatusStr,sizeof(StatusStr),"%d / %d",GetConnectedServerCount(),g_ServerList.GetSize());
  m_Status.SetPaneText(IDI_SERVER, StatusStr);

  GetTextExtentPoint32(hDC,StatusStr,strlen(StatusStr),&Size);
  m_Status.SetPaneWidth(IDI_SERVER,Size.cx + 16+14);

  m_Status.ReleaseDC(hDC);
}

void CMainFrame::RestoreWindow( void )
{
  if (m_TI.IsInstalled())
  {
    m_TI.Uninstall();

    if (m_WasMaximized)
      ShowWindow(SW_MAXIMIZE);
    else
      ShowWindow(SW_RESTORE);

    // the window doesn't come back to the top after the above showwindow for some reason
    // so we call SetWindowPos() to fix this.
    SetWindowPos(HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE );

    SetFocus();

    // make sure the dirty flag is cleared
    HWND hWnd = MDIGetActive();
    if (hWnd)
      ::SetFocus(hWnd);


    // FIXME: trying to remove channel monitor topmost setting, doesn't seem to work 100%
    if (g_pMainWnd->m_ChannelMonitorView.IsWindow())
    {
      DWORD dwStyle = g_pMainWnd->m_ChannelMonitorView.GetWindowLong(GWL_EXSTYLE); 
      dwStyle &= ~(WS_EX_APPWINDOW | WS_EX_TOPMOST); // remove the flags..
	    g_pMainWnd->m_ChannelMonitorView.SetWindowLong( GWL_EXSTYLE , dwStyle); // this ignores the WS_EX_TOPMOST style.
      g_pMainWnd->m_ChannelMonitorView.SetWindowPos(HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE); // so we have to do this too.
    }
  }
}

LRESULT CMainFrame::OnSystrayIconClick(LPARAM /*uMsg*/, BOOL& /*bHandled*/)
{
  RestoreWindow();

  return 0;
}

LRESULT CMainFrame::OnRequestAutoTile(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
  if (BOOLPREF(PREF_bAutoMaximize))
    return 1;

  if (BOOLPREF(PREF_bAutoTile) && ::IsWindow(m_hWndMDIClient))
  {
    BOOL Maximized = FALSE;
    HWND hWnd = MDIGetActive(&Maximized);

    if (!Maximized || hWnd == NULL)
      MDITile(MDITILE_HORIZONTAL);
  }
  bHandled = TRUE;
  return 1;
}

void CMainFrame::MinimizeToSystray( void )
{
  DWORD dwStyle = g_pMainWnd->m_ChannelMonitorView.GetWindowLong(GWL_EXSTYLE);
  dwStyle |= WS_EX_APPWINDOW | WS_EX_TOPMOST; // add the flags
  g_pMainWnd->m_ChannelMonitorView.SetWindowLong( GWL_EXSTYLE , dwStyle); // this ignores the WS_EX_TOPMOST style.
  g_pMainWnd->m_ChannelMonitorView.SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED); // so we have to do this too.

  m_WasMaximized = IsZoomed();
  ShowWindow(SW_HIDE);

  HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
  HMENU hMenu = ::LoadMenu(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_TASKBAR));
  m_TI.Install(m_hWnd, 1, hIcon, hMenu, _T("HydraIRC " VERSIONSTRING));
}

LRESULT CMainFrame::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;

  if(wParam == SIZE_MINIMIZED)
  {
	  if(BOOLPREF(PREF_bMinimizeToSysTray))
    {
      // this occurs when the user presses windows+m or some other app minimizes HydraIRC

      MinimizeToSystray();

      bHandled = TRUE;
    }
  }
  else
  {
    // can't tile here, the window has not been repositioned yet so tiling doesn't work
    // queue up request tile message
    PostMessage(WM_REQUESTAUTOTILE);

    // see CChildFrame::OnSize fr reason why this is commented out
    /*
    // if mdi windows are maximized, tell them to scroll to the end    
    if (::IsWindow(m_hWndMDIClient))
    {
      BOOL Maximized;
      HWND hActiveWnd = g_pMainWnd->MDIGetActive(&Maximized);
      if (Maximized)
        ::PostMessage(hActiveWnd,WM_REQUESTSCROLLTOEND,0,0);
    }
    */

  }

  return bHandled ? 0 : 1; // WM_SIZE - If an application processes this message, it should return zero.
}

LRESULT CMainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  bHandled = FALSE;

  if (wParam == SC_MINIMIZE)
  {
    if(g_pPrefs && BOOLPREF(PREF_bMinimizeToSysTray))
    {
      // this occurs when the user clicks on the minimize button
/*
	    DWORD dwStyle = g_pMainWnd->m_ChannelMonitorView.GetWindowLong(GWL_EXSTYLE);
      dwStyle |= WS_EX_APPWINDOW | WS_EX_TOPMOST; // add the flags
	    g_pMainWnd->m_ChannelMonitorView.SetWindowLong( GWL_EXSTYLE , dwStyle); // this ignores the WS_EX_TOPMOST style.
      g_pMainWnd->m_ChannelMonitorView.SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE); // so we have to do this too.

      m_WasMaximized = IsZoomed();
      ShowWindow(SW_HIDE);

      HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
      HMENU hMenu = ::LoadMenu(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_TASKBAR));
      m_TI.Install(m_hWnd, 1, hIcon, hMenu, NULL);
*/
      MinimizeToSystray();
      bHandled = TRUE;

    }
    /*
    // trying to get the channel monitor window to stay up when the main app is minimized
    // and when minimize to systray is OFF
    else
    {
      if (g_pMainWnd->m_ChannelMonitorView.IsWindowVisible())
      {
	      DWORD dwStyle = g_pMainWnd->m_ChannelMonitorView.GetWindowLong(GWL_EXSTYLE);
        dwStyle |= WS_EX_TOOLWINDOW | WS_EX_APPWINDOW | WS_EX_TOPMOST; // add the flags
	      g_pMainWnd->m_ChannelMonitorView.SetWindowLong( GWL_EXSTYLE , dwStyle); // this ignores the WS_EX_TOPMOST style.

	      dwStyle = g_pMainWnd->m_ChannelMonitorView.GetWindowLong(GWL_STYLE);
        dwStyle |= WS_POPUP; // add the flags
        dwStyle &= ~WS_CHILD;
	      g_pMainWnd->m_ChannelMonitorView.SetWindowLong( GWL_STYLE , dwStyle); // this ignores the WS_EX_TOPMOST style.

        g_pMainWnd->m_ChannelMonitorView.SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE); // so we have to do this too.
        g_pMainWnd->m_ChannelMonitorView.SetParent(NULL);
      }
    }
    */
  }
  return bHandled ? 0 : 1; // WM_SYSCOMMAND - An application should return zero if it processes this message
}

LRESULT CMainFrame::OnTaskBarMenuRestore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  RestoreWindow();

  return TRUE;
}

LRESULT CMainFrame::OnSettingChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	// Call the base class handler first
	BOOL bBaseHandled = FALSE;
	baseClass::OnSettingChange(uMsg,wParam,lParam,bBaseHandled);

	// Then broadcast to every descendant
	// (NOTE: If there are other top level windows,
	// they need to handle WM_SETTINGCHANGE themselves)
	this->SendMessageToDescendants(uMsg, wParam, lParam, TRUE);

	return 0;
}

LRESULT CMainFrame::OnSysColorChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// If baseClass defines OnSysColorChange, uncomment following code,
	// and removed "bHandled = FALSE;"
	bHandled = FALSE;

	//// Call the base class handler first
	//BOOL bBaseHandled = FALSE;
	//baseClass::OnSysColorChange(uMsg,wParam,lParam,bBaseHandled);

	// Then broadcast to every descendant
	// (NOTE: If there are other top level windows,
	// they need to handle WM_SETTINGCHANGE themselves)
	this->SendMessageToDescendants(uMsg, wParam, lParam, TRUE);

	return 0;
}

void CMainFrame::RefreshAllChildWindows( void )
{
  int i;
  CChildFrame *pChild;
  for (i = 0; i < g_ChildWndList.GetSize() ; i++)
  {
    pChild = CHILDFRAMEPTR(g_ChildWndList[i]);
    pChild->UpdateSettings(FALSE);
  }
}

LRESULT CMainFrame::OnRequestScrollToEnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
  bHandled = TRUE;

  // forward it on to the m_MsgView derived window pointed to by the lParam
  if (::IsWindow((HWND)lParam))
    ::PostMessage((HWND)lParam,WM_REQUESTSCROLLTOEND,0,0);

  return 0; // custom message
}

LRESULT CMainFrame::OnCopyData(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	char *CommandLine = (char *)(((COPYDATASTRUCT *)lParam)->lpData);
	HydraIRC_NewServer(CommandLine);
  return TRUE;
}

// override the normal MDIActivate() so that it always restores the window if the window was minimized
void CMainFrame::MDIActivate(HWND hWndChildToActivate)
{
	ATLASSERT(::IsWindow(m_hWndMDIClient));
	ATLASSERT(::IsWindow(hWndChildToActivate));

  WINDOWPLACEMENT WP;
  if (::GetWindowPlacement(hWndChildToActivate,&WP) &&
      WP.showCmd == SW_SHOWMINIMIZED)
    baseClass::MDIRestore(hWndChildToActivate);
  else
    baseClass::MDIActivate(hWndChildToActivate);
}


LRESULT CMainFrame::OnChildFrameClosed(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
#ifdef DEBUG
  sys_Printf(BIC_INFO,"Deleting child frame class");
#endif

  // we do this, because if we delete the class instance from the class's
  // OnFinalMessage() function then then some messages get processed after
  // the class has been deleted and the window is closed (only sometimes
  // though) so we delete the class here!

  CChildFrame *pChild = (CChildFrame *)lParam;
  delete pChild;

  return 0; // our message - no return value.
}
