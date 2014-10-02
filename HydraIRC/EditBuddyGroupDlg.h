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
