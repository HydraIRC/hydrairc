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

// Prefs_IdentitiesPage.h : interface of the CNotificationsPage class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
class CNotificationsPage : 
  public CDialogImpl<CNotificationsPage>,
  public CDialogResize<CNotificationsPage>,
  public CPrefsPage
{
private:
  CListViewCtrl m_EventsCtrl;
  CComboBox m_GroupCtrl;
public:
  enum { IDD = IDD_PREFS_NOTIFICATIONS };

	BEGIN_MSG_MAP(CNotificationsPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDC_NOTIFICATIONS_GROUP, CBN_SELCHANGE, OnCbnSelchangeNotificationsGroup)
    NOTIFY_HANDLER(IDC_NOTIFICATIONS_EVENTS, NM_RCLICK, OnNMRclickNotificationsEvents)
    NOTIFY_HANDLER(IDC_NOTIFICATIONS_EVENTS, NM_DBLCLK, OnNMDblclkNotificationsEvents)
    COMMAND_HANDLER(IDC_NOTIFICATIONS_NEW, BN_CLICKED, OnBnClickedNotificationsNew)
    COMMAND_HANDLER(IDC_NOTIFICATIONS_DELETE, BN_CLICKED, OnBnClickedNotificationsDelete)
    COMMAND_HANDLER(IDC_NOTIFICATIONS_EDIT, BN_CLICKED, OnBnClickedNotificationsEdit)
    CHAIN_MSG_MAP(CDialogResize<CNotificationsPage>)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CNotificationsPage)
    DLGRESIZE_CONTROL(IDC_NOTIFICATIONS_GROUP   ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_NOTIFICATIONS_EVENTS  ,DLSZ_SIZE_X | DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_STATIC_INFO           ,DLSZ_SIZE_X | DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_NOTIFICATIONS_NEW     ,DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_NOTIFICATIONS_DELETE  ,DLSZ_SIZE_X | DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_NOTIFICATIONS_EDIT    ,DLSZ_MOVE_X | DLSZ_MOVE_Y)
  END_DLGRESIZE_MAP()

  // page common stuff
  void OnPageDisplay ( void );
  void OnPageDone ( void );
  BOOL OnPageValidate ( void );
  
  // page specific stuff
  void UpdateEventView( int ItemNum );
  void CNotificationsPage::EditEvent(int ItemNum);

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);  
  LRESULT OnCbnSelchangeNotificationsGroup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  LRESULT OnNMRclickNotificationsEvents(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
  LRESULT OnNMDblclkNotificationsEvents(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
  LRESULT OnBnClickedNotificationsNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedNotificationsDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedNotificationsEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
