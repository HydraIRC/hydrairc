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

// EditBuddyGroupDlg.h : interface of the CEditBuddyGroupDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CEditBuddyGroupDlg : 
  public CDialogImpl<CEditBuddyGroupDlg>,
  public CDialogResize<CEditBuddyGroupDlg>
{
private:
  BOOL m_Initialised;

  // Controls
  CEdit m_NameCtrl;
  CComboBox m_NetworkCtrl;
  CButton m_OKCtrl;

public:
	enum { IDD = IDD_EDITBUDDYGROUP };

  char *m_Name;
  int m_NetworkID;

  CEditBuddyGroupDlg()
  {
    m_Name = NULL;
    m_NetworkID = NETWORK_ALL; // -1 = all networks
    m_Initialised = FALSE;
  }

  BOOL CEditBuddyGroupDlg::Validate( void );
  
	BEGIN_MSG_MAP(CEditBuddyGroupDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(ID_EBG_OK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
    COMMAND_HANDLER(IDC_EBG_NETWORK, CBN_SELCHANGE, OnSomethingChanged)
    COMMAND_HANDLER(IDC_EBG_NAME,    EN_CHANGE,     OnSomethingChanged)
    CHAIN_MSG_MAP(CDialogResize<CEditBuddyGroupDlg>)
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CEditBuddyGroupDlg)
    DLGRESIZE_CONTROL(IDC_EBG_NETWORK         ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_EBG_NAME            ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(ID_EBG_OK               ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDCANCEL                ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
  END_DLGRESIZE_MAP()


// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnSomethingChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
