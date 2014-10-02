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

// Prefs_HighlightingPage.h : interface of the CHighlightingPage class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CHighlightingPage : 
  public CDialogImpl<CHighlightingPage>,
  public CDialogResize<CHighlightingPage>,
  public CPrefsPage
{
private:
  CListBox m_HighlightCtrl;
public:
  enum { IDD = IDD_PREFS_HIGHLIGHT };

	BEGIN_MSG_MAP(CHighlightingPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDC_HIGHLIGHT_NEW, BN_CLICKED, OnBnClickedHighlightNew)
    COMMAND_HANDLER(IDC_HIGHLIGHT_EDIT, BN_CLICKED, OnBnClickedHighlightEdit)
    COMMAND_HANDLER(IDC_HIGHLIGHT_DELETE, BN_CLICKED, OnBnClickedHighlightDelete)
    COMMAND_HANDLER(IDC_HIGHLIGHT_LIST, LBN_DBLCLK, OnBnClickedHighlightEdit)
    CHAIN_MSG_MAP(CDialogResize<CHighlightingPage>)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CHighlightingPage)
    DLGRESIZE_CONTROL(IDC_STATIC_CONFIGURE      ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_HIGHLIGHT_NEW          ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_HIGHLIGHT_DELETE       ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_HIGHLIGHT_EDIT         ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_HIGHLIGHT_LIST       ,DLSZ_SIZE_X | DLSZ_SIZE_Y)
  END_DLGRESIZE_MAP()

  // page common stuff
  void OnPageDisplay ( void );
  void OnPageDone ( void );
  BOOL OnPageValidate ( void );

  // page specific stuff
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedHighlightNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedHighlightEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedHighlightDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnLbnDblclkHighlightingList(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedHighlightSetdefault(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
