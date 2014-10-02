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
