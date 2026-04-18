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

// AddNetworkDlg.h : interface of the CAddNetworkDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CAddNetworkDlg : 
  public CDialogImpl<CAddNetworkDlg>,
  public CDialogResize<CAddNetworkDlg>
{
private:
  BOOL m_Initialised;

  // Controls
  CEdit m_NameCtrl;
  CButton m_OKCtrl;
  CComboBox m_IdentityCtrl;


public:
	enum { IDD = IDD_ADDNETWORK };

  IRCNetworkDetails *m_pND;

  CAddNetworkDlg(IRCNetworkDetails *pND)
  {
    m_pND = pND;
    m_Initialised = FALSE;
  }

  BOOL CAddNetworkDlg::Validate( void );
  
	BEGIN_MSG_MAP(CAddNetworkDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(ID_ANET_OK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
    COMMAND_HANDLER(IDC_ANET_NAME,    EN_CHANGE,     OnSomethingChanged)
    CHAIN_MSG_MAP(CDialogResize<CAddNetworkDlg>)
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CAddNetworkDlg)
    DLGRESIZE_CONTROL(IDC_ANET_NAME           ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_ANET_IDENTITYLIST   ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(ID_ANET_OK              ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
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
