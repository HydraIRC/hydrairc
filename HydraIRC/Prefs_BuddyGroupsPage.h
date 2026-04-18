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
