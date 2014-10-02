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
