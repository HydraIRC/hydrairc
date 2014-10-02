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

// ChildCommon.cpp : comment here
//
// These routines are common to all MDI child windows.
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"

BOOL CChildCommon::IsActive( void )
{
  BOOL Maximized;
  return (IsWindow() && (m_hWnd == g_pMainWnd->MDIGetActive(&Maximized)) );
}

void CChildCommon::HideWindow( void )
{
  if (BOOLPREF(PREF_bHideNotMinimize))
  {
    BOOL FoundNonHiddenWindow = FALSE;
    int i;
    for (i = 0 ; i < g_ChildWndList.GetSize(); i++)
    {
      HWND hWnd = g_ChildWndList[i]->m_hWnd;

      
      if (hWnd == m_hWnd)
        continue; // ignore ourself

      DWORD dwStyle = ::GetWindowLong(hWnd,GWL_STYLE);

      if (dwStyle & WS_VISIBLE)
      {
        FoundNonHiddenWindow = TRUE;
        break;
      }
    }
    //SetWindowPos(NULL,0,0,0,0,SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);
    if (FoundNonHiddenWindow)
      g_pMainWnd->MDINext(m_hWnd);

    ShowWindow(SW_HIDE);

#ifdef USE_TABBEDMDI
    //SetTabText("HIDDEN");
	// TODO: FIX THIS
    ::SendMessage(GetParent(), UWM_MDICHILDHIDDEN, (WPARAM)m_hWnd, 0);
#endif
  }
  else
  {
    ShowWindow(SW_MINIMIZE);
  }
}

// unhide and display the window, if the current window is maximized make sure we
// maximize the window we're unhiding.
void CChildCommon::ActivateWindow( void )
{
  BOOL bMaximized = FALSE;
  CWindow ActiveWnd = g_pMainWnd->MDIGetActive(&bMaximized);
  CWindow ParentWnd = ActiveWnd.GetParent();

  if (bMaximized)
    ParentWnd.SetRedraw(FALSE);

  g_pMainWnd->MDIActivate(m_hWnd);
#ifdef USE_TABBEDMDI
	::SendMessage(GetParent(), UWM_MDICHILDACTIVATIONCHANGE, (WPARAM)m_hWnd, 0);
  SendMessage(WM_UPDATETAB, (WPARAM)0, (LPARAM)NULL);
  //SetTabText("SHOWN");
#endif

	if(bMaximized == TRUE)
  {
		ShowWindow(SW_SHOWMAXIMIZED);
		
		ParentWnd.SetRedraw(TRUE);
		ParentWnd.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
	}

  /*
  if (BOOLPREF(PREF_bHideNotMinimize))
  {
    SetWindowPos(HWND_TOP,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW );
    RedrawWindow(0,0,RDW_FRAME|RDW_INVALIDATE|RDW_ERASENOW|RDW_ALLCHILDREN);
  }
  else
  {
    g_pMainWnd->MDIActivate(m_hWnd);
  }
  */
}