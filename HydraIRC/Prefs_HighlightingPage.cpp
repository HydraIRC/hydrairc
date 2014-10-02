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

// Prefs_HighlightingPage.h : interface of the CHighlightingPage class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HydraIRC.h"
#include "prefs_highlightingpage.h"

void CHighlightingPage::OnPageDisplay( void )
{
  m_HighlightCtrl.Attach(GetDlgItem(IDC_HIGHLIGHT_LIST));

  UpdateWindow();
}

void CHighlightingPage::OnPageDone( void )
{
  while (m_HighlightCtrl.GetCount() > 0)
  {
    m_HighlightCtrl.DeleteString(0);
  }
  m_HighlightCtrl.Detach();
}

BOOL CHighlightingPage::OnPageValidate( void )
{
  return TRUE;
}

LRESULT CHighlightingPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // the real init is done when the page is OnPageDisplay'd, as other preference items might
  // make a difference to the data
  DlgResize_Init(false,true,0);
  return 0;
}

LRESULT CHighlightingPage::OnBnClickedHighlightNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  return 0;
}

LRESULT CHighlightingPage::OnBnClickedHighlightEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  return 0;
}

LRESULT CHighlightingPage::OnBnClickedHighlightDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  return 0;
}
