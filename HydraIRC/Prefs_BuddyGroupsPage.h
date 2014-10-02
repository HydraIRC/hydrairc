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

// Prefs_BuddyGroupsPage.h : interface of the CBuddyGroupsPage class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
class CBuddyGroupsPage : 
  public CDialogImpl<CBuddyGroupsPage>,
  public CDialogResize<CBuddyGroupsPage>,
  public CPrefsPage
{
private:
  CListViewCtrl m_NickListCtrl;
  CComboBox m_GroupCtrl;
  CEdit m_NickCtrl;
  CEdit m_HostCtrl;

public:
  enum { IDD = IDD_PREFS_BUDDYGROUPS };

	BEGIN_MSG_MAP(CBuddyGroupsPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDC_BUDDYGROUPS_GROUP,      CBN_SELCHANGE,    OnCbnSelchangeBuddyGroupsGroup)
    COMMAND_HANDLER(IDC_BUDDYGROUPS_ADD,        BN_CLICKED,       OnBnClickedBuddyGroupsAdd)
    COMMAND_HANDLER(IDC_BUDDYGROUPS_DELETE,     BN_CLICKED,       OnBnClickedBuddyGroupsDelete)
    COMMAND_HANDLER(IDC_BUDDYGROUPS_EDIT,       BN_CLICKED,       OnBnClickedBuddyGroupsEdit)
    COMMAND_HANDLER(IDC_BUDDYGROUPS_NICKDELETE, BN_CLICKED ,      OnBnClickedBuddygroupsNickdelete)
    COMMAND_HANDLER(IDC_BUDDYGROUPS_NICKADD,    BN_CLICKED,       OnBnClickedBuddygroupsNickadd)
    NOTIFY_HANDLER (IDC_BUDDYGROUPS_BUDDYLIST,  LVN_ENDLABELEDIT, OnLvnEndlabeleditBuddygroupsBuddylist)
    CHAIN_MSG_MAP(CDialogResize<CBuddyGroupsPage>)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP()

  BEGIN_DLGRESIZE_MAP(CBuddyGroupsPage)
    DLGRESIZE_CONTROL(IDC_BUDDYGROUPS_GROUP         ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_BUDDYGROUPS_GROUPSTATIC   ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_BUDDYGROUPS_ADD           ,DLSZ_MOVE_X)
    DLGRESIZE_CONTROL(IDC_BUDDYGROUPS_DELETE        ,DLSZ_MOVE_X)
    DLGRESIZE_CONTROL(IDC_BUDDYGROUPS_EDIT          ,DLSZ_MOVE_X)

    DLGRESIZE_CONTROL(IDC_BUDDYGROUPS_MEMBERSSTATIC ,DLSZ_SIZE_X | DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_BUDDYGROUPS_NICK          ,DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_BUDDYGROUPS_NICKADD       ,DLSZ_MOVE_X)
    DLGRESIZE_CONTROL(IDC_BUDDYGROUPS_NICKDELETE    ,DLSZ_MOVE_X)

    DLGRESIZE_CONTROL(IDC_BUDDYGROUPS_HOST          ,DLSZ_SIZE_X)

    DLGRESIZE_CONTROL(IDC_BUDDYGROUPS_BUDDYLIST     ,DLSZ_SIZE_X | DLSZ_SIZE_Y)
  END_DLGRESIZE_MAP()

  // page common stuff
  void OnPageDisplay ( void );
  void OnPageDone ( void );
  BOOL OnPageValidate ( void );
  
  // page specific stuff
  void UpdateNickList( int ItemNum );
  void UpdateGroupCtrl( void );
  void ResetDialog( int GroupNum );

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);  
  LRESULT OnCbnSelchangeBuddyGroupsGroup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedBuddyGroupsAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedBuddyGroupsDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedBuddyGroupsEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnLvnEndlabeleditBuddygroupsBuddylist(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
  LRESULT OnBnClickedBuddygroupsNickdelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedBuddygroupsNickadd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
