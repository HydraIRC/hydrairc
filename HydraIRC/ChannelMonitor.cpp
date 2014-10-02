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

// ChannelMonitorView.cpp : implementation of the COutput class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"


void CChannelMonitorView::OnDocked(HDOCKBAR hBar,bool bHorizontal)
{
  // we add remove these window types when the app is minimized/restored now
  /*  
	DWORD dwStyle = GetWindowLong(GWL_EXSTYLE) & (~(WS_EX_APPWINDOW | WS_EX_TOPMOST)); // remove the flags..
  SetWindowLong( GWL_EXSTYLE , dwStyle);
  */
  baseClass::OnDocked(hBar,bHorizontal);
}

void CChannelMonitorView::OnUndocked(HDOCKBAR hBar)
{
	baseClass::OnUndocked(hBar);
  // we add remove these window types when the app is minimized/restored now
  /*
	DWORD dwStyle = GetWindowLong(GWL_EXSTYLE);
  dwStyle |= WS_EX_APPWINDOW | WS_EX_TOPMOST; // add the flags
	SetWindowLong( GWL_EXSTYLE , dwStyle); // this ignores the WS_EX_TOPMOST style.
  SetWindowPos(HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE); // so we have to do this too.
  */
}

LRESULT CChannelMonitorView::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	if(wParam != SIZE_MINIMIZED )
	{
		RECT rc;
		GetClientRect(&rc);
		::SetWindowPos(m_MsgView.m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top ,SWP_NOZORDER | SWP_NOACTIVATE);
	}
  // m_MsgView.ScrollToEnd(); // calling this directly doesn't work as the richedit control window hasn't updated yet
  // so we do this instead:
  g_pMainWnd->PostMessage(WM_REQUESTSCROLLTOEND,0,(LPARAM)m_MsgView.m_hWnd); // and then it gets round to it...
	bHandled = FALSE;
  return bHandled ? 0 : 1; // WM_SIZE - If an application processes this message, it should return zero.
}

LRESULT CChannelMonitorView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(m_dwIcon), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

#ifdef USE_XPCOMMANDBAR
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
  m_CmdBar.Prepare();
#endif

  HWND out = m_MsgView.Create(m_hWnd,NULL,NULL,
                         WS_CHILD | WS_VISIBLE | 
                         WS_VSCROLL | ES_MULTILINE |
                         ES_DISABLENOSCROLL | ES_READONLY | 
                         ES_NOHIDESEL | ES_SAVESEL,
                         WS_EX_CLIENTEDGE);
  m_MsgView.SetControlCodeMode(CTRLCODE_MODE_INTERPRET);
  m_MsgView.SetContextMenu(IDR_CHANNELMONITOR_CONTEXT,m_hWnd); // the resource id of the menu, and the pointer to the window to receive the messages
  UpdateSettings();
	return 0;
}

void CChannelMonitorView::UpdateSettings( void )
{
  if (IsDocking())
  {
  	DWORD dwStyle = GetWindowLong(GWL_EXSTYLE) & (~(WS_EX_APPWINDOW | WS_EX_TOPMOST)); // remove the flags..
    SetWindowLong( GWL_EXSTYLE , dwStyle);
  }
  // m_MsgView.SetFilterList(g_DefaultFilterList_ChannelMonitor); // TODO: we don't do this anymore, BUT we will need to add m_Filter to CChannelMonitorView instead
  if (g_pPrefs)
  {
    m_MsgView.SetTimeStamps(BOOLPREF(PREF_bChannelMonitorTimestamps));
    m_MsgView.SetMaxBufferLines(INTPREF(PREF_nMaxScrollBufferLines));
    m_MsgView.SetFont(FONTPREF(PREF_fChannelMonitorFont));
    m_MsgView.SetColors(g_pPrefs->m_ColorPrefs);
  }
  m_MsgView.RedrawWindow();
}

BOOL CChannelMonitorView::IsMonitoring(IRCChannel *pChannel)
{
  return (m_ChannelList.Find(pChannel) != -1);
}

void CChannelMonitorView::AddChannel(IRCChannel *pChannel)
{
  if (!pChannel)
    return;

  if (IsMonitoring(pChannel))
    return;

  m_ChannelList.Add(pChannel);

  Message(pChannel,"\002Channel Monitoring Started!");
}

void CChannelMonitorView::RemoveChannel(IRCChannel *pChannel)
{
  if (m_ChannelList.Remove(pChannel))
  {
    Message(pChannel,"\002Channel Monitoring Stopped");
  }
}

void CChannelMonitorView::Message(IRCChannel *pChannel, char *Buffer)
{
  BufferItem BI(BIC_UNKNOWN,strdup(Buffer));
  Put(pChannel,&BI);
}

void CChannelMonitorView::Put(IRCChannel *pChannel, BufferItem *pBI)
{
  char *channelmonitorstr;

  if (!ProcessSimpleFilter(g_DefaultFilterList_ChannelMonitor,pBI->m_Contents))
  {
    channelmonitorstr = HydraIRC_BuildString(1024,"%s: %s",pChannel->m_pProperties->m_Name, pBI->m_Buffer);
    if (channelmonitorstr)
    {
      m_MsgView.Put(pBI->m_Contents, channelmonitorstr);

      if (BOOLPREF(PREF_bShowChannelMonitorMessageOnStatusBar))
      {
        stripcodes(channelmonitorstr);

        g_pMainWnd->SetStatusBar(STATUS_ICON_NOTE,channelmonitorstr);
      }
      free(channelmonitorstr);
    }
  }
}

LRESULT CChannelMonitorView::OnBnClickedStopAllMonitoring(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
{
  while (m_ChannelList.GetSize() > 0)
  {
    RemoveChannel(m_ChannelList[0]);
  }
  return 0;
}

void CChannelMonitorView::OnEvent(int EventID, void *pData)
{
  switch(EventID)
  {
    case EV_PREFSCHANGED:
      CChannelMonitorView::UpdateSettings();
      break;
  }
}

#ifdef USE_XPCOMMANDBAR
LRESULT CChannelMonitorView::OnMsgViewContextMenu(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  UINT ContextMenuID = (UINT)wParam;
	CMenu menuContext;
	menuContext.LoadMenu(IDR_CHANNELMONITOR_CONTEXT);        
	CMenuHandle menuPopup(menuContext.GetSubMenu(0));

  POINT pt;
  ::GetCursorPos(&pt);      
  m_CmdBar.TrackPopupMenu(menuPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y);
  //::TrackPopupMenu(menuPopup, 0, pt.x, pt.y, 0, m_hWnd, NULL);

  return 0; // ignored.
}
#endif