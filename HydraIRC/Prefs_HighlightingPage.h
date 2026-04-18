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

#pragma once

class CHighlightingPage : 
  public CDialogImpl<CHighlightingPage>,
  public CDialogResize<CHighlightingPage>,
  public CPrefsPage
{
private:
  CListBox m_HighlightCtrl;
public:
  enum { IDD = IDD_PREFS_HIGHLIGHT };

	BEGIN_MSG_MAP(CHighlightingPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDC_HIGHLIGHT_NEW, BN_CLICKED, OnBnClickedHighlightNew)
    COMMAND_HANDLER(IDC_HIGHLIGHT_EDIT, BN_CLICKED, OnBnClickedHighlightEdit)
    COMMAND_HANDLER(IDC_HIGHLIGHT_DELETE, BN_CLICKED, OnBnClickedHighlightDelete)
    COMMAND_HANDLER(IDC_HIGHLIGHT_LIST, LBN_DBLCLK, OnBnClickedHighlightEdit)
    CHAIN_MSG_MAP(CDialogResize<CHighlightingPage>)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CHighlightingPage)
    DLGRESIZE_CONTROL(IDC_STATIC_CONFIGURE      ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_HIGHLIGHT_NEW          ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_HIGHLIGHT_DELETE       ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_HIGHLIGHT_EDIT         ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_HIGHLIGHT_LIST       ,DLSZ_SIZE_X | DLSZ_SIZE_Y)
  END_DLGRESIZE_MAP()

  // page common stuff
  void OnPageDisplay ( void );
  void OnPageDone ( void );
  BOOL OnPageValidate ( void );

  // page specific stuff
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedHighlightNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedHighlightEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedHighlightDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnLbnDblclkHighlightingList(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedHighlightSetdefault(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
