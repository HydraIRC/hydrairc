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

// Output.cpp : implementation of the COutput class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

COutputView::COutputView(DWORD dwIcon /*= IDI_OUTPUT*/,CEventManager *pEventManager /*= NULL*/)
	: m_dwIcon(dwIcon),
    CListener(pEventManager)
{
}

void COutputView::OnFinalMessage(HWND hWnd)
{  
  //g_pMainWnd->m_CmdBar.RemoveCustomDrawWindow(m_hWnd);
}

LRESULT COutputView::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
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

LRESULT COutputView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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
  m_MsgView.SetControlCodeMode(CTRLCODE_MODE_DISPLAY);
  m_MsgView.SetContextMenu(IDR_OUTPUT_CONTEXT,m_hWnd); // the resource id of the menu, and the pointer to the window to receive the messages
  //g_pMainWnd->m_CmdBar.AddCustomDrawWindow(m_hWnd);
  UpdateSettings();
	return 0;
}

void COutputView::UpdateSettings( void )
{
  if (g_pPrefs)
  {
    m_MsgView.SetMaxBufferLines(INTPREF(PREF_nMaxScrollBufferLines));
    m_MsgView.SetTimeStamps(BOOLPREF(PREF_bOutputWindowTimestamps));
    m_MsgView.SetFont(FONTPREF(PREF_fOutputWindowFont));
    m_MsgView.SetColors(g_pPrefs->m_ColorPrefs);

    if (BOOLPREF(PREF_bSocketLogging))
      m_MsgView.SetFilterList(NULL);
    else
      m_MsgView.SetFilterList(g_DefaultFilterList_Output);
  }
  m_MsgView.RedrawWindow();
}

void COutputView::OnEvent(int EventID, void *pData)
{
  switch(EventID)
  {
    case EV_PREFSCHANGED:
      COutputView::UpdateSettings();
      break;
  }
}

#ifdef USE_XPCOMMANDBAR
LRESULT COutputView::OnMsgViewContextMenu(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  UINT ContextMenuID = (UINT)wParam;
	CMenu menuContext;
	menuContext.LoadMenu(IDR_OUTPUT_CONTEXT);        
	CMenuHandle menuPopup(menuContext.GetSubMenu(0));

  POINT pt;
  ::GetCursorPos(&pt);      
  m_CmdBar.TrackPopupMenu(menuPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y);
  //::TrackPopupMenu(menuPopup, 0, pt.x, pt.y, 0, m_hWnd, NULL);

  return 0; // ignored.
}
#endif