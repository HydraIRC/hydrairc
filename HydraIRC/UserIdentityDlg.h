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

// UserIdentityDlg.h : interface of the CUserIdentityDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CUserIdentityDlg : public CDialogImpl<CUserIdentityDlg>
{
private:
  UserIdentity *m_pIdentity;
  BOOL m_Initialised;
  char *m_TmpNick;
  CEdit m_DescriptionCtrl;
  CEdit m_RealNameCtrl;
  CEdit m_UserNameCtrl;
  CEdit m_IdentdOSCtrl;
  CEdit m_IdentdUserNameCtrl;
  CEdit m_NickCtrl;
  CListBox m_NickListCtrl;
  CButton m_OKCtrl;
public:
	enum { IDD = IDD_USERIDENTITY };

  CUserIdentityDlg(UserIdentity *pIdentity)
  {
    m_pIdentity = pIdentity;
    m_Initialised = FALSE;
    m_TmpNick = NULL;
  }
  BOOL CUserIdentityDlg::Validate( void );
  
	BEGIN_MSG_MAP(CUserIdentityDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_CODE_HANDLER(EN_CHANGE, OnEnChange)
    COMMAND_HANDLER(IDC_UI_ADD, BN_CLICKED, OnBnClickedIdentitiesAdd)
    COMMAND_HANDLER(IDC_UI_NICK, EN_SETFOCUS, OnEnSetfocusUiNick)
    COMMAND_HANDLER(IDC_UI_DELETE, BN_CLICKED, OnBnClickedUiDelete)
    COMMAND_HANDLER(IDC_UI_UP, BN_CLICKED, OnBnClickedUiUp)
    COMMAND_HANDLER(IDC_UI_DOWN, BN_CLICKED, OnBnClickedUiDown)
		COMMAND_ID_HANDLER(IDOK, OnBnClickedDefault)
		COMMAND_ID_HANDLER(ID_UI_OK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
  END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedIdentitiesAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnEnSetfocusUiNick(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnEnChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedUiDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedUiUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedUiDown(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedDefault(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  void AddClicked( void );
  void OKClicked( void );
};
