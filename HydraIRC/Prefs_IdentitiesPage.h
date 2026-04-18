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

// Prefs_IdentitiesPage.h : interface of the CIdentitiesPage class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CIdentitiesPage : 
  public CDialogImpl<CIdentitiesPage>,
  public CDialogResize<CIdentitiesPage>,
  public CPrefsPage
{
private:
  CListBox m_IdentityCtrl;
public:
  enum { IDD = IDD_PREFS_IDENTITIES };

	BEGIN_MSG_MAP(CIdentitiesPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDC_IDENTITY_NEW, BN_CLICKED, OnBnClickedIdentityNew)
    COMMAND_HANDLER(IDC_IDENTITY_EDIT, BN_CLICKED, OnBnClickedIdentityEdit)
    COMMAND_HANDLER(IDC_IDENTITY_DELETE, BN_CLICKED, OnBnClickedIdentityDelete)
    COMMAND_HANDLER(IDC_IDENTITIES_LIST, LBN_DBLCLK, OnBnClickedIdentityEdit)
    COMMAND_HANDLER(IDC_IDENTITY_SETDEFAULT, BN_CLICKED, OnBnClickedIdentitySetdefault)
    CHAIN_MSG_MAP(CDialogResize<CIdentitiesPage>)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CIdentitiesPage)
    DLGRESIZE_CONTROL(IDC_STATIC_CONFIGURE      ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_IDENTITY_NEW          ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_IDENTITY_DELETE       ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_IDENTITY_EDIT         ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_IDENTITY_SETDEFAULT   ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_IDENTITY_DEFAULT      ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_IDENTITY_NOTE         ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_IDENTITIES_LIST       ,DLSZ_SIZE_X | DLSZ_SIZE_Y)
  END_DLGRESIZE_MAP()

  // page common stuff
  void OnPageDisplay ( void );
  void OnPageDone ( void );
  BOOL OnPageValidate ( void );

  // page specific stuff
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedIdentityNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedIdentityEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedIdentityDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnLbnDblclkIdentitiesList(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedIdentitySetdefault(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
