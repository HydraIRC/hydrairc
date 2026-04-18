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

#pragma once

class IRCServerDetails;

class CEditServerDetailsDlg : 
  public CDialogImpl<CEditServerDetailsDlg>,
  public CDialogResize<CEditServerDetailsDlg>
{
private:
  BOOL m_Initialised;
  IRCServerDetails *m_pServerDetails;

  // Controls
  CEdit m_DescriptionCtrl;
  CEdit m_AddressCtrl;
  CEdit m_PortCtrl;
  CEdit m_PasswordCtrl;
  CEdit m_Password2Ctrl;
  CComboBox m_NetworkCtrl;
  CComboBox m_PriorityCtrl;
  CButton m_OKCtrl;

public:
	enum { IDD = IDD_EDITSERVER };

  CEditServerDetailsDlg(IRCServerDetails *pServerDetails)
  {
    m_pServerDetails = pServerDetails;
    m_Initialised = FALSE;
  }

  ~CEditServerDetailsDlg( void )
  {
  }

  BOOL CEditServerDetailsDlg::Validate( void );
  
	BEGIN_MSG_MAP(CEditServerDetailsDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(ID_ESRV_OK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
    COMMAND_HANDLER(IDC_ESRV_DESCRIPTION, EN_CHANGE,     OnSomethingChanged)
    COMMAND_HANDLER(IDC_ESRV_ADDRESS,     EN_CHANGE,     OnSomethingChanged)
    COMMAND_HANDLER(IDC_ESRV_PORT,        EN_CHANGE,     OnSomethingChanged)
    COMMAND_HANDLER(IDC_ESRV_PASSWORD,    EN_CHANGE,     OnSomethingChanged)
    COMMAND_HANDLER(IDC_ESRV_PASSWORD2,   EN_CHANGE,     OnSomethingChanged)
    COMMAND_HANDLER(IDC_ESRV_NETWORK,     CBN_SELCHANGE, OnSomethingChanged)
    COMMAND_HANDLER(IDC_ESRV_PRIORITY,    CBN_SELCHANGE, OnSomethingChanged)
    CHAIN_MSG_MAP(CDialogResize<CEditServerDetailsDlg>)
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CEditServerDetailsDlg)
    DLGRESIZE_CONTROL(IDC_ESRV_DESCRIPTION     ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_ESRV_ADDRESS         ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_ESRV_PORT            ,DLSZ_MOVE_X)
    DLGRESIZE_CONTROL(IDC_ESRV_NETWORK         ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_ESRV_PRIORITY        ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_ESRV_PASSWORD        ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_ESRV_PASSWORD2       ,DLSZ_MOVE_X)
    DLGRESIZE_CONTROL(ID_ESRV_OK               ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDCANCEL                 ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
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
