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
