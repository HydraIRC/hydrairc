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
