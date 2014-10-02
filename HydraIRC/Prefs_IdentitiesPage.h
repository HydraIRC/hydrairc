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
